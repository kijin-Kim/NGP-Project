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
	//MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);
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

void Network::Connect(const char* address)
{
	ZeroMemory(&m_ServerAddr, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = inet_addr(address);
	m_ServerAddr.sin_port = htons(SERVERPORT);

	retval = connect(m_Sock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr));
	if (retval == SOCKET_ERROR)	ErrQuit(L"connect error()");
}

void Network::ClientInfo()
{
	addrlen = sizeof(m_ClientAddr);
	getpeername(m_ClientSock, (SOCKADDR*)&m_ClientAddr, &addrlen);
}

void Network::BindAndListen()
{
	ZeroMemory(&m_ServerAddr, sizeof(m_ServerAddr));
	m_ServerAddr.sin_family = AF_INET;
	m_ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServerAddr.sin_port = htons(SERVERPORT);

	retval = bind(m_Sock, (SOCKADDR*)&m_ServerAddr, sizeof(m_ServerAddr));
	if (retval == SOCKET_ERROR) ErrQuit(L"bind error()");

	retval = listen(m_Sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) ErrQuit(L"listen error()");
}

void Network::Accept()
{
	addrlen = sizeof(m_ClientAddr);
	m_ClientSock = accept(m_Sock, (SOCKADDR*)&m_ClientAddr, &addrlen);
	if (m_ClientSock == INVALID_SOCKET) ErrDisplay(L"accept error()");
}

void Network::Send(char* buf, int dataSize)
{
	if (isServer == true)
		retval = send(m_ClientSock, buf, dataSize, 0);

	if (isServer == false)
		retval = send(m_Sock, buf, dataSize, 0);
}

void Network::Recv(char* buf, int dataSize)
{
	if (isServer == true)
		retval = Recvn(m_ClientSock, buf, dataSize, 0);

	if (isServer == false)
		retval = Recvn(m_Sock, buf, dataSize, 0);
}

void Network::Release(SOCKET sock)
{
	closesocket(sock);
	WSACleanup();
}
