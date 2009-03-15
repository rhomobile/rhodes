/*
 ============================================================================
 Name		: GeoLocation.cpp
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   : Copyright (C) 2008 Rhomobile. All rights reserved.

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
 Description : CGeoLocation implementation
 ============================================================================
 */

#include <rhodes.rsg>
#include "GeoLocation.h"

#ifdef SYM_GEOLOCATION

CGeoLocation::CGeoLocation()
	{
    }

CGeoLocation::~CGeoLocation()
	{
		DoCancel();
		iPositioner.Close();
	    iLocationServer.Close();
	    delete iAppName;
	}

CGeoLocation* CGeoLocation::NewLC(const TDesC& aAppName)
	{
	CGeoLocation* self = new (ELeave)CGeoLocation();
	CleanupStack::PushL(self);
	self->ConstructL(aAppName);
	return self;
	}

CGeoLocation* CGeoLocation::NewL(const TDesC& aAppName)
	{
	CGeoLocation* self=CGeoLocation::NewLC(aAppName);
	CleanupStack::Pop(); // self;
	return self;
	}

void CGeoLocation::ConstructL(const TDesC& aAppName)
	{
	    iAppName = aAppName.AllocL();
	}

void CGeoLocation::InitLocationServer()
	{
	
    // connect to location server
    User::LeaveIfError(iLocationServer.Connect());
    // open positioner
    User::LeaveIfError(iPositioner.Open(iLocationServer));
    // set our application as location requestor
    User::LeaveIfError(iPositioner.SetRequestor(
        CRequestor::ERequestorService, CRequestor::EFormatApplication, *iAppName));
    // set maximum allowed time for a location request
    TTimeIntervalMicroSeconds timeOut(30000000); // 30 sec
    TPositionUpdateOptions updateOptions;
    updateOptions.SetUpdateTimeOut(timeOut);
    User::LeaveIfError(iPositioner.SetUpdateOptions(updateOptions));
	
	}

TBool CGeoLocation::GetCurrentPostionL(TReal& aLatitude, TReal& aLongitude)
    {
    // request current location
    iPositioner.NotifyPositionUpdate(iPositionInfo, iStatus);
    User::WaitForRequest(iStatus);

    // process result
    if (iError == KErrNone)
        {
        // success, return given position
        TPosition pos;
        iPositionInfo.GetPosition(pos);
        aLatitude = pos.Latitude();
        aLongitude = pos.Longitude();
        return ETrue;
        }
    // fail
    return EFalse;
    }

void CGeoLocation::DoCancel()
    {
    // cancel location request
    iPositioner.CancelRequest(EPositionerNotifyPositionUpdate);
    }

void CGeoLocation::RunL()
    {
    // request ended, store error code and dismiss wait dialog
    iError = iStatus.Int();
    }
#endif
