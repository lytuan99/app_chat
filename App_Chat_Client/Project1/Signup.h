#pragma once
#define BTN_SIGNUP 201
#define BTN_BACK_LOGIN 202
#define EDIT_SU_USERNAME 203
#define EDIT_SU_PASSWORD 204
#define EDIT_SU_REPASSWORD 205
#define WD_SIGNUP 222
HWND hwnd_signup;
HWND hEdit_username_su;
HWND hEdit_password_su;
HWND hEdit_repassword_su;
LRESULT CALLBACK windowprocessforwindowSignup(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);

bool wcSignupRegisteredbefore = false;

int signUpCheck(char *username, char *pass, char *repass) {

	if (strlen(username) > 0 && strlen(pass) > 0 && strlen(repass) > 0 && strcmp(pass, repass) == 0)
		return 1;
	return 0;
}


void createwindowSignup(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd) {
	if (wcSignupRegisteredbefore == false) {
		ZeroMemory(&wc, sizeof(WNDCLASSEX));
		wc.cbClsExtra = NULL;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = NULL;
		wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInst;
		wc.lpfnWndProc = (WNDPROC)windowprocessforwindowSignup;
		wc.lpszClassName = "window class Signup";
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		if (!RegisterClassEx(&wc))
		{
			int nResult = GetLastError();
			MessageBox(NULL, "Window class Sign up creation failed", "Window Class Failed", MB_ICONERROR);
		}
		else
			wcSignupRegisteredbefore = true;
	}
	hwnd_signup = hwnd = CreateWindowEx(NULL, wc.lpszClassName, "Window Sign up", WS_OVERLAPPEDWINDOW, 200, 170, 420, 360, NULL,NULL , hInst, NULL);

	if (!hwnd)
	{
		int nResult = GetLastError();
		MessageBox(NULL, "Window Sign up creation failed", "Window Creation Failed", MB_ICONERROR);
	}

	ShowWindow(hwnd, nShowCmd);
}

void AddControlSignup(HWND hwnd) {
	CreateWindowW(L"STATIC", L" S I G N U P ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 131, 20, 80, 25, hwnd,
		NULL, NULL, NULL);
	// username
	CreateWindowW(L"STATIC", L"userName: ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 68, 57, 90, 25, hwnd,
		NULL, NULL, NULL);
	hEdit_username_su = CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 57, 140, 25, hwnd,
		(HMENU)EDIT_SU_USERNAME, NULL, NULL);
	// password
	CreateWindowW(L"static", L"password: ", WS_VISIBLE | WS_CHILD | WS_BORDER, 68, 92, 90, 25, hwnd,
		NULL, NULL, NULL);
	hEdit_password_su = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 160, 92, 140, 25, hwnd,
		(HMENU)EDIT_SU_PASSWORD, NULL, NULL);
	// re password

	CreateWindowW(L"static", L"re-password: ", WS_VISIBLE | WS_CHILD | WS_BORDER, 68, 123, 90, 25, hwnd,
		NULL, NULL, NULL);
	hEdit_repassword_su = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 160, 123, 140, 25, hwnd,
		(HMENU)EDIT_SU_REPASSWORD, NULL, NULL);
	CreateWindowW(L"Button", L"Sign up", WS_VISIBLE | WS_CHILD | WS_BORDER, 155, 160, 80, 30, hwnd,
		(HMENU)BTN_SIGNUP, NULL, NULL);

	CreateWindowW(L"static", L"you have had account?  => ", WS_VISIBLE | WS_CHILD | WS_BORDER, 110, 250, 180, 30, hwnd,
		NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Login", WS_VISIBLE | WS_CHILD | WS_BORDER, 305, 250, 80, 30, hwnd,
		(HMENU)BTN_BACK_LOGIN, NULL, NULL);

}






