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
	case WM_COMMAND:
		// HIWORD(wparam):通知码 0,BN_CLICKED, 1,BN_PAINT, 2,BN_HLITE, 3,BN_UNHILITE, 4,BN_DISABLE, 5,BN_DOUBLECLICKED
		// LOWORD(wparam):子窗口ID
		// lparam: 子窗口句柄

		hdc = GetDC(hwnd);
		SetBkMode(hdc,OPAQUE);
		 // select a random text and background color
         SetTextColor(hdc,RGB(0,255,0));
         SetBkColor(hdc,RGB(128,128,128));
		 // print out the wparam an lparam
         sprintf(buffer, "LOWORD(wparam) = %d, HIWORD(wparam) = %d                    ", 
                 LOWORD(wparam), HIWORD(wparam));
         // print text at a random location
         TextOut(hdc,10,300,buffer,strlen(buffer));           
         sprintf(buffer, "LOWORD(lparam) = 0X%X, HIWORD(lparam) = 0X%X                     ", 
                 LOWORD(lparam), HIWORD(lparam));
         // print text at a random location
         TextOut(hdc,10,320,buffer,strlen(buffer));
		 sprintf(buffer, "lparam = %d", lparam);
		 TextOut(hdc, 10,340, buffer, strlen(buffer));

		ReleaseDC(hwnd, hdc);
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



	// create the buttons
#define BUTTON_BASE_ID   100
#define NUM_BUTTONS      8

char *button_names[NUM_BUTTONS] = 
     {
     "PUSHBUTTON", 	
     "RADIOBUTTON", 	
     "CHECKBOX", 	
     "3STATE",  	
     "AUTO3STATE", 	
     "AUTOCHECKBOX", 	
     "AUTORADIOBUTTON",	
     "OWNERDRAW"};

long button_types[NUM_BUTTONS] = 
     {
     BS_PUSHBUTTON,
     BS_RADIOBUTTON,
     BS_CHECKBOX,
     BS_3STATE,  	
     BS_AUTO3STATE,
     BS_AUTOCHECKBOX, 	
     BS_AUTORADIOBUTTON,
     BS_OWNERDRAW
     };

	for (int button = 0; button < NUM_BUTTONS; button++)
	{
	CreateWindowEx(NULL,                  // extended style
				   "button",              // class
					button_names[button], // title
	   				WS_CHILD | WS_VISIBLE | button_types[button],
					10,10+button*36,	  // initial x,y
					strlen(button_names[button])*16,24,    // initial width, height
					hwnd,	  // handle to parent 
					(HMENU)(BUTTON_BASE_ID + button),	  // handle to menu
					hinstance,  // instance of this application
					NULL);      // extra creation parms

	} // end for button
	

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
	}
	ReleaseDC(hwnd, hdc);

	// return to Windows like this
	return msg.wParam;
}


