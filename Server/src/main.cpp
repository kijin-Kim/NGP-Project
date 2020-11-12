#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include<WinSock2.h>
#include<iostream>
#include<string>
#include<mutex>
#include<vector>
#include<queue>

#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAXCLIENT 4

using namespace std;


//Recv ����
HANDLE recvEvents[5] = {};//Recv ���� Event ��ü �ڵ�
HANDLE sendEvents[5];//Send ���� Event ��ü �ڵ�
std::queue<EVENT> eventQueues[5];//Ŭ���� �̺�Ʈ�� �޴� ť
int queueSizes[5];//�� ť�� ���� ������ �Ϸ�� �̺�Ʈ�� ����
int curClientNumber = 0;//���� ������ Ŭ���� ����
DWORD clientNoEventTime[5];
DWORD preEventTime[5];


//Send����
char buffer[100000];//�������� �� �ϳ��� ����ϴ� ���ۿ� ����
int bufOffset = 0;//���ۿ� ���� ����Ʈ ��
SOCKET client_socket[5];
std::queue<char*> senfQueues[5];//������ ť
int sendQueueSizes[5];


struct CLIENT_SOCKET
{
	int clientNum = 0;
	SOCKET s;
};


// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(const char* msg)
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

int recvn(SOCKET s, char* buf, int len, int flags)
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

DWORD WINAPI ListeningThread(LPVOID arg);
DWORD WINAPI LogicThread(LPVOID arg);

void InitServer()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	int retval = 0;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	// bind()

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	HANDLE hThread = NULL;
	int addrlen;
	
	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));


		//LogicThread ������ ����
		//hThread = CreateThread(NULL, 0, LogicThread, (LPVOID)&clientsock, 0, NULL);

	}
}


DWORD LogicThread(LPVOID arg)
{
	while (1) {
		//WaitForMultiple
		WaitForMultipleObjects(curClientNumber, recvEvents, TRUE, 10);
		for (int i = 0; i < curClientNumber; ++i) {
			if (queueSizes[i] && !eventQueues[i].empty()) {//ť�� � �̺�Ʈ�� ���εȰ� �ϳ��� �ִٸ�
				EVENT curEvent = eventQueues[i].front();//�̺�Ʈ�� ���� �޾Ƴ��� ���� �ٿ��� pop
				eventQueues[i].pop();
			}
		}

		for (int i = 0; i < curClientNumber; ++i)
		{
		}

		for (int i = 0; i < curClientNumber; ++i)
			SetEvent(sendEvents[i]);

		ZeroMemory(buffer, 100000);
		bufOffset = 0;
	}

	return 0;
}

DWORD SentThread(LPVOID arg)
{
	CLIENT_SOCKET sockStruct = *((CLIENT_SOCKET*)arg);
	SOCKET client_sock = sockStruct s;
	int myNumber = sockStruct.clientNum;

	while (1) 
	{
		WaitForSingleObject(sendEvents[myNumber], INFINITE);
		
		if (sendQueueSizes[myNumber] && !senfQueues[myNumber].empty());
		{
			char* pCurBuffer = senfQueues[myNumber].front();
			senfQueues[myNumber].pop();

			int tmpBuffOffset = 0;
			memcpy(&tmpBuffOffset, pCurBuffer, sizeof(int));

		}
	}
}