/*
 ============================================================================
 Name		: GeoLocationService.h
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

 Description : CGeoLocationService declaration
 ============================================================================
 */

#ifndef GEOLOCATIONSERVICE_H
#define GEOLOCATIONSERVICE_H

#ifdef SYM_GEOLOCATION

// INCLUDES
#include <e32std.h>
#include <e32base.h>

class CGeoLocation;
// CLASS DECLARATION

/**
 *  CGeoLocationService
 * 
 */
class CGeoLocationService : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGeoLocationService();

	/**
	 * Two-phased constructor.
	 */
	static CGeoLocationService* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CGeoLocationService* NewLC();

	void StopThread();
	
	TInt Execute();
	
	void ResumeThread();

	void SuspendThread();

private:

	TInt ExecuteL();
	/**
	 * Constructor for performing 1st stage construction
	 */
	CGeoLocationService();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	CGeoLocation* iGeoLocation;
	
	RThread thread;
	bool iClose;
	};
#endif

#endif // GEOLOCATIONSERVICE_H
