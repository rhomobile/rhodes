/*
 ============================================================================
 Name		 : ConnectionManager.h
 Author	     : Anton Antonov
 Version	 : 1.0
 Copyright   : Copyright (C) 2008 Rhomobile. All rights reserved.

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
 Description : CConnectionManager declaration
 ============================================================================
 */

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <es_sock.h>

#include "apengineconsts.h"

#include <http/RHTTPSession.h>
#include <http/mhttpauthenticationcallback.h>

class CRepository;

// CLASS DECLARATION

/**
 *  CConnectionManager
 * 
 */
class CConnectionManager : public CBase, MHTTPAuthenticationCallback
	{
public:
	
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CConnectionManager();

	/**
	 * Two-phased constructor.
	 */
	static CConnectionManager* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CConnectionManager* NewLC();

	//close session
	void Release();
	
	//setup connection
	TBool SetupConnection();
	
	//Accessors
	RConnection*  GetConnection(){ return &iConnection; }
	RSocketServ&  GetServerSocket(){ return iServerSocket; }
	TApBearerType GetBearerFilter(){ return iBearerFilter; }
	RHTTPSession  GetHTTPSession() { return iSessionHttp; }
	
	TBool 		  IsNewConnection(){ return iNewConnection; }
	TBool         IsOfflineMode();
	
	TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, RStringF aAuthenticationType, RString& aUsername, RString& aPassword);
	
protected:
	TBool FindExistingConnection();
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CConnectionManager();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	TInt SelectIAP();
	
private: //Data
	
	CRepository	*	iRepository;
	TInt			iPreviousProfileId;
	TUint32			iSelectedConnectionId;
	TBool			iConnectionSetup;
	TBool			iNewConnection;
	TApBearerType 	iBearerFilter;
	
	RSocketServ  iServerSocket;
	RConnection  iConnection;
	RHTTPSession iSessionHttp; 
	};

#endif // CONNECTIONMANAGER_H
