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
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(network->m_ClientAddr.sin_addr), ntohs(network->m_ClientAddr.sin_port));
	}
	
	network->Release();
}
