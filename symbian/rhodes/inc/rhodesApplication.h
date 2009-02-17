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

#ifndef __RHODESAPPLICATION_H__
#define __RHODESAPPLICATION_H__

// INCLUDES
#include <aknapp.h>
#include "rhodes.hrh"
#include "tcmalloc/rhomem.h"

// UID for the application;
// this should correspond to the uid defined in the mmp file
const TUid KUidRhodesApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CRhodesApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CRhodesApplication is the application part of the
 * AVKON application framework for the rhodes example application.
 */
class CRhodesApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidrhodesApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CRhodesDocument document object. The returned
	 * pointer in not owned by the CRhodesApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	};

#endif // __RHODESAPPLICATION_H__
// End of File
