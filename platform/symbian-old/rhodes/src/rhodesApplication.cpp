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
#include "rhodes.hrh"
#include "rhodesDocument.h"
#include "rhodesApplication.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRhodesApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CRhodesApplication::CreateDocumentL()
	{
	// Create an rhodes document, and return a pointer to it
	return CRhodesDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CRhodesApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CRhodesApplication::AppDllUid() const
	{
	// Return the UID for the rhodes application
	return KUidRhodesApp;
	}

// End of File
