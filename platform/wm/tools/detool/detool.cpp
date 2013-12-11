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

#include "detool.h"
#include "LogServer.h"
#include "rapi_helper.h"

#include "DeployEmuWebKitCommand.h"
#include "DeployDevCabCommand.h"
#include "DeployDevCommand.h"
#include "DeployDevWebKitCommand.h"
#include "DeployEmuCabCommand.h"
#include "DeployEmuCommand.h"
#include "DeployLogCommand.h"

TCHAR *app_name = NULL;

/**
 * detool emu "<emu_name|vmid>" "app-name" rhobundle_path exe_name
 * or
 * detool emucab "<emu_name|vmid>" app.cab "app-name"
 * or
   detool dev "app-name" rhobundle_path exe_name
   or 
 * detool devcab app.cab "app-name"
 */
void usage(void)
{
printf 
	("Rhodes deployment tool for Windows Mobile:          \n  \
	  detool emu \"<emu_name|vmid>\" app.cab \"app-name\" \n  \
	  or                                                  \n  \
	  detool dev app.cab \"app-name\"                     \n"
	 );
}

void startLogServer( TCHAR * log_file, TCHAR* log_port ) 
{
	// Declare and initialize variables
	WSADATA wsaData;
	int iResult;
	LogServer logSrv(log_file, log_port);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}
	
	if (logSrv.init())
	{
		logSrv.run();
	}

	WSACleanup();
}


bool str_ends_with(const TCHAR* str, const TCHAR* suffix)
{
	if( str == NULL || suffix == NULL )
		return 0;

	size_t str_len = wcslen(str);
	size_t suffix_len = wcslen(suffix);

	if(suffix_len > str_len)
		return 0;

	return (_wcsnicmp( str + str_len - suffix_len, suffix, suffix_len ) == 0);
}

EAppType getAppTypeFromCmdLine(int argc, TCHAR** argv)
{
    USES_CONVERSION;

    if (strcmp(T2A(argv[1]), "js") == 0) {
        return eJs;
    }

    return eRuby;
}

void getAppDir(TCHAR **app_dir)
{
    _tcscpy(*app_dir, TEXT("\\Program Files\\"));
    _tcscat(*app_dir, app_name);
    _tprintf( TEXT("%s\n"), *app_dir);
}

bool findCommand(const EmuCommand* currentCommand)
{
    return currentCommand->checkParameters();
}

bool initRapiLayer()
{
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED))) 
        return false;

    _tprintf( TEXT("Searching for Windows CE device..."));

    HRESULT hRapiResult;
    hRapiResult = CeRapiInit();
    
    if (FAILED(hRapiResult)) 
    {
        _tprintf( TEXT("FAILED\n"));
        return false;
    }
    _tprintf( TEXT("DONE\n"));

    wmdc::startWMDC();

    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (!initRapiLayer())
        return 1;

    typedef std::vector<EmuCommand*> command_vector;

    command_vector deployCommands;
    deployCommands.push_back(new DeployEmuWebKitCommand(argc, argv));
    deployCommands.push_back(new DeployDevCabCommand(argc, argv));
    deployCommands.push_back(new DeployDevCommand(argc, argv));
    deployCommands.push_back(new DeployDevWebKitCommand(argc, argv));
    deployCommands.push_back(new DeployEmuCabCommand(argc, argv));
    deployCommands.push_back(new DeployEmuCommand(argc, argv));
    deployCommands.push_back(new DeployLogCommand(argc, argv));

    command_vector::iterator findIt = std::find_if(deployCommands.begin(), deployCommands.end(), std::ptr_fun(&findCommand));

    if (findIt == deployCommands.end())
    {
        usage();
        return 1;
    }

    try
    {
        (*findIt)->parseParameters();
        (*findIt)->runObject();
    }
    catch (std::range_error&)
    {
        usage();
        return 1;
    }

    CoUninitialize();
    
    return 0;
}