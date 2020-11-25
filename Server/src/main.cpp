#include "Network/Network.h"

#define MAX_USER 4
DWORD ListeningThread(LPVOID);
DWORD CommunicationThread(LPVOID arg);

int ClientCount = 0;
HANDLE m_hClientsThreads;// clients threads
int recvn(SOCKET s, char* buf, int len, int flags);

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;
	//
	int percentrec = 0;
	int leng = len;
	float percent = 0;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		//
		percentrec += received;
		//
		left -= received;
		ptr += received;
		percent = ((float)percentrec / (float)leng) * 100;
	}

	return (len - left);//읽은 byte
}


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
		printf("LT_[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d,클라이언트 넘버=%d\n",
			inet_ntoa(network->m_ClientAddr.sin_addr),
			ntohs(network->m_ClientAddr.sin_port),
			ClientCount);
		network->ClientInfo();

		// ID 전송
		int retval = send(network->m_ClientSock, (char*)&id, sizeof(int), 0);
		if (SOCKET_ERROR == retval)network->ErrQuit(L"send ID error");

		m_hClientsThreads = CreateThread(NULL, 0, CommunicationThread,
			(LPVOID)network->m_ClientSock, 0, NULL);

		if (NULL == m_hClientsThreads)
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
	printf("CT_TCP 접속, ID : %d\n", id);

	SOCKADDR_IN clientAddr;
	int addrlen = sizeof(clientAddr);
	int retval;



	return 0;
}