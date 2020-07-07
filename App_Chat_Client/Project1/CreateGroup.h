#pragma once

#define EDIT_USERNAME_CR 501
#define EDIT_NAMEGROUP 502
#define BTN_CREATE 503
#define BTN_ADD 504
#define LB_USER_ADDED 505

bool wcCreateGroupRegisteredbefore = false;
LRESULT CALLBACK windowprocessforwindowCreateGroup(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);

HWND hEdit_username_cr;
HWND hEdit_nameGroup;
HWND hListUserAdded;

void createwindowCreateGroup(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd) {
	if (wcCreateGroupRegisteredbefore == false) {
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInst;
		wc.lpfnWndProc = (WNDPROC)windowprocessforwindowCreateGroup;
		wc.lpszClassName = "window class Create Group";
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wc))
		{
			int nResult = GetLastError();
			MessageBox(NULL, "Window class creation failed", "Window Class Creation Failed", MB_ICONERROR);
		}
		else
			wcCreateGroupRegisteredbefore = true;
	}
	hwnd = CreateWindowEx(NULL, wc.lpszClassName, "Window CreateGroup", WS_OVERLAPPEDWINDOW, 600, 70, 640, 280,
		NULL, NULL, hInst, NULL);

	if (!hwnd)
	{
		int nResult = GetLastError();
		MessageBox(NULL, "Window Create Group creation failed", "Window Creation Failed", MB_ICONERROR);
	}

	ShowWindow(hwnd, nShowCmd);
}

void addControlCreateGroup(HWND hwnd) {

	hListUserAdded = CreateWindowW(L"listbox", L"List user added", WS_VISIBLE | WS_CHILD, 30, 24, 230, 260, hwnd,
		(HMENU)LB_USER_ADDED, NULL, NULL);

	CreateWindowW(L"STATIC", L"add user", WS_VISIBLE | WS_CHILD, 300, 26, 200, 30, hwnd,
		NULL, NULL, NULL);
	hEdit_username_cr = CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 300, 57, 200, 30, hwnd,
		(HMENU)EDIT_USERNAME_CR, NULL, NULL);        // pointer not needed 
	CreateWindowW(L"button", L"ADD", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 510, 57, 50, 33, hwnd,
		(HMENU)BTN_ADD, NULL, NULL);

	CreateWindowW(L"static", L"group name: ", WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 100, 200, 25, hwnd,
		NULL, NULL, NULL);
	hEdit_nameGroup = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 300, 128, 200, 25, hwnd,
		(HMENU)EDIT_NAMEGROUP, NULL, NULL);

	CreateWindowW(L"button", L"CREATE", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 350, 165, 90, 40, hwnd,
		(HMENU)BTN_CREATE, NULL, NULL);
}