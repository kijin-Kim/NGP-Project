#pragma once
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

class Network
{
public:
	static Network* GetInstance();
	void BindAndListen(int retval);
	void Accept();
	void Connect();
	void Release();
	void SendData(int retval);
	void RecvData(int retval);
	void ErrQuit(const wchar_t* msg);
	void ErrDisplay(const wchar_t* msg);
	int Recvn(SOCKET s, char* buf, int len, int flags);

private:
	Network();
	~Network();

private:
	SOCKET m_Sock, m_ClientSock;
	SOCKADDR_IN m_ServerAddr;
	SOCKADDR_IN m_ClientAddr;
	char buf[BUFSIZE + 1];
	int addrlen;
};

