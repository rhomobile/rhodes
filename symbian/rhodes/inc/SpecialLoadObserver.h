/*
 ============================================================================
 Name		: SpecialLoadObserver.h
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
 Description : CSpecialLoadObserver declaration
 ============================================================================
 */

#ifndef SPECIALLOADOBSERVER_H
#define SPECIALLOADOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <BrCtlSpecialLoadObserver.h>
#include <BrCtlInterface.h>
#include <DocumentHandler.h> 

//FORWARD DECLARATION
class CConnectionManager;

// CLASS DECLARATION

/**
 *  CSpecialLoadObserver
 * 
 */
class CSpecialLoadObserver : public CBase, public MBrCtlSpecialLoadObserver
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CSpecialLoadObserver();

	/**
	 * Two-phased constructor.
	 */
	static CSpecialLoadObserver* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CSpecialLoadObserver* NewLC();
	
	/**
    * Request to create a network connection.
    * @since 2.8
    * @param aConnectionPtr A pointer to the new connection. If NULL, the proxy filter will automatically create a network connection
    * @param aSockSvrHandle A handle to the socket server.
    * @param aNewConn A flag if a new connection was created. If the connection is not new, proxy filter optimization will not read the proxy again from CommsBd
    * @param aBearerType The bearer type of the new connection
    * @return void
    */
    void NetworkConnectionNeededL(TInt* /*aConnectionPtr*/,
                                  TInt* /*aSockSvrHandle*/,
                                  TBool* /*aNewConn*/,
                                  TApBearerType* /*aBearerType*/) ;

    /**
    * Request the host applicaion to handle non-http request.
    * @since 2.8
    * @param aTypeArray Array of download parameter types. These types indicates the type of data contained within the DesArray
    * @param aDesArray Array of values associated with the types in the type array.
    * @return ETrue, If the request is handled by the host application. EFalse if not
    */
    TBool HandleRequestL(RArray<TUint>* aTypeArray, CDesCArrayFlat* aDesArray);

    /**
    * Request the host applicaion to handle downloads
    * @since 2.8
    * @param aTypeArray Array of download parameter types
    * @param aDesArray Array of values associated with the types in the type array
    * @return ETrue, If the request is handled by the host application. EFalse if not
    */
    TBool HandleDownloadL(RArray<TUint>* aTypeArray, CDesCArrayFlat* aDesArray);

    

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CSpecialLoadObserver();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

private: //data
	CConnectionManager* iConnectionManager;
	
	};

#endif // SPECIALLOADOBSERVER_H
