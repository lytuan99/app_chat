#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")
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
#define INPUT_SIZE 256

vector<string> listFriend;
vector<string> listTeam;

bool checkIsExistedName(char *username);

void getNotification(char * recvBuff, char notification[]) {

	notification[0] = recvBuff[0];
	notification[1] = recvBuff[1];
	notification[2] = 0;
}

char loginResponse(char * recvBuff) {
	if (recvBuff[2] == SUCCESS){
		return SUCCESS;
	}
	else if (recvBuff[2] == LOGGED_IN) {
		return LOGGED_IN;
	}
	else {
		return '9';
	}
}

bool logoutResponse(char*recvBuff) {
	if (recvBuff[2] == SUCCESS) {
		return true;
	}
	else //khong dang xuat dc
		std::cout << "WRONG ";

	return true;
}
bool signUpResponse(char *recvBuff) {
	if (recvBuff[2] == SUCCESS) // dang ky thanh cong 
		return true;
	else //khong dang ki dc
	return false;
}
bool receiveMessageFromFriend(char *recvBuff, HWND hwnd) {
	//format : [notification][username]|[messs]
	//get username;
	int x = 1;
	int i = 0;
	while (recvBuff[x] != '|') {
		x++;
	};
	//get message
	x++;
	i = 0;
	char message[INPUT_SIZE];
	while (recvBuff[x] != 0) {
		message[i] = recvBuff[x];
		x++;
		i++;
	}
	message[i] = 0;
	string messDisplay;
	messDisplay.append(message);
	messDisplay.append("\r\n");
	SendDlgItemMessage(hwnd, CHAT_BOX, EM_REPLACESEL, 0, (LPARAM)message);
	return true;
}
bool receiveMessageFromTeam(char*recvBuff) {
	//format : [notification][team id]|[username]|[message]
	//get team id
	char teamId[10];
	int x = 1;
	int index = 0;
	while (recvBuff[x] != '|') {
		teamId[index] = recvBuff[x];
		index++;
		x++;
	}
	teamId[index] = 0;
	//---------------------------------------------
	//get username
	char username[128];
	x++;
	index = 0;
	while (recvBuff[x] != '|') {
		username[index] = recvBuff[x];
		index++;
		x++;
	}
	username[index] = 0;
	//---------------------------------------------------
	//get message
	char message[INPUT_SIZE];
	x++;
	index = 0;
	while (recvBuff[x] != 0) {
		message[index] = recvBuff[x];
		index++;
		x++;
	}
	message[index] = 0;
	//cho nay in ra cai tin nhan tu thang username o team teamid
	return true;
}


bool makeTeamResponse(char* recvBuff) {
		int x = 2;
		char groupname[BUFF_SIZE];
		int i = 0;
	while (recvBuff[x] != '\0') {
			groupname[i] = recvBuff[x];
			x++;
			i++;
	}
	groupname[i] = 0;
	MessageBox(NULL, groupname, "test", MB_OK);
	SendMessage(hListGroups, LB_ADDSTRING, 0, (LPARAM)groupname);
	return true;
}

void getListFriendResponse(char* recvBuff) {

	//format : [notification][numofUser][username1]|[username2]
	// copy list friend from recevBUFF to list
	int x = 4;
	listFriend.clear();
	while (recvBuff[x] != '\0') {
		char username[BUFF_SIZE];
		//get username
		int i = 0;
		while (recvBuff[x] != '|') {
			username[i] = recvBuff[x];
			x++;
			i++;
		}
		username[i] = 0;
		x++;
		listFriend.push_back(username);
	}
	
	SendMessage(hListFriends, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < listFriend.size(); i++)
	{
		/*int pos = (int)SendMessage(hListFriends, LB_ADDSTRING, 0,
			(LPARAM)listFriend[i].c_str());
		SendMessage(hListFriends, LB_SETITEMDATA, pos, (LPARAM)listFriend[i].c_str());*/
		SendMessage(hListFriends, LB_ADDSTRING, 0, (LPARAM)listFriend[i].c_str());
	}
}

bool getListTeamResponse(char* recvBuff) {
	//format : [notification][numofTeam][team1]|[team2]
	MessageBox(NULL, recvBuff, "list team ne", MB_OK);
	listTeam.clear();
	// copy list team from recevBUFF to list
	int x = 4;

	while (recvBuff[x] != NULL) {
		//get team name
		string teamName;
		int i = 0;
		while (recvBuff[x] != '|') {
			teamName[i] = recvBuff[x];
			x++;
			i++;
		}
		x++;
		listTeam.push_back(teamName);
	}
	SendMessage(hListGroups, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < listTeam.size(); i++)
		SendMessage(hListGroups, LB_ADDSTRING, 0, (LPARAM)listTeam[i].c_str());
	return true;
}

bool checkIsExistedName(char *username) {
	for (int i = 0; i < listFriend.size(); i++) {
		if (listFriend[i].compare(username) == 0)
			return true;
	}
	return false;
}

bool checkUserInListFriend(string nameUser) {
	for (int i = 0; i < listFriend.size(); i++) {
		if (listFriend[i].compare(nameUser) == 0)
			return true;
	}
	return false;
}


