/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"

void GenerateKey (int vk, BOOL bExtended, BOOL doDown, BOOL doUp)
{
    KEYBDINPUT kb = {0};
    INPUT Input = {0};

    if (doDown) {
        // generate down 
        if (bExtended)
            kb.dwFlags = KEYEVENTF_EXTENDEDKEY;
        kb.wVk = vk;  
        Input.type = INPUT_KEYBOARD;

        Input.ki = kb;
        ::SendInput(1,&Input,sizeof(Input));
    }

    if (doUp) {
        // generate up 
        ::ZeroMemory(&kb,sizeof(KEYBDINPUT));
        ::ZeroMemory(&Input,sizeof(INPUT));
        kb.dwFlags = KEYEVENTF_KEYUP;
        if (bExtended)
            kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;

        kb.wVk = vk;
        Input.type = INPUT_KEYBOARD;
        Input.ki = kb;
        ::SendInput(1,&Input,sizeof(Input));
    }
}

HWND FindWMDCWindow() {
    return FindWindow(NULL, L"Windows Mobile Device Center");
}

int _tmain(int argc, _TCHAR* argv[])
{
    bool launched = false;
    HWND hwnd;

    printf("WMDC_connect v1.0 (c) 2012 Rhomobile, Inc.\n");

    OSVERSIONINFOW os_version;
    GetVersionEx(&os_version);
    if (os_version.dwMajorVersion < 6) {
        printf("This version of Windows does not use WMDC. Nothing to do.");
        return 0;
    }

    int pass = 0;
    do {
        hwnd = FindWMDCWindow();
        if (hwnd == 0) {
            if (pass == 0) {
                printf("WMDC is not running. Starting WMDC...\n");
        	    system("start %WINDIR%\\WindowsMobile\\wmdc.exe /show");
                launched = true;
            } else {
                printf("Waiting for WMDC...\n");
        		Sleep(1000);
            }
        }
        pass++;
    } while ((hwnd == 0) && (pass < 12));

    if (hwnd != 0) {
        printf("WMDC window found.\n");
        if (launched) {
            printf("Waiting for WMDC to connect to Device Emulator...\n");
	        Sleep(25000);
        } else {
            Sleep(200);
        }
        hwnd = FindWMDCWindow();
    }
    if (hwnd == 0) {
        printf("ERROR: Cannot find WMDC window.\n");
        return 1;
    }
    printf("Pushing Connect button...\n");
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);
    LockSetForegroundWindow(LSFW_LOCK);
    Sleep(100);
    GenerateKey(VK_MENU, TRUE, TRUE, FALSE);
    GenerateKey('C', FALSE, TRUE, TRUE);
    GenerateKey(VK_MENU, TRUE, FALSE, TRUE);
    LockSetForegroundWindow(LSFW_UNLOCK);

	return 0;
}
