/*
 ============================================================================
 Name		: ConnectionManager.cpp
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
 Description : CConnectionManager implementation
 ============================================================================
 */

#include "ConnectionManager.h"
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>

#include <ApEngineConsts.h>
#include <es_enum.h>
#include <activeapdb.h>
#include <apsettingshandlerui.h>
#include <badesca.h>
#include <stringloader.h>
#include <aknquerydialog.h>
#include <CommDbConnPref.h>

#include <httpstringconstants.h>

#include <stdio.h>
#include <string.h>

#include <aknlists.h>		// CAknSinglePopupMenuStyleListBox
#include <CommDb.h>

//#include "logging/RhoPlainLog.h"

extern TUint32 gSelectedConnectionId;

namespace EConnectionStatus
	{
	TInt Disconnected = 5;
	}

class TIapData
{
public:
	TBuf<128> iName;
	TUint32 iIap;
 
};

CConnectionManager::CConnectionManager()
	{
	// No implementation required
	}

CConnectionManager::~CConnectionManager()
	{
	}

TInt CConnectionManager::SelectIAP()
{
	CArrayFixFlat<TIapData>* iEApList=new (ELeave) CArrayFixFlat<TIapData>(2) ;
	TInt stack=0;
	// Make listitems. and PUSH it
	CAknSinglePopupMenuStyleListBox* list = new(ELeave) CAknSinglePopupMenuStyleListBox;
	CleanupStack::PushL(list);
	stack++; 
 
	// Create popup list and PUSH it.
	CAknPopupList* popupList = CAknPopupList::NewL(list,
		R_AVKON_SOFTKEYS_OK_CANCEL, AknPopupLayouts::EMenuWindow);
	CleanupStack::PushL(popupList);stack++; 
 
	CDesCArrayFlat* items = new (ELeave) CDesCArrayFlat(5);
	CleanupStack::PushL(items);stack++; 
	// initialize listbox.
	list->ConstructL(popupList, CEikListBox::ELeftDownInViewRect);
	list->CreateScrollBarFrameL(ETrue);
	list->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,
		CEikScrollBarFrame::EAuto);
 
 
	TBuf<52> iapfromtable;
	TInt err = KErrNone;
 
	CCommsDatabase* iCommsDB=CCommsDatabase::NewL(EDatabaseTypeIAP);
	CleanupStack::PushL(iCommsDB);stack++; 
#ifdef __SERIES60_3X__
	CCommsDbTableView* gprsTable = iCommsDB->OpenIAPTableViewMatchingBearerSetLC(
			iBearerFilter == EApBearerTypeWLAN 
				? ECommDbBearerWLAN 
				: ECommDbBearerGPRS|ECommDbBearerWLAN|ECommDbBearerVirtual,
		ECommDbConnectionDirectionOutgoing); 
#else
	CCommsDbTableView* gprsTable = iCommsDB->OpenIAPTableViewMatchingBearerSetLC(
		ECommDbBearerGPRS|ECommDbBearerVirtual,
		ECommDbConnectionDirectionOutgoing); 
#endif
	User::LeaveIfError(gprsTable->GotoFirstRecord());
	TInt i=0;
	TUint32 id;
	TIapData eap;	
 
	//TInt cur =0; //current value
	do
	{
		gprsTable->ReadTextL(TPtrC(COMMDB_NAME), iapfromtable);
		gprsTable->ReadUintL(TPtrC(COMMDB_ID), id);
		items->AppendL(iapfromtable);
		eap.iIap = id;
		eap.iName.Copy(iapfromtable);
		iEApList->AppendL(eap);
 
		err = gprsTable->GotoNextRecord();
		i++;
	}
	while (err == KErrNone);
	CleanupStack::PopAndDestroy(2); stack--; 
 
	// Set listitems.
	CTextListBoxModel* model = list->Model();
	model->SetItemTextArray(items);
	model->SetOwnershipType(ELbmOwnsItemArray);
	CleanupStack::Pop();    
 
	popupList->SetTitleL(_L("Access Point"));
	list->SetListBoxObserver(popupList);
	TInt popupOk = popupList->ExecuteLD();
	CleanupStack::Pop();    
	TInt iap=0;
	if (popupOk)
	{	
		TInt index = list->CurrentItemIndex();
		iap=(*iEApList)[index].iIap;
 
	}
	CleanupStack::PopAndDestroy();  
	iEApList->Reset();
	delete iEApList;
	return iap;
}

CConnectionManager* CConnectionManager::NewLC()
	{
	CConnectionManager* self = new (ELeave)CConnectionManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CConnectionManager* CConnectionManager::NewL()
	{
	CConnectionManager* self=CConnectionManager::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CConnectionManager::ConstructL()
	{
		iRepository            = CRepository::NewL(KCRUidProfileEngine);
	    iPreviousProfileId     = -1;
	    iSelectedConnectionId  = 0u-1;

	    iConnectionSetup  = EFalse;
	    iNewConnection = ETrue;
	}

void CConnectionManager::Release()
	{
	    if (iRepository)
	        delete iRepository;
	    
	    iConnection.Close();
	    iSessionHttp.Close();
	    iServerSocket.Close();
	}

TBool  CConnectionManager::IsOfflineMode()
	{
		return (iPreviousProfileId == EConnectionStatus::Disconnected) ? ETrue : EFalse;
	}

TBool CConnectionManager::SetupConnection()
	{
		TInt iCurrentProfileId;    
		iBearerFilter = EApBearerTypeAllBearers;

		ASSERT(iRepository);

		// Check whether we are offline or online
		iRepository->Get(KProEngActiveProfile, iCurrentProfileId);
		
	    // Close the connection only if
	    // a) this is not the first time and
	    // b) the profile has changed and
	    // c) either the previous or the current profile is Offline (5 = Offline)
	    if (iPreviousProfileId != -1 && iPreviousProfileId != iCurrentProfileId &&
	            (iPreviousProfileId == EConnectionStatus::Disconnected || iCurrentProfileId == EConnectionStatus::Disconnected))
	    {
	        // Close and uninitialize
	        iConnectionSetup = EFalse;
	        iSessionHttp .Close();
	        iConnection  .Close();
	        iServerSocket.Close();
	    }

	    // Save current profile id
	    iPreviousProfileId = iCurrentProfileId;

	    // Try to find an existing connection. If connection has not been set up,
	    // iConnection is not initialized and FindExistingConnection() fails.
	    // Thus, in that case, finding must not be carried out.
	    if (iConnectionSetup && !FindExistingConnection())
	    {
	        iConnectionSetup = EFalse;
	    }

	    if (iConnectionSetup)
	    {
	        // Connection setup is done
	    	iNewConnection = EFalse;
	        return ETrue;
	    }

	    // Open RHTTPSession with default protocol ("HTTP/TCP")
	    iSessionHttp.OpenL();

	    // Install this class as the callback for authentication requests. When
	    // page requires authentication the framework calls GetCredentialsL to get
	    // user name and password.
	    InstallAuthenticationL(iSessionHttp);

	    // In offline, only WLAN connections are available
	    if (iCurrentProfileId == EConnectionStatus::Disconnected)
	    {
	        iBearerFilter = EApBearerTypeWLAN;
	    }

#ifdef __WINSCW__
//SYM emulator: should not ask about access point
#ifdef __S60_32__ 
	    gSelectedConnectionId = 1; //FP2 
#else
	    gSelectedConnectionId = 3; //FP1
#endif
	    
#endif	    

	    // Show IAP selection dialog
	    if ( gSelectedConnectionId == -1 )
	    	gSelectedConnectionId = SelectIAP();
	    
	    iSelectedConnectionId =  gSelectedConnectionId;
	    
    	iNewConnection = ETrue;
    	
        // IAP Selected
        // Open socket server and start the connection
        User::LeaveIfError(iServerSocket.Connect(KESockDefaultMessageSlots));
        User::LeaveIfError(iConnection.Open(iServerSocket, KConnectionTypeDefault));
        // Now we have the iap Id. Use it to connect for the connection
        TCommDbConnPref connectPref;
		// Setup preferences 
		connectPref.SetDialogPreference(ECommDbDialogPrefDoNotPrompt);
		
		// Sets the CommDb ID of the IAP to use for this connection
        connectPref.SetIapId(iSelectedConnectionId);
        // Sets direction
        connectPref.SetDirection(ECommDbConnectionDirectionOutgoing);
        
        connectPref.SetBearerSet(iBearerFilter);
        
        // Start connection
        User::LeaveIfError(iConnection.Start(connectPref));
        
        //TInt retval = iConnection.Start();
        //User::LeaveIfError(retval);
	        
        // Set the sessions connection info...
        RStringPool strPool = iSessionHttp.StringPool();
        RHTTPConnectionInfo connInfo = iSessionHttp.ConnectionInfo();
        // ...to use our socket server and connection
        connInfo.SetPropertyL(strPool.StringF(HTTP::EHttpSocketServ, RHTTPSession::GetTable()), THTTPHdrVal (iServerSocket.Handle()));
        // ...to use our connection
        connInfo.SetPropertyL(strPool.StringF(HTTP::EHttpSocketConnection, RHTTPSession::GetTable()), THTTPHdrVal (REINTERPRET_CAST(TInt, &(iConnection))));

        iConnectionSetup = ETrue;
	
	    return ETrue;
}

TBool CConnectionManager::FindExistingConnection()
{
    TUint uiNbConnections;

    if (iConnection.EnumerateConnections(uiNbConnections) != KErrNone)
    {
        return EFalse;
    }

    TConnectionInfoBuf connInfo;
    
    for (TUint uiConnection = 1; uiConnection <= uiNbConnections; ++uiConnection)
    {
        // Note: GetConnectionInfo expects 1-based index.
        if (iConnection.GetConnectionInfo(uiConnection, connInfo) == KErrNone)
        {
            if (connInfo().iIapId == iSelectedConnectionId)
            {
                return ETrue;
            }
        }
    }

    return EFalse;
}


TBool CConnectionManager::GetCredentialsL(const TUriC8& /*aURI*/, RString /*aRealm*/, 
		RStringF /*aAuthenticationType*/, RString& /*aUsername*/, RString& /*aPassword*/)
{

    //// Return ETrue if user has given credentials (username and password),
    //// otherwise EFlase
    return EFalse;
}
