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

#include "AppSoftkeysObserver.h"
#include "StateChangeObserver.h"
#include "LayoutObserver.h"
#include "SpecialLoadObserver.h"

#include <eiklabel.h>
#include <avkon.hrh>

#include "rhodes.hrh"

#include <BrCtlInterface.h>
#include <CHARCONV.H>
#include <gdi.h>
#include <bitdev.h>

#include <commdb.h>

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
	// Create a window for this application view
	CreateWindowL();

	// Set the windows size
	SetRect(aRect);

	// Activate the window, which makes it ready to be drawn
	ActivateL();
	
	iCommandBase = TBrCtlDefs::ECommandIdBase;
	
	iAppSoftkeysObserver = CAppSoftkeysObserver::NewL(this);
	iStateChangeObserver = CStateChangeObserver::NewL(this);
	iLayoutObserver = CLayoutObserver::NewL(this);
	iSpecialLoadObserver = CSpecialLoadObserver::NewL();
	
	iBrCtlCapabilities = TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityLoadHttpFw |
	                     TBrCtlDefs::ECapabilityGraphicalHistory | TBrCtlDefs::ECapabilityGraphicalPage | 
	                     TBrCtlDefs::ECapabilityAccessKeys;

	CreateBasicBrowserControlL();

	// These observers can be added and removed dynamically
    iBrCtlInterface->AddStateChangeObserverL(iStateChangeObserver);
	
	if ( iBrCtlInterface != NULL )
		{
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
	
	CEikButtonGroupContainer* current = CEikButtonGroupContainer::Current();
	current->SetCommandSetL( R_BROWSER_DEFAULT_BUTTONS );
	
	//loading start page
	_LIT(KLoadingPage, "file:///c:\\Data\\Rho\\apps\\loading.html");
	
	LoadUrl(KLoadingPage);
	
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
	    {
	    	iBrCtlInterface->RemoveStateChangeObserver(iStateChangeObserver);
	        delete iBrCtlInterface;
	    }
		
		delete iStateChangeObserver;
		delete iAppSoftkeysObserver;
		delete iLayoutObserver;
		delete iSpecialLoadObserver;
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
	        case ECmdAppBack:
	        {
	            if (iBrCtlInterface)
	            {
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
        				_LIT(KHomePage, "http://127.0.0.1:8080/");
        				iBrCtlInterface->LoadUrlL(KHomePage);
        			}
        		break;
        	}
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
	            iAppSoftkeysObserver, 
	            NULL,
	            iSpecialLoadObserver,
	            iLayoutObserver, 
	            NULL);
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
    	return iBrCtlInterface->OfferKeyEventL(aKeyEvent, aType);
    }
    return EKeyWasNotConsumed;
}


// End of File

