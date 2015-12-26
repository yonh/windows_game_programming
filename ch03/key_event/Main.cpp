#define WIN32_LEAN_AND_MEAN

#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_CLASS_NAME "WINCLASS1"
HINSTANCE hinstance_app = NULL;
HWND main_window_handle = NULL;
char buffer[80];

#define KEYDOWN(vk_code) GetAsyncKeyState(vk_code) & 0x8000 ? 1:0
#define KEYUP(vk_code) GetAsyncKeyState(vk_code) & 0x8000 ? 1:0


LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT		ps;		// used in WM_PAINT
HDC				hdc;	// handle to a device context
char buffer[80];        // used to print strings

// what is the message 
switch(msg)
	{	
	case WM_CREATE: 
        {
		// do initialization stuff here
        // return success
		return(0);
		} break;
   
	case WM_PAINT: 
		{
		// simply validate the window 
   	    hdc = BeginPaint(hwnd,&ps);	 
        
        // end painting
        EndPaint(hwnd,&ps);

        // return success
		return(0);
   		} break;

	case WM_DESTROY: 
		{

		// kill the application, this sends a WM_QUIT message 
		PostQuitMessage(0);

        // return success
		return(0);
		} break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASSEX winclass; // this will hold the class we create
	HWND hwnd; //generic window handle
	MSG msg; //generic message
	HDC hdc; //graphics device context

	// first fill in the window class stucture
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// save hinstance in global
	hinstance_app = hinstance;

	// register the window class
	if (!RegisterClassEx(&winclass))
	{
		return 0;
	}

	// create the widnow
	if ( !(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME,  // extended style, class
									"GetAsyncKeyState() demo", //title
									WS_OVERLAPPEDWINDOW | WS_VISIBLE,
									0,0, // init x,y
									400,300, //init width,height
									NULL,NULL, // handle to parent, menu
									hinstance, //instance of this application
									NULL)))
	{
		return 0;
	}

	// save main window handle
	main_window_handle = hwnd;

	// enter main event loop, but this time we use PeekMessage()
	// instead of GetMessage() to retrieve message
	while (TRUE) {
		// test if there is a message in queue, if so get it
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
			{
				break;
			}

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		}
		// main game processing goes here
		// get a graphics context
		hdc = GetDC(hwnd);

		// set the foreground color to green
		SetTextColor(hdc, RGB(0,255,0));

		// set the background color to black
		SetBkColor(hdc, RGB(0,0,0));

		// set the transparency mod to OPAQUE
		SetBkMode(hdc, OPAQUE);

		// print out the state of each arrow key
		sprintf(buffer, "Up Arrow: = %d ", KEYDOWN(VK_UP));
		TextOut(hdc, 0, 0, buffer, strlen(buffer));
		
		sprintf(buffer, "Down Arrow: = %d ", KEYDOWN(VK_DOWN));
		TextOut(hdc, 0, 16, buffer, strlen(buffer));

		sprintf(buffer, "Right Arrow: = %d ", KEYDOWN(VK_RIGHT));
		TextOut(hdc, 0, 32, buffer, strlen(buffer));

		sprintf(buffer, "Left Arrow: = %d ", KEYDOWN(VK_LEFT));
		TextOut(hdc, 0, 48, buffer, strlen(buffer));

		// release the dc back
		ReleaseDC(hwnd, hdc);
	}

	return msg.wParam;
}

