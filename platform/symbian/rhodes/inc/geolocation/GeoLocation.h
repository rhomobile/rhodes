/*
 ============================================================================
 Name		: GeoLocation.h
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
 Description : CGeoLocation declaration
 ============================================================================
 */

#ifndef GEOLOCATION_H
#define GEOLOCATION_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CGeoLocation
 * 
 */
class CGeoLocation : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGeoLocation();

	/**
	 * Two-phased constructor.
	 */
	static CGeoLocation* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CGeoLocation* NewLC();

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CGeoLocation();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	};

#endif // GEOLOCATION_H
