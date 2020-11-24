#pragma once
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
<<<<<<< Updated upstream
=======
#define BUFSIZE    512
>>>>>>> Stashed changes

class Network
{
public:
	static Network* GetInstance();
	void Connect();
	void Release();
	void ErrQuit(const wchar_t* msg);
	void ErrDisplay(const wchar_t* msg);
	int Recvn(SOCKET s, char* buf, int len, int flags);

private:
	Network();
	~Network();

private:
	SOCKET m_Sock;
	SOCKADDR_IN m_ServerAddr;

};

