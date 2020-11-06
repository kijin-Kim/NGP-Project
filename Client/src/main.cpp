#include "Game.h"
#include "Network/Network.h"

Network* network = nullptr;

int main(void)
{
	Game* game = new Game(640, 480);
	game->Run();

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		network->err_quit("WSAStartup() Error");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		network->err_quit("socket()");

	network->GetInstance();
	network->Init();
	network->Connect(sock);

	network->Release(sock);

	delete game;

}