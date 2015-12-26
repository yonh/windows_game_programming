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
	case WM_PAINT:
		//该消息在窗口重画时发送
		hdc = BeginPaint(hwnd, &ps);

		SetTextColor(hdc, RGB(0,0,255));
		SetBkColor(hdc, RGB(0,0,0));
		SetBkMode(hdc, OPAQUE);

		sprintf(buffer, "WM_PAINT called %d times ", ++wm_paint_count);
		TextOut(hdc, 0,0, buffer, strlen(buffer));
		

		EndPaint(hwnd, &ps);

		/*MessageBox(NULL, "Window is repaint!!!",
                 "Title",
                  MB_OK | MB_ICONEXCLAMATION);*/

		return 0;
		break;
	case WM_ACTIVATE:
		//MessageBox(NULL, "al", "sdf", MB_OK);
		break;
	case WM_MOVE: // when window position is change
		int x, y;
		x = LOWORD(lparam);
		y = HIWORD(lparam);

		sprintf(buffer, "event: WM_MOVE ( %d, %d )\n", x, y);
		OutputDebugString(buffer);
		
		//sprintf(buffer, "window position is change \n x: %d, y: %d", x, y);
		//MessageBox(NULL, buffer, "", MB_OK);

		//ReleaseDC(hwnd, hdc);

		break;

	case WM_CHAR:
		int ascii_code,key_state;
		ascii_code = wparam;
		key_state = lparam;
		sprintf(buffer, "ascii: %c - %d, key_state: %X\n", ascii_code, ascii_code, key_state);
		OutputDebugString(buffer);

		#define ALT_STATE_BIT 0x20000000
		if (key_state & ALT_STATE_BIT)
		{
			OutputDebugString("alt is pressed\n");
		}

		break;
	case WM_KEYDOWN:
		int v_code,k_state;
		v_code = wparam;
		k_state = lparam;
		
		switch (v_code) {
		case VK_RIGHT:
			OutputDebugString("right is pressed\n");
			break;
		}
		
		break;
	case WM_SIZE : //resize window
		//MessageBox(NULL, "sdf", "dsf", MB_OK);
		int width, height;
		width = LOWORD(lparam);
		height = HIWORD(lparam);
		

		hdc = GetDC(hwnd);

		SetTextColor(hdc, RGB(0,0,255));
		SetBkColor(hdc, RGB(255,0,0));
		SetBkMode(hdc, OPAQUE);

		sprintf(buffer, "(%d,%d)", width, height);
		TextOut(hdc, 0, 40, buffer, strlen(buffer));

		ReleaseDC(hwnd, hdc);

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

	
	COLORREF old_fcolor, old_bcolor;
	int old_tmode;

	HDC hdc = GetDC(hwnd);
	old_fcolor = SetTextColor(hdc, RGB(0, 0, 255));
	old_bcolor = SetBkColor(hdc, RGB(0,0,0));
	old_tmode = SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 20, 20, "hello", strlen("hello"));
	SetTextColor(hdc, old_fcolor);
	SetBkColor(hdc, old_bcolor);
	SetBkMode(hdc, old_tmode);
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


