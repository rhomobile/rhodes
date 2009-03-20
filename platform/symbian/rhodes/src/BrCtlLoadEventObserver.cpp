/*
 ============================================================================
 Name		: CBrCtlLoadEventObserver.cpp
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

 Description : CCBrCtlLoadEventObserver implementation
 ============================================================================
 */

#include "BrCtlLoadEventObserver.h"
#include "RhodesAppView.h"

extern "C" void webview_set_current_location(char* url);

void 
CCBrCtlLoadEventObserver::HandleBrowserLoadEventL(TBrCtlDefs::TBrCtlLoadEvent aLoadEvent,
                                                          TUint aSize, 
                                                          TUint16 aTransactionId) 
    {
		if ( TBrCtlDefs::EEventLoadFinished == aLoadEvent ){
			
			if ( iContainer )
				webview_set_current_location( iContainer->GetCurrentPageUrl() );
		}
    }

CCBrCtlLoadEventObserver::CCBrCtlLoadEventObserver() {
	// No implementation required
}

CCBrCtlLoadEventObserver::~CCBrCtlLoadEventObserver() {
}

CCBrCtlLoadEventObserver* CCBrCtlLoadEventObserver::NewLC(CRhodesAppView* aContainer) {
	CCBrCtlLoadEventObserver* self = new (ELeave) CCBrCtlLoadEventObserver();
	CleanupStack::PushL(self);
	self->ConstructL(aContainer);
	return self;
}

CCBrCtlLoadEventObserver* CCBrCtlLoadEventObserver::NewL(CRhodesAppView* aContainer) {
	CCBrCtlLoadEventObserver* self = CCBrCtlLoadEventObserver::NewLC(aContainer);
	CleanupStack::Pop(); // self;
	return self;
}

void CCBrCtlLoadEventObserver::ConstructL(CRhodesAppView* aContainer) {
	iContainer = aContainer;
}
