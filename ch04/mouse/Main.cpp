#define WIN32_LEAN_AND_MEAN

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define WINDOW_CLASS_NAME "WINCLASS1"

HINSTANCE hinstance_app      = NULL; // globally track hinstance

#define KEYDOWN(vk_code) GetAsyncKeyState(vk_code) & 0x8000 ? 1:0
#define KEYUP(vk_code) GetAsyncKeyState(vk_code) & 0x8000 ? 1:0


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam,	LPARAM lparam)
{
	// this is the main message handler of the system 
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;// rectangle of window 使窗口有效
	static int wm_paint_count = 0;
	char buffer[80];

	switch (msg)
	{
	case WM_CREATE:
		// do initialization stuff here
		/*MessageBox(NULL, "Window is created!!!",
                 "Title",
                  MB_OK | MB_ICONEXCLAMATION);*/

		//hdc = BeginPaint(hwnd, &ps);
		//你可以在这里实现画图，但是你只能访问实际上需要重绘的部分
		//EndPaint(hwnd, &ps);

		if (!(hdc = GetDC(hwnd))) ERROR();
		ReleaseDC(hwnd, hdc);

		// get client rectangle of window - use win32 call
		GetClientRect(hwnd, &rect);
		// validate window
		ValidateRect(hwnd, &rect);

		return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		
		int result;
		result = MessageBox(hwnd, "Are you sure quit this application?", "Close", MB_YESNO | MB_ICONQUESTION);

		if (result == IDYES)
		{
			PostQuitMessage(0);
			return DefWindowProc(hwnd, msg, wparam, lparam);
		} else {
			return 0;
		}
		// kill the application, this sends a WM_QUIT message
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	hinstance_app = hinstance;
	WNDCLASSEX winclass;
	HWND hwnd;//window handle
	MSG msg;

	winclass.cbSize = sizeof(winclass);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//winclass.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//winclass.hCursor = LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR2));
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH) ;
	//winclass.lpszMenuName = NULL;
	winclass.lpszMenuName =  NULL;//MAKEINTRESOURCE(IDR_MENU1);// use menu
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//register the window class
	if (!RegisterClassEx(&winclass)) return 0;

	// create the window, bail if problem
	if (!(hwnd = CreateWindowEx(NULL, // exended style
								WINDOW_CLASS_NAME, //class
								"Basic Window", // title
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								0,0, // x,y
								400,400, ///window height,width
								NULL, // handle to parent
								NULL, // handle to menu
								hinstance,
								NULL)))
	{
		return 0;
	}

	HDC hdc = GetDC(hwnd);
	int index,x,y;

	// draw 1000 points
	for (index=0; index<1000; index++)
	{
		x = rand() % 400;
		y = rand() % 400;
		COLORREF color = RGB(rand()%255, rand()%255, rand()%255);
		SetPixel(hdc, x, y, color);
	}

	// draw a line
	POINT last_pos; // used to store last position
	HPEN pen, old_pen;


	pen = CreatePen(PS_SOLID, 2, RGB(255,0,0));

	HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
	SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Rectangle(hdc, 10,10, 80,40);

	RECT rect = {10, 50, 80,70};
	FillRect(hdc, &rect, brush);


	Ellipse(hdc, 10, 80, 10+32, 80+32);
	Ellipse(hdc, 250,175,350,225);

	ReleaseDC(hwnd, hdc);

	// enter main event loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//translate any accelerator keys
		TranslateMessage(&msg);

		//send the message to the window proc
		DispatchMessage(&msg);
	}

	// return to Windows like this
	return msg.wParam;
}


