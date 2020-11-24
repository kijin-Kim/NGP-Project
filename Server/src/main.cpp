#include "Network/Network.h"


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

	}
	return 1;
}

DWORD ListeningThread(LPVOID)
{
	Network* network = Network::GetInstance();
	char buf[BUFSIZE + 1];
	int id = 0;

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

DWORD ProcessRecvThread(LPVOID arg)
{
	Network* network = Network::GetInstance();

	int id = reinterpret_cast<int>(arg);
	printf("TCP ����, ID : %d\n", id);

	SOCKADDR_IN clientAddr;
	int addrlen;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientAddr);
	getpeername(network->m_ClientSock,
		(SOCKADDR*)&clientAddr, &addrlen);
	
	return 1;
}