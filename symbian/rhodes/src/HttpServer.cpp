/*
 ============================================================================
 Author	    : Anton Antonov
 Version	: 1.0
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

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
 ============================================================================
 */

#include "HttpServer.h"
#include "rhodes.pan"
#include <eikenv.h>
#include "rhodesApplication.h"
#include "rhodesAppUI.h"

#include "rhoruby/rhoruby.h"
#include <stdio.h>
#include <string.h>

#ifndef ENABLE_RUBY_VM_STAT
#define ENABLE_RUBY_VM_STAT
#endif

#include "stat/stat.h"

extern "C" struct shttpd_ctx *shttpd_init(int argc, char *argv[]);
extern "C" void shttpd_fini(struct shttpd_ctx *);
extern "C" void shttpd_poll(struct shttpd_ctx *, int milliseconds);
extern "C" int shttpd_set_option(struct shttpd_ctx *, const char *opt, const char *val);

TInt ThreadEntryPoint(TAny *aPtr)
{
	CHttpServer *pHttpServer = (CHttpServer *)aPtr;
	return pHttpServer->Execute();
}

CHttpServer::CHttpServer()
	:iClose(false)
	{
	// No implementation required
	}

CHttpServer::~CHttpServer()
	{
		iClose = true;
		
		shttpd_fini(ctx);
		
		thread.Terminate(0);
	}

CHttpServer* CHttpServer::NewLC()
	{
		CHttpServer* self = new (ELeave)CHttpServer();
		CleanupStack::PushL(self);
		self->ConstructL();
		return self;
	}

CHttpServer* CHttpServer::NewL()
	{
		CHttpServer* self=CHttpServer::NewLC();
		CleanupStack::Pop(); // self;
		return self;
	}

void CHttpServer::ConstructL()
	{
		//initialize shttpd
		InitHttpServer();
		
		//create suspended thread
		_LIT(KThreadName, "HttpServerThread");
		
		//KMinHeapSize, 256*KMinHeapSize
		TInt res = thread.Create(KThreadName, ThreadEntryPoint, 
				//0x1000000, 0x5000, 0x1000000,
				80000, 0x100000, 0x500000,
				this);
		
		if ( res != KErrNone )
			Panic( ERhodesHttpServerInit);
		
		thread.SetPriority(EPriorityNormal);
		
	}

TInt CHttpServer::Execute()
	{
		TInt err=KErrNoMemory;
		// Create a cleanup stack...
		CTrapCleanup* cleanup = CTrapCleanup::New();

		if(cleanup)
	    {
	    	TRAP(err,CHttpServer::ExecuteL());
	    	delete cleanup;
	    }
		return err;
	}

TInt CHttpServer::ExecuteL()
	{
		// Create and install the active scheduler
		CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler;
		CleanupStack::PushL(activeScheduler);
		CActiveScheduler::Install(activeScheduler);

		//Initialize Ruby
		RhoRubyStart();
	
		//Send open home event
		SendWindowEvent( ECmdAppHome );
		
#ifdef ENABLE_RUBY_VM_STAT		
		g_httpd_thread_loaded = 1;
#endif		
		
		while ( !iClose )
			{
			shttpd_poll(ctx, 1000);
			}

		RhoRubyStop();

		CleanupStack::PopAndDestroy(activeScheduler);
		
	 	return 0;
	}

void CHttpServer::SendWindowEvent(TInt aEvent)
{
	// Create a window server event
	RWsSession wsSession;
	TWsEvent event;

	if ( wsSession.Connect() == KErrNone )
	{
		// Set event data. eventType.data = KData;
		event.SetType(EEventUser + aEvent); // set event type
		event.SetTimeNow(); // set the event time
		event.SetHandle(wsSession.WsHandle()); // set window server handle
		
		// Send the created event
		wsSession.SendEventToAllWindowGroups(event);
	}
}


void CHttpServer::ResumeThread()
	{
		thread.Resume();
	}

void CHttpServer::SuspendThread()
	{
		thread.Suspend();
	}


void CHttpServer::InitHttpServer()
	{
		//Init http server
	  	ctx = shttpd_init(0,NULL);
		
	    //
	    const char *rootpath = RhoGetRootPath();
	    char httproot[260];
	    
	    sprintf(httproot,"%sapps",rootpath);
	    shttpd_set_option(ctx, "root",httproot);
	    //
	    //shttpd_register_uri(ctx, "/system/geolocation", &show_geolocation, NULL);
	  	
//	  	shttpd_set_option(ctx, "root", APPS_PATH);
	}
