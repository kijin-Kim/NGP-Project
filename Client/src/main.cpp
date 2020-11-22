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
	   "안녕하세요",
	   "반가워요",
	   "오늘따라 할 이야기가 많을 것 같네요",
	   "저도 그렇네요",
	};

	for (int i = 0; i < 4; i++)
	{
	/*	printf("입력: ");
		scanf("%s", &testdata);*/

		memset(buf, ' ', sizeof(buf));
		strncpy(buf, testdata[i], strlen(testdata[i]));

		network->ClientSend(buf);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"send()");
			break;
		}

		
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", network->retval);

		network->ClientRecv(buf);
		if (network->retval == SOCKET_ERROR) {
			network->ErrDisplay(L"recv()");
			break;
		}
		else if (network->retval == 0)
			break;
		//buf[m_RetVal] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", network->retval);
		printf("[받은 데이터] %s\n", buf);
	}
	network->Release(network->m_Sock);
	//delete game;

}