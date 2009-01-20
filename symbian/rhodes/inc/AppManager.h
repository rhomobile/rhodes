/*
 ============================================================================
 Name		: AppManager.h
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
 Description : CAppManager declaration
 ============================================================================
 */

#ifndef APPMANAGER_H
#define APPMANAGER_H

#ifdef ENABLE_DYNAMIC_RHOBUNDLE

// INCLUDES
#include <e32std.h>
#include <e32base.h>

class CHttpClient;
class CHttpFileManager;
class CRhodesAppUi;
// CLASS DECLARATION

/**
 *  CAppManager
 * 
 */
class CAppManager : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CAppManager();

	/**
	 * Two-phased constructor.
	 */
	static CAppManager* NewL(CRhodesAppUi * aAppUI);

	/**
	 * Two-phased constructor.
	 */
	static CAppManager* NewLC(CRhodesAppUi * aAppUI);

	/**
	 * @param aUrl - url of the zip file with rhobundle
	 */
	void reloadRhoBundle(const char* aUrl, const char* zipPassword);
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CAppManager();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
	void setAppUI(CRhodesAppUi * aAppUI){ iAppUI = aAppUI;}

private:
	CHttpClient* iHttpClient;
	CHttpFileManager* iHttpFileManager;
	CRhodesAppUi * iAppUI;
	};

#endif //ENABLE_DYNAMIC_RHOBUNDLE

#endif // APPMANAGER_H
