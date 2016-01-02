#define WIN32_LEAN_AND_MEAN

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
用于理解 fps的概念
*/


#define WINDOW_CLASS_NAME "WINCLASS1"

HINSTANCE hinstance_app      = NULL; // globally track hinstance

#define KEYDOWN(vk_code) GetAsyncKeyState(vk_code) & 0x8000 ? 1:0
#define KEYUP(vk_code) GetAsyncKeyState(vk_code) & 0x8000 ? 1:0

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

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
	MoveToEx(hdc, 10, 10, NULL);
	MoveToEx(hdc, 50, 50, &last_pos);
	HPEN green_pen = CreatePen(PS_SOLID, 1, RGB(0,255,0));
	HPEN old_pen = (HPEN)SelectObject(hdc, ((HGDIOBJ)green_pen));

	

	int x1 = 0, x2 = 0, y1 = 0, y2 = 10;
	int vx = rand()%5, vy = rand()%10,v2x = 5, v2y = 10;


	int i = 0;
	char buffer[20];

	int start_time = GetTickCount();

	// enter main event loop
	while (true)
	{
		int start_time2 = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			//translate any accelerator keys
			TranslateMessage(&msg);
			//send the message to the window proc
			DispatchMessage(&msg);
		}


		if ( GetTickCount() - start_time > 1000)
		{
			continue;
			//break;
		}
		

		// draw a line
		MoveToEx(hdc, x1,y1, NULL);
		LineTo(hdc, x2,y2);
			
		if ( y2<0 || y2 > WINDOW_HEIGHT-40 )
		{
			v2y = -v2y;
		}
			
		if ( x2 < 0 || x2 > WINDOW_WIDTH )
		{
			v2x = -v2x;
		}
		if ( y1<0 || y1 > WINDOW_HEIGHT-40 )
		{
			vy = -vy;
		}
			
		if ( x1 < 0 || x1 > WINDOW_WIDTH )
		{
			vx = -vx;
		}

		x1 +=vx;
		y1 += vy;
		x2 += v2x;
		y2 +=v2y;
		i++;
		sprintf(buffer, "%d, %d\n", i, GetTickCount() - start_time);
		//OutputDebugString(buffer);

		TextOut(hdc, i%2*70, i/2*15, buffer, strlen(buffer));

		while((GetTickCount() - start_time2) < 30) {
			/*sprintf(buffer, "%d, %d\n", i, GetTickCount() - start_time);
			OutputDebugString(buffer);*/
		}
		
	}
	ReleaseDC(hwnd, hdc);

	// return to Windows like this
	return msg.wParam;
}


