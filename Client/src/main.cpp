#include "Game.h"
#include "Network/Network.h"

int main(void)
{
	Game* game = new Game(640, 480);
	Network* network = Network::GetInstance();
	network->isServer = false;
	network->Connect();

	game->Run();
	

	delete game;
	
	

}