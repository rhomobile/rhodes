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
#include <e32math.h>  //Rand
#include "rhodes.hrh"

#include "HttpClient.h"
#include "HttpFileManager.h"
#include "HttpConstants.h"
#include "Constants.h"

#ifndef ENABLE_RUBY_VM_STAT
//#define ENABLE_RUBY_VM_STAT
#endif

#include "stat/stat.h"

//static sqlite3 * iDatabase = 0;

extern "C"
	{
#include "sqlite3.h"

	void start_sync_engine(sqlite3 *db);
	void stop_sync_engine();

	void lock_sync_mutex();
	void unlock_sync_mutex();

	void* sync_engine_main_routine(void* data);

	char *get_db_session(const char* source_url);
	int set_db_session(const char* source_url, const char * session);

	void delete_db_session(const char* source_url);
	const char* load_source_url();

	const char* RhoGetRootPath();

	int login(const char* login, const char* password);
	sqlite3 *get_database();

#include "tcmalloc/rhomem.h"
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
	}

TInt SyncThreadEntryPoint(TAny *aPtr)
	{
	CSyncEngineWrap *pSyncEngineWrap = (CSyncEngineWrap *) aPtr;
	return pSyncEngineWrap->Execute();
	}

CSyncEngineWrap::CSyncEngineWrap()
	{
	// No implementation required
	}

CSyncEngineWrap::~CSyncEngineWrap()
	{
	thread.Kill(KErrCancel);
	thread.Close();
	}

CSyncEngineWrap* CSyncEngineWrap::NewLC()
	{
	CSyncEngineWrap* self = new (ELeave) CSyncEngineWrap();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSyncEngineWrap* CSyncEngineWrap::NewL()
	{
	CSyncEngineWrap* self = CSyncEngineWrap::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CSyncEngineWrap::ConstructL()
	{
	//create suspended thread
	_LIT(KThreadName, "SyncThreadEntryPoint");

	TBuf<30> threadName;
	threadName.Append(KThreadName);

	TTime time;
	time.HomeTime();

	TInt64 aSeed = time.Int64();
	TInt randNum = Math::Rand(aSeed) % 1000;
	threadName.AppendNum(randNum);

	//KMinHeapSize, 256*KMinHeapSize                                 
	//TInt res = thread.Create(threadName, SyncThreadEntryPoint, 20000, 0x100000, 0x3D4000, this);
	TInt res = thread.Create(threadName, SyncThreadEntryPoint, 20000, 0x5000,
			0x600000, this);

	if (res != KErrNone)
		Panic(ERhodesSyncEngineInit);

	thread.SetPriority(EPriorityNormal);
	}

TInt CSyncEngineWrap::Execute()
	{
	TInt err = KErrNoMemory;
	// Create a cleanup stack...
	CTrapCleanup* cleanup = CTrapCleanup::New();

	if (cleanup)
		{
			TRAP(err,CSyncEngineWrap::ExecuteL());
		delete cleanup;
		}

	//Send start new SyncEvent
	//SendWindowEvent(ECmdAppStartNewSync);

	return err;
	}

void CSyncEngineWrap::SendWindowEvent(TInt aEvent)
	{
	// Create a window server event
	RWsSession wsSession;
	TWsEvent event;

	if (wsSession.Connect() == KErrNone)
		{
		// Set event data. eventType.data = KData;
		event.SetType(EEventUser + aEvent); // set event type
		event.SetTimeNow(); // set the event time
		event.SetHandle(wsSession.WsHandle()); // set window server handle

		// Send the created event
		wsSession.SendEventToAllWindowGroups(event);
		}
	}

TInt CSyncEngineWrap::ExecuteL()
	{
	// Create and install the active scheduler
	CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(activeScheduler);
	CActiveScheduler::Install(activeScheduler);

	//Initialize Ruby
	StartSyncEngine();

#ifdef ENABLE_RUBY_VM_STAT	
	g_sync_thread_loaded = 1;
#endif

	sync_engine_main_routine(NULL);//main 

	StopSyncEngine();

	CleanupStack::PopAndDestroy(activeScheduler);

	return 0;
	}

extern "C" {
void rhoInitDatabase(struct sqlite3 *pDb){
	start_sync_engine(pDb);
}
}

void CSyncEngineWrap::StartSyncEngine()
	{
		//lock_sync_mutex();
		if (!get_database())
		{
			char dbpath[KMaxFileName];
			sprintf(dbpath, "%sdb\\syncdb.sqlite", RhoGetRootPath());
			sqlite3 * iDatabase = 0;			
			sqlite3_open(dbpath, &iDatabase);

			rhoInitDatabase(iDatabase);
		}
		//unlock_sync_mutex();
	

	}

void CSyncEngineWrap::StopSyncEngine()
	{
	//lock_sync_mutex();

	//if (iDatabase)
		//sqlite3_close(iDatabase);

	//unlock_sync_mutex();
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

	//current source
	int g_cur_source = 0;

	char* fetch_remote_data(char* url)
		{
		char* cookie = 0;
		char* retval = 0;
		CHttpClient* gHttpClient = NULL;

		cookie = get_db_session(load_source_url());

		if (!cookie && !strstr(url, "clientcreate"))
			{
			return NULL;
			}

		gHttpClient = CHttpClient::NewL();

		gHttpClient->SetCookie(cookie);

		if (cookie)
			free(cookie);

		gHttpClient->InvokeHttpMethodL(CHttpConstants::EGet, (const TUint8*) url, strlen(url), NULL, 0, NULL);

		retval = gHttpClient->GetResponse();

		delete gHttpClient;
		
		return retval;
	}

	int push_remote_data(char* url, char* data, size_t data_size,char* contentType)
	{
		int retval = SYNC_PUSH_CHANGES_ERROR;
		char* szData = 0;
		char* cookie = 0;

		CHttpClient* gHttpClient = NULL;

		cookie = get_db_session(load_source_url());

		if (!cookie && !strstr(url, "clientcreate"))
			return SYNC_PUSH_CHANGES_ERROR;
		
		gHttpClient = CHttpClient::NewL();
		
		gHttpClient->SetCookie(cookie);
		
		gHttpClient->InvokeHttpMethodL(CHttpConstants::EPost,
				(const TUint8*) url, 
				strlen(url), 
				(const TUint8*) data,
				data_size, contentType);

		szData = gHttpClient->GetResponse();

		retval = szData ? SYNC_PUSH_CHANGES_OK : SYNC_PUSH_CHANGES_ERROR;

		if (szData)
			free(szData);

		delete gHttpClient;
		
		return retval;
	}

	void makeLoginRequest(char* url, char* data)
	{
		char* session = NULL;
		CHttpClient* gHttpClient = CHttpClient::NewL();

		gHttpClient->InvokeHttpMethodL(CHttpConstants::EPost,
				(const TUint8*) url, 
				strlen(url), 
				(const TUint8*) data, 
				strlen(data), NULL);

		session = gHttpClient->GetCookie();

		if (session)
			set_db_session(load_source_url(), session);
		else
			delete_db_session(load_source_url());

		delete gHttpClient;
	}

	/*
	 * Commented posix version
	 * 
	 #include "posix_http_client.h"
	 
	 char* fetch_remote_data(char* url) 
	 {
	 char* cookie = 0;
	 char* responseBody = 0; 
	 
	 cookie = get_db_session(load_source_url());
	 
	 if ( !cookie && !strstr(url, "clientcreate") ) {
	 return NULL;
	 }

	 sendHttpRequest(url, cookie, NULL, 0, &responseBody, NULL, 0 );
	 
	 if ( cookie )
	 free(cookie);

	 return responseBody;
	 }
	 
	 int push_remote_data(char* url, char* data, size_t data_size,char* contentType) 
	 {
	 int retval = 0;
	 char* responseBody = 0;
	 char* cookie = 0;
	 
	 cookie = get_db_session(load_source_url());
	 
	 if ( !cookie && !strstr(url, "clientcreate") ) {
	 return 0;
	 }
	 
	 sendHttpRequest(url, cookie, data, data_size, &responseBody, NULL, 1 );
	 
	 retval = responseBody ? 1 : 0;
	 
	 if ( responseBody )
	 free(responseBody);
	 
	 return retval;
	 }

	 void makeLoginRequest(char* url, char* data )
	 {
	 char* session = 0;
	 
	 if ( !url || !data)
	 return;
	 
	 sendHttpRequest(url, NULL, data, strlen(data), NULL, &session, 1 );
	 
	 if ( session )
	 set_db_session( load_source_url(), session );
	 else
	 delete_db_session(load_source_url());
	 
	 if ( session )
	 free(session);
	 }
	 */

	void delete_session(const char *url_string)
		{
		delete_db_session(url_string);
		}
	}
