#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Business.h"
#include "DataIO.h"



int classifyMessageAndSendToClient(SOCKET connSock, char buff[]);
int createTeamChat(char buff[], SOCKET connSock);
int leaveTheConversation(char buff[]);
int listTeamHasUser(char buff[], SOCKET connSock);
int listUserOnline(char buff[], SOCKET connSock);
int listUserInTeam(char buff[], SOCKET connSock);
int transferMessagesPrivateChat(char buff[], SOCKET currentConnSock);
int transferMessagesPublicChat(char buff[], SOCKET currentConnSock);
string registerProcess(char buff[], SOCKET connSock);
string loginProcess(char buff[], SOCKET connSock);
string logoutProcess(SOCKET connSock);
void logoutClientDisconnect(SOCKET connSock);

#define OPCODE_REQUEST_SIZE 2
#define BUFF_SIZE 2048
#define ACCOUNT_SIZE 2048

#define SYMBOL '|'
using namespace std;

int classifyMessageAndSendToClient(SOCKET connSock, char buff[]);

int transferMessagesPrivateChat(char buff[], SOCKET currentConnSock);
int transferMessagesPublicChat(char buff[], SOCKET currentConnSock);

int createTeamChat(char buff[], SOCKET connSock);
int leaveTheConversation(char buff[], SOCKET connSock);

//int listTeamHasUser(char buff[], SOCKET connSock);
int listUserOnline(char buff[], SOCKET connSock);
int listUserInTeam(char buff[], SOCKET connSock);





int classifyMessageAndSendToClient(SOCKET connSock, char buff[]) {
	char *typeMessage = getTypeNotification(buff);
	/*int i = 0;
	while (i < 2) {
	typeMessage[i] = buff[i];
	i++;
	}
	typeMessage[i] = 0;*/
	char sendBuff[BUFF_SIZE];
	if (strcmp(typeMessage, REGISTER) == 0) {
		string statusRegister = registerProcess(buff, connSock);
		packageBuff(REGISTER, statusRegister, sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	else if (strcmp(typeMessage, LOGIN) == 0) {
		string statusLogin = loginProcess(buff, connSock);
		packageBuff(LOGIN, statusLogin, sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	else if (strcmp(typeMessage, LOGOUT) == 0) {
		string statusLogout = logoutProcess(connSock);
		packageBuff(LOGOUT, statusLogout, sendBuff);
		printf("sendBuff : %s\n", sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	else if (strcmp(typeMessage, PRIVATE_MESSAGE) == 0) {
		return transferMessagesPrivateChat(buff, connSock);
	}
	else if (strcmp(typeMessage, PUBLIC_MESSAGE) == 0) {
		return transferMessagesPublicChat(buff, connSock);
	}

	else if (strcmp(typeMessage, CREATE_TEAM_CHAT) == 0) {
		return createTeamChat(buff, connSock);
	}

	else if (strcmp(typeMessage, LEAVE_THE_CONVERSATION) == 0) {
		cout << "test recv buff leav: " << buff << endl;
		return leaveTheConversation(buff, connSock);
	}

	/*else if (strcmp(typeMessage, LIST_TEAM_CHAT) == 0) {
	printf("  buff recv : %s", buff);
	return listTeamHasUser(buff, connSock);
	}*/
	else if (strcmp(typeMessage, LIST_USER_ONLINE) == 0) {
		return listUserOnline(buff, connSock);
	}
	else if (strcmp(typeMessage, LIST_USER_IN_TEAM) == 0) {
		return listUserInTeam(buff, connSock);
	}
}

int transferMessagesPrivateChat(char buff[], SOCKET currentConnSock) {
	string username;
	string messages;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&messages, buff, index);

	int indexCurrentUser = findUserNameBySocket(currentConnSock);
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
		packageBuff(PRIVATE_MESSAGE, payload, sendBuff);
		return sendToClient(listClient[indexUser].connSock, sendBuff);
	}
	return 1;
}

int transferMessagesPublicChat(char buff[], SOCKET currentConnSock) {
	char sendBuff[BUFF_SIZE];
	string idTeam;
	string messages;
	int index = getMessages(&idTeam, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&messages, buff, index);
	int id = stoi(idTeam, 0, 10);
	int indexCurrentUser = findUserNameBySocket(currentConnSock);
	if (indexCurrentUser < 0) {
		return 1;
	}
	int indexTeam = findTeamById(id);
	string payload;
	payload.append(listTeamChat[indexTeam].teamName);
	payload.append("|");
	payload.append(listClient[indexCurrentUser].userName);
	payload.append("|");
	payload.append(messages);
	packageBuff(PUBLIC_MESSAGE, payload, sendBuff);

	index = getMessages(&messages, buff, index);
	bool check = false;
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
	return 1;
}

int createTeamChat(char buff[], SOCKET connSock) {
	string numberUserString;
	string username[ACCOUNT_SIZE];
	string teamName;
	int id = listTeamChat.size() + 1;

	string idTeam = std::to_string(id);
	int index = getMessages(&numberUserString, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&teamName, buff, index);
	int numberOfUserInTeam = stoi(numberUserString, 0, 10);
	cout << " number user  " << numberOfUserInTeam << endl;
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
	for (int i = 0; i < numberOfUserInTeam; i++) {
		int a = checkUserOnline(username[i]);
		cout << username[i] << " user name " << endl;
		listUser.push_back(username[i]);
		if (a >= 0) {
			sendToClient(listClient[a].connSock, sendBuff);
		}
	}
	listUser.push_back(listClient[findUserNameBySocket(connSock)].userName);
	sendToClient(connSock, sendBuff);
	teamChat.listUsername = listUser;
	teamChat.teamName = teamName;
	listTeamChat.push_back(teamChat);
	return 1;
}


int leaveTheConversation(char buff[], SOCKET connSock) {
	char sendBuff[BUFF_SIZE];
	string idTeam;
	
	int index = getMessages(&idTeam, buff, OPCODE_REQUEST_SIZE);
	//index = getMessages(&username, buff, index);
	
	int id = stoi(idTeam, 0, 10);
	string username = listClient[findUserNameBySocket(connSock)].userName;
	string payload;
	payload += idTeam;
	payload += "|";
	payload += "Leave team!";

	packageBuff(PUBLIC_MESSAGE, payload, sendBuff);

	int indexTeamInList = findTeamById(id);
	vector<string> listUserInTeam = listTeamChat[indexTeamInList].listUsername;
	int indexUserInTeam = checkUserInTeam(username, indexTeamInList);
	if (indexUserInTeam < 0) {
		return 1;
	}

	listTeamChat[indexTeamInList].listUsername.erase(listTeamChat[indexTeamInList].listUsername.begin() + indexUserInTeam);

	for (int i = 0; i < listUserInTeam.size(); i++) {
		if (i != indexUserInTeam) {
			int indexUserInlistOnline = checkUserOnline(listUserInTeam[i]);
			if (indexUserInlistOnline >= 0) {
				sendToClient(listClient[i].connSock, sendBuff);
			}
		}
	}
	packageBuff(LEAVE_THE_CONVERSATION, SUCCESS, sendBuff);
	cout << "test leave: " << sendBuff << endl;
	sendToClient(connSock, sendBuff);
	return 1;
}


int listUserOnline(char buff[], SOCKET connSock) {
	int index = findUserNameBySocket(connSock);
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
		for (int i = 0; i < listClient.size(); i++) {
			if (listClient[i].userName.compare(listClient[index].userName) != 0) {
				payload.append(listClient[i].userName);
				payload.append("|");
			}

		}
	}

	char sendBuff[BUFF_SIZE];
	packageBuff(LIST_USER_ONLINE, payload, sendBuff);
	return sendToClient(connSock, sendBuff);
}

int listUserInTeam(char buff[], SOCKET connSock) {
	string idTeam;
	string username;
	int index = getMessages(&idTeam, buff, OPCODE_REQUEST_SIZE);
	string payload;
	int id = stoi(idTeam, 0, 10);
	int indexOfTeamInListTeam = findTeamById(id);
	int numberOfUserInTeam = listTeamChat[indexOfTeamInListTeam].listUsername.size();
	char converNumberUser[3];
	std::string s = std::to_string(numberOfUserInTeam);
	//_itoa(numberOfUserInRoom, converNumberUser, 10);
	payload.append(s);
	vector<string> listUser = listTeamChat[indexOfTeamInListTeam].listUsername;
	for (int i = 0; i < numberOfUserInTeam; i++) {
		if (listUser[i].compare(username) != 0) {
			payload.append(listUser[i]);
			payload.append("|");
		}
	}
	char sendBuff[BUFF_SIZE];
	packageBuff(LIST_USER_IN_TEAM, payload, sendBuff);
	return sendToClient(connSock, sendBuff);
}

string registerProcess(char buff[], SOCKET connSock) {
	string username;
	string password;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&password, buff, index);
	User user[ACCOUNT_SIZE];
	cout << " pass word " << password << endl;
	int numberOfUser = readFile(user);
	cout << " number of file " << numberOfUser << endl;
	for (int i = 0; i < numberOfUser; i++) {
		if (username == user[i].userName) {
			return ERROR;
		}
	}
	cout << "fuck ing ";
	user[numberOfUser].userName = username;
	user[numberOfUser].password = password;
	cout << " user passowrd " << user[numberOfUser].password << endl;
	writeFile(user, numberOfUser + 1);
	return SUCCESS;
}

string loginProcess(char buff[], SOCKET connSock) {
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

string logoutProcess(SOCKET connSock) {
	for (int i = 0; i < listClient.size(); i++) {
		if (listClient[i].connSock == connSock) {
			listClient.erase(listClient.begin() + i);
			return SUCCESS;
		}
	}
	return ERROR;
}
void logoutClientDisconnect(SOCKET connSock) {
	for (int i = 0; i < listClient.size(); i++) {
		if (listClient[i].connSock == connSock) {
			listClient.erase(listClient.begin() + i);
			return;
		}
	}
}