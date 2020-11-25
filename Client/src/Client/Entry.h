#pragma once
#include "Game.h"
#include "Network/Network.h"
#include "Network/Data.h"

extern Game* CreateGameApplication();

int main(void)
{
	auto game = CreateGameApplication();
	game->Run();
	delete game;

	ServerToClientInGame p1;
	for (int i = 0; i < 5; i++)
	{
		p1.ObjectPositions[i] = { (float)i,(float)i };
		p1.AnimationData[i].SpriteIndex = i + 1;
		p1.AnimationData[i].Status = i + 1;
	}

	p1.Scores[0] = 100;
	p1.Scores[1] = 200;
	
	const char* address = "127.0.0.1";
	Network* network = Network::GetInstance();
	network->isServer = false;
	network->Connect(address);
	char buf[BUFSIZE + 1];	
	
	for (int i = 0; i < 4; i++)
	{
	/*	printf("입력: ");
		scanf("%s", &testdata);*/

		memset(buf, ' ', sizeof(buf));

		network->Send((char*)&p1, BUFSIZE);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"send()");
			break;
		}

		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", network->retval);

		network->Recv(buf, BUFSIZE);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"recv()");
			break;
		}
		else if (network->retval == 0)
			break;

		buf[network->retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", network->retval);
		printf("[받은 데이터] %s\n", buf);
	}
	network->Release(network->m_Sock);
	//delete game;
}


