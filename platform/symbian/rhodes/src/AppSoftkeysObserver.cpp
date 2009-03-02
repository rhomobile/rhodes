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

#include <eikbtgpc.h>
#include "AppSoftkeysObserver.h"
#include <rhodes.rsg>
#include "rhodesAppView.h"

#include <avkon.rsg>

CAppSoftkeysObserver::CAppSoftkeysObserver()
	{
	// No implementation required
	}

CAppSoftkeysObserver::~CAppSoftkeysObserver()
	{
	}

CAppSoftkeysObserver* CAppSoftkeysObserver::NewLC( CRhodesAppView * aAppView )
	{
	CAppSoftkeysObserver* self = new (ELeave)CAppSoftkeysObserver();
	CleanupStack::PushL(self);
	self->ConstructL(aAppView);
	return self;
	}

CAppSoftkeysObserver* CAppSoftkeysObserver::NewL( CRhodesAppView * aAppView )
	{
	CAppSoftkeysObserver* self=CAppSoftkeysObserver::NewLC(aAppView);
	CleanupStack::Pop(); // self;
	return self;
	}

void CAppSoftkeysObserver::ConstructL(CRhodesAppView * aAppView)
	{
		iAppView = aAppView;
	}


// ---------------------------------------------------------
// CAppSoftkeysObserver::UpdateSoftkeyL
// ---------------------------------------------------------
//
void 
CAppSoftkeysObserver::UpdateSoftkeyL(TBrCtlKeySoftkey /*aKeySoftkey*/,
                                                const TDesC& /*aLabel*/,
                                                TUint32 /*aCommandId*/,
                                                TBrCtlSoftkeyChangeReason /*aBrCtlSoftkeyChangeReason*/) 
    {
    CBrCtlInterface* brCtl = iAppView->BrCtlInterface();

    TBrCtlDefs::TBrCtlElementType type = brCtl->FocusedElementType();

    CEikButtonGroupContainer* current = CEikButtonGroupContainer::Current();
    
    switch (type)
        {
    	case TBrCtlDefs::EElementActivatedObjectBox:
        case TBrCtlDefs::EElementActivatedInputBox:
            current->SetCommandSetL( R_AVKON_SOFTKEYS_CANCEL );
            break;
        
        default:
            current->SetCommandSetL( R_AVKON_SOFTKEYS_OPTIONS_BACK );
            break;
        }
    current->DrawNow();
    }

