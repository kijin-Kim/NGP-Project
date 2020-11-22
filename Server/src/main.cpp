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
		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
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
			printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", network->retval);
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
