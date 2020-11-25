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
	/*	printf("�Է�: ");
		scanf("%s", &testdata);*/

		memset(buf, ' ', sizeof(buf));

		network->Send((char*)&p1, BUFSIZE);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"send()");
			break;
		}

		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", network->retval);

		network->Recv(buf, BUFSIZE);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"recv()");
			break;
		}
		else if (network->retval == 0)
			break;

		buf[network->retval] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", network->retval);
		printf("[���� ������] %s\n", buf);
	}
	network->Release(network->m_Sock);
	//delete game;
}


