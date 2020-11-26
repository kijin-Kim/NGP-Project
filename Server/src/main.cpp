#include <queue>
#include "Network/Network.h"
#include "Network/Data.h"
#include "State.h"
#define MAX_USER 4

DWORD ListeningThreadProc(LPVOID);
DWORD CommunicationThreadProc(LPVOID arg);


std::queue<IData*> g_DataQueue;
IData* g_ProcessedData[MAX_USER];
ClientState g_ClientStates[MAX_USER] = { ClientState::Game, ClientState::Game, ClientState::Game, ClientState::Game, }; //TEMP
HANDLE g_ClientEvents[MAX_USER];


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

	char buf[BUFSIZE + 1];
	Network* network = Network::GetInstance();


	network->isServer = true;

	HANDLE hThread = CreateThread(NULL, 0, ListeningThreadProc,
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
			for (auto s : g_ClientStates)
			{
				switch (s)
				{
				case ClientState::Login:
					loginState->UpdateData(deltaTime, nullptr);
					break;
				case ClientState::Game:
					gameState->UpdateData(deltaTime, nullptr);
					break;
				
				}
			}
			continue;
		}


		//Ŭ�� ���� �����͸� ������.
		IData* data = g_DataQueue.front();	
		g_DataQueue.pop();

		//������ ����Ѵ�.
		
		switch (g_ClientStates[data->ID])
		{
		case ClientState::Login:
			loginState->UpdateData(deltaTime, data);
			break;
		case ClientState::Game:
			g_ProcessedData[data->ID] = gameState->UpdateData(deltaTime, data);
			break;
		default:
			break;
		}

		//���ο� �����͸� �ִ´�.
		SetEvent(g_ClientEvents[data->ID]);
	}

}

DWORD ListeningThreadProc(LPVOID)
{
	static int clientCount = 0;

	Network* network = Network::GetInstance();
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);


	network->BindAndListen(listeningSocket);

	while (clientCount <= 4)
	{
		SOCKET clientSocket = network->Accept(listeningSocket);
		printf("%d ��° Ŭ���̾�Ʈ ����!", clientCount);

		ClientInformation information = { clientCount++, clientSocket };

		// CLIENT ���� �����带 �������
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
	
	while (true) 
	{
		int recvBufferSize = 0;
		int sendBufferSize = 0;
		IData* recvBuffer = nullptr;
		switch (g_ClientStates[clientInformation.ID])
		{
		case ClientState::Login:
			recvBufferSize = sizeof(ClientToServerInLogin);
			recvBuffer = new ClientToServerInLogin();
			sendBufferSize = sizeof(ServerToClientInLogin);
			break;
		case ClientState::Lobby:
			recvBufferSize = sizeof(ClientToServerInLobby);
			recvBuffer = new ClientToServerInLobby();
			sendBufferSize = sizeof(ServerToClientInLobby);
			break;
		case ClientState::Game:
			recvBufferSize = sizeof(ClientToServerInGame);
			recvBuffer = new ClientToServerInGame();
			sendBufferSize = sizeof(ServerToClientInGame);
			break;		
		default:
			break;
		}
		
		if (recvBuffer)
		{
			// Ŭ���̾�Ʈ�κ��� �����͸� ����.
			network->Recv(clientInformation.Socket, (char*)recvBuffer, recvBufferSize);
			if (network->retval == SOCKET_ERROR) {
				network->ErrDisplay(L"recv()");
				break;
			}
			recvBuffer->ID = clientInformation.ID;

			if (network->retval == 0)
				break;

			// �����͸� Queue�� ����.
			g_DataQueue.push(recvBuffer);


			// ������ ���� ������ ��ٸ�.
			if (g_ClientEvents[recvBuffer->ID])
				WaitForSingleObject(g_ClientEvents[recvBuffer->ID], INFINITE);

			// ���� �����͸� ����
			if (g_ProcessedData[recvBuffer->ID])
			{			
				network->Send(clientInformation.Socket, (char*)g_ProcessedData[recvBuffer->ID], sendBufferSize);
				delete g_ProcessedData[recvBuffer->ID];
				g_ProcessedData[recvBuffer->ID] = nullptr;
			}
		}
		
	}

	return 0;
}
