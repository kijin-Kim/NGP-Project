#include "Game.h"
#include "Network/Network.h"

int main(void)
{
	Game* game = new Game(640, 480);
	game->Run();

	delete game;

	Network* network = Network::GetInstance();

	network->Connect();

}