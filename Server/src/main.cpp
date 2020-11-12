#include "Network/Network.h"

int main()
{
	int retval=0;

	Network* network = Network::GetInstance();
	network->isServer = true;
	network->BindAndListen(retval);

	network->Accept();
	network->Release();
}
