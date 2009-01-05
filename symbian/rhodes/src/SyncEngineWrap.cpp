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

#ifndef ENABLE_RUBY_VM_STAT
#define ENABLE_RUBY_VM_STAT
#endif

#include "stat/stat.h"

extern "C"
	{
		#include "sqlite3.h"
		
		void start_sync_engine(sqlite3 *db);
		void stop_sync_engine();
		
		void lock_sync_mutex();
		void unlock_sync_mutex();
		
		void* sync_engine_main_routine(void* data);

		char *get_db_session(char* source_url);
		
		const char* RhoGetRootPath();
		
		int login ( const char* login, const char* password );
//		int db_login ( const char* login, const char* password );
		
		void parse_source_url(char* url, char* source, int size);
		
		#include <fcntl.h>
		#include <string.h>
		#include <stdio.h>
		#include <stdlib.h>
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

#ifdef ENABLE_RUBY_VM_STAT	
		g_sync_thread_loaded = 1;
#endif
		
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
	static int g_logged_in = false;
	char* fetch_remote_data(char* url) 
	{
		char* cookie = 0;
		char source[1024] = {0};
		
		if (!gHttpClient) 
			gHttpClient = CHttpClient::NewL();
				
		parse_source_url(url, source, 1024);
		
		cookie = get_db_session(source);
		gHttpClient->SetCookie( cookie, strlen(cookie) );
		if ( cookie )
			free(cookie);

		gHttpClient->InvokeHttpMethodL(CHttpConstants::EGet, (const TUint8*)url, strlen(url), NULL, 0);
		
		return gHttpClient->GetResponse();
	}
	
	void parse_source_url(char* url, char* source, int size)
	{
		int i = 0;
		int count = strlen(url);
		
		for ( i = 0; i < count && url[i] != '?'; i++);
		
		if ( i <= size )
		{
			strncpy( source, url, i);
			source[i] = '\0';
		}
	}
	
	int push_remote_data(char* url, char* data, size_t data_size) 
	{
		int retval = 0;
		char* szData = 0;
		char* cookie = 0;
		
		if (!gHttpClient) 
			gHttpClient = CHttpClient::NewL();
				
		cookie = get_db_session(url);
		gHttpClient->SetCookie( cookie, strlen(cookie) );
		
		gHttpClient->InvokeHttpMethodL(CHttpConstants::EPost, (const TUint8*)url, strlen(url), (const TUint8*)data, data_size);
		
		szData = gHttpClient->GetResponse();
	
		retval = szData ? 1 : 0;
		
		if ( szData )
			delete szData;
		
		return retval;
	}
/*	
void parseCookie(const char* szCookie, char* session ){
  char* szPathPos = 0;
  char* szPathEnd = 0;

  while( *szCookie == ' ' ) szCookie++;

  szPathPos = strstr(szCookie, "path=");
  if ( szPathPos )
    szPathEnd = strchr( szPathPos, ';' );

  if ( strncmp(szCookie,"auth_token=", strlen("auth_token=")) == 0 )
  {
    char* szEndAuth = strchr( szCookie, ';' );
    if ( szEndAuth-szCookie > strlen("auth_token=")+1 )
      strncat(session, szCookie, szPathEnd-szCookie+1 );
  }else if ( strncmp(szCookie,"rhosync_session=", strlen("rhosync_session=")) == 0 ){
      strncat(session, szCookie, szPathEnd-szCookie+1 );
  }
}

char* parseHeadersForSession(const char* szHeaders){
  const char* szHeader = szHeaders;
  char * session = malloc(1024);
  session[0] = 0;
  while( szHeader && *szHeader ){
    if ( strncmp(szHeader,"Set-Cookie:", strlen("Set-Cookie:")) == 0 )
      parseCookie(szHeader+strlen("Set-Cookie:"),session);

    szHeader = szHeader + strlen(szHeader) + 1;
  }

  return session;
}
*/

  void makeLoginRequest(char* url, char* data ){
		if (!gHttpClient) 
			gHttpClient = CHttpClient::NewL();

		gHttpClient->InvokeHttpMethodL(CHttpConstants::EPost, (const TUint8*)url, strlen(url), (const TUint8*)data, strlen(data));

    /*gHttpClient->GetResponse();
      headers = get_session_from_login(login_url,data);
      if ( headers )
        session = parseHeadersForSession(headers);

			//save session to the sources database
			if ( session != NULL && strlen(session) > 0 )
			{
        printf("Session from login(%s): %s.\n", login_url, session);
				retval = set_db_session( source_list[i]->_source_url, session );
			}

			if ( headers )
				free(headers);

			if ( session )
				free(session);

    */
  }

}