#pragma once
LRESULT CALLBACK windowprocessforwindowChatbox(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);

#define EDIT_CHAT_BOX 301
#define BTN_SEND 303
#define BTN_REFRESH_FRIEND 304
#define BTN_REFRESH_GROUP 305
#define EDIT_TYPING_MESS 306
#define BTN_CREATE_GROUP 307
#define PROFILE_NAME 308
#define CHAT_BOX 309
#define LB_GROUP 310
#define LB_FRIEND 311
#define BTN_LOGOUT 312
#define BTN_EXIT_CHAT_BOX 313
#define EDIT_CHATING_NOW 314

HWND hEdit_chatbox;
HWND hEdit_username_profile;
HWND hEdit_typingMessage;
HWND hListGroups;
HWND hListFriends;
HWND hStaticChating;


bool wcChatboxRegisteredbefore = false;
void createwindowChatbox(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd) {
	if (wcChatboxRegisteredbefore == false) {
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInst;
		wc.lpfnWndProc = (WNDPROC)windowprocessforwindowChatbox;
		wc.lpszClassName = "window class Chat box";
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wc))
		{
			int nResult = GetLastError();
			MessageBox(NULL, "Window class Chat box creation failed", "Window Class Failed", MB_ICONERROR);
		}
		else
			wcChatboxRegisteredbefore = true;
	}
	hwnd = CreateWindowEx(NULL, wc.lpszClassName, "Window Chat box", WS_OVERLAPPEDWINDOW, 200, 150, 640, 440, NULL, NULL, hInst, NULL);

	if (!hwnd)
	{
		int nResult = GetLastError();
		MessageBox(NULL, "Window Chat box creation failed", "Window Creation Failed", MB_ICONERROR);
	}
	ShowWindow(hwnd, nShowCmd);
}

void addControlChatbox(HWND hwnd) {

	CreateWindowW(L"STATIC", L" CHAT BOX ", WS_VISIBLE | WS_CHILD| SS_CENTER, 110, 20, 80, 50, hwnd,
		NULL, NULL, NULL);
	hEdit_chatbox = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD|WS_BORDER | ES_MULTILINE| 
												ES_READONLY| ES_LEFT | WS_VSCROLL| ES_AUTOVSCROLL,
																	7, 45, 266, 236, hwnd, (HMENU)CHAT_BOX, NULL, NULL);
	// list box group
	CreateWindowW(L"STATIC", L" online groups ", WS_VISIBLE | WS_CHILD | SS_CENTER, 295, 20, 150, 18, hwnd,
		NULL, NULL, NULL);
	hListGroups = CreateWindowW(L"listbox", L"IDC_GROUP", WS_VISIBLE | WS_CHILD | WS_BORDER| ES_AUTOHSCROLL  | LBS_NOTIFY,
			290, 45, 141, 135, hwnd, (HMENU)LB_GROUP, NULL, NULL);	
	CreateWindowW(L"BUTTON", L"Refresh", WS_VISIBLE | WS_CHILD | SS_CENTER, 340, 182, 70, 27, hwnd,
		(HMENU)BTN_REFRESH_GROUP, NULL, NULL);

	// list box friends
	CreateWindowW(L"STATIC", L" online friends ", WS_VISIBLE | WS_CHILD | SS_CENTER, 462, 20, 150, 18, hwnd,
		NULL, NULL, NULL);
	hListFriends = CreateWindowExW(NULL, L"listbox", L"IDC_FRIEND", WS_VISIBLE | WS_CHILD |WS_BORDER |WS_VSCROLL| LBS_NOTIFY,
			466, 45, 141, 135, hwnd, (HMENU)LB_FRIEND, NULL, NULL);	
	CreateWindowW(L"BUTTON", L"Refresh", WS_VISIBLE | WS_CHILD | SS_CENTER| LBS_NOTIFY, 530, 182, 70, 27, hwnd,
		(HMENU)BTN_REFRESH_FRIEND, NULL, NULL);


	CreateWindowW(L"STATIC", L"--------------------------------------------------------------------",
		WS_VISIBLE | WS_CHILD | SS_CENTER, 302, 215, 252, 10, hwnd, NULL, NULL, NULL);

	CreateWindowW(L"STATIC", L"user name ", WS_VISIBLE | WS_CHILD | SS_CENTER, 390, 232, 80, 20, hwnd,
		NULL, NULL, NULL);
	hEdit_username_profile = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER| ES_READONLY,
						469, 233, 140, 30, hwnd, (HMENU)PROFILE_NAME, NULL, NULL);
	CreateWindowW(L"BUTTON", L"Logout", WS_VISIBLE | WS_CHILD | SS_CENTER, 542, 272, 60, 25, hwnd,
		(HMENU)BTN_LOGOUT, NULL, NULL);

	hEdit_typingMessage = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_BORDER | WS_CHILD | WS_EX_LEFTSCROLLBAR, 7, 293, 266, 50, hwnd,
		(HMENU)EDIT_TYPING_MESS, NULL, NULL);
	CreateWindowW(L"BUTTON", L"SEND", WS_VISIBLE | WS_CHILD | SS_CENTER, 277, 293, 50, 50, hwnd,
		(HMENU)BTN_SEND, NULL, NULL);

	/*CreateWindowW(L"BUTTON", L"CREATE GROUP", WS_VISIBLE | WS_CHILD | SS_CENTER, 331, 293, 120, 50, hwnd,
		(HMENU)BTN_CREATE_GROUP, NULL, NULL);*/
	

	CreateWindowW(L"BUTTON", L"Exit", WS_VISIBLE | WS_CHILD | SS_CENTER, 542, 315, 55, 30, hwnd,
		(HMENU)BTN_EXIT_CHAT_BOX, NULL, NULL);

	CreateWindowW(L"static", L"you are chating with:", WS_VISIBLE | WS_CHILD | SS_CENTER, 7, 348, 150, 30, hwnd,
		NULL, NULL, NULL);
	hStaticChating = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, 159, 348, 100, 30, hwnd,
		(HMENU)EDIT_CHATING_NOW, NULL, NULL);




}