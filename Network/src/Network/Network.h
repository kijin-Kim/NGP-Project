#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

class Network
{
private:
	Network();
	~Network();

public:
	static Network* GetInstance();
	static void Init();
	static void Release();
	static void Connect();

	void err_quit(const char* msg);
	void err_display(const char* msg);
	int recvn(SOCKET s, char* buf, int len, int flags);

private:
	SOCKET sock = INVALID_SOCKET;
	SOCKADDR_IN serveraddr;
	int addrlen = sizeof(serveraddr);
};

