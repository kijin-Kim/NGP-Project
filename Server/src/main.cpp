#include <queue>
#include <fstream>
#include "Network/Network.h"
#include "Network/Data.h"
#include "State.h"
#define MAX_USER 4

DWORD ListeningThreadProc(LPVOID);
DWORD CommunicationThreadProc(LPVOID arg);


std::queue<IData*> g_DataQueue;
IData* g_ProcessedData[MAX_USER];
ClientState g_ClientStates[MAX_USER] = { ClientState::Login, ClientState::Login, ClientState::Login, ClientState::Login, }; //TEMP
HANDLE g_ClientEvents[MAX_USER];
int g_ClientCount = 0;
HANDLE g_ListeningThreadHandle;
std::string g_ClientAddress[MAX_USER];
std::fstream g_OutFile;

CRITICAL_SECTION cs;

void Log(const std::string& addr, const std::string& messageFormat)
{
	if (!messageFormat.empty())
	{
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);
		char logBuffer[500];
		sprintf(logBuffer, "[%04d-%02d-%02dT%02d:%02d:%02d+09:00]        %s        %s\n",
			currentTime.wYear,
			currentTime.wMonth,
			currentTime.wDay,
			currentTime.wHour,
			currentTime.wSecond,
			currentTime.wMinute,
			addr.c_str(),
			messageFormat.c_str());
		g_OutFile << logBuffer;
	}
	
}

struct ClientInformation
{
	unsigned int ID = 0;
	SOCKET Socket = NULL;
};


int main()
{	
	LobbyState* lobbyState = new LobbyState();
	GameState* gameState = new GameState();
	LoginState* loginState = new LoginState();

	g_OutFile.open("Server.log", std::ios_base::out);

	InitializeCriticalSection(&cs);

	char buf[BUFSIZE + 1];
	Network* network = Network::GetInstance();


	network->isServer = true;

	g_ListeningThreadHandle = CreateThread(NULL, 0, ListeningThreadProc,
		NULL, 0, NULL);

	int64_t countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	int64_t currentCounts;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCounts);

	double secondsPerCount = 1.0 / (double)countsPerSec;
	int64_t lastCounts = currentCounts;

	while (true)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentCounts);
		float deltaTime = (currentCounts - lastCounts) * secondsPerCount;
		lastCounts = currentCounts;
		while (deltaTime < 0.016f)  // FPS Limit
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&currentCounts);
			deltaTime += (currentCounts - lastCounts) * secondsPerCount;
			lastCounts = currentCounts;
		}
		
		if (g_DataQueue.empty())
		{
			EnterCriticalSection(&cs);
			for (auto s : g_ClientStates)
			{
				switch (s)
				{
				case ClientState::Game:
					gameState->UpdateData(deltaTime, nullptr);
					break;
				default:
					break;
				}
			}
			LeaveCriticalSection(&cs);
			continue;
		}


		//클라가 보낸 데이터를 꺼낸다.
		IData* data = g_DataQueue.front();	
		g_DataQueue.pop();

		//로직을 계산한다.
		
		EnterCriticalSection(&cs);
		switch (g_ClientStates[data->ID])
		{
		case ClientState::Login:
		{
			LeaveCriticalSection(&cs);
			g_ProcessedData[data->ID] = loginState->UpdateData(deltaTime, data);
			break;
		}
		case ClientState::Lobby:
			LeaveCriticalSection(&cs);
			g_ProcessedData[data->ID] = lobbyState->UpdateData(deltaTime, data);
			break;
		case ClientState::Game:
			LeaveCriticalSection(&cs);
			g_ProcessedData[data->ID] = gameState->UpdateData(deltaTime, data);
			break;
		default:
			break;
		}

		//새로운 데이터를 넣는다.
		SetEvent(g_ClientEvents[data->ID]);
	}

	DeleteCriticalSection(&cs);


	g_OutFile.close();
}

DWORD ListeningThreadProc(LPVOID)
{
	Network* network = Network::GetInstance();
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);


	network->BindAndListen(listeningSocket);

	while (true)
	{
		
		SOCKET clientSocket = network->Accept(listeningSocket);

		ClientInformation information = { g_ClientCount++, clientSocket };
		SOCKADDR_IN socketAddr = {};
		int socketAddrSize = sizeof(socketAddr);
		getpeername(information.Socket, (SOCKADDR*)&socketAddr, &socketAddrSize);
		g_ClientAddress[information.ID] = inet_ntoa(socketAddr.sin_addr);
		Log(g_ClientAddress[information.ID], "클라이언트 접속");
		// CLIENT 마다 쓰레드를 만들어줌
		HANDLE hThread = CreateThread(NULL, 0, CommunicationThreadProc, (LPVOID)&information, 0, NULL);
		if (!hThread)
		{
			// SOMETHING
		}

	}

	network->Release(listeningSocket);
	return 0;
}

DWORD CommunicationThreadProc(LPVOID arg)
{
	Network* network = Network::GetInstance();

	ClientInformation clientInformation = *(ClientInformation*)arg;
	
	g_ClientEvents[clientInformation.ID] = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (clientInformation.ID >= 3)
		TerminateThread(g_ListeningThreadHandle, 0);

	
	while (true) 
	{
		int recvBufferSize = 0;
		int sendBufferSize = 0;

		IData* recvBuffer = nullptr;

		ClientToServerInLogin ToServerLogin = {};
		ClientToServerInLobby ToServerLobby = {};
		ClientToServerInGame ToServerGame = {};

		ClientState currentClientState;

		EnterCriticalSection(&cs);
		switch (g_ClientStates[clientInformation.ID])
		{
		case ClientState::Login:
			currentClientState = ClientState::Login;
			recvBufferSize = sizeof(ClientToServerInLogin);
			recvBuffer = &ToServerLogin;
			sendBufferSize = sizeof(ServerToClientInLogin);
			break;
		case ClientState::Lobby:
			currentClientState = ClientState::Lobby;
			recvBufferSize = sizeof(ClientToServerInLobby);
			recvBuffer = &ToServerLobby;
			sendBufferSize = sizeof(ServerToClientInLobby);
			break;
		case ClientState::Game:
			currentClientState = ClientState::Game;
			recvBufferSize = sizeof(ClientToServerInGame);
			recvBuffer = &ToServerGame;
			sendBufferSize = sizeof(ServerToClientInGame);
			break;		
		default:
			break;
		}
		LeaveCriticalSection(&cs);

		
		if (recvBuffer)
		{
			// 클라이언트로부터 데이터를 받음.
			network->Recv(clientInformation.Socket, (char*)recvBuffer, recvBufferSize);
			
			if (network->retval == SOCKET_ERROR) {
				network->ErrDisplay(L"recv()");
				break;
			}
			recvBuffer->ID = clientInformation.ID;

			if (network->retval == 0)
				break;

			char logRecvbuffer[200] = {};
	
			switch (currentClientState)
			{
			case ClientState::Login:
			{
				std::string outNickName;
				outNickName.assign(std::begin(((ClientToServerInLogin*)recvBuffer)->NickName), std::end(((ClientToServerInLogin*)recvBuffer)->NickName));
				if (!(((ClientToServerInLogin*)recvBuffer)->NickName[0] == 0))
				{
					sprintf(logRecvbuffer, "%d바이트 받음        NickName : %s", network->retval, outNickName.c_str());
					Log(g_ClientAddress[clientInformation.ID], logRecvbuffer);
				}
				break;
			}
			case ClientState::Lobby:
			{
				std::string outChat;
				outChat.assign(std::begin(((ClientToServerInLobby*)recvBuffer)->Chat), std::end(((ClientToServerInLobby*)recvBuffer)->Chat));
				if (!(((ClientToServerInLobby*)recvBuffer)->Chat[0] == 0))
				{
					sprintf(logRecvbuffer, "%d바이트 받음        Chat : %s", network->retval, outChat.c_str());
					Log(g_ClientAddress[clientInformation.ID], logRecvbuffer);
				}
				break;
			}
			case ClientState::Game:
			{
				if (((ClientToServerInGame*)recvBuffer)->Input.Action != -1)
				{
					char actionInfo[50] = {};
					switch (((ClientToServerInGame*)recvBuffer)->Input.Action)
					{
					case GLFW_PRESS:
						sprintf(actionInfo, "Action : GLFW_PRESS");
						break;
					case GLFW_RELEASE:
						sprintf(actionInfo, "Action : GLFW_RELEASE");
						break;
					case GLFW_REPEAT:
						sprintf(actionInfo, "Action : GLFW_REPEAT");
						break;
					default:
						break;
					}


					char keyInfo[50] = {};
					switch (((ClientToServerInGame*)recvBuffer)->Input.Key)
					{
					case GLFW_KEY_LEFT:
						sprintf(keyInfo, "Key : GLFW_KEY_LEFT");
						break;
					case GLFW_KEY_RIGHT:
						sprintf(keyInfo, "Key : GLFW_KEY_RIGHT");
						break;
					case GLFW_KEY_UP:
						sprintf(keyInfo, "Key : GLFW_KEY_UP");
						break;
					case GLFW_KEY_DOWN:
						sprintf(keyInfo, "Key : GLFW_KEY_DOWN");
						break;
					case GLFW_KEY_SPACE:
						sprintf(keyInfo, "Key : GLFW_KEY_SPACE");
						break;
					default:
						break;
					}

					sprintf(logRecvbuffer, "%d바이트 받음        %s        %s", network->retval, actionInfo, keyInfo);
					Log(g_ClientAddress[clientInformation.ID], logRecvbuffer);
				}
			}
			default:
				break;
			}
			


			// 데이터를 Queue에 넣음.
			g_DataQueue.push(recvBuffer);


			// 로직이 계산될 때까지 기다림.
			if (g_ClientEvents[recvBuffer->ID])
				WaitForSingleObject(g_ClientEvents[recvBuffer->ID], INFINITE);

			// 계산된 데이터를 보냄
			if (recvBuffer)
			{
				if (g_ProcessedData[recvBuffer->ID])
				{
					if (g_ClientCount >= 4)
					{
						bool bShouldChangeState = true;
						EnterCriticalSection(&cs);
						for (int i = 0; i < _countof(g_ClientStates); i++)
						{
							if (g_ClientStates[i] == ClientState::Login)
								bShouldChangeState = false;

						}
						LeaveCriticalSection(&cs);

						if (bShouldChangeState)
						{
							EnterCriticalSection(&cs);
							for (int i = 0; i < _countof(g_ClientStates); i++)
							{
								g_ClientStates[i] = ClientState::Game;
							}
							LeaveCriticalSection(&cs);
							((ServerToClientInLobby*)g_ProcessedData[recvBuffer->ID])->bShouldStartMatch = true;
						}
					}
					network->Send(clientInformation.Socket, (char*)g_ProcessedData[recvBuffer->ID], sendBufferSize);



					switch (currentClientState)
					{
					case ClientState::Login:
					{
						char logSendBuffer[200] = {};
						char resultBuffer[20] = {};
						switch (((ServerToClientInLogin*)g_ProcessedData[recvBuffer->ID])->Result)
						{
						case LoginResult::None:
							break;
						case LoginResult::Succeded:
							sprintf(resultBuffer, "Result : 성공!");
							sprintf(logSendBuffer, "%d바이트 보냄        %s", sendBufferSize, resultBuffer);
							Log(g_ClientAddress[clientInformation.ID], logSendBuffer);
							break;
						case LoginResult::Failed:
							sprintf(resultBuffer, "Result : 실패!");
							sprintf(logSendBuffer, "%d바이트 보냄       %s", sendBufferSize, resultBuffer);
							Log(g_ClientAddress[clientInformation.ID], logSendBuffer);
							break;
						default:
							break;
						}
						break;
					}
						break;
					case ClientState::Lobby:
					{
						char logSendBuffer[200] = {};
						if (!(((ClientToServerInLobby*)recvBuffer)->Chat[0] == 0))
						{
							ChatLine* Chats = ((ServerToClientInLobby*)g_ProcessedData[recvBuffer->ID])->Chats;
							bool bShouldStartMatch = ((ServerToClientInLobby*)g_ProcessedData[recvBuffer->ID])->bShouldStartMatch;
							std::wstring chatInfo;
							for (int i = 0; i < 16; i++)
							{
								if (Chats[i].Line[i] != 0)
								{
									chatInfo += Chats[i].Line;
									chatInfo += L" ";
								}
							}
							std::string cChatInfo;
							cChatInfo.assign(chatInfo.begin(), chatInfo.end());

							if (bShouldStartMatch)
							{
								sprintf(logSendBuffer, "%d바이트 보냄       %s        매칭을 시작 해야하는가 : True", sendBufferSize, cChatInfo.c_str());
								Log(g_ClientAddress[clientInformation.ID], logSendBuffer);
							}
							else
							{
								sprintf(logSendBuffer, "%d바이트 보냄       %s        매칭을 시작 해야하는가 : False", sendBufferSize, cChatInfo.c_str());
								Log(g_ClientAddress[clientInformation.ID], logSendBuffer);
							}
						}
						
						break;
					}
						
					case ClientState::Game:
					{
						char logSendBuffer[500] = {};
						float2* positions = ((ServerToClientInGame*)g_ProcessedData[recvBuffer->ID])->ObjectPositions;
						uint8_t* scores = ((ServerToClientInGame*)g_ProcessedData[recvBuffer->ID])->Scores;

						char logPositionBuffer[300];
						sprintf(logPositionBuffer, "positions[0] : X : %f, Y : %f positions[1] : X : %f, Y : %f positions[2] : X : %f, Y : %f positions[3] : X : %f, Y : %f positions[4] : X : %f, Y : %f",
							positions[0].X, positions[0].Y, positions[1].X, positions[1].Y, positions[2].X, positions[2].Y, positions[3].X, positions[3].Y, positions[4].X, positions[4].Y);

						char logScoreBuffer[100];
						sprintf(logScoreBuffer, "Left Team Score : %d, Right Team Score : %d", scores[0], scores[1]);

						sprintf(logSendBuffer, "%d바이트 보냄       %s        %s", sendBufferSize, logPositionBuffer, logScoreBuffer);
						Log(g_ClientAddress[clientInformation.ID], logSendBuffer);
						break;
					}
					default:
						break;
					}

					EnterCriticalSection(&cs);
					switch (g_ClientStates[clientInformation.ID])
					{
					case ClientState::Login:
						if (((ServerToClientInLogin*)g_ProcessedData[recvBuffer->ID])->Result == LoginResult::Succeded)
						{
							g_ClientStates[recvBuffer->ID] = ClientState::Lobby;
						}
						break;

					default:
						break;
					}
					LeaveCriticalSection(&cs);

					if (g_ProcessedData[recvBuffer->ID] && recvBuffer)
					{
						delete g_ProcessedData[recvBuffer->ID];
						g_ProcessedData[recvBuffer->ID] = nullptr;
						recvBuffer = nullptr;
					}

				}
			}
			
		}
		
	}

	return 0;
}
