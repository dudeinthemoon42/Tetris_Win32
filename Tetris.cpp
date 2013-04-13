#include<windows.h>
#include <iostream>
#include"Piece.h"
#include"Board.h"
#include <sstream>
#include <string.h>
#include <stdlib.h>

#define ID_FILE_EXIT 9002
#define ID_STUFF_GO 9003
#define ID_HELP_ABOUT 9004
#define IDT_TIMER1 (WM_USER + 3)
//assign IDT_TIMER1 a 32-bit hex number starting in the WM_USER memory space (the upper 2GB theoretically in 32-bit Windows)

using namespace std;

typedef VOID (*DLLPROC) (LPTSTR); //this typedef is used to declare/call an exported function name as a string in parentheses

/*a HANDLE is a typdef - it is officially a pointer to a pointer, but the pointer it points to is a specific object type (e.g. window, rect, button, etc)
in a kernel table that uniquely identifies it - this ID just happens to be a 32-bit unsigned int
note: do not try to change a HANDLE's value with arithmetic

imagine it is a typdef pointer to a pointer to a struct in some table, somewhat like a file descriptor
the only way to dereference the object in memory is using the HANDLE and passing it through functions.
e.g. GetDC takes a handle to a window, and a generic device context, then properly sets the device context to be for that window
once the DC is set for that window, all DC operations (using a HANDLE to DC) directly affect the referenced window*/

//forward declarations - function prototypes
LRESULT CALLBACK OurWindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM parameter1,LPARAM parameter2);
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

HWND NewWindow(LPCTSTR str_Title,int int_XPos, int int_YPos, int int_Width, int int_Height);

void ShowFileProperties(HWND hwnd, LPSTR lpszFile);

void getkey();

bool running = true;

int playingtime = 1; //initial playing timg will display after 1 second has passed

char thetime[] = "Playing Time: ";
int size = (sizeof(thetime) / sizeof(char));
char cl_score[] = "Score: ";
int cl_points = 0;
//string Result;
char* buff;
char* timepointer = &(*thetime); //timepointer points to the address of the first element, which is what an array does
char nextP;
Board brd (20, 20, 20, 100, 0);

/*UINT uResult = SetTimer(hWnd,  // handle to main window
         IDT_TIMER1,            // timer identifier
         1000,                 // 1-second interval
         (TIMERPROC) NULL);     // no timer callback

if(uResult == 0)
    {
        MessageBox(NULL, "No timer available", "Error", MB_OK);
    }*/

LRESULT CALLBACK OurWindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM parameter1,LPARAM parameter2)
{ //OurWindowProcedure contains the structure of a message sent from the OS to the window specified as "han_Wind", which includes two parameters
	switch(uint_Message)
	{
	    case WM_CREATE:
	    {
	        cout << "Welcome to Tetris build 10.12.22! This program was developed using Microsoft's\n Win32 GDI API. All rights reserved." << endl;
	        cout << "This window will keep track of your playing time in seconds. Please be patient, as this project is still in Alpha and is buggy. Good luck!" << endl;
	        cout << endl << endl << "HI SOFEH!!!!!! <3" << endl << endl;
	        cout << "Current playing time (in seconds): " << endl;
	        cout << *(timepointer+12);
            HMENU hMenu, hSubMenu;
            //HICON hIcon, hIconSm;

            hMenu = CreateMenu(); //creates the main menu line

            hSubMenu = CreatePopupMenu(); //creates a popupmenu from a particular menu

            AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "E&xit");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING | MF_GRAYED, ID_STUFF_GO, "&Go"); //OR it with the grayed flag
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Stuff");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
            //AppendMenu(hSubMenu, MF_SEPARATOR, NULL, "-");
            AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");

            /* Note that every menu option should come before the main menu is modified
            Because these options are made at runtime, so it operates in runtime order */

            SetMenu(han_Wind, hMenu);

            HINSTANCE hinstDLL;
            DLLPROC dllProc;
            BOOL fFreeDll;

            hinstDLL = LoadLibrary("sampledll.dll");
            if(hinstDLL != NULL)
            {
                dllProc = (DLLPROC) GetProcAddress(hinstDLL, "HelloWorld"); // returns the address of an exported process, the function we named HelloWorld()
                if(dllProc != NULL)
                    (dllProc);
                fFreeDll = FreeLibrary(hinstDLL);
            }
            if(!hinstDLL)
                cout << "Library could not be loaded." << endl << endl;*/

	        break;
	    }

	    case WM_PAINT: //case WM_PAINT identifies that the window has been updated and needs to be repainted
	    {
	        //cout << "Painting" << endl;

	        unsigned h = 0;
	        h = 'a'+'b'+'c'; //ASCII code 97+98+99 = 294, no matter the permutation - which means it is a bad hash algorithm

	        cout << "The hash is: " << h << endl;
	        if(parameter2 != 0) //if there is an included parameter
                cout << (parameter2) << "\tYou have chosen to exit this game. Goodbye!" << endl; //output it
	    	break;
	    }
	    case WM_CLOSE:
	    {
            PostMessage(han_Wind, WM_CLOSE, 0, 0);
	        break;
	    }
	    case WM_DESTROY:
	    {
	        PostMessage(han_Wind, WM_QUIT, 0, 0);
	        break;
	    }
	    case WM_QUIT:
	    {
	        PostQuitMessage (0);
	        break;
	    }
	    case WM_TIMER:
	    {
	        switch (parameter1) //wParam carries timer data within the window's message
	        //wParam usually carries handles or integers, in this case it carries an integer, the IDT_TIMER1 int
	        {
	            case IDT_TIMER1:
	            {
	                cout << " " << playingtime << " sec. " << endl;
                    playingtime++;
                    break;
	            }
	        }
	    }
	    /*case WM_MOUSEWHEEL:
	    {
	        POINT pt;
	        GetCursorPos(&pt);
	        pt.x = 0;
	        pt.y = 0;
	        SetCursorPos(pt.x, pt.y);
	    }*/
        case WM_COMMAND: //WM_COMMAND is the message sent when there are commands clicked in menus, this will help us handle them
        {
            switch(LOWORD(parameter1)) //these messages are within the low order bits of the WPARAM int
            {
                case ID_FILE_EXIT:
                {
                    PostMessage(han_Wind, WM_CLOSE, 0, 0);
                    break;
                }
                case ID_STUFF_GO:
                {
                    /*int value = MessageBox(han_Wind, "This submenu does not currently have any function, as of build 10.12.22", MB_OK | MB_ICONINFORMATION);
                    switch(value):
                    {
                        case IDOK:
                        break;
                    }*/
                    break;
                }

                case ID_HELP_ABOUT:
                {
                    int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(9007), han_Wind, AboutDlgProc); //create our dialog, using the data from our resource file
                    //then handle all incoming messages for the dialog to the dialog procedure, AboutDlgProc
                    //this is a modal
                    if(ret == IDOK)
                    {
                        MessageBox(han_Wind, "Dialog exited with IDOK.", "Notice", MB_OK | MB_ICONINFORMATION);
                    }
                    else if(ret == IDCANCEL)
                    {
                        MessageBox(han_Wind, "Dialog exited with IDCANCEL.", "Notice", MB_OK | MB_ICONINFORMATION);
                    }
                    else if(ret == -1)
                    {
                        MessageBox(han_Wind, "Dialog failed!", "Error", MB_OK | MB_ICONINFORMATION);
                    }
                }

            }
        }
        /*case WM_EXITMENULOOP:
            cout << "RETURNING TO WNDPROC\n";
        break;*/


	}

    return DefWindowProc(han_Wind,uint_Message,parameter1,parameter2); //returns and processes any other messages the WinProc doesn't process
}
HWND NewWindow(LPCTSTR str_Title,int int_XPos, int int_YPos, int int_Width, int int_Height)
{
    WNDCLASSEX wnd_Structure;

    wnd_Structure.cbSize = sizeof(WNDCLASSEX);
    wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;

     wnd_Structure.lpfnWndProc = OurWindowProcedure;
     wnd_Structure.cbClsExtra = 0;
     wnd_Structure.cbWndExtra = 0;
     wnd_Structure.hInstance = GetModuleHandle(NULL);
     wnd_Structure.hIcon = NULL;
     wnd_Structure.hCursor = NULL;
     wnd_Structure.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
     wnd_Structure.lpszMenuName = NULL;
     wnd_Structure.lpszClassName = "WindowClassName";
     wnd_Structure.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

     RegisterClassEx(&wnd_Structure);

     //then return the handle to the window
     return CreateWindowEx(WS_EX_CONTROLPARENT, "WindowClassName", str_Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, int_XPos, int_YPos, int_Width, int_Height, NULL, NULL, GetModuleHandle(NULL), NULL);
 }

 int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
 {

     HWND hWnd = NewWindow("Tetris - Press F1 to Pause, ESC to close",300,50,800,600);

     MSG msg;

     UINT uResult = SetTimer(hWnd,  // handle to main window
     IDT_TIMER1,            // timer identifier
     1000,                 // 1-second interval
     (TIMERPROC) NULL);     // no timer callback

     if(uResult == 0)
     {
         MessageBox(NULL, "No timer available", "Error", MB_OK);
     }

     /*HWND hwndButton = CreateWindow(
        L"BUTTON",   // Predefined class; Unicode assumed.
        L"OK",       // Button text.
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles.
        10,         // x position.
        10,         // y position.
        100,        // Button width.
        100,        // Button height.
        hWnd,       // Parent window.
        NULL,       // No menu.
        (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), //returns the handle to the instance of the window proc
        NULL);      // Pointer not needed.

     bool bResult = Button_Enable(hwndButton, 1); //enable the button

     if(!bResult)
        MessageBox(NULL, "Button not enabled!", "Error", MB_OK);*/

     //int c = 0;
     //COLORREF colour = RGB(0, 0, 0);
     //int messagecounter = 0;
     HDC hdc; //handle to the device context
     PAINTSTRUCT pstruct; //paint data for BeginPaint and EndPaint functions
     //hdc = BeginPaint (hWnd, &pstruct);
     SetTextColor(hdc, RGB(255, 0, 0));

     hdc = GetDC(hWnd); //the handle to the device context now "handles" the "hot" window hWnd - therefore we are handling a graphics context in the main window hWnd
     //in other words, the handle hdc handles the window hWnd, so any changes to the graphics context(e.g. drawing) will apply to this window

     char type = 'o';
     //HANDLE j = hWnd; //proves HANDLES are pointers to pointers

     while(running)
	 {
        //if(uResult)
            //PostMessage(hWnd, WM_PAINT, NULL, NULL); //PostMessage parameters - the window whose wind proc should receive the msg, the msg itself (in this case WM_PAINT), and wParam and lParam
        ShowWindow(hWnd, nCmdShow);

	 	//all functions that are passed hdc will now be making changes to the window
	 	//GetTime
	 	TextOut(hdc, 100, 500, thetime, sizeof(thetime));
	 	cl_points += brd.getpoints();
	 	//ostringstream convert;
	 	//convert << cl_points; //input cl_points into convert
	 	//Result = convert.str();
	 	TextOut(hdc, 250, 500, cl_score, sizeof(cl_score));
	 	//_itoa(cl_points, buff, sizeof(int)); //the int, the pointer to convert to, size of buffer, base (in this case base 10)
        //TextOut(hdc, 200, 500, buff, sizeof(buff));
	 	//SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	 	brd.display(hdc);
	 	type = brd.nextPiece(type);
	 	getkey();
	 	//colour = RGB(255,0,0);
		brd.update();
		brd.scorePoints();
		brd.spawn(type, RGB(255,0,0));

		running = brd.lose();
		//brd.spinRight();
		//running = false;
		Sleep(175);
		//MessageBox(NULL, "hi", NULL, NULL);
	 	//c++;
	 	//if (c==50)
	 	//	running = false;

	 	bool esc = GetAsyncKeyState(VK_ESCAPE)<0;
	 	if (esc) running = false;

	 	bool playsound = GetAsyncKeyState(VK_RBUTTON)<0;
	 	if(playsound)
            Beep(300, 300);

	 	bool showinfo = GetAsyncKeyState(VK_LSHIFT)<0;
	 	if (showinfo)
	 	{
	 	    ShowFileProperties(hWnd, lpcmdline);

	 	    while(showinfo)
	 	    {
                Sleep(100);
                    int MessageBoxID = MessageBox(NULL, "You have paused the game\nDo you wish to resume?", "Game Paused", MB_ICONEXCLAMATION | MB_OKCANCEL | MB_DEFBUTTON1);
                    //messagecounter++;

                    switch(MessageBoxID)
                    {
                        case IDCANCEL:
                        {
                            return 0;
                        }
                        case IDOK:
                        {
                            showinfo = false;
                        }
                    }
                bool doit = GetAsyncKeyState(VK_LSHIFT)<0;
                if(doit)
                    showinfo = false;
	 	    }
	 	}

	 	bool pausegame = GetAsyncKeyState(VK_F1)<0;
	 	if(pausegame)
	 	{
	 	    while(pausegame)
	 	    {
	 	        Sleep(100);
                int MessageBoxID = MessageBox(hWnd, "You have paused the game\nDo you wish to resume?", "Game Paused", MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON1);

                    switch(MessageBoxID)
                    {
                        case IDNO:
                        {
                            DestroyWindow(hWnd);
                        }
                        case IDYES:
                        {
                            pausegame = false;
                        }
                    }
	 	    }
	 	}
	 	//closing the window, moving it, etc. will all send WM_X messages to the message queue, in order of their execution
	 	//PeekMessage peeks at the messages and returns 1 when it copies the data in the front of the queue to a MSG struct, which can be "previewed" by the user and handled
	 	//before being sent to the Message Procedure for handling

	     if(PeekMessage(&msg,hWnd,0,0,PM_REMOVE)) //if it returns as 1, then the message was successfully filled within the MSG structure without being removed from the queue
	     {
	         if(msg.message == WM_QUIT || msg.message == WM_CLOSE)
	         {
	             LPARAM pStr = (LPARAM)"Window Termination"; //LPARAM is a pointer - send it with the message to our message proc
	             SendMessage(hWnd, WM_PAINT, NULL, pStr);
	             MessageBox(hWnd, "Termination Notice", "Event Verified", MB_ICONEXCLAMATION | MB_OK);
	             running = false;
	             return msg.wParam;
	         }
	         /*if(msg.message == WM_PAINT)
	         {
	             cout << "This is happening" << endl;
	         }*/
	         if(!IsDialogMessage(hWnd,&msg)) //if this message isn't meant for a dialog box we've created - only applies if it has a separate dialog proc
	         {
	             TranslateMessage(&msg);
	             DispatchMessage(&msg);
	         }
	         /*if(msg.message == WM_TIMER)
	         {
	             //msg.hwnd = hWnd;
	             //PostThreadMessage;
	             //cout << endl << "Keeping track";
	         }*/
	     }
        //ReleaseDC(hWnd, hdc);
	 }
	 ReleaseDC(hWnd, hdc);
	 //EndPaint (hWnd, &pstruct);


	 //ReleaseDC(hWnd, hdc);
	 //KillTimer(hWnd, IDT_TIMER1);

	 int MsgBoxID = MessageBox(hWnd, "Thanks for playing!\nWould you like to play again?", "Play again?", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1);
        switch(MsgBoxID)
        {
            case IDNO:
            {
                DestroyWindow(hWnd);
            }
            case IDYES:
            {
                //brd.clearboard();
                running = true;
                HWND hwnd = NewWindow("You Fail!", 300, 100, 800, 600);
                Sleep(1000);
            }
        }

     return 0;
 }

 void getkey()
 {

 	bool left;
 	bool right;
  	bool up;

  	left = GetAsyncKeyState (VK_LEFT)<0;
	right = GetAsyncKeyState (VK_RIGHT)<0;
  	up = GetAsyncKeyState (VK_UP)<0;

	if (left)
		brd.moveleft();
	if (right)
		brd.moveright();
	if (up)
		brd.spinRight();

 }

 void ShowFileProperties(HWND hwnd, LPSTR lpszFile)
{
	SHELLEXECUTEINFO ShExecInfo = {0};

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST ;
	ShExecInfo.hwnd = hwnd;
	ShExecInfo.lpVerb = "properties";
	ShExecInfo.lpFile = lpszFile;
	ShExecInfo.lpParameters = "";
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:

        /*HWND hwndButton = CreateWindow(
        "BUTTON",   // Predefined class; Unicode assumed.
        "OK",       // Button text.
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles.
        50,         // x position.
        50,         // y position.
        100,        // Button width.
        100,        // Button height.
        hwnd,       // Parent window.
        NULL,       // No menu.
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), //returns the handle to the instance of the window proc
        NULL);      // Pointer not needed.*/

        return TRUE;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    EndDialog(hwnd, IDOK);
                break;
                case IDCANCEL:
                    EndDialog(hwnd, IDCANCEL);
                break;
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}
