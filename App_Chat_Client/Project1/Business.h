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


struct Team
{
	int id;
	string teamName;
};
vector<string> listFriend;
vector<Team> listTeam;



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
	SendDlgItemMessage(hwnd, CHAT_BOX, EM_REPLACESEL, 0, (LPARAM)messDisplay.c_str());
	return true;
}
bool receiveMessageFromTeam(char*recvBuff, HWND hwnd) {
	//format : [notification][team id]|[username]|[message]
	//get team id
	char teamName[10];
	int x = 2;
	int index = 0;
	while (recvBuff[x] != '|') {
		teamName[index] = recvBuff[x];
		index++;
		x++;
	}
	teamName[index] = 0;
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

	string messDisplay;
	messDisplay.append("<");
	messDisplay.append(teamName);
	messDisplay.append(">");
	messDisplay.append(message);
	SendDlgItemMessage(hwnd, CHAT_BOX, EM_REPLACESEL, 0, (LPARAM)messDisplay.c_str());
	return true;
}


bool makeTeamResponse(char* recvBuff) {
	//format   [NOTIFICATION][Team ID][Team Name]
	int x = 2;
	string teamName;
	char c_teamId[128];
	int teamId;
	//get teamId from buff
	int i = 0;
	while (recvBuff[x] != '|') {
		c_teamId[i] = recvBuff[x];
		x++;
		i++;
	}
	x++;
	c_teamId[i] = 0;
	teamId = atoi(c_teamId);
	//get teamName from buff
	while (recvBuff[x] != 0) {
		teamName += recvBuff[x];
		x++;
	}
	//add team to list team
	Team team;
	team.id = teamId;
	team.teamName = teamName;
	listTeam.push_back(team);
	SendMessage(hListGroups, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < listTeam.size(); i++)
	{
		SendMessage(hListGroups, LB_ADDSTRING, 0, (LPARAM)listTeam[i].teamName.c_str());
	}
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
		SendMessage(hListFriends, LB_ADDSTRING, 0, (LPARAM)listFriend[i].c_str());
	}
}


void deleteTeam(int teamId) {
	for (int i = 0; i < listTeam.size(); i++) {
		if (teamId == listTeam[i].id) listTeam.erase(listTeam.begin + i);
	}
}


bool leaveTeamResponse(char * recvBuff) {
	char c_teamId[128];
	int x = 2;
	int index = 0;
	while (recvBuff[x] != 0) {
		c_teamId[index] = recvBuff[x];
		x++;
		index++;
	}
	c_teamId[index] = 0;
	int teamId = atoi(c_teamId);
	deleteTeam(teamId);

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


int findIdTeam(string teamName) {
	for (int i = 0; i < listTeam.size(); i++) {
		if (teamName.compare(listTeam[i].teamName) == 0) return listTeam[i].id;
	}
	return 0;
}

