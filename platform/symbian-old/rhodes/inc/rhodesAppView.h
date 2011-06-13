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

#ifndef __RHODESAPPVIEW_h__
#define __RHODESAPPVIEW_h__

// INCLUDES
#include <coecntrl.h>

//include browser control
#include "BrCtlInterface.h"

//FORWARD DECLARATION
class CRhodesAppUi;
class CSpecialLoadObserver;
class CCBrCtlLoadEventObserver;
//class CAppSoftkeysObserver;
class CRhodesAppView;
class CRhoCamera;

//pointer to the CRhodesAppView object, used by method written in pure C
extern CRhodesAppView* g_RhodesAppView;

// CLASS DECLARATION
class CRhodesAppView : public CCoeControl, MCoeControlObserver, MBrCtlDataLoadSupplier
	{
public:
	// New methods

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Create a CrhodesAppView object, which will draw itself to aRect.
	 * @param aRect The rectangle this view will be drawn to.
	 * @return a pointer to the created instance of CrhodesAppView.
	 */
	static CRhodesAppView* NewL(const TRect& aRect, CRhodesAppUi * aAppUI);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Create a CrhodesAppView object, which will draw itself
	 * to aRect.
	 * @param aRect Rectangle this view will be drawn to.
	 * @return A pointer to the created instance of CrhodesAppView.
	 */
	static CRhodesAppView* NewLC(const TRect& aRect, CRhodesAppUi * aAppUI);

	/**
	 * ~CRhodesAppView
	 * Virtual Destructor.
	 */
	virtual ~CRhodesAppView();

    /**
     * Load url
     */
    void LoadUrl(const TDesC& aUrl);
    void LoadUrl(char* aUrl);
    
    //void Refresh();
    //char* GetCurrentPageUrl();
    
    void DrawImageNow(CFbsBitmap* aBitmap);
    
    
public:
	// Functions from base classes

	/**
    * Pass a command to the Browser Control
    * @since 2.8
    * @param aCommand The command that the Browser Control should process
    * @return void
    */
    void HandleCommandL(TInt aCommand);

    /**
    * Dynamically initialises a menu pane. The Uikon framework calls this 
    *   function, if it is implemented in a menu’s observer, immediately before 
    *   the menu pane is activated. 
    * @since 2.8
    * @param aResourceId Resource ID identifying the menu pane to initialise
    * @param aMenuPane The in-memory representation of the menu pane.
    * @return void
    */
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

	/**
     * Accessor method iBrCtlInterface
     * @since 2.8
     * @param void
     * @return CBrCtlInterface* A pointer to the browser control interface as a convenience to the observers
     */
    inline CBrCtlInterface* BrCtlInterface() const {return iBrCtlInterface;}

    /**
    * Handles key events
    * @since 2.8
    * @param aKeyEvent The key event that occurred.
    * @param aType The window server event type that is being handled
    * @return TKeyResponse Value indicates whether or not the key event was consumed by the control. The default implementation simply returns EKeyWasNotConsumed.
    */
    TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
        
    /**
     * Init start page
     */
    void InitStartPage();
    
    void InitOptions();

    void SetCallbackUrl( char* url)
    	{
			callbackUrl = url;
    	}
    
    const char* GetCallbackUrl() const
		{
			return callbackUrl;
		}
    
    char* CanonicalizeURL(const char* path);
    
protected:

	void CreateOffScreenBitmapL();
	
	void ReDrawOffScreenBitmap();
	
    void DrawImage(CWindowGc& aGc, const TRect& aRect) const;
    
	/**
    * From CoeControl,SizeChanged.
    */
    void SizeChanged();

    /**
    * From CoeControl,CountComponentControls.
    */
    TInt CountComponentControls() const;

    /**
    * From CCoeControl,ComponentControl.
    */
    CCoeControl* ComponentControl(TInt aIndex) const;

    /**
    * From CCoeControl,Draw.
    */
    void Draw(const TRect& aRect) const;

    /**
    * Handle key events.
    */
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
            
    /**
    * From MBrCtlDataLoadSupplier, Cancel an on-going load
    */
    void CancelLoad();

    /**
    * Create a Basic Browser Control that does not have observers with scrolling
    * and HTTP framework capabilities
    */
    void CreateBasicBrowserControlL();
    
private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 * Perform the second phase construction of a
	 * CrhodesAppView object.
	 * @param aRect The rectangle this view will be drawn to.
	 */
	void ConstructL(const TRect& aRect);

	/**
    * Handles an event from an observed control.
    */
    void HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType);
	
	/**
	 * CrhodesAppView.
	 * C++ default constructor.
	 */
	CRhodesAppView();

	void ChoosePicture();
private: //data 
        // Pointer to the browser control interface
        CBrCtlInterface* iBrCtlInterface;
        // Desired capabilities of the browser control
        TUint iBrCtlCapabilities;
        // Command Base
        TInt iCommandBase;
        // Informs whether or not CancelLoad has been called
        TBool iCancelInitDataLoad;

        // Special Load Observer
        CSpecialLoadObserver* iSpecialLoadObserver;
        //Softkeys Observer
        //CAppSoftkeysObserver* iAppSoftkeysObserver;
        TBuf<1024> iStartPage;
        
        TBuf<1024> iOptionsPage;

        CRhoCamera* iRhoCamera;
        
        CFbsBitmap* iBitmap; 
		CWsBitmap* iOffScreenBitmap; 
		CFbsBitGc* iFbsBitGc; 
		CFbsBitmapDevice* iBmpDevice;
		
		TBool iOffScreenBitmapCreated;
		
		char* callbackUrl;
	};

#endif // __RHODESAPPVIEW_h__
// End of File
