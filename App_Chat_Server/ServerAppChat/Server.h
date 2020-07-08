#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Business.h"
#include "DataIO.h"





#define OPCODE_REQUEST_SIZE 2
#define BUFF_SIZE 2048
#define ACCOUNT_SIZE 2048

#define SYMBOL '|'
using namespace std;


int classifyRequest(SOCKET connSock, char buff[]);
int handleRequestCreateTeamChat(char buff[], SOCKET connSock);
int handleRequestLeaveTeam(char buff[], SOCKET connSock);
int handleRequestListUserOnline(char buff[], SOCKET connSock);
int handleRequestChatWithFriend(char buff[], SOCKET currentConnSock);
int handleRequestChatWithTeam(char buff[], SOCKET currentConnSock);
string handleRequestRegister(char buff[], SOCKET connSock);
string handleRequestLogin(char buff[], SOCKET connSock);
string handleRequestLogout(SOCKET connSock);
void handleLogoutClientDisconnect(SOCKET connSock);



/* classify request form client */
int classifyRequest(SOCKET connSock, char buff[]) {
	char *typeRequest = getTypeNotification(buff);
	char sendBuff[BUFF_SIZE];
	if (strcmp(typeRequest, REGISTER) == 0) {
		string statusRegister = handleRequestRegister(buff, connSock);
		packageBuff(REGISTER, statusRegister, sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	else if (strcmp(typeRequest, LOGIN) == 0) {
		string statusLogin = handleRequestLogin(buff, connSock);
		packageBuff(LOGIN, statusLogin, sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	else if (strcmp(typeRequest, LOGOUT) == 0) {
		string statusLogout = handleRequestLogout(connSock);
		packageBuff(LOGOUT, statusLogout, sendBuff);
		printf("sendBuff : %s\n", sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	else if (strcmp(typeRequest, CHAT_FRIEND) == 0) {
		return handleRequestChatWithFriend(buff, connSock);
	}
	else if (strcmp(typeRequest, CHAT_TEAM) == 0) {
		return handleRequestChatWithTeam(buff, connSock);
	}

	else if (strcmp(typeRequest, CREATE_TEAM_CHAT) == 0) {
		return handleRequestCreateTeamChat(buff, connSock);
	}

	else if (strcmp(typeRequest, LEAVE_TEAM) == 0) {
		return handleRequestLeaveTeam(buff, connSock);
	}
	else if (strcmp(typeRequest, LIST_USER_ONLINE) == 0) {
		return handleRequestListUserOnline(buff, connSock);
	}
}

/* Handle request chat with friend  */
int handleRequestChatWithFriend(char buff[], SOCKET currentConnSock) {
	string username;
	string messages;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&messages, buff, index);

	int indexCurrentUser = findIndexUserBySocket(currentConnSock);
	if (indexCurrentUser < 0) {
		return 1;
	}
	string payload;
	payload.append(listClient[indexCurrentUser].userName);
	payload.append("|");
	payload.append(messages);
	char sendBuff[BUFF_SIZE];
	int indexUser = checkUserOnline(username);
	if (indexUser >= 0) {
		packageBuff(CHAT_FRIEND, payload, sendBuff);
		return sendToClient(listClient[indexUser].connSock, sendBuff);
	}
	else {
		string body;
		body += "System|" + username + " was offline!\r\n";
		packageBuff(CHAT_FRIEND, body, sendBuff);
		return sendToClient(currentConnSock, sendBuff);
	}
	return 1;
}

/* Handle request chat with team */
int handleRequestChatWithTeam(char buff[], SOCKET currentConnSock) {
	char sendBuff[BUFF_SIZE];
	string idTeam;
	string messages;
	int index = getMessages(&idTeam, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&messages, buff, index);
	int id = stoi(idTeam, 0, 10);
	int indexCurrentUser = findIndexUserBySocket(currentConnSock);
	if (indexCurrentUser < 0) {
		return 1;
	}
	int indexTeam = findIndexTeamById(id);
	string payload;
	payload.append(listTeamChat[indexTeam].teamName);
	payload.append("|");
	payload.append(listClient[indexCurrentUser].userName);
	payload.append("|");
	payload.append(messages);

	packageBuff(CHAT_TEAM, payload, sendBuff);

	bool check = false;
	//Send message to all user in team
	for (int i = 0; i < listTeamChat.size(); i++) {
		if (listTeamChat[i].id == id) {
			check = true;
			for (int j = 0; j < listTeamChat[i].listUsername.size(); j++) {
				index = checkUserOnline(listTeamChat[i].listUsername[j]);
				if (index >= 0) {
					if (listClient[index].connSock != currentConnSock) {
						sendToClient(listClient[index].connSock, sendBuff);
					}
				}
			}
			break;
		}
	}
	//----------------------------------------------------
	return 1;
}

/* Handle request create team chat */
int handleRequestCreateTeamChat(char buff[], SOCKET connSock) {
	string numberUserString;
	string username[ACCOUNT_SIZE];
	string teamName;
	int id = listTeamChat.size() + 1;

	string idTeam = std::to_string(id);
	int index = getMessages(&numberUserString, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&teamName, buff, index);
	int numberOfUserInTeam = stoi(numberUserString, 0, 10);
	int i = 0;
	while (i < numberOfUserInTeam) {
		index = getMessages(&username[i], buff, index);
		i++;
	}
	TeamChat teamChat;
	teamChat.id = id;
	vector<string> listUser;
	char sendBuff[BUFF_SIZE];
	string payload;
	payload += to_string(id);
	payload.append("|");
	payload += teamName;
	packageBuff(CREATE_TEAM_CHAT, payload, sendBuff);

	//send notification create team to all user in team
	for (int i = 0; i < numberOfUserInTeam; i++) {
		int a = checkUserOnline(username[i]);
		cout << username[i] << " user name " << endl;
		listUser.push_back(username[i]);
		if (a >= 0) {
			sendToClient(listClient[a].connSock, sendBuff);
		}
	}
	//-------------------
	listUser.push_back(listClient[findIndexUserBySocket(connSock)].userName);
	sendToClient(connSock, sendBuff);
	teamChat.listUsername = listUser;
	teamChat.teamName = teamName;
	listTeamChat.push_back(teamChat);
	return 1;
}

/* Handle request leave team */
int handleRequestLeaveTeam(char buff[], SOCKET connSock) {
	char sendBuff[BUFF_SIZE];
	if (listTeamChat.size() == 0) {
		packageBuff(LEAVE_TEAM, ERROR, sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	string idTeam;

	int index = getMessages(&idTeam, buff, OPCODE_REQUEST_SIZE);
	//index = getMessages(&username, buff, index);
	int id = stoi(idTeam, 0, 10);
	string username = listClient[findIndexUserBySocket(connSock)].userName;
	int indexTeamInList = findIndexTeamById(id);
	if (indexTeamInList < 0) {
		return 1;
	}
	string payload;
	payload += idTeam;
	payload += "|";
	payload += username + " left team " + listTeamChat[indexTeamInList].teamName + ".\r\n";

	packageBuff(LEAVING_TEAM_RESPONSE, payload, sendBuff);

	vector<string> listUserInTeam = listTeamChat[indexTeamInList].listUsername;
	int indexUserInTeam = checkUserInTeam(username, indexTeamInList);
	if (indexUserInTeam < 0) {
		return 1;
	}

	listUserInTeam.erase(listUserInTeam.begin() + indexUserInTeam);
	listTeamChat[indexTeamInList].listUsername = listUserInTeam;
	//Send notification a user left team to all user in team 
	for (int i = 0; i < listUserInTeam.size(); i++) {
		int indexUserInlistOnline = checkUserOnline(listUserInTeam[i]);
		if (indexUserInlistOnline >= 0)
			sendToClient(listClient[indexUserInlistOnline].connSock, sendBuff);
	}
	//---------------------
	packageBuff(LEAVE_TEAM, idTeam, sendBuff);
	sendToClient(connSock, sendBuff);
	return 1;
}

/* Handle request get list user online */
int handleRequestListUserOnline(char buff[], SOCKET connSock) {
	int index = findIndexUserBySocket(connSock);
	if (index < -1) {
		return 1;
	}
	string payload;
	if (listClient.size() <= 9) {
		payload += "0";
	}
	int numberUserOnline = (listClient.size() - 1) > 0 ? listClient.size() : 0;
	//itoa(numberUserOnline, numberOfUser, 10);
	std::string s = std::to_string(numberUserOnline);
	payload.append(s);
	if (numberUserOnline > 0) {
		//Get list user online
		for (int i = 0; i < listClient.size(); i++) {
			if (listClient[i].userName.compare(listClient[index].userName) != 0) {
				payload.append(listClient[i].userName);
				payload.append("|");
			}
		}
		//----------
	}

	char sendBuff[BUFF_SIZE];
	packageBuff(LIST_USER_ONLINE, payload, sendBuff);
	return sendToClient(connSock, sendBuff);
}

/* Handle request register */
string handleRequestRegister(char buff[], SOCKET connSock) {
	string username;
	string password;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&password, buff, index);
	User user[ACCOUNT_SIZE];
	int numberOfUser = readFile(user);
	//Check username existed
	for (int i = 0; i < numberOfUser; i++) {
		if (username == user[i].userName) {
			return ERROR;
		}
	}
	//--------------------
	user[numberOfUser].userName = username;
	user[numberOfUser].password = password;
	writeFile(user, numberOfUser + 1);
	return SUCCESS;
}

/* Handle request login */
string handleRequestLogin(char buff[], SOCKET connSock) {
	string username;
	string password;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&password, buff, index);
	for (int i = 0; i < listClient.size(); i++) {
		if (username == listClient[i].userName) {
			return LOGGED_IN;
		}
	}
	Client client;

	User user[ACCOUNT_SIZE];

	int numberOfUser = readFile(user);
	for (int i = 0; i < numberOfUser; i++) {
		if (username == (user[i].userName)) {
			if (password == user[i].password) {
				client.userName = username;
				client.connSock = connSock;
				listClient.push_back(client);
				return SUCCESS;
			}
		}
	}
	return ERROR;
}

/* Handle request logout */
string handleRequestLogout(SOCKET connSock) {
	for (int i = 0; i < listClient.size(); i++) {
		if (listClient[i].connSock == connSock) {
			listClient.erase(listClient.begin() + i);
			return SUCCESS;
		}
	}
	return ERROR;
}

/* Handle logout when client disconnect */
void handleLogoutClientDisconnect(SOCKET connSock) {
	for (int i = 0; i < listClient.size(); i++) {
		if (listClient[i].connSock == connSock) {
			listClient.erase(listClient.begin() + i);
			return;
		}
	}
}