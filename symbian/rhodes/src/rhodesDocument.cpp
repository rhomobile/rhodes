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
#include "rhodesAppUi.h"
#include "rhodesDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CRhodesDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRhodesDocument* CRhodesDocument::NewL(CEikApplication& aApp)
	{
	CRhodesDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CRhodesDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRhodesDocument* CRhodesDocument::NewLC(CEikApplication& aApp)
	{
	CRhodesDocument* self = new ( ELeave ) CRhodesDocument( aApp );

	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CRhodesDocument::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CRhodesDocument::ConstructL()
	{
	// No implementation required
	}

// -----------------------------------------------------------------------------
// CRhodesDocument::CRhodesDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CRhodesDocument::CRhodesDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CRhodesDocument::~CRhodesDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CRhodesDocument::~CRhodesDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CRhodesDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CRhodesDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it;
	// the framework takes ownership of this object
	return new ( ELeave )CRhodesAppUi;
	}

// End of File
