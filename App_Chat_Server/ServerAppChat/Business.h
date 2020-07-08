#include "DataIO.h"



#define OPCODE_RESPONSE_SIZE 3
#define BUFF_SIZE 2048
#define ACCOUNT_SIZE 2048

#define ERROR "0"
#define SUCCESS "1"
#define LOGGED_IN "2"

#define REGISTER "01"
#define LOGIN "02"
#define LOGOUT "03"
#define CREATE_TEAM_CHAT "06"
#define CHAT_FRIEND "04"
#define CHAT_TEAM "05"
#define LEAVE_TEAM "07"
#define LIST_USER_ONLINE "09"


//Response
#define LEAVING_TEAM_RESPONSE "08"
#define SYMBOL '|'

typedef struct Client
{
	string userName;
	SOCKET connSock;
};

typedef struct TeamChat
{
	int id;
	string teamName;
	vector<string> listUsername;
};

vector<Client> listClient;
vector<TeamChat> listTeamChat;
using namespace std;


int sendToClient(SOCKET currentConnSock, char buff[]);
int recvFromClient(SOCKET connSock, char buff[]);
int getMessages(string *messages, char buff[], int start);
int checkUserOnline(string username);
int findIndexTeamById(int id);
int checkUserInTeam(string username, int index);
void packageBuff(char *active, string message, char *buff);
char* getTypeNotification(char *buff);
int findIndexUserBySocket(SOCKET connSock);

/* Get index user in list by socket */
int findIndexUserBySocket(SOCKET connSock) {
	if (listClient.size() == 0) {
		return -1;
	}
	for (int i = 0; i < listClient.size(); i++) {
		if (listClient[i].connSock == connSock) {
			return i;
		}
	}
	return -1;
}

/* Get object in payload form receive buff */
int getMessages(string *messages, char buff[], int start) {
	char tmp[BUFF_SIZE];

	while (start < strlen(buff)) {
		if (buff[start] == SYMBOL) {
			break;
		}
		*messages += buff[start++];
	}
	return start + 1;
}
/* Get opcode from receive buff */
char* getTypeNotification(char *buff) {
	char typeRequest[3];
	int i = 0;
	while (i < 2) {
		typeRequest[i] = buff[i];
		i++;
	}
	typeRequest[i] = 0;
	return typeRequest;
}
/* Receive message from client */
int recvFromClient(SOCKET connSock, char buff[]) {
	int ret = recv(connSock, buff, BUFF_SIZE, 0);
	return ret;
}

/* Package buff before send to client */
void packageBuff(char *typeActive, string messages, char *buff) {
	int i = 0;
	int j = 0;
	for (j = 0; j < strlen(typeActive); j++) {
		buff[i++] = typeActive[j];
	}
	j = 0;
	while (j < messages.length()) {
		buff[i++] = messages[j++];
	}
	buff[i] = 0;
}

/* send message to client */
int sendToClient(SOCKET connSock, char buff[]) {
	int ret = send(connSock, buff, strlen(buff), 0);
	return ret;

	return 1;
}

/* Check user online in list and return index user in list */
int checkUserOnline(string username) {
	for (int i = 0; i < listClient.size(); i++) {
		if (listClient[i].userName == username) {
			return i;
		}
	}
	return -1;
}

/* Get index of team in list Team by id */
int findIndexTeamById(int id) {
	if (listTeamChat.size() == 0) {
		return -1;
	}
	for (int i = 0; i < listTeamChat.size(); i++) {
		if (listTeamChat[i].id == id) {
			return i;
		}
	}
	return -1;
}

/* Check user in team. return -1 if team have not user */
int checkUserInTeam(string username, int index) {
	if (listTeamChat[index].listUsername.size() == 0) {
		cout << "  list deo co nha  " << endl;
		return -1;
	}
	for (int i = 0; i < listTeamChat[index].listUsername.size(); i++) {
		cout << listTeamChat[index].listUsername[i] << "  username lits " << endl;
		if (listTeamChat[index].listUsername[i].compare(username) == 0) {
			return i;
		}
	}
	return -1;
}