#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <Windows.h>
#include <iostream>
#include <vector>

#define BUFF_SIZE 2048
#define SUCCESS '1'
#define LOGGED_IN '2'
#define FAILED '0'
#define LOGIN_NOTIFICATION "04"
#define LOGOUT_NOTIFICATION "05"
#define RECEIVE_FROM_FRIEND_NOTIFICATION "07"
#define RECEIVE_FROM_TEAM_NOTIFICATION "08"
#define MAKE_TEAM_NOTIFICATION "06"
#define SEND_TO_FRIEND_NOTIFICATION "07"
#define SEND_TO_TEAM_NOTIFICATION "08"
#define REQUEST_MAKE_TEAM_NOTIFICATION "06"
#define SIGNUP_NOTIFICATION  "03"
#define GET_LIST_FRIEND_NOTIFICATION "11"
#define GET_LIST_TEAM_NOTIFICATION "10"
#define LEAVE_GROUP_NOTIFICATION "09"
#define INPUT_SIZE 256

class Client
{
public:
	Client(int PORT);
	bool Connect();
	inline void StartSubRoutine(HWND hwnd) { clientThread = std::thread(ClientHandler, hwnd); }
	bool DM_Failed;
	static SOCKET ServerConnection;
	int loginRequest(char * username, char*password);
	int signUpRequest(char * username, char*password);
	int logoutRequest();
	int requestMakeTeam(int numOfMember, char* teamName, std::vector<std::string> listFriend);
	int getListTeamRequest();
	int sendToFriend(char*friendID, char* message);
	int sendToTeam(char* teamID, char* message);
	int getListFriendRequest();
	int leaveTeamRequest(char* teamName);
	static void ClientHandler(HWND hwnd);
	bool CloseConnection();

	int iResult;

	addrinfo *result = NULL;
	addrinfo *ptr = NULL;
	addrinfo hints;
	std::thread clientThread;
};

static Client* clientPtr;
