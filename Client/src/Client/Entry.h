#pragma once
#include "Game.h"
#include "Network/Network.h"


extern Game* CreateGameApplication();

int main(void)
{
	auto game = CreateGameApplication();
	game->Run();
	delete game;
	Network* network = Network::GetInstance();
	network->Connect();

}
