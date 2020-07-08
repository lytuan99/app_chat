#pragma once

#define BTN_LEAVE 701
#define EDIT_TEAMNAME_TO_LEAVE 702

LRESULT CALLBACK windowProcessForWindowLeaveTeam(HWND handleforwindowLeaveGroup, UINT message, WPARAM wParam, LPARAM lParam);

HWND hEdit_leaveTeam;

/* create window leave team*/
void createwindowLeaveTeam(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd) {
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInst;
		wc.lpfnWndProc = (WNDPROC)windowProcessForWindowLeaveTeam;
		wc.lpszClassName = "window class Leave team";
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wc))
		{
			int nResult = GetLastError();
		}
		
	hwnd = CreateWindowEx(NULL, wc.lpszClassName, "Window LeaveGroup", WS_OVERLAPPEDWINDOW, 400, 70, 440, 180,
		NULL, NULL, hInst, NULL);

	if (!hwnd)
	{
		int nResult = GetLastError();
	}

	ShowWindow(hwnd, nShowCmd);
}

/*draw UI leave team*/
void addControlLeaveTeam(HWND hwnd) {
	CreateWindowW(L"static", L"Enter group name you want to leave: ", WS_VISIBLE | WS_CHILD, 70, 40, 280, 30, hwnd,
		NULL, NULL, NULL);
	hEdit_leaveTeam = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 70, 71, 230, 25, hwnd,
		(HMENU)EDIT_TEAMNAME_TO_LEAVE, NULL, NULL);
	CreateWindowW(L"button", L"LEAVE", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 120, 95, 90, 40, hwnd,
		(HMENU)BTN_LEAVE, NULL, NULL);
}