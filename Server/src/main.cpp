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
	network->BindAndListen();
	HANDLE hThread;

	float fTimeElapsed = 0.16f;

	hThread = CreateThread(NULL, 0, ListeningThreadProc,
		(LPVOID)network->m_ClientSock, 0, NULL);
	if (hThread == NULL) {
		closesocket(network->m_ClientSock);
	}
	else {
		CloseHandle(hThread);
	}


	while (true)
	{
		if (g_DataQueue.empty())
		{
			for (auto s : g_ClientStates)
			{
				switch (s)
				{
				case ClientState::Game:
					gameState->UpdateData(fTimeElapsed, nullptr);
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
		case ClientState::Game:
			g_ProcessedData[data->ID] = gameState->UpdateData(fTimeElapsed, data);
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
	static int ClientCount = 0;

	Network* network = Network::GetInstance();
	char buf[BUFSIZE + 1];
	int id = 0;


	while (1)
	{
		network->Accept();
		
		printf("LT_[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d,Ŭ���̾�Ʈ �ѹ�=%d\n",
			inet_ntoa(network->m_ClientAddr.sin_addr),
			ntohs(network->m_ClientAddr.sin_port),
			ClientCount);

		ClientInformation information = { ClientCount++, network->m_ClientSock };

		// CLIENT ���� �����带 �������
		HANDLE hThread = CreateThread(NULL, 0, CommunicationThreadProc, (LPVOID)&information, 0, NULL);
		if (!hThread)
		{
			// SOMETHING
		}

	}

	network->Release(network->m_Sock);
}

DWORD CommunicationThreadProc(LPVOID arg)
{
	Network* network = Network::GetInstance();


	ClientInformation clientInformation = *(ClientInformation*)arg;
	
	g_ClientEvents[clientInformation.ID] = CreateEvent(NULL, FALSE, FALSE, NULL);

	while (1) {

		int recvBufferSize = 0;
		IData* recvBuffer = nullptr;
		switch (g_ClientStates[clientInformation.ID])
		{
		case ClientState::Game:
			recvBufferSize = sizeof(ClientToServerInGame);
			recvBuffer = new ClientToServerInGame();
			break;
		default:
			break;
		}
		// Ŭ���̾�Ʈ�κ��� �����͸� ����.

		if (recvBuffer)
		{
			network->Recv((char*)recvBuffer, recvBufferSize);
			if (network->retval == SOCKET_ERROR) {
				network->ErrDisplay(L"recv()");
				break;
			}
			else if (network->retval == 0)
				break;

			// �����͸� Queue�� ����.
			g_DataQueue.push(recvBuffer);


			// ������ ���� ������ ��ٸ�.
			if (g_ClientEvents[recvBuffer->ID])
				WaitForSingleObject(g_ClientEvents[recvBuffer->ID], INFINITE);

			// ���� �����͸� ����
			if (g_ProcessedData[recvBuffer->ID])
			{
				network->Send((char*)g_ProcessedData[recvBuffer->ID], 108);
				delete g_ProcessedData[recvBuffer->ID];
				g_ProcessedData[recvBuffer->ID] = nullptr;
			}
		}
		
	}


	return 0;
}
