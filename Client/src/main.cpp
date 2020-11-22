#include "Game.h"
#include "Network/Network.h"

int main(void)
{
	//Game* game = new Game(640, 480);
	Network* network = Network::GetInstance();
	network->isServer = false;
	network->Connect();
	char buf[BUFSIZE + 1];

	//game->Run();
	const char* testdata[] = {
	   "�ȳ��ϼ���",
	   "�ݰ�����",
	   "���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
	   "���� �׷��׿�",
	};

	for (int i = 0; i < 4; i++)
	{
	/*	printf("�Է�: ");
		scanf("%s", &testdata);*/

		memset(buf, ' ', sizeof(buf));
		strncpy(buf, testdata[i], strlen(testdata[i]));

		network->ClientSend(buf);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"send()");
			break;
		}

		
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", network->retval);

		network->ClientRecv(buf);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"recv()");
			break;
		}
		else if (network->retval == 0)
			break;
		//buf[m_RetVal] = '\0';
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", network->retval);
		printf("[���� ������] %s\n", buf);
	}
	network->Release(network->m_Sock);
	//delete game;

}