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
#include <eikstart.h>
#include "rhodesApplication.h"

extern "C" void InitRhoLog(const char* szRootPath);
extern "C" const char* RhoGetRootPath();
extern "C" void runAllProfilingTests();

LOCAL_C CApaApplication* NewApplication()
	{
	return new CRhodesApplication;
	}

GLDEF_C TInt E32Main()
	{
    InitRhoLog(RhoGetRootPath());
    
    //runAllProfilingTests();
    
	return EikStart::RunApplication( NewApplication );
	}

