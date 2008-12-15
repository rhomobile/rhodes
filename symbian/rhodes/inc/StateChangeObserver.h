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

#ifndef STATECHANGEOBSERVER_H
#define STATECHANGEOBSERVER_H

// INCLUDES
#include <BrCtlInterface.h>

// FORWARD DECLARATIONS
class CRhodesAppView;

// CLASS DECLARATION

/** 
* CStateChangeObserver class.
* This class inherits from the MBrCtlStateChangeObserver interface which is used to 
* receive state-changed events.
*/
class CStateChangeObserver : public CBase, public MBrCtlStateChangeObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CStateChangeObserver* NewL(CRhodesAppView* aContainer);

        /**
        * Destructor.
        */
        ~CStateChangeObserver();

        /**
        * A state change events notification
        * @since 2.8
        * @param aState The state, currently the only state is EStateImageMapView  
        * @param aValue This value will be true if we going to image map view and
        *               it will be false if we are returning from image map view
        * @return void
        */
        void StateChanged(TBrCtlDefs::TBrCtlState aState, TInt aValue);

    private:
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CRhodesAppView* aContainer);

    private:   // data
        // Pointer to the container class associated with this observer
    	CRhodesAppView* iContainer;
    };
            
// End of File


#endif // STATECHANGEOBSERVER_H
