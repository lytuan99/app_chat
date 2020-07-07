#include"Client.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

#pragma warning(disable:4996)
Client::Client(int PORT)
{
	
	WSADATA wsaData;

	// Validate the parameters

	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		exit(0);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", "5500", &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}

	clientPtr = this;
}
SOCKET Client::ServerConnection;

	
bool Client::Connect()
{
	bool connected = false;

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{

		// Create a SOCKET for connecting to server
		ServerConnection = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ServerConnection == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(ServerConnection, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "Could not connect to server, error: " << WSAGetLastError() << std::endl;
			closesocket(ServerConnection);
			ServerConnection = INVALID_SOCKET;
			continue;
		}
		else
		{
			connected = true;
			break;

		}
	}

	return connected;
}


int Client::getListFriendRequest() {
	// format of message:    [NOTIFICATION]
	int ret;
	char buff[BUFF_SIZE];
	strcpy(buff, GET_LIST_FRIEND_NOTIFICATION);
	buff[strlen(buff)] = 0;
	// send to server
	ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return 1;
	}
	return 0;

}

/*send login request*/
int Client::loginRequest(char * username, char*password) {

	// format of message:    [NOTIFICATION][username]|[password]
	int ret;
	char buff[BUFF_SIZE];
	strcpy(buff, LOGIN_NOTIFICATION);
	strcat(buff, username);
	strcat(buff, "|");
	strcat(buff, password);
	buff[strlen(buff)] = 0;
	ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return 1;
	}
	return 0;
}

/*send logout request*/
int Client::logoutRequest() {

	char buff[BUFF_SIZE];
	// format of message:    [NOTIFICATION][loginStatus]
	strcpy(buff, LOGOUT_NOTIFICATION);
	buff[2] = 0;
	int ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return 1;
	}
	return 0;
}

int Client::sendToFriend(char*friendID, char* message) {
	// format of message:    [NOTIFICATION][FriendID]|[Message]
	int ret;
	char buff[BUFF_SIZE];
	strcpy(buff, SEND_TO_FRIEND_NOTIFICATION);
	strcat(buff, friendID);
	strcat(buff, "|");
	strcat(buff, message);
	buff[strlen(buff)] = 0;

	// send to server
	ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return -1;
	}
	return 0;
}




int Client::signUpRequest(char * username, char*password) {
	// format of message:    [NOTIFICATION][username]|[password]
	int ret;
	char buff[BUFF_SIZE];
	strcpy(buff, SIGNUP_NOTIFICATION);
	strcat(buff, username);
	strcat(buff, "|");
	strcat(buff, password);
	buff[strlen(buff)] = 0;

	// send to server
	ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return 1;
	}
	return 0;

}

bool Client::CloseConnection()
{
	if (closesocket(ServerConnection == SOCKET_ERROR))
	{
		if (WSAGetLastError() == WSAENOTSOCK)
			return true;

		std::cout << "Failed to close the socket. Winsock Error: " << WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

int Client::requestMakeTeam(int numOfMember, char* teamName, std::vector<std::string> listFriend) {
	// format of message:    [NOTIFICATION][numOfMember][teamName]|[listFriend]
	int ret;
	char buff[BUFF_SIZE];
	strcpy(buff, MAKE_TEAM_NOTIFICATION);
	char c_numOfMember[5];
	itoa(numOfMember, c_numOfMember, 10);
	strcat(buff, c_numOfMember);
	strcat(buff, "|");
	strcat(buff, teamName);
	strcat(buff, "|");
	for (int i = 0; i < numOfMember; i++) {
		strcat(buff, listFriend[i].c_str());
		strcat(buff, "|");
	}
	buff[strlen(buff)] = 0;
	// send to server
	ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return 1;
	}
	return 0;
}

int Client::getListTeamRequest() {
	int ret;
	char buff[BUFF_SIZE];
	strcpy(buff, GET_LIST_TEAM_NOTIFICATION);
	buff[strlen(buff)] = 0;

	// send to server
	ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return -1;
	}
	return 0;
}
