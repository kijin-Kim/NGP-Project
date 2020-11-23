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

#include<Network/Data.h>



#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAXCLIENT 4

using namespace std;

//Recv 관련
HANDLE recvEvents[5] = {};//Recv 관련 Event 객체 핸들
HANDLE sendEvents[5];//Send 관련 Event 객체 핸들
queue<EVENT> eventQueues[5];//클라의 이벤트를 받는 큐
int queueSizes[5];//각 큐에 현재 삽입이 완료된 이벤트의 개수
int curClientNumber = 0;//현재 접속한 클라의 숫자
DWORD clientNoEventTime[5];

//Send관련
char buffer[100000];//서버에서 딱 하나만 사용하는 전송용 버퍼
int bufOffset = 0;//버퍼에 넣은 바이트 수
SOCKET client_socket[5];
queue<char*> sendQueues[5];//보내는 큐
int sendQueueSizes[5];

DWORD WINAPI LogicThread(LPVOID arg);


struct EVENT
{
	// 이벤트 비트플래그
	unsigned short eventFlag;
};


struct CLIENT_SOCKET
{
	int clientNum = 0;
	SOCKET s;
};


// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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


void InitServer()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	int retval = 0;

	ZeroMemory(sendEvents, sizeof(HANDLE) * 5);
	ZeroMemory(sendQueueSizes, sizeof(int) * 5);


	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	// bind()

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	HANDLE hThread = NULL;
	int addrlen;

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)err_quit("listen()");

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		CLIENT_SOCKET clientSock;
		clientSock.s = client_sock;
		clientSock.clientNum = curClientNumber;
		
		if (hThread == NULL) { closesocket(client_sock); }
		else
		{
			if (curClientNumber == 0)
			{
				//첫 클라이언트 접속 시 로직 쓰레드 한번만 생성
				HANDLE logicThread = CreateThread(NULL, 0, LogicThread, (LPVOID)client_sock, 0, NULL);
				if (logicThread)
					CloseHandle(logicThread);
			}
		}
		recvEvents[curClientNumber] = CreateEvent(NULL, FALSE, FALSE, NULL);
		sendEvents[curClientNumber] = CreateEvent(NULL, FALSE, FALSE, NULL);

		client_socket[curClientNumber] = client_sock;
		//클라이언트 접속 성공
		curClientNumber++;
		CloseHandle(hThread);

	}
}

//기능별로 나누고 싶어서 RecvThread 생성. 
DWORD __stdcall RecvThread(LPVOID arg)
{
	CLIENT_SOCKET sockStruct = *((CLIENT_SOCKET*)arg);
	SOCKET client_sock = sockStruct.s;
	int myNumber = sockStruct.clientNum;

	SOCKADDR_IN address;
	int addrlen = sizeof(address);

	getpeername(client_sock, (SOCKADDR*)&address, &addrlen);

	EVENT event;

	cout << "내 클라이언트 번호 : " << myNumber << endl;


	//// 클라이언트와 전송
	while (1)
	{
		recvn(client_sock, (char*)(&event), sizeof(EVENT), 0);

		eventQueues[myNumber].push(event);
		queueSizes[myNumber]++;

		// SetEvent하기
		SetEvent(recvEvents[myNumber]);
	}

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(address.sin_addr), ntohs(address.sin_port));
	cout << "------------------------------------------------------------------" << endl;

	return 0;
}


DWORD LogicThread(LPVOID arg)
{
	
	while (1)
	{
		//WaitForMultiple
		WaitForMultipleObjects(curClientNumber, recvEvents, TRUE, 10);

		for (int i = 0; i < curClientNumber; ++i)
		{
			if (queueSizes[i] && !eventQueues[i].empty()) 
			{//큐에 어떤 이벤트가 삽인된게 하나라도 있다면
				EVENT curEvent = eventQueues[i].front();//이벤트를 먼저 받아놓고 다음 줄에서 pop
				eventQueues[i].pop();

				//플레이어에 대한 정보를 받아와야함.
				//플레이어 위치,상태(점프,,)

				queueSizes[i]--;
				//플레이어마다 Update 진행
			}
		}


		//버퍼를 채운다.
		//플레이어
		for (int i = 0; i < curClientNumber; ++i)
		{
			UserInput userinput;
			userinput.Action;
			userinput.Key;
		}

		//스코어
		for (int i = 0; i < curClientNumber; ++i)
		{

		}

		//버퍼 채우기가 끝나면
		//완료된 버퍼를 보낸다(send)

		for (int i = 0; i < curClientNumber; ++i)
		{
			char* pNew = new char[bufOffset + 8];
			memcpy(pNew, &bufOffset, 4);//버퍼 사이즈 담기
			memcpy(pNew + 4, buffer, bufOffset);//버퍼 내용 담기

			sendQueues[i].push(pNew);
			sendQueueSizes[i]++;
		}

		for (int i = 0; i < curClientNumber; ++i)
		{
			SetEvent(sendEvents[i]);
		}
		ZeroMemory(buffer, 100000);
		bufOffset = 0;

	}

	return 0;
}

DWORD SendThread(LPVOID arg)
{
	CLIENT_SOCKET sockStruct = *((CLIENT_SOCKET*)arg);
	SOCKET client_sock = sockStruct.s;
	int myNumber = sockStruct.clientNum;

	while (1)
	{
		WaitForSingleObject(sendEvents[myNumber], INFINITE);

		if (sendQueueSizes[myNumber] && !sendQueues[myNumber].empty());
		{
			char* pCurBuffer = sendQueues[myNumber].front();
			sendQueues[myNumber].pop();

			int tmpBuffOffset = 0;
			memcpy(&tmpBuffOffset, pCurBuffer, sizeof(int));

			//먼저 offset(버퍼의 찬 Size)를 보낸다.
			send(client_socket[myNumber], (char*)&tmpBuffOffset, 4, 0);

			//클라번호도 보낸다
			send(client_socket[myNumber], (char*)&myNumber, 4, 0);

			//Buffer를 send
			send(client_socket[myNumber], pCurBuffer +4, tmpBuffOffset, 0);

			sendQueueSizes[myNumber]--;
			delete[] pCurBuffer;

		}
	}
	return 0;
}