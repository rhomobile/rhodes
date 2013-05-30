// RhoLaunch.cpp : Defines the entry point for the application.
//

#include "windows.h"
#include "RhoLaunch.h"


#define MAX_LOADSTRING 100
#define PB_WINDOW_RESTORE				WM_USER + 10



// Global Functions:
int LaunchProcess( LPTSTR pFilePath, LPTSTR pCmdLine);


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)
{
	
	COPYDATASTRUCT launchData;
	
	//validate the command line
	int iLen = wcslen(lpCmdLine);
	//if(iLen < 3){
	//	return 1;
	//}

	//LPWSTR pAppName = new WCHAR[iLen + 3];//allow space for the index
	LPWSTR pAppName = 0,pTabName = 0,pTemp = 0;
	pAppName = pTemp= lpCmdLine;
	
	
	
	int iLoop;
	bool bStart,bTabStart,bAppName;
	bStart = bTabStart = bAppName= false;

	for(iLoop = 0;iLoop < iLen ;iLoop++)
	{
		if(lpCmdLine[iLoop] == L' '){
			
			if(bStart){
				if(!bAppName){
					*pTemp = NULL;//terminate the app name
					pTemp++;
					bAppName = true;
					pTabName = pTemp;

				}
				else if(bTabStart){
					*pTemp = NULL;//terminate the tab name
					break;//finished so exit loop
				}
				
			}
			continue;//ignore spaces
		
		}		




		if(lpCmdLine[iLoop] == L'/'){
			if(!bStart){
				bStart = !bStart;
				continue;

			}
			
			if(!bAppName){
				*pTemp= NULL;
				bAppName = !bAppName;
				bTabStart = true;
			}

			if(!bTabStart){
				bTabStart = true;
				continue;
			}
			
			
			
		
		}
		*pTemp= lpCmdLine[iLoop];
		pTemp++;

	}
	*pTemp = NULL;
			
	WCHAR pAppNameBuf[MAX_PATH + 1];
	if (!pAppName || !*pAppName)
    {
		if(!(GetModuleFileName(NULL,pAppNameBuf,MAX_PATH))){
			return 1;//error could not find the path 
		}

        WCHAR* szSlash = wcsrchr( pAppNameBuf, L'\\');
        if ( szSlash )
        {
            pAppName = szSlash+1;
            WCHAR* szDot = wcsrchr( pAppName, L'.');
            if (szDot)
                *szDot = 0;
        }else
            pAppName = L"";
    }

	HWND hwndRunningRE = FindWindow(NULL, pAppName);
		
	if (hwndRunningRE){
		//  Found the running instance
		
		//  switch to it
		ShowWindow(hwndRunningRE, SW_RESTORE);
        SendMessage( hwndRunningRE, PB_WINDOW_RESTORE, NULL, TRUE);
		SetForegroundWindow(hwndRunningRE);
	

		if ( pTabName )
        {
		    //rhode expects a MultiByte string so convert
		    int ilen = wcslen(pTabName);
		    char *pTabNameMB = new char[ilen+1];
		    wcstombs(pTabNameMB, pTabName,ilen);	
    	
    	
		    launchData.lpData = pTabNameMB;
		    launchData.cbData = (ilen+1);




		    //send a message to inform Rho of the requested index
		    SendMessage(hwndRunningRE,WM_COPYDATA,(WPARAM)NULL,(LPARAM) (LPVOID) &launchData );
    		
		    delete [] pTabNameMB;
        }

	}
	else{
		//no window handle, so try to start the app
		//will only work if RhoLaunch is in the same directory as the app
		
		//build the Rho app name
		WCHAR pApp[MAX_PATH + 1];
		int iIndex,iLen;

		if(!(iLen = GetModuleFileName(NULL,pApp,MAX_PATH))){
			return 1;//error could not find the path 
		}

		//remove 'RhoLaunch' from the path
		for(iIndex = iLen - 1;pApp[iIndex]!=L'\\';iIndex--);
		pApp[iIndex+ 1] = NULL;

		
		//LPWSTR pApp = new WCHAR[wcslen(pAppName)+10];
		wcscat(pApp,pAppName);
		wcscat(pApp,L"_main.exe");
		
        if (pTabName && *pTabName)
        {
		    LPWSTR pTabCommand = new WCHAR[wcslen(pTabName)+20];
		    wsprintf(pTabCommand,L"/tabname=\"%s\"",pTabName);
		    LaunchProcess(pApp,pTabCommand); 
		    delete [] pTabCommand;
        }else
            LaunchProcess(pApp,L"");
	}
	
	return 0;
}

int LaunchProcess( LPTSTR pFilePath, LPTSTR pCmdLine)
{
	
	STARTUPINFO				StartInfo;
	PROCESS_INFORMATION		ProcessInfo;
	BOOL					bOk;


	//	Reset the contents of the 'Process Information' and 'Start Up' structures.

	memset( &ProcessInfo, 0, sizeof( PROCESS_INFORMATION));
	memset( &StartInfo, 0, sizeof( STARTUPINFO));
	StartInfo.cb = sizeof( STARTUPINFO);


	//	Create the 'browser' in a seperate process.

	bOk = CreateProcess( pFilePath,		//	LPCTSTR lpApplicationName,//L"\\program files\\Neon\\BIN\\SVC_Controls.exe",
						 pCmdLine,		//	LPTSTR lpCommandLine,
						 NULL,			//	LPSECURITY_ATTRIBUTES lpProcessAttributes,
						 NULL,			//	LPSECURITY_ATTRIBUTES lpThreadAttributes,
						 FALSE,			//	BOOL bInheritHandles,
						 0,				//	DWORD dwCreationFlags,
						 NULL,			//	LPVOID lpEnvironment,
						 NULL,			//	LPCTSTR lpCurrentDirectory,
						 &StartInfo,	//	LPSTARTUPINFO lpStartupInfo,
						 &ProcessInfo);	//	LPPROCESS_INFORMATION lpProcessInformation
	//m_dwProcessID = ProcessInfo.dwProcessId;

	//	If all is Ok, then return '0' else return the last error code.

	return bOk ? 0 : GetLastError();
}