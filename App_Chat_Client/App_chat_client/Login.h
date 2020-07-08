#pragma once

#define BTN_LOGIN 101
#define BTN_REGISTER 202
#define EDIT_USERNAME 103
#define EDIT_PASSWORD 104
#define WD_LOGIN 111
HWND hwnd_login;
HWND hEdit_username;
HWND hEdit_password;
LRESULT CALLBACK windowProcessForWindowLogin(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
bool wcLoginRegisteredbefore = false;

/* create window login */
void createwindowLogin(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd) {
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hInstance = hInst;
	wc.lpfnWndProc = (WNDPROC)windowProcessForWindowLogin;
	wc.lpszClassName = "window class Login";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc))
	{
		int nResult = GetLastError();
	}

	hwnd_login = hwnd = CreateWindowEx(NULL, wc.lpszClassName, "Login", WS_OVERLAPPEDWINDOW, 600, 240, 360, 280,
		NULL, NULL, hInst, NULL);

	if (!hwnd)
	{
		int nResult = GetLastError();
		MessageBox(NULL, "Window Login creation failed", "Window Creation Failed", MB_ICONERROR);
	}

	ShowWindow(hwnd, nShowCmd);
}

/*draw window login UI*/
void addControlLogin(HWND hwnd) {
	CreateWindowW(L"STATIC", L" L O G I N ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 131, 20, 80, 25, hwnd,
		NULL, NULL, NULL);

	CreateWindowW(L"STATIC", L"userName: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 73, 57, 80, 25, hwnd,
		NULL, NULL, NULL);
	hEdit_username = CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 57, 140, 25, hwnd,
		(HMENU)EDIT_USERNAME, NULL, NULL);        // pointer not needed 
	CreateWindowW(L"static", L"password: ", WS_VISIBLE | WS_CHILD | WS_BORDER, 73, 92, 80, 25, hwnd,
		NULL, NULL, NULL);
	hEdit_password = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 160, 92, 140, 25, hwnd,
		(HMENU)EDIT_PASSWORD, NULL, NULL);

	CreateWindowW(L"Button", L"Login", WS_VISIBLE | WS_CHILD | WS_BORDER, 155, 131, 80, 30, hwnd,
		(HMENU)BTN_LOGIN, NULL, NULL);

	CreateWindowW(L"Button", L"Register", WS_VISIBLE | WS_CHILD | WS_BORDER, 5, 191, 80, 30, hwnd,
		(HMENU)BTN_REGISTER, NULL, NULL);

}










