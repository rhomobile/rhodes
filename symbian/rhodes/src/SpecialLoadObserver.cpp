/*
 ============================================================================
 Name		: SpecialLoadObserver.cpp
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
 Description : CSpecialLoadObserver implementation
 ============================================================================
 */

#include "SpecialLoadObserver.h"
#include "ConnectionManager.h"
#include "apengineconsts.h"

CSpecialLoadObserver::CSpecialLoadObserver()
	{
	// No implementation required
	}

CSpecialLoadObserver::~CSpecialLoadObserver()
	{
		if ( iConnectionManager )
			{
			iConnectionManager->Release();
			delete iConnectionManager;
			}
	}

CSpecialLoadObserver* CSpecialLoadObserver::NewLC()
	{
	CSpecialLoadObserver* self = new (ELeave)CSpecialLoadObserver();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSpecialLoadObserver* CSpecialLoadObserver::NewL()
	{
	CSpecialLoadObserver* self=CSpecialLoadObserver::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CSpecialLoadObserver::ConstructL()
	{
		iConnectionManager=CConnectionManager::NewL();
	}

// ---------------------------------------------------------
// CSpecialLoadObserver::NetworkConnectionNeededL
// ---------------------------------------------------------
//
void CSpecialLoadObserver::NetworkConnectionNeededL(TInt* aConnectionPtr,
                                      TInt* aSockSvrHandle,
                                      TBool* aNewConn,
                                      TApBearerType* aBearerType) 
{
	iConnectionManager->SetupConnection();
	
	*aNewConn = iConnectionManager->IsNewConnection();
	*aBearerType = iConnectionManager->GetBearerFilter();
	*aConnectionPtr = reinterpret_cast<TInt>(iConnectionManager->GetConnection());
	*aSockSvrHandle = iConnectionManager->GetServerSocket().Handle();
}

// ---------------------------------------------------------
// CSpecialLoadObserver::HandleRequestL
// ---------------------------------------------------------
//
TBool CSpecialLoadObserver::HandleRequestL(RArray<TUint>* /*aTypeArray*/, CDesCArrayFlat* /*aDesArray*/) 
{
	return EFalse;
}

// ---------------------------------------------------------
// CSpecialLoadObserver::HandleDownloadL
// ---------------------------------------------------------
//
TBool CSpecialLoadObserver::HandleDownloadL(RArray<TUint>* /*aTypeArray*/, CDesCArrayFlat* /*aDesArray*/)
{
	return EFalse;
}