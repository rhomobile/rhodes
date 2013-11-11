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

#include <rapi.h>
#include <strsafe.h>

#include "detool.h"

namespace wmdc
{

void simulateKeyInput (int vk, BOOL bExtended, BOOL doDown, BOOL doUp)
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

bool doUseWMDC() {
    OSVERSIONINFOW os_version;
    ZeroMemory(&os_version, sizeof(OSVERSIONINFO));
    os_version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&os_version);
	return (os_version.dwMajorVersion >= 6);
}

HWND findWMDCWindow() {
    return FindWindow(NULL, L"Windows Mobile Device Center");
}

void startWMDC() {
	if (doUseWMDC()) {
        if (findWMDCWindow() == 0) {
            _tprintf( TEXT("WMDC is not running. Starting WMDC...\n") );
			::system("start %WINDIR%\\WindowsMobile\\wmdc.exe /show");
			::Sleep(5000);
		}
	}
}

void connectWMDC() 
{
	if (doUseWMDC()) {
		HWND hwnd = findWMDCWindow();
        if (hwnd == 0) {
            _tprintf( TEXT("Cannot find WMDC window to establish network connection to device emulator.\n") );
		} else {
			::SetForegroundWindow(hwnd);
			::SetFocus(hwnd);
			::LockSetForegroundWindow(LSFW_LOCK);
			::Sleep(100);
			simulateKeyInput(VK_MENU, TRUE, TRUE, FALSE);
			::Sleep(20);
			simulateKeyInput('C', FALSE, TRUE, TRUE);
			::Sleep(20);
			simulateKeyInput(VK_MENU, TRUE, FALSE, TRUE);
			::LockSetForegroundWindow(LSFW_UNLOCK);
		}
	}
}

} //end wmdc