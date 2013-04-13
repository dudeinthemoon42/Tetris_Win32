#define WIN32_LEAN_AND_MEAN 

#include <windows.h> 
#include "Piece.h"
#include "Board.h"



#define WINDOW_CLASS_NAME ÒWINCLASS1Ó
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400


#define BALL_RADIUS 30


HWND main_window_handle = NULL; // save the window handle
HPEN red_pen, black_pen; // pens to draw with


LRESULT CALLBACK WindowProc (HWND hwnd, 
UINT msg, 
WPARAM wparam, 
LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT ps; // used in WM_PAINT
	HDC hdc; // handle to a device context
	
	// what is the message 
	switch(msg)
	{ 
		case WM_CREATE: 
		{
			// do initialization stuff here
			// make the pens
			red_pen = CreatePen (PS_SOLID,0, RGB(255,0,0));
			black_pen = CreatePen (PS_SOLID,0, RGB(0,0,0));
			return(0);
		} break;
		
		case WM_PAINT: 
		{
			// validate the window
			hdc = BeginPaint(hwnd,&ps); 
			EndPaint(hwnd,&ps);
			return(0);
		} break;
		
		case WM_DESTROY: 
		{
			// end the application 
			
			//DONT FORGET TO DELETE THE PENS
			DeleteObject(red_pen);
			DeleteObject(black_pen);
			
			PostQuitMessage(0);
			return(0);
		} break;
		
		default:break;
		
	} // end switch
	
	// process any messages that we didn't take care of 
	return (DefWindowProc (hwnd, msg, wparam, lparam));

} // end WinProc


int WINAPI WinMain (HINSTANCE hinstance,
HINSTANCE hprevinstance,
LPSTR lpcmdline,
int ncmdshow)
{

	WNDCLASS winclass; // this will hold the class we make
	HWND hwnd; // standard window handle
	MSG msg; // standard message
	HDC hdc; // standard dc
	PAINTSTRUCT ps; // standard paintstruct
	RECT rect; // standard rectangle
	HBRUSH hbrush; // standard brush handle
	
	int Ball_x = WINDOW_WIDTH/2, // initial position of ball
	Ball_y = WINDOW_WIDTH/2;
	
	int dx = 4, // initial velocity of ball 
	dy = 2;
	
	// first fill in the window class stucture
	winclass.style = CS_DBLCLKS | CS_OWNDC | 
	CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = "WindowClassName";
	
	// register the window class
	if (! RegisterClass(&winclass))
		return(0);
	
	// make the window
	hwnd = CreateWindowEx(WS_EX_CONTROLPARENT, "WindowClassName", "Keegan", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 300, 200, 500, 700, NULL, NULL, GetModuleHandle(NULL), NULL);
	// save the window handle in a global
	main_window_handle = hwnd;
	
	Board brd (15, 20, 30, 25, 35);
	
	// enter main event loop
	while(1)
	{
		if (PeekMessage(&msg, NULL,0,0, PM_REMOVE))
		{ 
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;
			
			// translate any accelerator keys
			TranslateMessage(&msg);
			
			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if
		
		//Begin main programming
		
		// get the graphics device context 
		hdc = GetDC(hwnd);
		
		brd.display(hdc);
		
		// fill in rect structure
		//rect.left = Ball_x - BALL_RADIUS;
		//rect.right = Ball_x + BALL_RADIUS;
		//rect.top = Ball_y - BALL_RADIUS;
		//rect.bottom= Ball_y + BALL_RADIUS;
		
		// erase the ball
		SelectObject (hdc, GetStockObject(BLACK_BRUSH));
		//SelectObject (hdc, black_pen);
		//Ellipse (hdc, rect.left, rect.top, rect.right, rect.bottom);
		
		// fill in rect structure with new position
		//rect.left = Ball_x - BALL_RADIUS;
		//rect.right = Ball_x + BALL_RADIUS;
		//rect.top = Ball_y - BALL_RADIUS;
		//rect.bottom= Ball_y + BALL_RADIUS;
		
		// draw the ball
		//SelectObject (hdc, red_pen);
		//Ellipse (hdc, rect.left, rect.top, rect.right, rect.bottom);
		
		// release the device context
		ReleaseDC(hwnd, hdc);
		
		// slow things down a bit
		Sleep(100);
	
	} // end while
	
	// return to Windows like this
	return (msg.wParam);

} // end WinMain