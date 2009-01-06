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

// INCLUDE FILES
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>

#include <rhodes.rsg>

#include "rhodes.hlp.hrh"
#include "rhodes.hrh"
#include "rhodes.pan"
#include "rhodesApplication.h"
#include "rhodesAppUi.h"
#include "rhodesAppView.h"

#include "HttpServer.h"
#include "SyncEngineWrap.h"

#ifndef ENABLE_RUBY_VM_STAT
#define ENABLE_RUBY_VM_STAT
#endif

#include "stat/stat.h"

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>

extern "C"
{
	void dosync();
}

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CRhodesAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRhodesAppUi::ConstructL()
	{
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin);

	TRect rect = ClientRect();
	rect.iTl.iY = 0;
	// Create view object
	iAppView = CRhodesAppView::NewL(rect, this );

	//Load http server
	iHttpServer = CHttpServer::NewL();
	iHttpServer->ResumeThread();

	//start sunc engine
	iSyncEngineWrap = CSyncEngineWrap::NewL(); 
	
	}
// -----------------------------------------------------------------------------
// CRhodesAppUi::CRhodesAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CRhodesAppUi::CRhodesAppUi()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CRhodesAppUi::~CRhodesAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CRhodesAppUi::~CRhodesAppUi()
	{
	if (iAppView)
		{
		delete iAppView;
		iAppView = NULL;
		}

	if (iHttpServer)
		{
		delete iHttpServer;
		iHttpServer = NULL;
		}
	
	if ( iSyncEngineWrap )
		{
		delete iSyncEngineWrap;
		iSyncEngineWrap = NULL;
		}
	}

void CRhodesAppUi::HandleApplicationSpecificEventL(TInt aType, const TWsEvent& aEvent)
	{
		if ( aType == (EEventUser + ECmdAppHome))
		{ 
			if ( iSyncEngineWrap )
				iSyncEngineWrap->ResumeThread();
			HandleCommandL(ECmdAppHome);
		}
		else
		{
			// Call the base class implementation
	        CEikAppUi::HandleApplicationSpecificEventL(aType, aEvent);
		}
	}


// -----------------------------------------------------------------------------
// CRhodesAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CRhodesAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EEikCmdExit:
		case EAknSoftkeyExit:
			
			if ( iSyncEngineWrap )
				iSyncEngineWrap->TerminateThread();
			
			Exit();
			break;
		case ESync:
			{
				dosync();
				break;
			}
		case EHelp:
			{

			CArrayFix<TCoeHelpContext>* buf = CCoeAppUi::AppHelpContextL();
			HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), buf);
			}
			break;
		case EAbout:
			{

			CAknMessageQueryDialog* dlg = new (ELeave)CAknMessageQueryDialog();
			dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
			HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
			dlg->QueryHeading()->SetTextL(*title);
			CleanupStack::PopAndDestroy(); //title
			HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
			dlg->SetMessageTextL(*msg);
			CleanupStack::PopAndDestroy(); //msg
			dlg->RunLD();
			}
			break;
#ifdef ENABLE_RUBY_VM_STAT			
		case EStat:
			{
			CAknMessageQueryDialog* dlg = new (ELeave)CAknMessageQueryDialog();
			dlg->PrepareLC(R_STAT_QUERY_DIALOG);
			HBufC* title = iEikonEnv->AllocReadResourceLC(R_STAT_DIALOG_TITLE);
			dlg->QueryHeading()->SetTextL(*title);
			CleanupStack::PopAndDestroy(); //title
			char buf[500] = {0};
			sprintf( buf,    "stat:\n___________________\n"
							 "iceq stat:\n "
							 "iseq binread: %u%s\n"
							 "iseq marshal: %u%s\n"
							 "require_compiled: %u%s\n"
							 "httpd thread loaded: %d\n"
							 "sync thread loaded: %d\n",
							 g_iseq_binread_msec, "msec",
							 g_iseq_marshal_load_msec, "msec",
							 g_require_compiled_msec, "msec",
							 g_httpd_thread_loaded,
							 g_sync_thread_loaded);
			
			TPtrC8 ptr8((TUint8*)buf);
			HBufC *msg = HBufC::NewLC(ptr8.Length());
			msg->Des().Copy(ptr8);
			dlg->SetMessageTextL(*msg);
			CleanupStack::PopAndDestroy(msg);
			dlg->RunLD();
			}	
			break;
#endif			
		default:
			iAppView->HandleCommandL(aCommand);
			break;
		}
	}
// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CRhodesAppUi::HandleStatusPaneSizeChange()
	{
	iAppView->SetRect(ClientRect() );
	}

CArrayFix<TCoeHelpContext>* CRhodesAppUi::HelpContextL() const
	{
#warning "Please see comment about help and UID3..."
	// Note: Help will not work if the application uid3 is not in the
	// protected range.  The default uid3 range for projects created
	// from this template (0xE0000000 - 0xEFFFFFFF) are not in the protected range so that they
	// can be self signed and installed on the device during testing.
	// Once you get your official uid3 from Symbian Ltd. and find/replace
	// all occurrences of uid3 in your project, the context help will
	// work.
	CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(array);
	array->AppendL(TCoeHelpContext(KUidRhodesApp, KGeneral_Information));
	CleanupStack::Pop(array);
	return array;
	}

// ----------------------------------------------------
// CBrowserDemoAppUi::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// ?implementation_description
// ----------------------------------------------------
//
TKeyResponse CRhodesAppUi::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
    return iAppView->HandleKeyEventL(aKeyEvent, aType);
}

void CRhodesAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
{
    iAppView->DynInitMenuPaneL(aResourceId, aMenuPane);
}

// End of File
