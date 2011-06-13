/*
 ============================================================================
 Name		: GeoLocationService.cpp
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

 Description : CGeoLocationService implementation
 ============================================================================
 */

#include "GeoLocationService.h"

#ifdef SYM_GEOLOCATION

#include "rhodes.pan"
#include <eikenv.h>
#include "rhodesApplication.h"
#include "rhodesAppUI.h"
#include "GeoLocation.h" 


#include <e32math.h>  //Rand

#include <unistd.h>

extern "C" TReal g_geo_latitude;
extern "C" TReal g_geo_longitude;

TInt GeoThreadEntryPoint(TAny *aPtr)
{
	CGeoLocationService *pGeoLocationService = (CGeoLocationService *)aPtr;
	return pGeoLocationService->Execute();
}

CGeoLocationService::CGeoLocationService()
:iClose(false)
	{
	// No implementation required
	}

CGeoLocationService::~CGeoLocationService()
	{
		iClose = true;
	
		thread.Kill( KErrCancel );
		thread.Close();
		
		if ( iGeoLocation )
			delete iGeoLocation;
	}

CGeoLocationService* CGeoLocationService::NewLC()
	{
	CGeoLocationService* self = new (ELeave) CGeoLocationService();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CGeoLocationService* CGeoLocationService::NewL()
	{
	CGeoLocationService* self = CGeoLocationService::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CGeoLocationService::ConstructL()
	{
		_LIT(KAppName, "Rhodes Platform");
		iGeoLocation = CGeoLocation::NewL(KAppName);
	
		_LIT(KThreadName, "GeoLocationThread");
	
		TBuf<30> threadName;
		threadName.Append(KThreadName);

		TTime time;
		time.HomeTime();

		TInt64 aSeed = time.Int64();
		TInt randNum = Math::Rand(aSeed) % 1000;
		threadName.AppendNum(randNum);
				
		TInt res = thread.Create(threadName, GeoThreadEntryPoint, 
				1000, KMinHeapSize, 256*KMinHeapSize,
				this);
		
		if ( res != KErrNone )
			Panic( ERhodesGeolocationInit );
		
		thread.SetPriority(EPriorityLess);
	}

TInt CGeoLocationService::Execute()
	{
		TInt err=KErrNoMemory;
		// Create a cleanup stack...
		CTrapCleanup* cleanup = CTrapCleanup::New();

		if(cleanup)
	    {
	    	TRAP(err,CGeoLocationService::ExecuteL());
	    	delete cleanup;
	    }
		return err;
	}

TInt CGeoLocationService::ExecuteL()
	{
		iGeoLocation->InitLocationServer();
		
		while ( !iClose )
		{
			iGeoLocation->GetCurrentPostionL(g_geo_latitude, g_geo_longitude);
			sleep(10);
		}
		
	 	return 0;
	}

void CGeoLocationService::StopThread()
	{
	SuspendThread();
	iClose = true;
	ResumeThread();
	}

void CGeoLocationService::ResumeThread()
	{
		thread.Resume();
	}

void CGeoLocationService::SuspendThread()
	{
		thread.Suspend();
	}
#endif
