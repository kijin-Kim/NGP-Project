#include "Game.h"

int main(void)
{
	Game* game = new Game(640, 480);
	game->Run();
	delete game;
}