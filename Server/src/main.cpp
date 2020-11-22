#include "Network/Network.h"

int main()
{
	char buf[BUFSIZE + 1];
	Network* network = Network::GetInstance();
	network->isServer = true;
	network->BindAndListen();
	
	while (1)
	{
		network->Accept();
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(network->m_ClientAddr.sin_addr), ntohs(network->m_ClientAddr.sin_port));
		network->ClientInfo();
		while (1)
		{
			network->ServerRecv(buf);
			if (network->retval == SOCKET_ERROR) {
				network->ErrDisplay(L"recv()");
				break;
			}
			else if (network->retval == 0)
				break;
			buf[network->retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(network->m_ClientAddr.sin_addr),
				ntohs(network->m_ClientAddr.sin_port), buf);
			printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", network->retval);
			//
			

			network->ServerSend(buf);
			if (network->retval == SOCKET_ERROR) {
				network->ErrDisplay(L"send()");
				break;
			}
		}

		closesocket(network->m_ClientSock);
		
	}

	network->Release(network->m_Sock);
}
