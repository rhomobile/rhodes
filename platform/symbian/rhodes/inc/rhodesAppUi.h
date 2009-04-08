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

#ifndef __RHODESAPPUI_h__
#define __RHODESAPPUI_h__

// INCLUDES
#include <aknappui.h>

//#include "logging/RhoLog.h"

// FORWARD DECLARATIONS
class CRhodesAppView;
class CHttpServer;
class CSyncEngineWrap;
class CGeoLocationService;
class CAppManager;
class CRhoCamera;

struct sqlite3;
// CLASS DECLARATION
/**
 * Interacts with the user through the UI and request message processing
 * CRhodesAppUi application UI class.
 * from the handler class
 */
class CRhodesAppUi : public CAknAppUi
	{
    //DEFINE_LOGCLASS;
    
public:
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CRhodesAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	CRhodesAppUi();

	/**
	 * ~CRhodesAppUi.
	 * Virtual Destructor.
	 */
	virtual ~CRhodesAppUi();
	
	const TDesC& GetStartPage();

	void StopRubyFramework();
	void StartRubyFramework();
	
	/**
	 * Stop HttpServer and SyncEngine
	 */
	void StopThreads();
	
	/**
	 * 
	 */
	void ShowInfoMessage(TInt aTitleId, TInt aMessageId);
private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

    void HandleApplicationSpecificEventL(TInt aType, const TWsEvent& aEvent);
    void HandleForegroundEventL(TBool aForeground);
    
    /**
     *  From MEikMenuObserver
     */
    void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);

    
	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();

	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 */
	CArrayFix<TCoeHelpContext>* HelpContextL() const;
	
	/**
    * From CEikAppUi, handles key events.
    * @param aKeyEvent Event to handled.
    * @param aType Type of the key event. 
    * @return Response code (EKeyWasConsumed, EKeyWasNotConsumed). 
    */
    virtual TKeyResponse HandleKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);


private:
	// Data

	CHttpServer* iHttpServer;
	CSyncEngineWrap* iSyncEngineWrap;
	
#ifdef SYM_GEOLOCATION	
	CGeoLocationService* iGeoLocationService;
#endif	
	/**
	 * The application view
	 * Owned by CRhodesAppUi
	 */
	CRhodesAppView* iAppView;
	
//    sqlite3 * iDatabase;

#ifdef ENABLE_DYNAMIC_RHOBUNDLE
	CAppManager* iAppManager;

    char* szRhoBundleZipUrl;
    bool iRhoBundleReloadEnabled;
#endif    
	};

#endif // __RHODESAPPUI_h__
// End of File
