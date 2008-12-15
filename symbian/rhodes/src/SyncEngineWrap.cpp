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
extern "C" const char* RhoGetRootPath();

extern "C"
	{
		#include "sqlite3.h"
	
		int	 sprintf(char * __restrict, const char * __restrict, ...);
		
		void start_sync_engine(sqlite3 *db);
		void stop_sync_engine();
		
		void lock_sync_mutex();
		void unlock_sync_mutex();
		
		void* sync_engine_main_routine(void* data);

		extern const char* ROOT_PATH;
	}

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
		TInt res = thread.Create(KThreadName, SyncThreadEntryPoint, 0x10000, 0x5000, 0x50000, this);
		
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
		//Initialize Ruby
		StartSyncEngine();
	
		sync_engine_main_routine(NULL);//main 
		
		StopSyncEngine();

	 	return 0;
	}

void CSyncEngineWrap::StartSyncEngine()
	{
		lock_sync_mutex();
		
		char dbpath[256];
		sprintf(dbpath,"%sdb\\syncdb.sqlite",RhoGetRootPath());
		sqlite3_open(dbpath,& iDatabase);
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
	#include <sys/types.h>
	
	char* fetch_remote_data(char* url) {
		return NULL;
	}
	
	int push_remote_data(char* url, char* data, size_t data_size) {
		return 0;
	}
	
	}