#include "Network/Network.h"

int main()
{
	int retval=0;

	Network* network = Network::GetInstance();
	network->isServer = true;
	network->BindAndListen(retval);

	while (1)
	{
		network->Accept();
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(network->m_ClientAddr.sin_addr), ntohs(network->m_ClientAddr.sin_port));
	}
	
	network->Release();
}
