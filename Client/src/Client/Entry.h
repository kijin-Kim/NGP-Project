#pragma once
#include "Game.h"
#include "Network/Network.h"


extern Game* CreateGameApplication();

int main(void)
{
	//Game* game = new Game(432, 304);
	auto game = CreateGameApplication();
	game->Run();

	delete game;

	Network* network = Network::GetInstance();

	network->Connect();

}
