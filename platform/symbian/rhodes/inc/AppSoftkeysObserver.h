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

#ifndef APPSOFTKEYSOBSERVER_H
#define APPSOFTKEYSOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <BrCtlSoftkeysObserver.h>
#include <BrCtlInterface.h>


// FORWARD DECLARATION 
class CRhodesAppView;

// CLASS DECLARATION

/**
 *  CAppSoftkeysObserver
 * 
 */
class CAppSoftkeysObserver : public CBase, public MBrCtlSoftkeysObserver
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAppSoftkeysObserver();

	/**
	 * Two-phased constructor.
	 */
	static CAppSoftkeysObserver* NewL( CRhodesAppView * aAppView );

	/**
	 * Two-phased constructor.
	 */
	static CAppSoftkeysObserver* NewLC( CRhodesAppView * aAppView );
	
    /**
    * Browser Control requests to update a softkey
    * @since 2.8
    * @param aKeySoftkey Update the left softkey or the right softkey
    * @param aLabel The label associated with the softkey update
    * @param aCommandId The command to use if the softkey is selected by the user
    * @param aBrCtlSoftkeyChangeReason The reason for the softkey change
    * @return void
    */
    void UpdateSoftkeyL(TBrCtlKeySoftkey /*aKeySoftkey*/,
                        const TDesC& /*aLabel*/,
                        TUint32 /*aCommandId*/,
                        TBrCtlSoftkeyChangeReason /*aBrCtlSoftkeyChangeReason*/) ;


private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAppSoftkeysObserver();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL( CRhodesAppView * aAppView );

private: //data
	// Pointer to the container class associated with this observer
	CRhodesAppView* iAppView;
	
	};

#endif // APPSOFTKEYSOBSERVER_H
