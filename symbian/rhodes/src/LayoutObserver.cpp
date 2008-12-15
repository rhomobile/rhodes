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
#include "rhodesAppView.h"
#include "LayoutObserver.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLayoutObserver::UpdateBrowserVScrollBarL
// ---------------------------------------------------------
//
void CLayoutObserver::UpdateBrowserVScrollBarL(TInt aDocumentHeight, TInt aDisplayHeight, TInt aDisplayPosY ) 
{
    iContainer->DrawNow();
}

// ---------------------------------------------------------
// CLayoutObserver::UpdateBrowserHScrollBarL
// ---------------------------------------------------------
//
void CLayoutObserver::UpdateBrowserHScrollBarL(TInt aDocumentWidth, TInt aDisplayWidth, TInt aDisplayPosX ) 
{
    iContainer->DrawNow();
}

// ---------------------------------------------------------
// CLayoutObserver::NotifyLayoutChange
// ---------------------------------------------------------
//
void CLayoutObserver::NotifyLayoutChange( TBrCtlLayout aNewLayout ) 
{
    iContainer->DrawNow();
}

// ---------------------------------------------------------
// CLayoutObserver::UpdateTitle
// ---------------------------------------------------------
//
void CLayoutObserver::UpdateTitleL( const TDesC& /*aTitle*/ )
{
}

//----------------------------------------------------------
// CLayoutObserver::ConstructL
// ---------------------------------------------------------
//
void CLayoutObserver::ConstructL(CRhodesAppView* aContainer)
{
    iContainer = aContainer;
}

// ---------------------------------------------------------
// CLayoutObserver::~CLayoutObserver
// ---------------------------------------------------------
//
CLayoutObserver::~CLayoutObserver()
{
}


// ---------------------------------------------------------
// CObserver::NewL
// ---------------------------------------------------------
//
CLayoutObserver* CLayoutObserver::NewL(CRhodesAppView* aContainer)
{
	CLayoutObserver* self = new(ELeave)CLayoutObserver();
	CleanupStack::PushL(self);
	self->ConstructL(aContainer);
	CleanupStack::Pop();
	
	return self;
}

// End of File  

