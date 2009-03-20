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
#include <coemain.h>
#include <eikbtgpc.h>
#include <rhodes.rsg>
#include "rhodesApplication.h"
#include "rhodesAppView.h"
#include "rhodesAppUI.h"

//#include "AppSoftkeysObserver.h"
#include "SpecialLoadObserver.h"
#include "BrCtlLoadEventObserver.h"

#include <eiklabel.h>
#include <avkon.hrh>
#include <avkon.rsg>

#include "rhodes.hrh"

#include <BrCtlInterface.h>
#include <CHARCONV.H>
#include <gdi.h>
#include <bitdev.h>

#include <commdb.h>

#include "tcmalloc/rhomem.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "UniversalLock.h"

CRhodesAppView* g_RhodesAppView = NULL;

extern "C" {

	char* g_current_url = NULL;

	INIT_LOCK(change_url);

	char* webview_current_location() {
	    LOCK(change_url);

	    char* retval = NULL;
	    try {
	    	retval = strdup(g_current_url);
	    }catch(...){
	    }
	    UNLOCK(change_url);
	    
	    return retval;
	}

	void webview_set_current_location(char* url) {
		LOCK(change_url);
		
		if ( g_current_url )
			free(g_current_url);
		
		g_current_url = url;
		
		UNLOCK(change_url);
	}


	char* callGetStartPage();
	char* callGetOptionsPage();

}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRhodesAppView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRhodesAppView* CRhodesAppView::NewL(const TRect& aRect, CRhodesAppUi * aAppUI)
	{
	CRhodesAppView* self = CRhodesAppView::NewLC(aRect, aAppUI);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CRhodesAppView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRhodesAppView* CRhodesAppView::NewLC(const TRect& aRect, CRhodesAppUi * aAppUI)
	{
	CRhodesAppView* self = new ( ELeave ) CRhodesAppView;
	CleanupStack::PushL(self);
	self->SetMopParent(aAppUI);
	self->ConstructL(aRect);
	return self;
	}

// -----------------------------------------------------------------------------
// CRhodesAppView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRhodesAppView::ConstructL(const TRect& aRect)
	{
	//save a pointer to our self
	g_RhodesAppView = this;
	
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

	// Activate the window, which makes it ready to be drawn
	ActivateL();
	
	iCommandBase = TBrCtlDefs::ECommandIdBase;
	
	//iAppSoftkeysObserver = CAppSoftkeysObserver::NewL(this);
	iSpecialLoadObserver = CSpecialLoadObserver::NewL();
	
	iCBrCtlLoadEventObserver = CCBrCtlLoadEventObserver::NewL(this);
	
	iBrCtlCapabilities = TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityLoadHttpFw | TBrCtlDefs::ECapabilityCursorNavigation |
	                     TBrCtlDefs::ECapabilityGraphicalPage | TBrCtlDefs::ECapabilityAccessKeys ;

	CreateBasicBrowserControlL();

	if ( iBrCtlInterface != NULL )
		{
			iBrCtlInterface->AddLoadEventObserverL(iCBrCtlLoadEventObserver);
		
			TRect rect( Position(), Size() );
		    iBrCtlInterface->SetRect( rect );
		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 1);
		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSecurityWarnings, 1);
		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsAutoLoadImages, 1);
		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsCSSFetchEnabled, 1);
		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsECMAScriptEnabled, 1);
		    iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsBrowserUtf8Encoding, 1);
		    
		    //iBrCtlInterface->SetExtent(Position(), Size());
		    
		}
	
	//loading start page
	_LIT(KLoadingPage, "\\Data\\Rho\\apps\\loading.html");
	
	RFs rfs;
    RFile file;
    User::LeaveIfError(rfs.Connect());
    CleanupClosePushL(rfs);
    if ( file.Open(rfs, KLoadingPage(), EFileShareReadersOrWriters) == KErrNone )
	{    
		CleanupClosePushL(file);
	    iBrCtlInterface->LoadFileL(file);
	    file.Close();
	    CleanupStack::PopAndDestroy(1); // file
	}
    CleanupStack::PopAndDestroy(1); // rfs
	
    iBrCtlInterface->HandleCommandL(TBrCtlDefs::ECommandIdBase + TBrCtlDefs::ECommandClearHistory);
    iBrCtlInterface->ClearCache();
    
    }

// -----------------------------------------------------------------------------
// CRhodesAppView::CRhodesAppView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CRhodesAppView::CRhodesAppView()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CRhodesAppView::~CRhodesAppView()
// Destructor.
// -----------------------------------------------------------------------------
//
CRhodesAppView::~CRhodesAppView()
	{
		if (iBrCtlInterface)
	        delete iBrCtlInterface;
		
		//if ( iAppSoftkeysObserver )
		//	delete iAppSoftkeysObserver;
		
		delete iSpecialLoadObserver;
		
		iBrCtlInterface->RemoveLoadEventObserver(iCBrCtlLoadEventObserver);
		
		if ( iCBrCtlLoadEventObserver )
			delete iCBrCtlLoadEventObserver;
	}


void CRhodesAppView::InitStartPage()
{
	char* slash = "";
	char* start_page = callGetStartPage();
	if (!start_page) {
		start_page = "";
	} else if ( (*start_page!='/')&&(*start_page!='\\') ) {
		slash = "/";
	}
	int len = strlen("http://127.0.0.1:8080")+strlen(slash)+strlen(start_page);
	char* sp = (char*) malloc(len+1);
	sprintf(sp,"http://127.0.0.1:8080%s%s",slash,start_page);

	RFs session;
	User::LeaveIfError(session.Connect());
	
	CCnvCharacterSetConverter *converter = CCnvCharacterSetConverter::NewL();
  	converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierUtf8, session);
  
  	TPtrC8 ptr((const unsigned char*)sp);

  	int state = CCnvCharacterSetConverter::KStateDefault;
  	converter->ConvertToUnicode(iStartPage, ptr, state);
	
//  	_LIT(KHomePage, "http://127.0.0.1:8080/");
//  	iStartPage = KHomePage;
  	
  	session.Close();
  	
	free(sp);
}

void CRhodesAppView::InitOptions()
	{
		char* slash = "";
		char* options_page = callGetOptionsPage();
		if (!options_page) {
		options_page = "";
		} else if ( (*options_page!='/')&&(*options_page!='\\') ) {
			slash = "/";
		}
		int len = strlen("http://127.0.0.1:8080")+strlen(slash)+strlen(options_page);
		char* sp = (char*) malloc(len+1);
		sprintf(sp,"http://127.0.0.1:8080%s%s",slash,options_page);

		RFs session;
		User::LeaveIfError(session.Connect());
		
		CCnvCharacterSetConverter *converter = CCnvCharacterSetConverter::NewL();
	  	converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierUtf8, session);
	  
	  	TPtrC8 ptr((const unsigned char*)sp);

	  	int state = CCnvCharacterSetConverter::KStateDefault;
	  	converter->ConvertToUnicode(iOptionsPage, ptr, state);
		
	  	session.Close();
	  	
		free(sp);
	}

// -----------------------------------------------------------------------------
// CRhodesAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CRhodesAppView::Draw(const TRect& aRect) const
	{
		// Get the standard graphics context
		CWindowGc& gc = SystemGc();
		
		gc.SetPenStyle(CGraphicsContext::ESolidPen);
	    gc.SetBrushColor(KRgbWhite);
	    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
	    gc.DrawRect(aRect);	}

// -----------------------------------------------------------------------------
// CRhodesAppView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CRhodesAppView::SizeChanged()
	{
		if (iBrCtlInterface)
			iBrCtlInterface->SetRect(Rect());
	}

// ---------------------------------------------------------
// CRhodesAppView::CountComponentControls() const
// ---------------------------------------------------------
//
TInt CRhodesAppView::CountComponentControls() const
	{
	    if (iBrCtlInterface)
	        return 1;
	    return 0;
	}

// ---------------------------------------------------------
// CRhodesAppView::ComponentControl(TInt aIndex) const
// ---------------------------------------------------------
//
CCoeControl* CRhodesAppView::ComponentControl(TInt aIndex) const
	{
	    switch ( aIndex )
	    {
	        case 0:
	            return iBrCtlInterface; // Could be NULL
	        default:
	            return NULL;
	    }
	}


// ----------------------------------------------------
// CBrowserDemoAppView::CancelLoad()
// ----------------------------------------------------
//
void CRhodesAppView::CancelLoad()
	{
	    iCancelInitDataLoad = ETrue;
	}


// ----------------------------------------------------
// CRhodesAppView::HandleCommandL(TInt aCommand)
// ----------------------------------------------------
//
void CRhodesAppView::HandleCommandL(TInt aCommand)
	{
		if ( iBrCtlInterface )
		{
			iBrCtlInterface->ClearCache();
		}
		
	    switch ( aCommand )
	    {
	        case ECmdAppRestoreSetting:
	        {
	            if (iBrCtlInterface)
	            {
	                TRect rect( Position(), Size() );
	                iBrCtlInterface->SetRect( rect );
	                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSmallScreen, 1);
	                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsSecurityWarnings, 1);
	                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsAutoLoadImages, 1);
	                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsCSSFetchEnabled, 1);
	                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsECMAScriptEnabled, 1);
	                iBrCtlInterface->SetBrowserSettingL(TBrCtlDefs::ESettingsBrowserUtf8Encoding, 1);
	            }
	            break;
	        }
	        case ECmdAppReload:
	        {
	            if (iBrCtlInterface)
	            {
	                iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandReload);
	            }
	            break;
	        }
	        case EAknSoftkeyBack:
        	{
        		if (iBrCtlInterface)
    			{
					iBrCtlInterface->ClearCache();
		            TBrCtlDefs::TBrCtlElementType type = iBrCtlInterface->FocusedElementType();
		            
		            if(type == TBrCtlDefs::EElementActivatedInputBox )
		            	break;
    			}
    		}
	        case ECmdAppBack:
	        {
	            if (iBrCtlInterface)
	            {
					iBrCtlInterface->ClearCache();
	                iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandBack);
	            }
	            break;
	        }
	        case ECmdAppForward:
	        {
	            if (iBrCtlInterface)
	            {
	                iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandForward);
				}
	            break;
	        }
	        case ECmdAppHome:
        	{
        		if (iBrCtlInterface)
        			{
						//_LIT(KHomePage, "http://127.0.0.1:8080/");
        				iBrCtlInterface->LoadUrlL(iStartPage);
        				iBrCtlInterface->ClearCache();
        			}
        		break;
        	}
	        case EOptions:
			{
				if (iBrCtlInterface)
					{
						iBrCtlInterface->LoadUrlL(iOptionsPage);
					    iBrCtlInterface->ClearCache();
					}
				break;
			}	
	        /*case EAknSoftkeyCancel:
        	{
        		CEikButtonGroupContainer* current = CEikButtonGroupContainer::Current();
	        	if ( current )
        		{
	        		current->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );
	        		current->SetFocus(EFalse, EDrawNow);
        		}
	        	break;
        	}*/
	        default:
	        	if (aCommand >= iCommandBase &&
	                aCommand < iCommandBase + TBrCtlDefs::ECommandIdWMLBase &&
	                iBrCtlInterface)
	                {
	                iBrCtlInterface->HandleCommandL(aCommand);
	                }

	            if (aCommand >= iCommandBase + TBrCtlDefs::ECommandIdWMLBase &&
	                aCommand < iCommandBase + TBrCtlDefs::ECommandIdRange &&
	                iBrCtlInterface)
	                {
	                iBrCtlInterface->HandleCommandL(aCommand);
	                }              
	            break;      
	    }
	    
	}

// ----------------------------------------------------
// CRhodesAppView::CreateBasicBrowserControlL()
// ----------------------------------------------------
//
void CRhodesAppView::CreateBasicBrowserControlL()
	{
	    if (iBrCtlInterface != NULL)
	    {
	        delete iBrCtlInterface;
	        iBrCtlInterface = NULL;
	    }
	    else
	    {
	        TRect rect(Position(), Size());
	        
	        iBrCtlInterface = CreateBrowserControlL( this, 
	            rect, 
	            iBrCtlCapabilities,
	            iCommandBase, 
	            /*iAppSoftkeysObserver*/NULL, 
	            NULL,
	            iSpecialLoadObserver,
	            NULL, 
	            NULL);
	        
	        iBrCtlInterface->ClearCache();
		}
	}

// ----------------------------------------------------
// CRhodesAppView::LoadUrl()
// ----------------------------------------------------
//
void CRhodesAppView::LoadUrl(const TDesC& aUrl)
	{
		if ( iBrCtlInterface == NULL )
			CreateBasicBrowserControlL();
		
		if ( iBrCtlInterface != NULL )
			iBrCtlInterface->LoadUrlL(aUrl);
	}

void CRhodesAppView::LoadUrl(char* aUrl)
	{
		TPtrC8 ptr(reinterpret_cast<const TUint8*>(aUrl));
	    HBufC* buffer = HBufC::NewL(ptr.Length());
	    buffer->Des().Copy(ptr);
	    
	    TPtrC aDescriptor;
		aDescriptor.Set(*buffer);
		
	    LoadUrl(aDescriptor);
	    delete buffer;
	
	}

// ----------------------------------------------------
// CRhodesAppView::GetCurrentUrl()
// ----------------------------------------------------
//
char* CRhodesAppView::GetCurrentPageUrl()
	{
		if ( iBrCtlInterface == NULL )
			CreateBasicBrowserControlL();
			
		if ( iBrCtlInterface != NULL )
			{
			HBufC* pBuf = iBrCtlInterface->PageInfoLC(TBrCtlDefs::EPageInfoUrl);
			TPtrC aDescriptor;
			aDescriptor.Set(*pBuf);
			
			TInt length = aDescriptor.Length();
		 
			HBufC8* buffer = HBufC8::NewLC(length);
			buffer->Des().Copy(aDescriptor);
		 
			char* str = (char*)malloc(length + 1);
			Mem::Copy(str, buffer->Ptr(), length);
			str[length] = '\0';
		 
			CleanupStack::PopAndDestroy(buffer);
		 
			return str;
			}
		return NULL;
	}

// ----------------------------------------------------
// CRhodesAppView::Refresh()
// ----------------------------------------------------
//

void CRhodesAppView::Refresh()
	{
		if ( iBrCtlInterface == NULL )
			CreateBasicBrowserControlL();
			
		if ( iBrCtlInterface != NULL )
			iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandReload);
	}

// ----------------------------------------------------
// CRhodesAppView::OfferKeyEventL(const 
//               TKeyEvent& aKeyEvent,TEventCode aType)
// ----------------------------------------------------
//
TKeyResponse CRhodesAppView::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
    if (iBrCtlInterface)
    {
        return iBrCtlInterface->OfferKeyEventL(aKeyEvent, aType);
    }
    return EKeyWasConsumed;
}

// ---------------------------------------------------------
// CRhodesAppView::HandleControlEventL(
//     CCoeControl* aControl,TCoeEvent aEventType)
// ---------------------------------------------------------
//
void CRhodesAppView::HandleControlEventL( CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
{
    // TODO: Add control event handler code here
}

// ----------------------------------------------------
// CRhodesAppView::HandleKeyEventL(
//     const TKeyEvent& aKeyEvent,TEventCode /*aType*/)
// ----------------------------------------------------
//
TKeyResponse CRhodesAppView::HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	if (iBrCtlInterface)
    {	
    	TBrCtlDefs::TBrCtlElementType type = iBrCtlInterface->FocusedElementType();
		            
    	if(type != TBrCtlDefs::EElementActivatedInputBox )
    		return iBrCtlInterface->OfferKeyEventL(aKeyEvent, aType);
    }
    return EKeyWasNotConsumed;
}

void CRhodesAppView::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
{
    if (iBrCtlInterface && aResourceId == R_MENU )
    {
        iBrCtlInterface->AddOptionMenuItemsL(*aMenuPane, aResourceId);
        RPointerArray<TBrCtlWmlServiceOption>* options;
        options = iBrCtlInterface->WMLOptionMenuItemsL();
        TInt i;
        TInt count = options->Count();
        for (i = 0; i < count; i++)
    	{
            TBrCtlWmlServiceOption* option = (*options)[i];
            if (option != NULL)
            {
                CEikMenuPaneItem::SData item;
                item.iText.Copy(option->Text());
                item.iCommandId = option->ElemID();
                item.iFlags = 0;
                item.iCascadeId = 0;
                aMenuPane->InsertMenuItemL(item, 0);
            }
        }
    }
}
// End of File

