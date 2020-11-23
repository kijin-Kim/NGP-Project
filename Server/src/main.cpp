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

//Recv ����
HANDLE recvEvents[5] = {};//Recv ���� Event ��ü �ڵ�
HANDLE sendEvents[5];//Send ���� Event ��ü �ڵ�
queue<EVENT> eventQueues[5];//Ŭ���� �̺�Ʈ�� �޴� ť
int queueSizes[5];//�� ť�� ���� ������ �Ϸ�� �̺�Ʈ�� ����
int curClientNumber = 0;//���� ������ Ŭ���� ����
DWORD clientNoEventTime[5];

//Send����
char buffer[100000];//�������� �� �ϳ��� ����ϴ� ���ۿ� ����
int bufOffset = 0;//���ۿ� ���� ����Ʈ ��
SOCKET client_socket[5];
queue<char*> sendQueues[5];//������ ť
int sendQueueSizes[5];

DWORD WINAPI LogicThread(LPVOID arg);


struct EVENT
{
	// �̺�Ʈ ��Ʈ�÷���
	unsigned short eventFlag;
};


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

	// ������ ��ſ� ����� ����
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

		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		CLIENT_SOCKET clientSock;
		clientSock.s = client_sock;
		clientSock.clientNum = curClientNumber;
		
		if (hThread == NULL) { closesocket(client_sock); }
		else
		{
			if (curClientNumber == 0)
			{
				//ù Ŭ���̾�Ʈ ���� �� ���� ������ �ѹ��� ����
				HANDLE logicThread = CreateThread(NULL, 0, LogicThread, (LPVOID)client_sock, 0, NULL);
				if (logicThread)
					CloseHandle(logicThread);
			}
		}
		recvEvents[curClientNumber] = CreateEvent(NULL, FALSE, FALSE, NULL);
		sendEvents[curClientNumber] = CreateEvent(NULL, FALSE, FALSE, NULL);

		client_socket[curClientNumber] = client_sock;
		//Ŭ���̾�Ʈ ���� ����
		curClientNumber++;
		CloseHandle(hThread);

	}
}

//��ɺ��� ������ �; RecvThread ����. 
DWORD __stdcall RecvThread(LPVOID arg)
{
	CLIENT_SOCKET sockStruct = *((CLIENT_SOCKET*)arg);
	SOCKET client_sock = sockStruct.s;
	int myNumber = sockStruct.clientNum;

	SOCKADDR_IN address;
	int addrlen = sizeof(address);

	getpeername(client_sock, (SOCKADDR*)&address, &addrlen);

	EVENT event;

	cout << "�� Ŭ���̾�Ʈ ��ȣ : " << myNumber << endl;


	//// Ŭ���̾�Ʈ�� ����
	while (1)
	{
		recvn(client_sock, (char*)(&event), sizeof(EVENT), 0);

		eventQueues[myNumber].push(event);
		queueSizes[myNumber]++;

		// SetEvent�ϱ�
		SetEvent(recvEvents[myNumber]);
	}

	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
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
			{//ť�� � �̺�Ʈ�� ���εȰ� �ϳ��� �ִٸ�
				EVENT curEvent = eventQueues[i].front();//�̺�Ʈ�� ���� �޾Ƴ��� ���� �ٿ��� pop
				eventQueues[i].pop();

				//�÷��̾ ���� ������ �޾ƿ;���.
				//�÷��̾� ��ġ,����(����,,)

				queueSizes[i]--;
				//�÷��̾�� Update ����
			}
		}


		//���۸� ä���.
		//�÷��̾�
		for (int i = 0; i < curClientNumber; ++i)
		{
			UserInput userinput;
			userinput.Action;
			userinput.Key;
		}

		//���ھ�
		for (int i = 0; i < curClientNumber; ++i)
		{

		}

		//���� ä��Ⱑ ������
		//�Ϸ�� ���۸� ������(send)

		for (int i = 0; i < curClientNumber; ++i)
		{
			char* pNew = new char[bufOffset + 8];
			memcpy(pNew, &bufOffset, 4);//���� ������ ���
			memcpy(pNew + 4, buffer, bufOffset);//���� ���� ���

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

			//���� offset(������ �� Size)�� ������.
			send(client_socket[myNumber], (char*)&tmpBuffOffset, 4, 0);

			//Ŭ���ȣ�� ������
			send(client_socket[myNumber], (char*)&myNumber, 4, 0);

			//Buffer�� send
			send(client_socket[myNumber], pCurBuffer +4, tmpBuffOffset, 0);

			sendQueueSizes[myNumber]--;
			delete[] pCurBuffer;

		}
	}
	return 0;
}