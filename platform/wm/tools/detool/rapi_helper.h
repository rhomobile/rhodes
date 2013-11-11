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

#ifndef __RAPI_HELPER_H_
#define __RAPI_HELPER_H_

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

enum EAppType
{
    eRuby,
    eJs,
};

namespace rapi
{
void checkMDEstart(HRESULT hr);
DWORD WINAPI startDEM(LPVOID lpParam);
BOOL FindDevice(const CComBSTR& deviceIdentifier, IDeviceEmulatorManagerVMID** pDeviceVMID);
bool emuConnect(const CComBSTR& deviceIdentifier);
bool emuBringToFront(const CComBSTR& deviceIdentifier);
bool emuCradle(const CComBSTR& deviceIdentifier);
bool emuUncradle(const CComBSTR& deviceIdentifier);
bool emuReset(const CComBSTR& deviceIdentifier);
bool emuShutdown(const CComBSTR& deviceIdentifier);
bool wceConnect (void);
void wceDisconnect(void);
}

namespace wmdc
{
void connectWMDC();
void startWMDC();
HWND findWMDCWindow();
bool doUseWMDC();
void simulateKeyInput (int vk, BOOL bExtended, BOOL doDown, BOOL doUp);
}

namespace file
{
int copyLicenseDll (TCHAR *file_name, TCHAR *app_dir);
int copyBundle (TCHAR *parent_dir, TCHAR *file, TCHAR *app_dir);
int copyExecutable (TCHAR *file_name, TCHAR *app_dir, bool use_shared_runtime);
}

#endif