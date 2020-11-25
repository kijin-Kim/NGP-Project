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
	network->isServer = true;
	network->BindAndListen();
	HANDLE hThread;

	while (1) {
		hThread = CreateThread(NULL, 0, ListeningThread,
			(LPVOID)network->m_ClientSock, 0, NULL);
		if (hThread == NULL) {
			closesocket(network->m_ClientSock);
		}
		else {
			CloseHandle(hThread);
		}
		//Logic 
	}
	return 1;
}

DWORD ListeningThread(LPVOID)
{
	Network* network = Network::GetInstance();
	char buf[BUFSIZE + 1];

	int id = ClientCount;

	while (1)
	{
		network->Accept();
		++ClientCount;
		printf("LT_[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d,Ŭ���̾�Ʈ �ѹ�=%d\n",
			inet_ntoa(network->m_ClientAddr.sin_addr),
			ntohs(network->m_ClientAddr.sin_port),
			ClientCount);
		network->ClientInfo();

		// ID ����

		int retval = send(network->m_ClientSock, (char*)&id, sizeof(int), 0);
		if (SOCKET_ERROR == retval)network->ErrQuit(L"send ID error");

		m_hClientsThreads[id]= CreateThread(NULL, 0, ProcessRecvThread,
			(LPVOID)network->m_ClientSock,0,NULL);
	
		if (NULL == m_hClientsThreads[id]) 
		{
			closesocket(network->m_ClientSock);
		}

	}

	network->Release(network->m_Sock);
}


DWORD CommunicationThread(LPVOID arg)
{
	Network* network = Network::GetInstance();
	char buf[BUFSIZE + 1];

	int id = reinterpret_cast<int>(arg);
	printf("CT_TCP ����, ID : %d\n", id);

	SOCKADDR_IN clientAddr;
	int addrlen = sizeof(clientAddr);
	int retval;



	return 0;
}
DWORD ProcessRecvThread(LPVOID arg)
{
	Network* network = Network::GetInstance();
	char buf[BUFSIZE + 1];
	ServerToClientInGame p1;

	int id = reinterpret_cast<int>(arg);
	printf("TCP ����, ID : %d\n", id);

	//Ŭ���̾�Ʈ ���� ����
	network->ClientInfo();
	while (1)
	{
		network->Recv(buf, BUFSIZE);
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
		printf("[TCP/%s:%d] %s\n", inet_ntoa(network->m_ClientAddr.sin_addr),
		ntohs(network->m_ClientAddr.sin_port), buf);
		
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", network->retval);
		
		network->Send(buf, BUFSIZE);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"send()");
			break;
		}
	}
	
	return 1;

}