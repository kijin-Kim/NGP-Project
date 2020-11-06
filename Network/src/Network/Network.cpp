#include "Network.h"

Network* p_Network = nullptr;

Network::Network()
{
}

Network::~Network()
{
}

Network* Network::GetInstance()
{
	if (p_Network == nullptr)
	{
		p_Network = new Network;
	}
	return p_Network;
}

void Network::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Network::err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int Network::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;	

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

void Network::Init()
{
	/*WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		p_Network->err_quit("WSAStartup() Error");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) 
		p_Network->err_quit("socket()");*/

	ZeroMemory(&p_Network->serveraddr, sizeof(serveraddr));
	p_Network->serveraddr.sin_family = AF_INET;
	p_Network->serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	p_Network->serveraddr.sin_port = htons(SERVERPORT);
}

void Network::Release(SOCKET sock)
{
	closesocket(sock);
	WSACleanup();
}

void Network::Connect(SOCKET sock)
{
	int retval = connect(sock, (SOCKADDR*)&p_Network->serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)	p_Network->err_quit("connect()");
}
