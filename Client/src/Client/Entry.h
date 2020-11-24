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
	network->isServer = false;
	network->Connect();
	char buf[BUFSIZE + 1];

	const char* testdata[] = {
	   "HELLO",
	   "NICE TO MEET YOU",
	   "I'm sure we have a lot to talk about today.",
	   "So do I.",
	};

	for (int i = 0; i < 4; i++)
	{
	/*	printf("�Է�: ");
		scanf("%s", &testdata);*/

		memset(buf, ' ', sizeof(buf));
		strncpy(buf, testdata[i], strlen(testdata[i]));

		network->Send(buf, BUFSIZE);
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


