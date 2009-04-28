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
#include <akncommondialogs.h>
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>

#include "logging/RhoLog.h"

#include <eikbtgpc.h>
#include <rhodes.rsg>
#include "rhodesApplication.h"
#include "rhodesAppView.h"
#include "rhodesAppUI.h"
#include "RhoCamera.h"

//#include "AppSoftkeysObserver.h"
#include "SpecialLoadObserver.h"

#include "LogOptionsDialog.h"

#include <eiklabel.h>
#include <avkon.hrh>
#include <avkon.rsg>

#include "rhodes.hrh"

#include <w32std.h>   
#include <coemain.h>   
#include <aknsutils.h>   
#include <aknnavide.h>   
#include <barsread.h>   
#include <akntabgrp.h> 

#include <BrCtlInterface.h>
#include <CHARCONV.H>
#include <gdi.h>
#include <bitdev.h>

#include <commdb.h>

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "logging/RhoLog.h"

#include "ports_mngt.h"

_LIT(KTitle, "Select image file");

CRhodesAppView* g_RhodesAppView = NULL;

// Constants   
const TInt KErrorMsgXPos = 0;   
const TInt KErrorMsgYPos = 50;   
const TInt KErrorMaxWidth = 160;   
const TInt KRedColor = 35; 
const TInt KWhiteColor = 0;

extern "C" {
	void perform_notification(char* callback, char* params);
	void webview_refresh();
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
	
	callbackUrl = NULL;
	iBitmap = NULL;
	iOffScreenBitmap = NULL;
	iFbsBitGc = NULL;
	iBmpDevice = NULL;
	
	iRhoCamera = CRhoCamera::NewL(this, aRect);
	
	// This should be called after the windows has been activated   
	CreateOffScreenBitmapL();   
	
	iCommandBase = TBrCtlDefs::ECommandIdBase;
	
	//iAppSoftkeysObserver = CAppSoftkeysObserver::NewL(this);
	iSpecialLoadObserver = CSpecialLoadObserver::NewL();
	
	iBrCtlCapabilities = TBrCtlDefs::ECapabilityDisplayScrollBar | TBrCtlDefs::ECapabilityLoadHttpFw | TBrCtlDefs::ECapabilityCursorNavigation |
	                     TBrCtlDefs::ECapabilityGraphicalPage | TBrCtlDefs::ECapabilityAccessKeys ;

	CreateBasicBrowserControlL();

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
		if (iOffScreenBitmap)   
	        delete iOffScreenBitmap;   
	    if (iFbsBitGc)   
	        delete iFbsBitGc;   
	    if (iBmpDevice)       
	        delete iBmpDevice;   
	       
	    iBitmap = NULL;
	    
		if ( iRhoCamera )
			{
			delete iRhoCamera;
			iRhoCamera = NULL;
			}
	    
		if (iBrCtlInterface)
	        delete iBrCtlInterface;
		
		//if ( iAppSoftkeysObserver )
		//	delete iAppSoftkeysObserver;
		
		delete iSpecialLoadObserver;
		
	}

void CRhodesAppView::DrawImage(CWindowGc& aGc,   
    const TRect& aRect) const   
    {   
    TSize bmpSizeInPixels( iBitmap->SizeInPixels() );   
    TInt xDelta = (aRect.Width() - bmpSizeInPixels.iWidth) / 2;   
    TInt yDelta = (aRect.Height() - bmpSizeInPixels.iHeight) / 2;   
    TPoint pos( xDelta, yDelta ); // displacement vector   
    pos += aRect.iTl; // bitmap top left corner position   
   
    // Drawing viewfinder image to bitmap   
    iFbsBitGc->BitBlt( pos, iBitmap, TRect( TPoint( 0, 0 ), bmpSizeInPixels ));   
   
    // Draws bitmap with indicators on the screen   
    TSize size( iOffScreenBitmap->SizeInPixels() );   
    aGc.BitBlt( TPoint(0,0), iOffScreenBitmap, TRect( TPoint(0,0), size ) );   
    }

void CRhodesAppView::CreateOffScreenBitmapL()   
    {   
    iOffScreenBitmap = new (ELeave) CWsBitmap( iCoeEnv->WsSession() );   
       
    TSize size = Rect().Size();   
   
    TInt createError = iOffScreenBitmap->Create( size, iRhoCamera->DisplayMode());   
    User::LeaveIfError( createError );   
   
    iFbsBitGc = CFbsBitGc::NewL(); //graphic context   
    iBmpDevice = CFbsBitmapDevice::NewL(iOffScreenBitmap);   
    iFbsBitGc->Activate(iBmpDevice);   
    iFbsBitGc->SetBrushColor( AKN_LAF_COLOR( KWhiteColor ) );   
   
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();   
    CFbsBitmap* bitmap = AknsUtils::GetCachedBitmap( skin,    
        KAknsIIDQsnBgAreaMain);   
    if ( bitmap )    
        {   
        iFbsBitGc->BitBlt( TPoint(0,0), bitmap );   
        }   
   
    iOffScreenBitmapCreated = ETrue;   
    } 

void CRhodesAppView::DrawImageNow(CFbsBitmap* aBitmap)   
    {   
    iBitmap = aBitmap;   
    DrawNow();   
    }

void CRhodesAppView::ReDrawOffScreenBitmap()   
    {   
    iOffScreenBitmapCreated = ETrue;   
   
    iFbsBitGc->SetBrushColor( AKN_LAF_COLOR( KWhiteColor ) );   
    iFbsBitGc->Clear();   
   
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();   
    CFbsBitmap* bitmap = AknsUtils::GetCachedBitmap( skin,    
        KAknsIIDQsnBgAreaMain );   
    if ( bitmap )    
        {   
        iFbsBitGc->BitBlt( TPoint(0,0), bitmap );   
        }   
   
    DrawNow();  // clear screen    
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
	int len = strlen(get_home_url())+strlen(slash)+strlen(start_page);
	char* sp = (char*) malloc(len+1);
	sprintf(sp,"%s%s%s",get_home_url(),slash,start_page);

	RFs session;
	User::LeaveIfError(session.Connect());
	
	CCnvCharacterSetConverter *converter = CCnvCharacterSetConverter::NewL();
  	converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierUtf8, session);
  
  	TPtrC8 ptr((const unsigned char*)sp);

  	int state = CCnvCharacterSetConverter::KStateDefault;
  	converter->ConvertToUnicode(iStartPage, ptr, state);
	
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
		int len = strlen(get_home_url())+strlen(slash)+strlen(options_page);
		char* sp = (char*) malloc(len+1);
		sprintf(sp,"%s%s%s",get_home_url(),slash,options_page);

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
		CWindowGc& gc = SystemGc();
		
		TRect drawingRect = Rect(); 
		// Get the standard graphics context
		
		TBool drawImage = EFalse;
		
		if ( iOffScreenBitmapCreated )   
			{   
				if ( iBitmap) //Viewfinding ongoing   
					{   
					drawImage = ETrue;
					MAknsSkinInstance* skin = AknsUtils::SkinInstance();   
					CFbsBitmap* bitmap = AknsUtils::GetCachedBitmap( skin,    
						KAknsIIDQsnBgAreaMain );   
					if ( bitmap )    
						{   
						gc.BitBlt( TPoint(0,0), bitmap);   
						}   
					else   
						{   
						// Draws bitmap with indicators on the screen   
						TSize size( iOffScreenBitmap->SizeInPixels() );   
						gc.BitBlt( TPoint(0,0), iOffScreenBitmap,    
							TRect( TPoint(0,0), size ) );   
						
						gc.BitBlt( TPoint(0,0), iOffScreenBitmap);
						
						}      
					}
			}

		if ( !drawImage )
			{
			gc.SetPenStyle(CGraphicsContext::ESolidPen);
			gc.SetBrushColor(KRgbWhite);
			gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
			gc.DrawRect(aRect);	
			}    
	}

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
		/*if ( iBrCtlInterface )
		{
			iBrCtlInterface->ClearCache();
		}*/
		
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
	            /*if (iBrCtlInterface)
	            {
	                iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandReload);
	            }*/
				webview_refresh();
				
				break;
	        }
	        case EAknSoftkeyBack:
        	{
        		if (iBrCtlInterface)
    			{
					//iBrCtlInterface->ClearCache();
		            TBrCtlDefs::TBrCtlElementType type = iBrCtlInterface->FocusedElementType();
		            
		            if(type == TBrCtlDefs::EElementActivatedInputBox )
		            	break;
    			}
    		}
	        case ECmdAppBack:
	        {
	            if (iBrCtlInterface)
	            {
					//iBrCtlInterface->ClearCache();
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
        				iBrCtlInterface->LoadUrlL(iStartPage);
        			}
        		break;
        	}
	        case EOptions:
			{
				if (iBrCtlInterface)
					{
						iBrCtlInterface->LoadUrlL(iOptionsPage);
					    //iBrCtlInterface->ClearCache();
					}
				break;
			}	
	        case ECmdTakePicture:
	        	{
	        	iRhoCamera->StopViewFinder();
				iRhoCamera->StartViewFinderL();
	        	break;
	        	}
	        case ECmdChoosePicture:
	        	{
	        	ChoosePicture();	
	        	break;
	        	}
			case ELoggingOptions:
				{
					
					iBrCtlInterface->SetDimmed( ETrue);
					iBrCtlInterface->MakeVisible( EFalse );
					iBrCtlInterface->DrawNow();
					
					 CLogOptionsDialog::RunDlgLD();
					 
					 iBrCtlInterface->SetDimmed( EFalse );
					 iBrCtlInterface->MakeVisible( ETrue );
					 iBrCtlInterface->DrawNow();
					 
					 break;
				}
			case ELogView:
				{
					rho::String strText;
					LOGCONF().getLogText(strText);
						
					TPtrC8 ptr8((TUint8*)strText.c_str());
					HBufC *msg = HBufC::NewLC(ptr8.Length());
					msg->Des().Copy(ptr8);

					CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*msg);
					dlg->PrepareLC(R_STAT_QUERY_DIALOG);
					HBufC* title = iEikonEnv->AllocReadResourceLC(R_LOG_VIEW_DIALOG_TITLE);
					dlg->QueryHeading()->SetTextL(*title);
					CleanupStack::PopAndDestroy(); //title
					
					dlg->SetExtentToWholeScreen();
					dlg->RunLD();
					
					CleanupStack::PopAndDestroy(msg);
										
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
	        
	        //iBrCtlInterface->ClearCache();
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
			{
				iBrCtlInterface->HandleCommandL(TBrCtlDefs::ECommandIdBase + TBrCtlDefs::ECommandClearHistory);
			    iBrCtlInterface->ClearCache();
				iBrCtlInterface->LoadUrlL(aUrl, -1, TBrCtlDefs::ECacheModeNoCache);
				
				//iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandReload);
			}
	}

void CRhodesAppView::LoadUrl(char* aUrl)
	{
		char* url = CanonicalizeURL(aUrl);
		TPtrC8 ptr(reinterpret_cast<const TUint8*>(url));
	    HBufC* buffer = HBufC::NewL(ptr.Length());
	    buffer->Des().Copy(ptr);
	    
	    TPtrC aDescriptor;
		aDescriptor.Set(*buffer);
		
	    LoadUrl(aDescriptor);
	    delete buffer;
	    free(url);
	
	}

// ----------------------------------------------------
// CRhodesAppView::GetCurrentUrl()
// ----------------------------------------------------
//
/*char* CRhodesAppView::GetCurrentPageUrl()
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
*/
// ----------------------------------------------------
// CRhodesAppView::Refresh()
// ----------------------------------------------------
//

/*void CRhodesAppView::Refresh()
	{
		if ( iBrCtlInterface == NULL )
			CreateBasicBrowserControlL();
			
		if ( iBrCtlInterface != NULL )
			iBrCtlInterface->HandleCommandL(iCommandBase + TBrCtlDefs::ECommandReload);
	}
*/
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

char* CRhodesAppView::CanonicalizeURL(const char* path)
	{
		if (!path) {
			return strdup(get_home_url());
		}

		if ( strncmp("http://",path,7)==0 ) {
			return strdup(path);
		}

		char* slash = "";
		if ( (*path!='/')&&(*path!='\\') ) {
			slash = "/";
		}

		int len = strlen(get_home_url())+strlen(slash)+strlen(path);
		char* url = (char*) malloc(len+1);
		sprintf(url,"%s%s%s",get_home_url(),slash,path);
		
		return url;
	}

void CRhodesAppView::ChoosePicture()
	{
		if ( GetCallbackUrl() )
		{
			TFileName file(KNullDesC);
			char params[500] = {0};
								
			if ( AknCommonDialogs::RunSelectDlgLD(file, R_FILE_SELECTION_DIALOG, KTitle))
				{
					//send callback
					TInt length = file.Length();
						 
					HBufC8* buffer = HBufC8::NewLC(length);
					buffer->Des().Copy(file);
				 
					char* fileName = (char*)malloc(length + 1);
					Mem::Copy(fileName, buffer->Ptr(), length);
					fileName[length] = '\0';
				 
					CleanupStack::PopAndDestroy(buffer);
					
					sprintf(params,"status=ok&image_uri=%s",fileName);
					free(fileName);
										
					char* tmp = CanonicalizeURL( GetCallbackUrl() );
					perform_notification(tmp, params);
					free(tmp);
				}
			else
				{
					sprintf(params,"status=cancel&message=%s","User canceled operation");
										
					char* tmp = CanonicalizeURL( GetCallbackUrl() );
					perform_notification(tmp, params);
					free(tmp);
				}
		}
	}
// End of File

