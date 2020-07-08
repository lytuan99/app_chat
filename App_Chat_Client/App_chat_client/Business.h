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
#define SIGNUP_NOTIFICATION  "01"
#define LOGIN_NOTIFICATION "02"
#define LOGOUT_NOTIFICATION "03"
#define CHAT_WITH_FRIEND_NOTIFICATION "04"
#define CHAT_WITH_TEAM_NOTIFICATION "05"
#define MAKE_TEAM_NOTIFICATION "06"
#define LEAVE_TEAM_NOTIFICATION "07"
#define FRIEND_LEAVING_TEAM_NOTIFICATION "08"
#define GET_LIST_FRIEND_NOTIFICATION "09"
#define INPUT_SIZE 256
using namespace std;

struct Team
{
	int id;
	string teamName;
};
vector<string> listFriend;
vector<Team> listTeam;

bool checkIsExistedName(char *username);
void getNotification(char * recvBuff, char notification[]);
char loginResponse(char * recvBuff);
bool logoutResponse(char*recvBuff);
bool signUpResponse(char *recvBuff);
bool receiveMessageFromFriend(char *recvBuff, HWND hwnd);
bool receiveMessageFromTeam(char*recvBuff, HWND hwnd);
void friendLeavingTeamResponse(char *recvBuff, HWND hwnd);
bool makeTeamResponse(char* recvBuff);
void getListFriendResponse(char* recvBuff);
string deleteTeam(int teamId);
void leaveTeamResponse(char * recvBuff);
bool checkIsExistedName(char *username);
bool checkUserInListFriend(string nameUser);
int findIdTeam(string teamName);
string convertToString(char* a, int size);
string getTextFromEdit(HWND hwnd, int EDIT_ID);
string getMessageToBox(string mess, string name);


/*
get notification from receiced buff from server
*/
void getNotification(char * recvBuff, char notification[]) {

	notification[0] = recvBuff[0];
	notification[1] = recvBuff[1];
	notification[2] = 0;
}

/*
	handle message reponse login from server
*/ 
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

/*
	handle message reponse logout from server
*/
bool logoutResponse(char*recvBuff) {
	if (recvBuff[2] == SUCCESS) {
		return true;
	}
	else //khong dang xuat dc
		std::cout << "WRONG ";

	return true;
}

/*
handle message reponse signup from server
*/
bool signUpResponse(char *recvBuff) {
	if (recvBuff[2] == SUCCESS)
		return true;
	else
	return false;
}

/*
	handle message reponse from friend and view to messageBox
*/ 
bool receiveMessageFromFriend(char *recvBuff, HWND hwnd) {
	//format : [notification][username]|[messs]
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
	SendDlgItemMessage(hwnd, CHAT_BOX, EM_REPLACESEL, 0, (LPARAM)messDisplay.c_str());
	return true;
}

/*
	 handle message received from member in a team and view to messageBox
*/
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

/*
	handle message from server when there is a user leave out of team
*/ 
void friendLeavingTeamResponse(char *recvBuff, HWND hwnd) {
	//get team id
	int x = 4;
	int index = 0;
	char message[BUFF_SIZE];
	while (recvBuff[x] != '\0') {
		message[index] = recvBuff[x];
		index++;
		x++;
	}
	message[index] = 0;
	string messDisplay;
	messDisplay.append(message);
	SendDlgItemMessage(hwnd, CHAT_BOX, EM_REPLACESEL, 0, (LPARAM)messDisplay.c_str());
}


/*
	handle message received from server when user create Team group
*/
bool makeTeamResponse(char* recvBuff) {
	//format  [NOTIFICATION][Team ID][Team Name]
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
	SendMessage(hListTeams, LB_RESETCONTENT, 0, 0);	// reset list team
	// view list team
	for (int i = 0; i < listTeam.size(); i++)
	{
		SendMessage(hListTeams, LB_ADDSTRING, 0, (LPARAM)listTeam[i].teamName.c_str());
	}
	return true;
}


/*
	handle message that include list friend received from server
*/
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
	// view to message box
	SendMessage(hListFriends, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < listFriend.size(); i++)
	{
		SendMessage(hListFriends, LB_ADDSTRING, 0, (LPARAM)listFriend[i].c_str());
	}
}


/*
	delete Team out of listTeam that user is member
*/
string deleteTeam(int teamId) {
	for (std::vector<Team>::iterator iter = listTeam.begin(); iter != listTeam.end(); ++iter)
	{
		if (iter->id == teamId)
		{	
			string nameEnd = iter->teamName;
			listTeam.erase(iter);
			return nameEnd;
		}
	}
	return "";
}


/*
	handle message response from server when user request leave Team
*/
void leaveTeamResponse(char * recvBuff) {
	char c_teamId[128];
	int x = 2;
	if (recvBuff[x] == '0') {
		MessageBox(NULL, "group is not existed!", "error", MB_OK);
	}
	else {
		int index = 0;
		while (recvBuff[x] != 0) {
			c_teamId[index] = recvBuff[x];
			x++;
			index++;
		}
		c_teamId[index] = 0;
		int teamId = atoi(c_teamId);

		string message;
		string teamNameQuited = deleteTeam(teamId);
		message.append("you have left team ");
		message.append(teamNameQuited);
		message.append("!");

		SendMessage(hListTeams, LB_RESETCONTENT, 0, 0);
		for (int i = 0; i < listTeam.size(); i++)
		{
			SendMessage(hListTeams, LB_ADDSTRING, 0, (LPARAM)listTeam[i].teamName.c_str());
		}
	}
}


/*
	check nameUser is existed in listFriend yet
*/
bool checkIsExistedName(char *username) {
	for (int i = 0; i < listFriend.size(); i++) {
		if (listFriend[i].compare(username) == 0)
			return true;
	}
	return false;
}


/*
 check has nameuser param existed in listFriend?
*/
bool checkUserInListFriend(string nameUser) {
	for (int i = 0; i < listFriend.size(); i++) {
		if (listFriend[i].compare(nameUser) == 0)
			return true;
	}
	return false;
}

/*
	 find id Team in list team.. retrun index if existed otherwise 0
*/
int findIdTeam(string teamName) {
	for (int i = 0; i < listTeam.size(); i++) {
		if (teamName.compare(listTeam[i].teamName) == 0) return listTeam[i].id;
	}
	return 0;
}

/*
	convert char* to string	
*/
string convertToString(char* a, int size){
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

/* get string from edit controls*/
string getTextFromEdit(HWND hwnd, int EDIT_ID) {
	WORD len = (WORD)SendDlgItemMessage(hwnd, EDIT_ID, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
	char buff[2048];
	*((LPWORD)buff) = len;
	SendDlgItemMessage(hwnd, EDIT_ID, EM_GETLINE, (WPARAM)0, (LPARAM)buff);
	buff[len] = 0;
	string result = convertToString(buff, len);
	return result;
}

/*
	get message to display to ChatBox
	params: 
	- mess: user typing from keyboard
	- name: name of user send message
*/
string getMessageToBox(string mess, string name) {
	string messToBox;
	messToBox.append(name);
	messToBox.append(":  ");
	messToBox.append(mess);
	messToBox.append("\r\n");
	return messToBox;
}

