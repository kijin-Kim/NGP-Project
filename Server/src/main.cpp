#include "Network/Network.h"
#include "Network/Data.h"
#define MAX_USER 4

DWORD ListeningThread(LPVOID);
DWORD ProcessRecvThread(LPVOID arg);

int ClientCount = 0;
HANDLE m_hClientsThreads[MAX_USER];// clients threads


int main()
{
	char buf[BUFSIZE + 1];
	Network* network = Network::GetInstance();

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		network->ErrQuit(L"socket()");

	network->isServer = true;
	network->BindAndListen(sock);
	HANDLE hThread;

	while (1) {
		
		hThread = CreateThread(NULL, 0, ListeningThread,
			(LPVOID)sock, 0, NULL);
		if (hThread == NULL) {
			closesocket(sock);
		}
		else {
			CloseHandle(hThread);
		}

	}
	return 1;
}

DWORD ListeningThread(LPVOID arg)
{
	Network* network = Network::GetInstance();
	SOCKET sock = (SOCKET)arg;
	SOCKET clientSock;
	char buf[BUFSIZE + 1];
	int id = 0;

	while (1)
	{
		

		++ClientCount;
		

		// ID ����
		//network->Send((char*)&id, sizeof(int));

		m_hClientsThreads[id]= CreateThread(NULL, 0, ProcessRecvThread,
			(LPVOID)clientSock,0,NULL);
	
		if (NULL == m_hClientsThreads[id]) 
		{
			closesocket(clientSock);
		}
	}

	network->Release(clientSock);
}

DWORD ProcessRecvThread(LPVOID arg)
{
	Network* network = Network::GetInstance();
	SOCKET clientSock = (SOCKET)arg;
	char buf[BUFSIZE + 1];
	ServerToClientInGame p1;

	int id = reinterpret_cast<int>(arg);
	printf("TCP ����, ID : %d\n", id);

	//Ŭ���̾�Ʈ ���� ����

	while (1)
	{
		network->Recv(clientSock, buf, BUFSIZE);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"recv()");
			break;
		}
		else if (network->retval == 0)
			break;
		p1 = *(ServerToClientInGame*)buf;
		printf("%d\n", p1.AnimationData[0].SpriteIndex);
		printf("%d\n", p1.Scores[0]);
	

		buf[network->retval] = '\0';
		
		
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", network->retval);
		
		network->Send(clientSock, buf, BUFSIZE);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"send()");
			break;
		}
	}
	
	return 1;
}