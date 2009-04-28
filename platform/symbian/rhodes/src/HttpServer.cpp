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

#include <e32math.h>  //Rand
#include <AknGlobalNote.h>

#include "tcmalloc/rhomem.h"

#include "rhoruby/rhoruby.h"
#include <stdio.h>
#include <string.h>

#ifndef ENABLE_RUBY_VM_STAT
#define ENABLE_RUBY_VM_STAT
#endif

#include "stat/stat.h"

#include "ports_mngt.h"

CHttpServer* g_http_server = NULL; 

extern "C" 
{
	struct shttpd_ctx *shttpd_init(int argc, char *argv[]);
	void shttpd_fini(struct shttpd_ctx *);
	void shttpd_poll(struct shttpd_ctx *, int milliseconds);
	int	set_ports(struct shttpd_ctx *ctx, const char *p);

	int shttpd_set_option(struct shttpd_ctx *, const char *opt, const char *val);
	const char* shttpd_get_index_names(struct shttpd_ctx *ctx);
	
	void shutdown_poll(struct shttpd_ctx *);	
	void rb_gc(void); 
	char* rho_resolve_url(char* url, const char* root,const char *index_names);

	const char* RhoGetRootPath();
	
	int g_need_launch_gc = 0;
}

TInt ThreadEntryPoint(TAny *aPtr)
{
	CHttpServer *pHttpServer = (CHttpServer *)aPtr;
	return pHttpServer->Execute();
}

CHttpServer::CHttpServer()
	:iClose(false), iStopRubyFramework(false), iStartRubyFramework(false)
	{
	// No implementation required
	}

CHttpServer::~CHttpServer()
	{
		iClose = true;
		
		//shttpd_fini(ctx);
		
		//thread.Kill( KErrCancel );
		thread.Close();
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
		g_http_server = this;
		//initialize shttpd
		//InitHttpServer();
		
		//create suspended thread
		_LIT(KThreadName, "HttpServerThread");
		
		TBuf<30> threadName;
		threadName.Append(KThreadName);

		TTime time;
		time.HomeTime();

		TInt64 aSeed = time.Int64();
		TInt randNum = Math::Rand(aSeed) % 1000;
		threadName.AppendNum(randNum);
				
		//KMinHeapSize, 256*KMinHeapSize
		TInt res = thread.Create(threadName, ThreadEntryPoint, 
				//0x1000000, 0x5000, 0x1000000,
				80000, 0x5000, 0x1000000,
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
	
		InitHttpServer();
		
		//Send open home event
		SendWindowEvent( ECmdAppHome );
		
#ifdef ENABLE_RUBY_VM_STAT		
		g_httpd_thread_loaded = 1;
#endif		
		
		while ( !iClose )
		{
			if ( iStopRubyFramework )
			{
				iStopRubyFramework = false;
				RhoRubyStop();
			}
			
			if ( iStartRubyFramework )
			{
				iStartRubyFramework = false;
				RhoRubyStart();
			}
			
			if ( g_need_launch_gc )
			{
				g_need_launch_gc = 0;
				rb_gc();
			}
			
			shttpd_poll(ctx, 100000);
		}

		//shttpd_fini(ctx);
		
		//RhoRubyStop();

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

void CHttpServer::StopThread()
	{
	//SuspendThread();
	thread.Kill( KErrCancel );
	iClose = true;
	shutdown_poll(ctx);
	shttpd_fini(ctx);
	//ResumeThread();
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
	    
	    set_ports(ctx, get_free_listening_port());
	    //
	    //shttpd_register_uri(ctx, "/system/geolocation", &show_geolocation, NULL);
	  	
//	  	shttpd_set_option(ctx, "root", APPS_PATH);
	}

struct shttpd_ctx* CHttpServer::GetHttpdContext()
	{
		return ctx;
	}

extern "C" char* HTTPResolveUrl(char* url) {
	const char* empty_str = "";
    char* ret = NULL;
	if (g_http_server) {
		struct shttpd_ctx* ctx = g_http_server->GetHttpdContext();
		char httproot[1024];
		const char *rootpath = RhoGetRootPath();
		sprintf(httproot,"%sapps",rootpath);
		ret = rho_resolve_url(url, httproot, shttpd_get_index_names(ctx));
	}
	return ret ? ret : strdup(empty_str);
}
