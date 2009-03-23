/*
 ============================================================================
 Name		: CBrCtlLoadEventObserver.h
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   :  Copyright (C) 2008 Rhomobile. All rights reserved.
 
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

 Description : CCBrCtlLoadEventObserver declaration
 ============================================================================
 */

#ifndef CBRCTLLOADEVENTOBSERVER_H
#define CBRCTLLOADEVENTOBSERVER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <BrCtlInterface.h>

// CLASS DECLARATION
class CRhodesAppView;

/**
 *  CCBrCtlLoadEventObserver
 * 
 */
class CCBrCtlLoadEventObserver: public CBase, public MBrCtlLoadEventObserver {
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CCBrCtlLoadEventObserver();

	/**
	 * Two-phased constructor.
	 */
	static CCBrCtlLoadEventObserver* NewL(CRhodesAppView* aContainer);

	/**
	 * Two-phased constructor.
	 */
	static CCBrCtlLoadEventObserver* NewLC(CRhodesAppView* aContainer);

	/**
	 * A load events notification
	 * @since 2.8
	 * @param aLoadEvent The load event   
	 * @param aSize Size depends on the event
	 * @param aTransactionId The transaction id of the transaction that had this event
	 * @return void
	 */
	void HandleBrowserLoadEventL(TBrCtlDefs::TBrCtlLoadEvent aLoadEvent,
			TUint aSize, TUint16 aTransactionId);
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CCBrCtlLoadEventObserver();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(CRhodesAppView* aContainer);

private:
	CRhodesAppView* iContainer;
};

#endif // CBRCTLLOADEVENTOBSERVER_H
