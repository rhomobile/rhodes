// wmdc_connect.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
    HWND hwnd;
    int pass = 0;
    do {
        hwnd = FindWindow(NULL, L"Windows Mobile Device Center");
        if (hwnd == 0) {
            if (pass == 0) {
                printf("WMDC is not running. Starting WMDC...\n");
        	    system("start %WINDIR%\\WindowsMobile\\wmdc.exe /show");
            } else {
                printf("Waiting for WMDC...\n");
        		Sleep(10);
            }
        }
    } while ((hwnd == 0) && (pass < 12));

    if (hwnd == 0) {
    }

    printf("Waiting for WMDC to connect to Device Emulator...\n");
	Sleep(200);

    printf("Pushing the Connect button...\n");
	RECT r;
	GetWindowRect(hwnd, &r);
	RECT cr;
	GetClientRect(hwnd, &cr);
 
	int x = r.left + cr.right - 243;
	int y = r.top + cr.bottom - 168;
 
	SetCursorPos(x,y);
 
	POINT p;
	GetCursorPos(&p);
	hwnd = WindowFromPoint(p);
 
	GetWindowRect(hwnd, &r);
	GetClientRect(hwnd, &cr);
 
	mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
    Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);

	return 0;
}

