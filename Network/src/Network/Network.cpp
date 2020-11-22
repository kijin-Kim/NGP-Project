#include "Network.h"

Network::Network()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		ErrQuit(L"WSAStartup() Error");

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Sock == INVALID_SOCKET)
		ErrQuit(L"socket()");

	ZeroMemory(&m_ServerAddr, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	if (isServer == false)
		m_ServerAddr.sin_addr.s_addr = inet_addr(SERVERIP);
	if (isServer == true)
		m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServerAddr.sin_port = htons(SERVERPORT);
}

Network::~Network()
{
}

Network* Network::GetInstance()
{
	static Network Instance;
	return &Instance;
}

void Network::ErrQuit(const wchar_t* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void Network::ErrDisplay(const wchar_t* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int Network::Recvn(SOCKET s, char* buf, int len, int flags)
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


void Network::Connect()
{
	retval = connect(m_Sock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr));
	if (retval == SOCKET_ERROR)	ErrQuit(L"connect()");
}

void Network::ClientInfo()
{
	addrlen = sizeof(m_ClientAddr);
	getpeername(m_ClientSock, (SOCKADDR*)&m_ClientAddr, &addrlen);
}

void Network::BindAndListen()
{
	retval = bind(m_Sock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr));
	if (retval == SOCKET_ERROR) ErrQuit(L"bind()");

	retval = listen(m_Sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) ErrQuit(L"listen()");
}

void Network::Accept()
{
	addrlen = sizeof(m_ClientAddr);
	m_ClientSock = accept(m_Sock, (SOCKADDR*)&m_ClientAddr, &addrlen);
	if (m_ClientSock == INVALID_SOCKET) ErrDisplay(L"accept()");
}

void Network::ServerSend(char* buf)
{
	//임시 SendData
	retval = send(m_ClientSock, buf, BUFSIZE, 0);
	/*if (retval == SOCKET_ERROR) {
		ErrDisplay(L"send()");
	}*/
}

void Network::ClientSend(char* buf)
{
	//임시 SendData
	retval = send(m_Sock, buf, BUFSIZE, 0);
}

void Network::ServerRecv(char* buf)
{
	//임시 RecvData
	retval = recv(m_ClientSock, buf, BUFSIZE , 0);
}

void Network::ClientRecv(char* buf)
{
	//임시 RecvData
	retval = Recvn(m_Sock, buf, BUFSIZE, 0);
	/*if (retval == SOCKET_ERROR) {
		ErrDisplay(L"recv fixed()");
		exit(1);
	}
	else if (retval == 0)
		ErrDisplay(L"recv retval = 0 ()");*/
}
void Network::Release(SOCKET sock)
{
	closesocket(sock);
	WSACleanup();
}
