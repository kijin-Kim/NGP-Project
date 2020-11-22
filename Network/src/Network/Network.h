#pragma once
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    50

class Network
{
public:
	static Network* GetInstance();
	void BindAndListen();
	void Accept();
	void Connect();
	void ClientInfo();
	void Release(SOCKET sock);
	void ServerSend(char* buf);
	void ClientSend(char* buf);
	void ServerRecv(char* buf);
	void ClientRecv(char* buf);
	void ErrQuit(const wchar_t* msg);
	void ErrDisplay(const wchar_t* msg);
	int Recvn(SOCKET s, char* buf, int len, int flags);

	bool isServer;
private:
	Network();
	~Network();

public:
	SOCKET m_ClientSock;
	SOCKET m_Sock;
	SOCKADDR_IN m_ClientAddr;
	int retval;

private:
	SOCKADDR_IN m_ServerAddr;
	int addrlen;
};

