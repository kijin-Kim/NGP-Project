#include "Network/Network.h"

DWORD ListeningThread(LPVOID);

int main()
{
	char buf[BUFSIZE + 1];
	Network* network = Network::GetInstance();
	network->isServer = true;
	network->BindAndListen();
	HANDLE hThread;
	while (1) {
		hThread = CreateThread(NULL, 0, ListeningThread, (LPVOID)network->m_ClientSock, 0, NULL);
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
	while (1)
	{
		network->Accept();
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(network->m_ClientAddr.sin_addr), ntohs(network->m_ClientAddr.sin_port));
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

			buf[network->retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(network->m_ClientAddr.sin_addr),
				ntohs(network->m_ClientAddr.sin_port), buf);

			printf("LT_[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", network->retval);

			network->Send(buf, BUFSIZE);
			if (network->retval == SOCKET_ERROR) {
				network->ErrDisplay(L"send()");
				break;
			}
		}

		closesocket(network->m_ClientSock);

	}

	network->Release(network->m_Sock);
}
