/*
 ============================================================================
 Name		: SyncEngine.cpp
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   :   Copyright (C) 2008 Rhomobile. All rights reserved.

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
 Description : CSyncEngineWrap implementation
 ============================================================================
 */

#include "SyncEngineWrap.h"
#include "rhodes.pan"
#include <eikenv.h>

#include "HttpClient.h"
#include "HttpFileManager.h"
#include "HttpConstants.h"

extern "C"
	{
		#include "sqlite3.h"
	
		int	 sprintf(char * __restrict, const char * __restrict, ...);
		int	 strlen(const char *);
		
		void start_sync_engine(sqlite3 *db);
		void stop_sync_engine();
		
		void lock_sync_mutex();
		void unlock_sync_mutex();
		
		void* sync_engine_main_routine(void* data);

		const char* RhoGetRootPath();
		
		#include <fcntl.h>
		#include <stdio.h>
		#include <time.h>
		#include <sys/types.h>
	}

CHttpClient* gHttpClient; //Http client

TInt SyncThreadEntryPoint(TAny *aPtr)
{
	CSyncEngineWrap *pSyncEngineWrap = (CSyncEngineWrap *)aPtr;
	return pSyncEngineWrap->Execute();
}


CSyncEngineWrap::CSyncEngineWrap()
	{
	// No implementation required
	}

CSyncEngineWrap::~CSyncEngineWrap()
	{
		thread.Terminate(NULL);
	}

CSyncEngineWrap* CSyncEngineWrap::NewLC()
	{
	CSyncEngineWrap* self = new (ELeave)CSyncEngineWrap();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSyncEngineWrap* CSyncEngineWrap::NewL()
	{
	CSyncEngineWrap* self=CSyncEngineWrap::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CSyncEngineWrap::ConstructL()
	{
		//create suspended thread
		_LIT(KThreadName, "SyncThreadEntryPoint");
		
		//KMinHeapSize, 256*KMinHeapSize
		TInt res = thread.Create(KThreadName, SyncThreadEntryPoint, 20000, 0x5000, 0x200000, this);
		
		if ( res != KErrNone )
			Panic( ERhodesSyncEngineInit);
		
		thread.SetPriority(EPriorityNormal);
	}

TInt CSyncEngineWrap::Execute()
	{
		TInt err=KErrNoMemory;
		// Create a cleanup stack...
		CTrapCleanup* cleanup = CTrapCleanup::New();

		if(cleanup)
	    {
	    	TRAP(err,CSyncEngineWrap::ExecuteL());
	    	delete cleanup;
	    }
		return err;
	}

TInt CSyncEngineWrap::ExecuteL()
	{
		// Create and install the active scheduler
		CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler;
		CleanupStack::PushL(activeScheduler);
		CActiveScheduler::Install(activeScheduler);

		//Initialize Ruby
		StartSyncEngine();
	
		sync_engine_main_routine(NULL);//main 
		
		StopSyncEngine();

		CleanupStack::PopAndDestroy(activeScheduler);
		
	 	return 0;
	}

void CSyncEngineWrap::StartSyncEngine()
	{
		lock_sync_mutex();
		
		char dbpath[KMaxFileName];
		sprintf(dbpath,"%sdb\\syncdb.sqlite",RhoGetRootPath());
		sqlite3_open(dbpath, &iDatabase);
		start_sync_engine(iDatabase);

		unlock_sync_mutex();
	}

void CSyncEngineWrap::StopSyncEngine()
	{
		lock_sync_mutex();
		
		if (iDatabase)
			sqlite3_close(iDatabase);
		
		unlock_sync_mutex();
	}

void CSyncEngineWrap::ResumeThread()
	{
		thread.Resume();
	}

void CSyncEngineWrap::SuspendThread()
	{
		thread.Suspend();
	}

void CSyncEngineWrap::TerminateThread()
	{
		stop_sync_engine();
	}

extern "C"
	{
	char* fetch_remote_data(char* url) 
	{
		char* szData = NULL;
		CHttpFileManager* iHttpFileManager = CHttpFileManager::NewL();
		if (!gHttpClient) 
			gHttpClient = CHttpClient::NewL();
				
		if ( iHttpFileManager && gHttpClient )
		{
			if ( iHttpFileManager->GetFilesCount( ETrue) == 0 ) //currently only one file will be used
			{
				if ( iHttpFileManager->CreateRequestFile( (const TUint8*)url, strlen(url), NULL, 0 ) ) 
					gHttpClient->InvokeHttpMethodL(CHttpConstants::EGet);
			}
			
			TFileName respFile;
			iHttpFileManager->GetOldestFile( respFile, EFalse );
			
			if ( respFile.Size() > 0 )
			{
				TFileName respFilePath;
				respFilePath.Append(CHttpConstants::KHttpOUT);
				respFilePath.Append(respFile);

				szData = iHttpFileManager->ReadResponseFile( respFilePath );
				iHttpFileManager->DeleteFile(respFilePath);
			}
		}
		delete iHttpFileManager;
		
		return szData;
	}
	
	int push_remote_data(char* url, char* data, size_t data_size) 
	{
		int retval = 0;
		char* szData = NULL;
		CHttpFileManager* iHttpFileManager = CHttpFileManager::NewL();
		if (!gHttpClient) 
			gHttpClient = CHttpClient::NewL();
				
		if ( iHttpFileManager && gHttpClient )
		{
			if ( iHttpFileManager->GetFilesCount( ETrue) == 0 ) //currently only one file will be used
			{
				if ( iHttpFileManager->CreateRequestFile( (const TUint8*)url, strlen(url), (const TUint8*)data, data_size ) ) 
					gHttpClient->InvokeHttpMethodL(CHttpConstants::EPost);
			}
			
			TFileName respFile;
			iHttpFileManager->GetOldestFile( respFile, EFalse );
			
			if ( respFile.Size() > 0 )
			{
				TFileName respFilePath;
				respFilePath.Append(CHttpConstants::KHttpOUT);
				respFilePath.Append(respFile);

				szData = iHttpFileManager->ReadResponseFile( respFilePath );
				iHttpFileManager->DeleteFile(respFilePath);
			}
		}
		delete iHttpFileManager;

		if ( szData )
			delete szData;
		else
			retval = 1;
	
		return retval;
	}
	
	}