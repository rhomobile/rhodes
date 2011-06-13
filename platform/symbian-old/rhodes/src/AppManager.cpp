/*
 ============================================================================
 Name		: AppManager.cpp
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 Description : CAppManager implementation
 ============================================================================
 */

#include "AppManager.h"

#ifdef ENABLE_DYNAMIC_RHOBUNDLE

#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>

#include "unzip.h"
#include "rhoruby/rhoruby.h"

#include "HttpConstants.h"
#include "HttpFileManager.h"
#include "HttpClient.h"

#include "rhodesAppUi.h"

#include <string.h>

#include <rhodes.rsg>

HZIP OpenZip(const TCHAR *fn, const char *password);

CAppManager::CAppManager()
	{
	// No implementation required
	}

CAppManager::~CAppManager()
	{
		if ( iHttpClient )
			delete iHttpClient;
		
		if ( iHttpFileManager )
			delete iHttpFileManager;

	}

CAppManager* CAppManager::NewLC(CRhodesAppUi * aAppUI)
	{
	CAppManager* self = new (ELeave) CAppManager();
	CleanupStack::PushL(self);
	self->setAppUI(aAppUI);
	self->ConstructL();
	return self;
	}

CAppManager* CAppManager::NewL(CRhodesAppUi * aAppUI)
	{
	CAppManager* self = CAppManager::NewLC(aAppUI);
	CleanupStack::Pop(); // self;
	return self;
	}

void CAppManager::ConstructL()
	{
	iHttpClient = CHttpClient::NewL();
	iHttpFileManager = CHttpFileManager::NewL();
	}

void CAppManager::reloadRhoBundle(const char* aUrl, const char* zipPassword)
{
	if ( aUrl )
	{
		if ( iHttpFileManager->CreateRequestFile( (const TUint8*) aUrl, strlen(aUrl), NULL, 0) )
		{
			iHttpClient->InvokeHttpMethodL(CHttpConstants::EGet);
			
			TFileName filePath;
			TFileName fileName;
			iHttpFileManager->GetOldestFile(fileName, EFalse);
			
			filePath.Append(CHttpConstants::KHttpOUT);
			filePath.Append(fileName);
			
			if ( filePath.Length() > 0 )
			{
				//I hate symbian descriptors...
				HBufC8* buffer = HBufC8::NewLC(filePath.Length()+1);
				Mem::Fill((void*)buffer->Ptr(), filePath.Length()+1, 0);
				buffer->Des().Copy(filePath);
				
				const char* pFilePath = (const char*)buffer->Ptr();
				
				//TODO: Add error handling
				
				ZIPENTRY ze; 
				// Open zip file
				HZIP hz = OpenZip(pFilePath, zipPassword);
				
				if ( hz )
				{
					//Stop HTTP Server
					//iAppUI->StopRubyFramework();
					iAppUI->StopThreads();
				
					// Set base for unziping
					SetUnzipBaseDir(hz, RhoGetRootPath());
					
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
				}
				
				CleanupStack::PopAndDestroy(buffer);
				
				iHttpFileManager->DeleteFile(filePath);
				
				//iAppUI->StartRubyFramework();
				iAppUI->ShowInfoMessage(R_UPDATE_RHOBUNDLE_DIALOG_TITLE, R_UPDATE_RHOBUNDLE_DIALOG_TEXT);
				
			}
		}
	}
}

#endif
