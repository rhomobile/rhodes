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
#include "StateChangeObserver.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CStateChangeObserver::StateChanged
// ---------------------------------------------------------
//
void CStateChangeObserver::StateChanged(TBrCtlDefs::TBrCtlState aState, TInt aValue)
{
    iContainer->DrawNow();
}

// ---------------------------------------------------------
// CStateChangeObserver::ConstructL
// ---------------------------------------------------------
//
void CStateChangeObserver::ConstructL(CRhodesAppView* aContainer)
{
    iContainer = aContainer;
}

// ---------------------------------------------------------
// CStateChangeObserver::~CStateChangeObserver
// ---------------------------------------------------------
//
CStateChangeObserver::~CStateChangeObserver()
{
}

// ---------------------------------------------------------
// CStateChangeObserver::NewL
// ---------------------------------------------------------
//
CStateChangeObserver* CStateChangeObserver::NewL(CRhodesAppView* aContainer)
{
	CStateChangeObserver* self = new(ELeave)CStateChangeObserver;
	CleanupStack::PushL(self);
	self->ConstructL(aContainer);
	CleanupStack::Pop();
	return self;
}

// End of File  

