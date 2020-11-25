#pragma once
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <assert.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
<<<<<<< HEAD
#define BUFSIZE    50
=======
#define BUFSIZE    108
>>>>>>> master

class Network
{
public:
	static Network* GetInstance();
	void BindAndListen();
	void Accept();
<<<<<<< HEAD
	void Connect();
=======
	void Connect(const char* address);
>>>>>>> master
	void ClientInfo();
	void Release(SOCKET sock);
	void Send(char* buf, int dataSize);
	void Recv(char* buf, int dataSize);
	void ErrQuit(const wchar_t* msg);
	void ErrDisplay(const wchar_t* msg);
<<<<<<< HEAD

=======
	
>>>>>>> master
private:
	Network();
	~Network();
	int Recvn(SOCKET s, char* buf, int len, int flags);

public:
	SOCKET m_ClientSock;
	SOCKET m_Sock;
	SOCKADDR_IN m_ClientAddr;
	int retval;
	bool isServer;

private:
	SOCKADDR_IN m_ServerAddr;
	int addrlen;
};

