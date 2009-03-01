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

#ifndef __RHODESDOCUMENT_h__
#define __RHODESDOCUMENT_h__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CRhodesAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CRhodesDocument application class.
 * An instance of class CRhodesDocument is the Document part of the
 * AVKON application framework for the rhodes example application.
 */
class CRhodesDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CRhodesDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CRhodesDocument.
	 */
	static CRhodesDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CRhodesDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CRhodesDocument.
	 */
	static CRhodesDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CRhodesDocument
	 * Virtual Destructor.
	 */
	virtual ~CRhodesDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CRhodesAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * ConstructL
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CRhodesDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CRhodesDocument(CEikApplication& aApp);

	};

#endif // __RHODESDOCUMENT_h__
// End of File
