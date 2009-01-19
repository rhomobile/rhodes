#include "AppManager.h"

#ifdef ENABLE_DYNAMIC_RHOBUNDLE

#include "HttpServer.h"

#include "unzip.h"
#include "rhoruby/rhoruby.h"

extern char* remote_data(LPWSTR verb, char* url, char* body, size_t body_size, bool bGetHeaders, bool bGetRawData, bool bCheckSession, DWORD* pdwDataSize);
extern "C" wchar_t* wce_mbtowc(const char* a);

CAppManager::CAppManager(void)
{
}

CAppManager::~CAppManager(void)
{
}

void CAppManager::ReloadRhoBundle(const char* szUrl, const char* szZipPassword)
{
	if ( szUrl )
	{
		//get zip file with rhodes
		DWORD dwDataSize = 0;
		char* zipData = remote_data( L"GET", const_cast<char*>(szUrl), NULL, 0, false, true, false, &dwDataSize );

		LPWSTR rootw = wce_mbtowc(RhoGetRootPath());  

		//TODO: Add error handling
		if ( zipData && dwDataSize > 0 )
		{
			ZIPENTRY ze; 
			// Open zip file
			HZIP hz = OpenZip(zipData, dwDataSize, szZipPassword);
			
			if ( hz )
			{
				//Stop HTTP Server
				CHttpServer::Instance()->FreezeThread();

				// Set base for unziping
				SetUnzipBaseDir(hz, rootw);
				
				// Get info about the zip
				// -1 gives overall information about the zipfile
				GetZipItem(hz,-1,&ze);
				int numitems = ze.index;
				
				// Iterate through items and unzip them
				for (int zi = 0; zi<numitems; zi++)
				{ 
					// fetch individual details, e.g. the item's name.
					GetZipItem(hz,zi,&ze); 
					// unzip item
					UnzipItem(hz, zi, ze.name);         
				}
				
				CloseZip(hz);

				//Show MessageBox
				MessageBox(NULL, _T("Rhobundle has been updated successfully.\n\nPlease restart application."), _T("Information"), MB_OK | MB_ICONINFORMATION );
			}
		}

		if ( rootw )
			free(rootw);

		if ( zipData )
			delete zipData;
	}
}

#endif