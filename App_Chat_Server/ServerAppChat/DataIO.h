#pragma once
#include <iostream>
#include <fstream>
#include <string.h>

#define PATH "E:/account.txt"
using namespace std;
typedef struct User
{
	string userName;
	string password;
};

int readFile(struct User user[]);
void writeFile(struct User user[], int numberOfUser);

int readFile(struct User user[]) {
	fstream f;
	f.open(PATH, ios::in);
	string username, password;
	int i = 0;
	while (f >> username >> password) {
		user[i].userName = username;
		user[i].password = password;
		i++;
	}
	int numberUser = i;
	f.close();
	return numberUser;
}

void writeFile(struct User user[], int numberOfUser) {
	fstream f;
	f.open(PATH, ios::out);
	string username, password;
	int i = 0;
	while (i < numberOfUser) {
		username = user[i].userName;
		password = user[i].password;
		f << username << " " << password << endl;
		i++;
	}
	f.close();
}