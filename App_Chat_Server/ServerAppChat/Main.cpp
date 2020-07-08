// ServerAppChat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <process.h>
#include <string>
#include <iostream>
#include <vector>
#include "Server.h"

#pragma comment(lib, "Ws2_32.lib")
using namespace std;


#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048

unsigned __stdcall echoThread(void *param) {
	char buff[BUFF_SIZE];
	int ret;
	SOCKET connectedSocket = (SOCKET)param;
	while (1) {
		int ret = recvFromClient(connectedSocket, buff);
		if (ret < 0) {
			cout << "Socket closed." << WSAGetLastError() << endl;
			handleLogoutClientDisconnect(connectedSocket);
			closesocket(connectedSocket);
			break;
		}
		else {
			buff[ret] = 0;
			ret = classifyRequest(connectedSocket, buff);
			if (ret < 0) {
				printf("Socket closed\n.");
				closesocket(connectedSocket);
				break;
			}
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	DWORD dwRetval;
	if (WSAStartup(wVersion, &wsaData))  printf("Version is not supported\n");

	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		printf("Error! Cannot bind this address.");
		_getch();
		return 0;
	}

	if (listen(listenSock, 10)) {
		printf("Error! cannot listen.");
		_getch();
		return 0;
	}
	printf("Server started!\n");

	sockaddr_in clientAddr;
	int clientAddeLen = sizeof(clientAddr);


	while (1) {
		SOCKET connSock;
		connSock = accept(listenSock, (sockaddr *)&clientAddr, &clientAddeLen);
		cout << "ConnSocket: " << connSock << endl;
		_beginthreadex(0, 0, echoThread, (void *)connSock, 0, 0);
	}
	closesocket(listenSock);
	WSACleanup();
	return 0;
}

