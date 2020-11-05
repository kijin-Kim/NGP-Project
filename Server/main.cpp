#include <WinSock2.h>
int main()
{
	{
		// Initialize WinSock
		WSAData wsaData = {};
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			return -1;
		}


		// Create socket
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (clientSocket == INVALID_SOCKET)
		{
			return -1;
		}

	}
}