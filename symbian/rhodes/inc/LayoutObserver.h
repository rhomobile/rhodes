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

#ifndef LAYOUTOBSERVER_H
#define LAYOUTOBSERVER_H

// INCLUDES
#include <BrCtlLayoutObserver.h>
#include <BrCtlInterface.h>

// FORWARD DECLARATIONS
class CRhodesAppView;

// CLASS DECLARATION
/**
* CLayoutObserver class.
* This class inherits from the MBrCtlLayoutObserver interface. Methods on this class will be
* called when layout events occur. 
*/
class CLayoutObserver : public CBase, public MBrCtlLayoutObserver
{
public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CLayoutObserver* NewL(CRhodesAppView* aContainer);

        /**
        * Destructor.
        */
        ~CLayoutObserver();

        /**
        * Update the position of vertical scrollbar.
        * @since 2.8
        * @param aDocumentHeight The total height of the markup page
        * @param aDisplayHeight The height of the display
        * @param aDisplayPosY The current Y position
        * @return void
        */
        void UpdateBrowserVScrollBarL(TInt aDocumentHeight, 
                                      TInt aDisplayHeight,
                                      TInt aDisplayPosY ) ;

        /**
        * Update the position of horizontal scrollbar.
        * @since 2.8
        * @param aDocumentWidth The total width of the markup page
        * @param aDisplayWidth The width of the display
        * @param aDisplayPosX The current X position
        * @return void
        */
        void UpdateBrowserHScrollBarL(TInt aDocumentWidth, 
                                      TInt aDisplayWidth,
                                      TInt aDisplayPosX) ;

        /**
        * Inform the layout of the page: right to left or left to right. Useful when the application draws the scrollbar itself.
        * @since 2.8
        * @param aNewLayout RTL or LTR
        * @return void
        */
        void NotifyLayoutChange( TBrCtlLayout aNewLayout) ;

        /**
        * Update the title of the page in history view
        * @since 3.0
        * @param aTitle Title of the page
        * @return void
        */
        void UpdateTitleL( const TDesC& aTitle );  

private:
	    /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(CRhodesAppView* aContainer);

private:   // data
        // Pointer to the container class for our convenience.
	CRhodesAppView* iContainer;
};

#endif      // LAYOUTOBSERVER_H
            
// End of File

