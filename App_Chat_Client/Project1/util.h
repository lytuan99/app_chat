#pragma once
using namespace std;
string convertToString(char* a, int size)
{
	int i;
	string s = "";
	for (i = 0; i < size; i++) {
		s = s + a[i];
	}
	return s;
}

//char* getTextFromEdit(HWND hwnd, int EDIT_ID) {
//	WORD len = (WORD)SendDlgItemMessage(hwnd, EDIT_ID, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
//	char buff[2048];
//	*((LPWORD)buff) = len;
//	SendDlgItemMessage(hwnd, EDIT_ID, EM_GETLINE, (WPARAM)0, (LPARAM)buff);
//	buff[len] = 0;
//	//string result = convertToString(buff, len);
//	return buff;
//}

string getTextFromEdit(HWND hwnd, int EDIT_ID) {
	WORD len = (WORD)SendDlgItemMessage(hwnd, EDIT_ID, EM_LINELENGTH, (WPARAM)0, (LPARAM)0);
	char buff[2048];
	*((LPWORD)buff) = len;
	SendDlgItemMessage(hwnd, EDIT_ID, EM_GETLINE, (WPARAM)0, (LPARAM)buff);
	buff[len] = 0;
	string result = convertToString(buff, len);
	return result;
}


string getMessageToBox(string mess, string name ) {

	string messToBox;
	messToBox.append(name);
	messToBox.append(":  ");
	messToBox.append(mess);
	messToBox.append("\r\n");
	return messToBox;
}