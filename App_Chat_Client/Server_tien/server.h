

#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "business.h"



int classifyMessageAndSendToClient(SOCKET connSock, char buff[]);
int createRoomChat(char buff[], SOCKET connSock);
int leaveTheConversation(char buff[]);
int listRoomHasUser(char buff[], SOCKET connSock);
int listUserOnline(char buff[], SOCKET connSock);
int listUserInRoom(char buff[], SOCKET connSock);
int transferMessagesPrivateChat(char buff[], SOCKET currentConnSock);
int transferMessagesPublicChat(char buff[], SOCKET currentConnSock);
string registerProcess(char buff[], SOCKET connSock);
string loginProcess(char buff[], SOCKET connSock);
string logoutProcess(SOCKET connSock);
void logoutClientDisconnect(SOCKET connSock);

#define OPCODE_RESPONSE_SIZE 3
#define OPCODE_REQUEST_SIZE 2
#define BUFF_SIZE 2048
#define ACCOUNT_SIZE 2048

#define REGISTER "03"
#define LOGIN "04"
#define LOGOUT "05"
#define CREATE_ROOM_CHAT "06"
#define PRIVATE_MESSAGE "07"
#define PUBLIC_MESSAGE "08"
#define LEAVE_THE_CONVERSATION "09"
#define LIST_ROOM_CHAT "10"
#define LIST_USER_ONLINE "11"
#define LIST_USER_ỈN_ROOM "12"
// message response


//thông báo
#define NOTIFICATION '0'

// tin nhắn
#define MESSAGES '1'

#define SYMBOL '|'
using namespace std;

int classifyMessageAndSendToClient(SOCKET connSock, char buff[]);

int transferMessagesPrivateChat(char buff[], SOCKET currentConnSock);
int transferMessagesPublicChat(char buff[], SOCKET currentConnSock);

int createRoomChat(char buff[], SOCKET connSock);
int leaveTheConversation(char buff[]);

int listRoomHasUser(char buff[], SOCKET connSock);
int listUserOnline(char buff[], SOCKET connSock);
int listUserInRoom(char buff[], SOCKET connSock);





int classifyMessageAndSendToClient(SOCKET connSock, char buff1[]) {
	char buff[BUFF_SIZE];
	gets_s(buff, 2048);
	buff[strlen(buff)] = 0;
	printf("input : %s\n", &buff);
	char typeMessage[3];
	int i = 0;
	while (i < 2) {
		typeMessage[i] = buff[i];
		i++;

	}
	typeMessage[i] = 0;
	char sendBuff[2048];
	if (strcmp(typeMessage, REGISTER) == 0) {
		string statusRegister = registerProcess(buff, connSock);
		packageBuff(REGISTER, statusRegister, sendBuff);
		return sendToClient(connSock, sendBuff);
	}
	else if (strcmp(typeMessage, LOGIN) == 0) {

		string statusLogin = loginProcess(buff, connSock);
		packageBuff(LOGIN, statusLogin, sendBuff);
		printf("sendBuff : %s\n", sendBuff);
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

	else if (strcmp(typeMessage, CREATE_ROOM_CHAT) == 0) {
		return createRoomChat(buff, connSock);
	}

	else if (strcmp(typeMessage, LEAVE_THE_CONVERSATION) == 0) {
		return leaveTheConversation(buff);
	}
	else if (strcmp(typeMessage, LIST_ROOM_CHAT) == 0) {
		return listRoomHasUser(buff, connSock);
	}
	else if (strcmp(typeMessage, LIST_USER_ONLINE) == 0) {
		return listUserOnline(buff, connSock);
	}
	else if (strcmp(typeMessage, LIST_USER_ỈN_ROOM) == 0) {
		return listUserInRoom(buff, connSock);
	}
}

int transferMessagesPrivateChat(char buff[], SOCKET currentConnSock) {
	string username;
	string messages;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&messages, buff, index);
	index = checkUserOnline(username);
	int indexCurrentUser = findUserNameBySocket(currentConnSock);
	if (indexCurrentUser < 0) {
		return 1;
	}
	string payload;
	payload.append(listClient[indexCurrentUser].userName);
	payload.append("|");
	payload.append(messages);
	char *sendBuff;
	if (index > 0) {
		packageBuff(PRIVATE_MESSAGE, payload, sendBuff);
		return sendToClient(listClient[index].connSock, sendBuff);
	}
	return 1;
}

int transferMessagesPublicChat(char buff[], SOCKET currentConnSock) {
	char *sendBuff;
	string idRoom;
	string messages;
	int index = getMessages(&idRoom, buff, OPCODE_REQUEST_SIZE);
	int id = stoi(idRoom, 0, 10);
	int indexCurrentUser = findUserNameBySocket(currentConnSock);
	if (indexCurrentUser < 0) {
		return 1;
	}
	string payload;
	payload.append(idRoom);
	payload.append("|");
	payload.append(listClient[indexCurrentUser].userName);
	payload.append("|");
	payload.append(messages);
	packageBuff(PUBLIC_MESSAGE, payload, sendBuff);

	index = getMessages(&messages, buff, index);
	bool check = false;
	for (int i = 0; i < listRoomChat.size(); i++) {
		if (listRoomChat[i].id == id) {
			check = true;
			for (int j = 0; j < listRoomChat[i].listUsername.size(); j++) {
				index = checkUserOnline(listRoomChat[i].listUsername[j]);
				if (index > 0) {
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

int createRoomChat(char buff[], SOCKET connSock) {
	string numberUserString;
	string username[ACCOUNT_SIZE];
	string roomName;
	int id = listRoomChat.size() + 1;

	string idRoom = std::to_string(id);
	int index = getMessages(&numberUserString, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&roomName, buff, index);
	int numberOfUserInRoom = stoi(numberUserString, 0, 10);
	int i = 0;
	while (i < numberOfUserInRoom) {
		index = getMessages(&username[i], buff, index);
		i++;
	}
	RoomChat roomChat;
	roomChat.id = id;
	vector<string> listUser;
	char *sendBuff;

	for (int i = 0; i < numberOfUserInRoom; i++) {
		int a = checkUserOnline(username[i]);
		if (a > 0) {
			listUser.push_back(listClient[a].userName);
			packageBuff(CREATE_ROOM_CHAT, SUCCESS, sendBuff);
			sendToClient(listClient[a].connSock, sendBuff);
		}
	}
	roomChat.listUsername = listUser;
	roomChat.roomName = roomName;
	listRoomChat.push_back(roomChat);
	return 1;
}

int leaveTheConversation(char buff[]) {
	char *sendBuff;
	string idRoom;
	string username;
	int index = getMessages(&idRoom, buff, OPCODE_REQUEST_SIZE);
	index = getMessages(&username, buff, index);
	int id = stoi(idRoom, 0, 10);

	packageBuff(LEAVE_THE_CONVERSATION, username, sendBuff);

	//get index by room in listRoom 
	int indexRoomInList = findRoomById(id);
	vector<string> listUserInRoom = listRoomChat[indexRoomInList].listUsername;
	int indexUserInRoom = checkUserInRoom(username, index);
	if (indexUserInRoom < 0) {
		return 1;
	}

	listRoomChat[indexRoomInList].listUsername.erase(listRoomChat[indexRoomInList].listUsername.begin() + indexUserInRoom);

	for (int i = 0; i < listUserInRoom.size(); i++) {
		if (i != indexUserInRoom) {
			int indexUserInlistOnline = checkUserOnline(listUserInRoom[i]);
			if (indexUserInlistOnline > 0) {
				sendToClient(listClient[i].connSock, sendBuff);
			}
		}
	}
	return 1;
}

int listRoomHasUser(char buff[], SOCKET connSock) {
	string username;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	string str;
	string payload;
	int numberOfRoomHasUser;
	for (int i = 0; i < listRoomChat.size(); i++) {
		index = checkUserInRoom(username, i);
		if (index > 0) {
			numberOfRoomHasUser++;
			str += listRoomChat[i].id;
			str.append("|");
			str.append(listRoomChat[i].roomName);
			str.append("|");
		}
		str.append("|");
	}
	payload += numberOfRoomHasUser;
	payload.append(str);
	char *sendBuff;
	packageBuff(LIST_ROOM_CHAT, payload, sendBuff);
	return sendToClient(connSock, sendBuff);
}

int listUserOnline(char buff[], SOCKET connSock) {
	string username;
	int index = getMessages(&username, buff, OPCODE_REQUEST_SIZE);
	string payload;
	int numberOfRoomHasUser;
	if (listClient.size() <= 9) {
		payload += "0";
	}
	payload += listClient.size();

	for (int i = 0; i < listClient.size(); i++) {
		payload.append(listClient[i].userName);
		if (i != listClient.size() - 1) {
			payload.append("|");
		}
	}
	char *sendBuff;
	packageBuff(LIST_USER_ONLINE, payload, sendBuff);
	return sendToClient(connSock, sendBuff);
}

int listUserInRoom(char buff[], SOCKET connSock) {
	string idRoom;
	string username;
	int index = getMessages(&idRoom, buff, OPCODE_REQUEST_SIZE);
	string payload;
	int id = stoi(idRoom, 0, 10);
	int indexOfRoomInListRoom = findRoomById(id);
	int numberOfUserInRoom = listRoomChat[indexOfRoomInListRoom].listUsername.size();
	payload += numberOfUserInRoom;
	vector<string> listUser = listRoomChat[indexOfRoomInListRoom].listUsername;
	for (int i = 0; i < numberOfUserInRoom; i++) {
		if (listUser[i].compare(username) != 0) {
			payload.append(listUser[i]);
			payload.append("|");
		}
	}
	char *sendBuff;
	packageBuff(LIST_USER_ỈN_ROOM, payload, sendBuff);
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