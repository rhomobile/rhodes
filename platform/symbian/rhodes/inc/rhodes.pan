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


#ifndef __RHODES_PAN__
#define __RHODES_PAN__

/** rhodes application panic codes */
enum TRhodesPanics
	{
	ERhodesUi = 1,
	ERhodesHttpServerInit,
	ERhodesSyncEngineInit,
	ERhodesGeolocationInit
	// add further panics here
	};

inline void Panic(TRhodesPanics aReason)
	{
	_LIT(applicationName, "rhodes");
	User::Panic(applicationName, aReason);
	}

#endif // __RHODES_PAN__
