

#include "Client.h"
#include "Login.h"
#include "Signup.h"
#include "Chatbox.h"
#include "CreateTeam.h"
#include "LeaveTeam.h"
#include "Business.h"

#pragma warning(disable:4996)
using namespace std;

#define _AFXDLL
#define OPTION_MENU_LEAVE_TEAM 654
#define OPTION_MENU_CREATE_TEAM 655
#define OPTION_MENU_EXIT 666
#define DEFAULT_PORT 27015
#define BTN_START 999
LRESULT CALLBACK windowProcessForWindowMain(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowProcessForWindowLogin(HWND handleforwindow2, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowProcessForWindowSignup(HWND handleforwindow3, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowProcessForWindowChatbox(HWND handleforwindow4, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowProcessForWindowCreateTeam(HWND handleforwindow5, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowProcessForWindowLeaveTeam(HWND handleforwindow6, UINT message, WPARAM wParam, LPARAM lParam);
bool checkUserInListUserTemp(char* nameUser);
void createMenu(HWND hwnd);
bool windowLoginOpen, windowSignupOpen, windowChatboxOpen, windowCreateTeamOpen, windowLeaveTeamOpen = false;
bool windowMainOpen = false;


enum windowtoopenenumt {windowMain, windowLogin, windowSignup, windowChatbox, windowCreateTeam, windowLeaveTeam
};
windowtoopenenumt windowtoopenenum = windowMain;

vector<string> listUserAddedTemp;
HWND hwnd;
HMENU hMenu;
Client* myClient;
string nameUser;
string friendChating;
string teamChating;


// main
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	myClient = new Client(DEFAULT_PORT);
	if (!myClient->Connect())
	{
		system("pause");
		return 1;
	}
	bool endprogram = false;
	WNDCLASSEX wcforwindowSignup;
	WNDCLASSEX wcforwindowChatbox;
	WNDCLASSEX wcforwindowCreateTeam;
	WNDCLASSEX wcforwindowLogin;
	WNDCLASSEX wcforwindowLeaveTeam;
	HWND handleforwindowLeaveTeam;

	//create window Main (1)		default manage all messages
	WNDCLASSEX wcforwindowMain;
	ZeroMemory(&wcforwindowMain, sizeof(WNDCLASSEX));
	wcforwindowMain.cbClsExtra = NULL;
	wcforwindowMain.cbSize = sizeof(WNDCLASSEX);
	wcforwindowMain.cbWndExtra = NULL;
	wcforwindowMain.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wcforwindowMain.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcforwindowMain.hIcon = NULL;
	wcforwindowMain.hIconSm = NULL;
	wcforwindowMain.hInstance = hInst;
	wcforwindowMain.lpfnWndProc = (WNDPROC)windowProcessForWindowMain;
	wcforwindowMain.lpszClassName = "window class Main";
	wcforwindowMain.lpszMenuName = NULL;
	wcforwindowMain.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcforwindowMain))
	{
		int nResult = GetLastError();
		MessageBox(NULL,"Window class creation failed", "Window Class Failed", MB_ICONERROR);
	}

	HWND handleforwindowMain = CreateWindowEx(NULL, wcforwindowMain.lpszClassName, "main", WS_OVERLAPPEDWINDOW, 020, 020, 200, 170,
		NULL, NULL, hInst, NULL);

	if (!handleforwindowMain)
	{
		int nResult = GetLastError();
		MessageBox(NULL, "Window creation failed", "Window Creation Failed", MB_ICONERROR);
	}

	ShowWindow(handleforwindowMain, nShowCmd);


	// begin handling events
	MSG msg;
	bool endloop = false;

	while (endloop == false) {
		if (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (windowtoopenenum != windowMain) {
			switch (windowtoopenenum) {
			case windowLogin:
				if (windowLoginOpen == false) {
					createwindowLogin(wcforwindowLogin, hwnd, hInst, nShowCmd);
					break;
				}
			case windowSignup:
				if (windowSignupOpen == false) {
					createwindowSignup(wcforwindowSignup, hwnd, hInst, nShowCmd);
				}
				break;
			case windowChatbox:
				if (windowChatboxOpen == false) {
					createwindowChatbox(wcforwindowChatbox, hwnd, hInst, nShowCmd);
					myClient->StartSubRoutine(hwnd);	// Start a data receiving thread
				}
				break;
			case windowCreateTeam:
				if (windowCreateTeamOpen == false) {
					createwindowCreateTeam(wcforwindowCreateTeam, hwnd, hInst, nShowCmd);
				}
				break;
			case windowLeaveTeam:
				if (windowLeaveTeamOpen == false) {
					createwindowLeaveTeam(wcforwindowLeaveTeam, handleforwindowLeaveTeam, hInst, nShowCmd);
				}
				break;
			}
			
			windowtoopenenum = windowMain;
		}


		if (windowLoginOpen == false && windowSignupOpen == false && windowChatboxOpen == false && windowCreateTeamOpen == false && windowMainOpen == false)
			endloop = true;
	}	// end while

	MessageBox(NULL, "All Windows are closed. Program will now close.", "Message", MB_ICONINFORMATION);
}


 /*
	----------------------------------------
	windows process functions proceduce
	Handle events
	----------------------------------------
 */

LRESULT CALLBACK windowProcessForWindowMain(HWND hwndMain, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		windowMainOpen = true;
		
		CreateWindowEx(NULL,"BUTTON","START",WS_TABSTOP | WS_VISIBLE |WS_CHILD | BS_DEFPUSHBUTTON,
		50, 50,100,35,hwndMain ,(HMENU)BTN_START,GetModuleHandle(NULL),NULL);
		break;
	case WM_DESTROY:
		windowLoginOpen = false;
		break;
	case WM_CLOSE:
		DestroyWindow(hwndMain);
		windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateTeamOpen = false;
		break;
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case BTN_START:
			windowtoopenenum = windowLogin;
			break;
		}
	}
	return DefWindowProc(hwndMain, message, wParam, lParam);

}

LRESULT CALLBACK windowProcessForWindowLogin(HWND hwndLogin, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		windowLoginOpen = true;
		addControlLogin(hwndLogin);
		break;
	case WM_CLOSE:
		DestroyWindow(hwndLogin);
		windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateTeamOpen = false;
		break;
	case WM_DESTROY:
		windowLoginOpen = false;
		break;
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case BTN_REGISTER:
			DestroyWindow(hwndLogin);
			windowtoopenenum = windowSignup;
			break;
		case BTN_LOGIN:
			string nameLog = getTextFromEdit(hwndLogin, EDIT_USERNAME);
			string passLog = getTextFromEdit(hwndLogin, EDIT_PASSWORD);
			myClient->loginRequest((char*)nameLog.c_str(), (char*)passLog.c_str());
			char recvBuff[BUFF_SIZE];
			int len = recv(myClient->ServerConnection, recvBuff, BUFF_SIZE, 0);
			char notification[3];
			getNotification(recvBuff, notification);
			char status = loginResponse(recvBuff);
			if (status == SUCCESS) {
				nameUser.append(nameLog);
				windowtoopenenum = windowChatbox;
				DestroyWindow(hwndLogin);
			}
			else if (status == LOGGED_IN) {
				MessageBox(NULL, "this account have logined in other client!", "ERROR", MB_OK);
			}
			else {
				MessageBox(NULL, "user name or password wrong!", "ERROR", MB_OK);
			}
			break;

		}
	}
		return DefWindowProc(hwndLogin, message, wParam, lParam);
}

LRESULT CALLBACK windowProcessForWindowSignup(HWND hwndSignup, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		windowSignupOpen = true;
		AddControlSignup(hwndSignup);
		break;
	case WM_CLOSE:
		DestroyWindow(hwndSignup);
		windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateTeamOpen = false;
		break;
	case WM_DESTROY:
		windowSignupOpen = false;
		break;
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case BTN_BACK_LOGIN:
			windowtoopenenum = windowLogin;
			DestroyWindow(hwndSignup);
			break;
		case BTN_SIGNUP:
			char userNameStr[BUFF_SIZE] = "", passStr[BUFF_SIZE] = "";
			char repassStr[BUFF_SIZE] = "";
			GetWindowText(hEdit_username_su, userNameStr, BUFF_SIZE);
			GetWindowText(hEdit_password_su, passStr, BUFF_SIZE);
			GetWindowText(hEdit_repassword_su, repassStr, BUFF_SIZE);
			int check = signUpCheck(userNameStr, passStr, repassStr);
			if (check == 1) {
				myClient->signUpRequest(userNameStr, passStr);
				char recvBuff[BUFF_SIZE];
				int len = recv(myClient->ServerConnection, recvBuff, BUFF_SIZE, 0);
				char notification[3];
				getNotification(recvBuff, notification);
				if (signUpResponse(recvBuff)) {

					windowtoopenenum = windowLogin;	// redirect to login window
					DestroyWindow(hwndSignup);
				}
				else {
					MessageBox(NULL, "this username is existed", "ERROR", MB_OK);
				}
				
				break;
			}
			else {
				MessageBox(hwndSignup, "error username or password", "Error", MB_OK);
				break;
			}
			break;
		}
	}
	return DefWindowProc(hwndSignup, message, wParam, lParam);
}

LRESULT CALLBACK windowProcessForWindowChatbox(HWND hwndChatbox, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) {
		case WM_CREATE:
			windowChatboxOpen = true;
			createMenu(hwndChatbox);
			addControlChatbox(hwndChatbox);
			SetWindowText(hEdit_username_profile, nameUser.c_str()); // set name profile
			
			break;
		case WM_CLOSE:
			DestroyWindow(hwndChatbox);
			windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateTeamOpen = false;
			break;
		case WM_DESTROY:
			windowChatboxOpen = false;
			break;
		case WM_COMMAND:
			switch LOWORD(wParam) {
			case OPTION_MENU_CREATE_TEAM:
				windowtoopenenum = windowCreateTeam;		// open createTeam window
				break;
			case OPTION_MENU_LEAVE_TEAM:
				windowtoopenenum = windowLeaveTeam;			// open leaveTeam window
				break;
			case OPTION_MENU_EXIT:
				DestroyWindow(hwndChatbox);
				windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateTeamOpen = false;
				break;
			case LB_FRIEND:
				switch (HIWORD(wParam)) {
				case LBN_DBLCLK:			// capture event click double int list Friend
					char buffer[256];
					int index = SendMessage((HWND)hListFriends, LB_GETCARETINDEX, 0, 0);
					SendMessage((HWND)hListFriends, LB_GETTEXT, (LPARAM)index, (WPARAM)buffer);	// view message to messageBox
					if (strlen(buffer) == 0) {
						MessageBox(NULL, "list empty!", "error", MB_OK);
						break;
					}
					friendChating.clear();
					teamChating.clear();
					friendChating.append(buffer);
					string mess = "chat with friend: " + friendChating;
					MessageBox(NULL, mess.c_str(), "turn", MB_OK);		// alert message 
					SetWindowText(hStaticChating, friendChating.c_str());		// view name friend is chating
				}
				break;
			case LB_TEAM:
				switch (HIWORD(wParam)) {
				case LBN_DBLCLK:			// capture event click double int  list Team
					char Buffer[256] = "";
					int index = SendMessage((HWND)hListTeams, LB_GETCARETINDEX, 0, 0);
					SendMessage((HWND)hListTeams, LB_GETTEXT, (LPARAM)index, (WPARAM)Buffer);
					if (strlen(Buffer) == 0) {
						MessageBox(NULL, "list empty!", "error", MB_OK);
						break;
					}

					friendChating.clear();
					teamChating.clear();
					teamChating.append(Buffer);
					string mess = "chat with Team: " + teamChating;
					MessageBox(NULL, mess.c_str(), "turn", MB_OK);
					//SetWindowText(hEdit_chatbox, "");			// reset message box
					SetWindowText(hStaticChating, teamChating.c_str());
				}
				break;

			case BTN_SEND: {
				if (friendChating.empty() && teamChating.empty()) {
					MessageBox(NULL, "you haven't selected a friend or Team to chat!", "notification", MB_OK);
					break;
				}
				string mess = getTextFromEdit(hwndChatbox, EDIT_TYPING_MESS);
				SetWindowText(hEdit_typingMessage, "");
				if (friendChating.empty()) {
					string messDisplay;
					messDisplay.append("<");
					messDisplay.append(teamChating);
					messDisplay.append(">");
					string messToBox = getMessageToBox(mess, nameUser);
					messDisplay.append(messToBox);
					SendMessage(hEdit_chatbox, EM_REPLACESEL, 0, (LPARAM)messDisplay.c_str());
					myClient->sendToTeam((char*)teamChating.c_str(), (char*)messToBox.c_str());
				}
				else {
					string messToBox = getMessageToBox(mess, nameUser);
					SendMessage(hEdit_chatbox, EM_REPLACESEL, 0, (LPARAM)messToBox.c_str());
					myClient->sendToFriend((char*)friendChating.c_str(), (char*)messToBox.c_str());
				}
				
			}
				break;
			case BTN_REFRESH_FRIEND:
				myClient->getListFriendRequest();
				break;
			case BTN_LOGOUT:
				myClient->logoutRequest();
				windowtoopenenum = windowLogin;
				DestroyWindow(hwndChatbox);
				break;
			case BTN_EXIT_CHAT_BOX:
				DestroyWindow(hwndChatbox);
				windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateTeamOpen = windowLeaveTeamOpen = false;
				break;
			}
	}
	return DefWindowProc(hwndChatbox, message, wParam, lParam);
}

LRESULT CALLBACK windowProcessForWindowCreateTeam(HWND hwndCreateTeam, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		windowCreateTeamOpen = false;
		break;
	case WM_CREATE:
		windowCreateTeamOpen = true;	
		addControlCreateTeam(hwndCreateTeam);
		listUserAddedTemp.clear();
		break;
	case WM_CLOSE:
		DestroyWindow(hwndCreateTeam);
		break;
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case LB_USER_ADDED:
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK:		
				char buffer[256];
				int index = SendMessage((HWND)hListFriends, LB_GETCARETINDEX, 0, 0);
				SendMessage((HWND)hListFriends, LB_GETTEXT, (LPARAM)index, (WPARAM)buffer);
				SetWindowText(hEdit_chatbox, "");
			}
			break;
		case BTN_ADD: {

			string userAdd = getTextFromEdit(hwndCreateTeam, EDIT_USERNAME_CR);
			SetWindowText(hEdit_username_cr, "");
			if (!checkUserInListFriend(userAdd)) {
				MessageBox(NULL, "This friend is not online now!", "not found!", MB_OK);
				break;
			}
			else {
				if (checkUserInListUserTemp((char*)userAdd.c_str())) {
					MessageBox(NULL, "This friend has added!", "OK", MB_OK);
					break;
				}
				SendMessage(hListUserAdded, LB_RESETCONTENT, 0, 0);
				
				listUserAddedTemp.push_back(userAdd);
				for (int i = 0; i < listUserAddedTemp.size(); i++)
					SendMessage(hListUserAdded, LB_ADDSTRING, 0, (LPARAM)listUserAddedTemp[i].c_str());
			}
		}
			break;
		case BTN_CREATE:
			string nameTeam = getTextFromEdit(hwndCreateTeam, EDIT_TEAMNAME);
			if (nameTeam.empty()) {
				MessageBox(NULL, "you haven't typed name Team.", "error", MB_OK);
				break;
			}
			else {
				myClient->requestMakeTeam(listUserAddedTemp.size(), (char*)nameTeam.c_str(), listUserAddedTemp);
				DestroyWindow(hwndCreateTeam);
				break;
			}
		
		}
	}
	return DefWindowProc(hwndCreateTeam, message, wParam, lParam);
}

LRESULT CALLBACK windowProcessForWindowLeaveTeam(HWND hwndLeaveTeam, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		windowLeaveTeamOpen = false;
		break;
	case WM_CREATE:
		windowLeaveTeamOpen = true;	
		addControlLeaveTeam(hwndLeaveTeam);
		break;
	case WM_CLOSE:
		DestroyWindow(hwndLeaveTeam);
		break;
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case BTN_LEAVE:
			string teamName = getTextFromEdit(hwndLeaveTeam, EDIT_TEAMNAME_TO_LEAVE);
			if (teamName.empty()) {
				MessageBox(NULL, "you haven't typed anything yet!", "empty!", MB_OK);
			}
			else {
				myClient->leaveTeamRequest((char*)teamName.c_str());
				if (teamName.compare(teamChating) == 0) {
					teamChating.clear();
					SetWindowText(hStaticChating, "");
				}
				DestroyWindow(hwndLeaveTeam);
			}
			break;
		}
	}
	return DefWindowProc(hwndLeaveTeam, message, wParam, lParam);
}

/* create menu option (navigation bar)*/
void createMenu(HWND hwnd) {
	hMenu = CreateMenu();
	HMENU hOptionMenu = CreateMenu();
	AppendMenu(hOptionMenu, MF_STRING, OPTION_MENU_CREATE_TEAM, "Create Team");
	AppendMenu(hOptionMenu, MF_STRING, OPTION_MENU_LEAVE_TEAM, "Exit Team");
	AppendMenu(hOptionMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hOptionMenu, MF_STRING, OPTION_MENU_EXIT, "Exit");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hOptionMenu, "Option");
	SetMenu(hwnd, hMenu);
}

/* has nameUser existed in listUserTemp yet? if yes return true otherwise false*/
bool checkUserInListUserTemp(char* nameUser) {
	for (int i = 0; i < listUserAddedTemp.size(); i++) {
		if (listUserAddedTemp[i].compare(nameUser) == 0)
			return true;
	}
	return false;
}


// this is thread to handle receive thread
void Client::ClientHandler(HWND hwnd)
{
	char recvBuff[BUFF_SIZE];

	while (true)
	{
		int ret = recv(ServerConnection, recvBuff, BUFF_SIZE, NULL);
		recvBuff[ret] = 0;
		char notification[3];
		getNotification(recvBuff, notification);
		if (strcmp(notification, LOGOUT_NOTIFICATION) == 0) {
			logoutResponse(recvBuff);
			break;
		}

		else if (strcmp(notification, CHAT_WITH_FRIEND_NOTIFICATION) == 0) receiveMessageFromFriend(recvBuff, hwnd);
		else if (strcmp(notification, CHAT_WITH_TEAM_NOTIFICATION) == 0)   receiveMessageFromTeam(recvBuff, hwnd);
		else if (strcmp(notification, MAKE_TEAM_NOTIFICATION) == 0)  makeTeamResponse(recvBuff);
		else if (strcmp(notification, GET_LIST_FRIEND_NOTIFICATION) == 0) getListFriendResponse(recvBuff);
		else if (strcmp(notification, LEAVE_TEAM_NOTIFICATION) == 0) leaveTeamResponse(recvBuff);
		else if (strcmp(notification, FRIEND_LEAVING_TEAM_NOTIFICATION) == 0) friendLeavingTeamResponse(recvBuff, hwnd);
	}
}

// send message to Team that name is teamName
int Client::sendToTeam(char* teamName, char* message) {

	// format of message:    [NOTIFICATION][teamID]|[Message]
	int ret;
	char buff[BUFF_SIZE];
	int teamId = findIdTeam(teamName);
	char c_teamId[128];
	itoa(teamId, c_teamId, 10);
	strcpy(buff, CHAT_WITH_TEAM_NOTIFICATION);
	strcat(buff, c_teamId);
	strcat(buff, "|");
	strcat(buff, message);
	buff[strlen(buff)] = 0;
	// send to server
	ret = send(ServerConnection, buff, strlen(buff), 0);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return -1;
	}
	return 0;
}


// send request leave team that name is teamName
int Client::leaveTeamRequest(char* teamName) {
	// format of message:    [NOTIFICATION][teamID]
	int ret;
	int teamId = findIdTeam(teamName);
	if (teamId == 0) {
		MessageBox(NULL, "Team is not existed!", "error", MB_OK);
		return -1;
	}
	char c_teamId[128];
	itoa(teamId, c_teamId, 10);
	char buff[BUFF_SIZE];
	strcpy(buff, LEAVE_TEAM_NOTIFICATION);
	strcat(buff, c_teamId);
	buff[strlen(buff)] = 0;
	//MessageBox(NULL, buff, "ok test leave", MB_OK);
	// send to server
	ret = send(myClient->ServerConnection, buff, strlen(buff), 0);
	//MessageBox(NULL, "DA GUI ROI NHE", "ok test leave", MB_OK);
	if (ret == SOCKET_ERROR) {
		printf("Error! cannot send message. %d ", WSAGetLastError());
		return -1;
	}

	return 0;
}
