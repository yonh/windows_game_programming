#define WIN32_LEAN_AND_MEAN

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define WINDOW_CLASS_NAME "WINCLASS1"

HINSTANCE hinstance_app      = NULL; // globally track hinstance

#define WINDOW_WIDTH  400
#define WINDOW_HEIGHT 300


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
								WINDOW_WIDTH,WINDOW_HEIGHT, ///window height,width
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


	// enter main event loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		HPEN   pen_color   = CreatePen(PS_SOLID, 1, RGB(rand()%256,rand()%256,rand()%256));
		HBRUSH brush_color = CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
		// select them into dc
		SelectObject(hdc,pen_color);
		SelectObject(hdc,brush_color);


		int point_count = 3 + rand()%8;
		POINT point_list[10];
		for(int i=0; i<point_count; i++)
		{
			point_list[i].x = rand()%WINDOW_WIDTH;
			point_list[i].y = rand()%WINDOW_HEIGHT;
		}
		Polygon(hdc, point_list, point_count);

		Sleep(800);
	
		ReleaseDC(hwnd, hdc);



	
		//translate any accelerator keys
		TranslateMessage(&msg);

		//send the message to the window proc
		DispatchMessage(&msg);
	}

	// return to Windows like this
	return msg.wParam;
}


