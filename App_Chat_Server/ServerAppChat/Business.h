#include "DataIO.h"



#define OPCODE_RESPONSE_SIZE 3
#define BUFF_SIZE 2048
#define ACCOUNT_SIZE 2048

#define ERROR "0"
#define SUCCESS "1"
#define LOGGED_IN "2"

//--------
#define REGISTER "03"
#define LOGIN "04"
#define LOGOUT "05"
#define CREATE_TEAM_CHAT "06"
#define PRIVATE_MESSAGE "07"
#define PUBLIC_MESSAGE "08"
#define LEAVE_THE_CONVERSATION "09"
#define LIST_TEAM_CHAT "10"
#define LIST_USER_ONLINE "11"
#define LIST_USER_IN_TEAM "12"

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

int classifyMessageAndSendToClient(SOCKET connSock, char buff[]);
int sendToClient(SOCKET currentConnSock, char buff[]);
int recvFromClient(SOCKET connSock, char buff[]);
int getMessages(string *messages, char buff[], int start);
int checkUserOnline(string username);
int findTeamById(int id);
int checkUserInTeam(string username, int index);
void packageBuff(char *active, string message, char *buff);
char* getTypeNotification(char *buff);
int findUserNameBySocket(SOCKET connSock);


int findUserNameBySocket(SOCKET connSock) {
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
int recvFromClient(SOCKET connSock, char buff[]) {
	int ret = recv(connSock, buff, BUFF_SIZE, 0);
	return ret;
}

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

int sendToClient(SOCKET connSock, char buff[]) {
	//printf(" Buff send lytuan : %s\n", buff);
	int ret = send(connSock, buff, strlen(buff), 0);
	return ret;

	return 1;
}

int checkUserOnline(string username) {
	for (int i = 0; i < listClient.size(); i++) {
		if (listClient[i].userName == username) {
			return i;
		}
	}
	return -1;
}

int findTeamById(int id) {
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