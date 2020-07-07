#pragma once

#define BTN_LOGIN 101
#define BTN_REGISTER 202
#define EDIT_USERNAME 103
#define EDIT_PASSWORD 104
#define WD_LOGIN 111
HWND hwnd_login;
HWND hEdit_username;
HWND hEdit_password;
LRESULT CALLBACK windowprocessforwindowLogin(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);
bool wcLoginRegisteredbefore = false;


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
	wc.lpfnWndProc = (WNDPROC)windowprocessforwindowLogin;
	wc.lpszClassName = "window class Login";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc))
	{
		int nResult = GetLastError();
		/*MessageBox(NULL, "Window class Login creation failed", "Window Class Failed", MB_ICONERROR);*/
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


	/*hEdit_out = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 150, 170, 220, 100, hwnd,
		NULL, NULL, NULL);*/

}






//int createWindow(HINSTANCE hInstance, int nCmdShow) {
//	// BƯỚC 2: Sử dụng window class trên để tạo cửa sổ:
//	HWND hwnd;
//	hwnd = CreateWindowW(
//		L"myWindowClass",				//tên của Class, để hệ thống hiểu được ta đang thao tác với Class nào
//		L"CHAT",		//Tiêu đề(Title Bar hoặc Caption ở bài viết 1).
//		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// một kiểu của window style
//		100,		//	tọa độ X, Y khi phần mềm bắt đầu(góc bên trái trên cùng
//		100,	//		), giá trị CW_USEDEFAULT là để hệ thống tự chọn giá trị cho nó
//		540,	// chiều rộng và chiều cao của window.
//		420,	//chiều rộng và chiều cao của window.
//		NULL,	//Cửa sổ “cha” của cửa sổ này.Khi đi sâu vào loạt bài viết này thì sẽ có khái niệm cha và con, hiểu đơn giản, trong một cửa sổ có nhiều thành phần ví dụ : 1 cửa sổ, trong cửa sổ đó có 1 nút thì nút đó là “con” của cửa sổ còn cửa sổ đó là “cha” của nút.Ở đây vì đây là cửa sổ đầu tiên(Top level) nên giá trị này là NULL.
//		NULL,	//handle của menu chương trình, cài NULL vì chưa có(tìm hiểu sau).
//		hInstance,	//Instance của chương trình.
//		NULL);	//là con trỏ, dùng để gửi các thông tin bổ sung(tìm hiểu sau).
//
//	if (hwnd == NULL)
//	{
//		return 0;
//	}
//
//	ShowWindow(hwnd, nCmdShow);
//	UpdateWindow(hwnd);
//
//	MSG msg = { 0 };
//	while (GetMessage(&msg, NULL, NULL, NULL) > 0) {
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//	return 1;
//}
//
//int registerClass(HINSTANCE hInstance) {
//	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW); //tương tự như icon nhưng là background.
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  //tương tự như Icon nhưng ở đây là con trỏ.
//	wc.hInstance = hInstance;	//Quản lý thông tin của cửa sổ, tương đương với giá trị ta khai báo ở WinMain()
//	wc.lpszMenuName = NULL;							//  con trỏ trỏ về dữ liệu của các thanh menu
//	wc.lpszClassName = L"myWindowClass";
//	wc.lpfnWndProc = WndProc;		// thủ tục của window (window procedure) là con trỏ trỏ tới WinProc như ở trên
//
//	if (!RegisterClassW(&wc)) {
//		return 0;
//	}
//	return 1;
//}
//
//int LoginWindow(HINSTANCE hInstance, int nCmdShow) {
//
//	int checkRegister = registerClass(hInstance);
//	if (checkRegister == 0) {
//		MessageBox(NULL, "Cannot Resgister window ", "Error", MB_ICONEXCLAMATION | MB_OK);
//		return 0;
//	}
//	int checkCreate = createWindow(hInstance, nCmdShow);
//	if (checkCreate == 0) {
//		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
//		return 0;
//	}
//}



