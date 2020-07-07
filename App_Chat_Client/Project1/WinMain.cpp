


#include "Client.h"
#include "Login.h"
#include "Signup.h"
#include "Chatbox.h"
#include "CreateGroup.h"
#include "util.h"
#include"Business.h"

using namespace std;

#define _AFXDLL
#define OPTION_MENU_CREATE_GROUP 104
#define OPTION_MENU_EXIT 111

LRESULT CALLBACK windowprocessforwindowMain(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindowLogin(HWND handleforwindow2, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindowSignup(HWND handleforwindow3, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindowChatbox(HWND handleforwindow4, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK windowprocessforwindowCreateGroup(HWND handleforwindow5, UINT message, WPARAM wParam, LPARAM lParam);
bool checkUserInListUserTemp(char* nameUser);

#define BTN_START 1000

bool windowLoginOpen, windowSignupOpen, windowChatboxOpen, windowCreateGroupOpen = false;
bool windowMainOpen = false;


enum windowtoopenenumt {windowMain, windowLogin, windowSignup, windowChatbox, windowCreateGroup };

windowtoopenenumt windowtoopenenum = windowMain;

HWND hwnd;
void createMenu(HWND hwnd);
HMENU hMenu;
Client* myClient;
string nameUser;
string friendChating;
string groupChating;
//struct {
//	string listUserAddedTemp[100];
//	int numberOfMember = 0;
//}groupTemp;

vector<string> listUserAddedTemp;

#define DEFAULT_PORT 27015

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
		else if (strcmp(notification, RECEIVE_FROM_FRIEND_NOTIFICATION) == 0) receiveMessageFromFriend(recvBuff, hwnd);
		else if (strcmp(notification, RECEIVE_FROM_TEAM_NOTIFICATION) == 0)   receiveMessageFromTeam(recvBuff);
		else if (strcmp(notification, MAKE_TEAM_NOTIFICATION) == 0)  makeTeamResponse(recvBuff);
		else if (strcmp(notification, GET_LIST_FRIEND_NOTIFICATION) == 0)	getListFriendResponse(recvBuff);
		else if (strcmp(notification, GET_LIST_TEAM_NOTIFICATION) == 0) getListTeamResponse(recvBuff);
	}
}



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
	WNDCLASSEX wcforwindowCreateGroup;
	WNDCLASSEX wcforwindowLogin;

	/*HWND handleforwindowLogin;
	HWND handleforwindowSignup;
	HWND handleforwindowChatbox;
	HWND handleforwindowCreateGroup;*/

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
	wcforwindowMain.lpfnWndProc = (WNDPROC)windowprocessforwindowMain;
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


	// bắt đầu xử lý các sự kiện
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
					myClient->StartSubRoutine(hwnd);	// mở luồng nhận dữ liệu
				}
				break;
			case windowCreateGroup:
				if (windowCreateGroupOpen == false) {
					createwindowCreateGroup(wcforwindowChatbox, hwnd, hInst, nShowCmd);
				}
				break;
			}
			windowtoopenenum = windowMain;
		}


		if (windowLoginOpen == false && windowSignupOpen == false && windowChatboxOpen == false && windowCreateGroupOpen == false && windowMainOpen == false)
			endloop = true;
	}	// end while

	MessageBox(NULL, "All Windows are closed. Program will now close.", "Message", MB_ICONINFORMATION);

}


// windows process functions proceduce

LRESULT CALLBACK windowprocessforwindowMain(HWND hwndMain, UINT message, WPARAM wParam, LPARAM lParam) {
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
		windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateGroupOpen = false;
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

LRESULT CALLBACK windowprocessforwindowLogin(HWND hwndLogin, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		windowLoginOpen = true;
		createMenu(hwndLogin);
		addControlLogin(hwndLogin);
		break;
	case WM_CLOSE:
		DestroyWindow(hwndLogin);
		windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateGroupOpen = false;
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
			if (status == '1') {
				nameUser.append(nameLog);
				windowtoopenenum = windowChatbox;
				DestroyWindow(hwndLogin);
			}
			else if (status == '2') {
				MessageBox(NULL, "Dang nhap o noi khac!", "ERROR", MB_OK);
			}
			else {
				MessageBox(NULL, "sai mat khau", "ERROR", MB_OK);
			}
			break;

		}
	}

		return DefWindowProc(hwndLogin, message, wParam, lParam);
}

LRESULT CALLBACK windowprocessforwindowSignup(HWND hwndSignup, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:

		windowSignupOpen = true;
		AddControlSignup(hwndSignup);
		/*CreateWindowEx(NULL,"BUTTON","Open Window 3",WS_TABSTOP | WS_VISIBLE |WS_CHILD | BS_DEFPUSHBUTTON,
			50,220,150,24,hwnd,(HMENU)createwindowbuttoninwindow2,GetModuleHandle(NULL),NULL);*/
		break;
	case WM_CLOSE:
		DestroyWindow(hwndSignup);
		windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateGroupOpen = false;
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

					windowtoopenenum = windowLogin;	// chuyển hướng đến trang login
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

LRESULT CALLBACK windowprocessforwindowChatbox(HWND hwndChatbox, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) {
		case WM_CREATE:
			windowChatboxOpen = true;
			createMenu(hwndChatbox);
			addControlChatbox(hwndChatbox);
			SetWindowText(hEdit_username_profile, nameUser.c_str()); // set name profile
			
			break;
		case WM_CLOSE:
			DestroyWindow(hwndChatbox);
			windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateGroupOpen = false;
			break;
		case WM_DESTROY:
			windowChatboxOpen = false;
			break;
		case WM_COMMAND:
			switch LOWORD(wParam) {
			case LB_FRIEND:
				switch (HIWORD(wParam)) {
				case LBN_DBLCLK:			// bắt sự kiện click double trong list Friend
					char buffer[256];
					int index = SendMessage((HWND)hListFriends, LB_GETCARETINDEX, 0, 0);
					SendMessage((HWND)hListFriends, LB_GETTEXT, (LPARAM)index, (WPARAM)buffer);
					if (strlen(buffer) == 0) {
						MessageBox(NULL, "list empty!", "error", MB_OK);
						break;
					}
					friendChating.clear();
					groupChating.clear();
					friendChating.append(buffer);
					string mess = "chat with friend: " + friendChating;
					MessageBox(NULL, mess.c_str(), "turn", MB_OK);
					SetWindowText(hEdit_chatbox, "");
					SetWindowText(hStaticChating, friendChating.c_str());
				}
				break;
			case LB_GROUP:
				switch (HIWORD(wParam)) {
				case LBN_DBLCLK:			// bắt sự kiện click double trong list group
					char Buffer[256] = "";
					int index = SendMessage((HWND)hListGroups, LB_GETCARETINDEX, 0, 0);
					SendMessage((HWND)hListGroups, LB_GETTEXT, (LPARAM)index, (WPARAM)Buffer);
					if (strlen(Buffer) == 0) {
						MessageBox(NULL, "list empty!", "error", MB_OK);
						break;
					}

					friendChating.clear();
					groupChating.clear();
					groupChating.append(Buffer);
					string mess = "chat with group: " + groupChating;
					MessageBox(NULL, mess.c_str(), "turn", MB_OK);
					SetWindowText(hEdit_chatbox, "");
					SetWindowText(hStaticChating, groupChating.c_str());
				}
				break;

			case BTN_SEND: {
				if (friendChating.empty() && groupChating.empty()) {
					MessageBox(NULL, "you haven't selected a friend or group to chat!", "notification", MB_OK);
					break;
				}
				string mess = getTextFromEdit(hwndChatbox, EDIT_TYPING_MESS);
				string messToBox = getMessageToBox(mess, nameUser);
				SendMessage(hEdit_chatbox, EM_REPLACESEL, 0, (LPARAM)messToBox.c_str());
				SetWindowText(hEdit_typingMessage, "");
				myClient->sendToFriend((char*)friendChating.c_str(), (char*)messToBox.c_str());
			}
				break;
			case BTN_CREATE_GROUP:
				windowtoopenenum = windowCreateGroup;
				break;
			case BTN_REFRESH_FRIEND:
				myClient->getListFriendRequest();
				break;
			case BTN_REFRESH_GROUP:
				myClient->getListTeamRequest();
				break;
			case BTN_LOGOUT:
				myClient->logoutRequest();
				windowtoopenenum = windowLogin;
				DestroyWindow(hwndChatbox);
				break;
			case BTN_EXIT_CHAT_BOX:
				DestroyWindow(hwndChatbox);
				windowLoginOpen = windowMainOpen = windowSignupOpen = windowChatboxOpen = windowCreateGroupOpen = false;
				break;
			}
	}
	return DefWindowProc(hwndChatbox, message, wParam, lParam);
}

LRESULT CALLBACK windowprocessforwindowCreateGroup(HWND hwndCreategroup, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		windowCreateGroupOpen = false;
		break;
	case WM_CREATE:
		windowCreateGroupOpen = false;
		addControlCreateGroup(hwndCreategroup);
		listUserAddedTemp.clear();
		break;
	case WM_CLOSE:
		DestroyWindow(hwndCreategroup);
		break;
	case WM_COMMAND:
		switch LOWORD(wParam) {
		case LB_USER_ADDED:
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK:			// bắt sự kiện click double trong list Friend
				char buffer[256];
				int index = SendMessage((HWND)hListFriends, LB_GETCARETINDEX, 0, 0);
				SendMessage((HWND)hListFriends, LB_GETTEXT, (LPARAM)index, (WPARAM)buffer);
				SetWindowText(hEdit_chatbox, "");
			}
			break;
		case BTN_ADD: {

			string userAdd = getTextFromEdit(hwndCreategroup, EDIT_USERNAME_CR);
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
			string nameGroup = getTextFromEdit(hwndCreategroup, EDIT_NAMEGROUP);
			if (nameGroup.empty()) {
				MessageBox(NULL, "you haven't typed name group.", "error", MB_OK);
				break;
			}
			else {
				myClient->requestMakeTeam(listUserAddedTemp.size(), (char*)nameGroup.c_str(), listUserAddedTemp);
				DestroyWindow(hwndCreategroup);
				break;
			}
		
		}
	}
	return DefWindowProc(hwndCreategroup, message, wParam, lParam);
}

void createMenu(HWND hwnd) {
	hMenu = CreateMenu();
	HMENU hOptionMenu = CreateMenu();
	AppendMenu(hOptionMenu, MF_STRING, OPTION_MENU_CREATE_GROUP, "Create group");
	AppendMenu(hOptionMenu, MF_STRING, OPTION_MENU_CREATE_GROUP, "New Friend");
	AppendMenu(hOptionMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hOptionMenu, MF_STRING, OPTION_MENU_EXIT, "Exit");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hOptionMenu, "Option");
	AppendMenu(hMenu, MF_STRING, NULL, "Help");
	SetMenu(hwnd, hMenu);
}

bool checkUserInListUserTemp(char* nameUser) {
	for (int i = 0; i < listUserAddedTemp.size(); i++) {
		if (listUserAddedTemp[i].compare(nameUser) == 0)
			return true;
	}
	return false;
}