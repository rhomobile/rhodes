/*
 ============================================================================
 Name		: HttpFileManager.h
 Author	  : Rhomobile
 Version	 : 1.0
 Copyright   :   Copyright (C) 2008 Anton Antonov. All rights reserved.

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
 Description : CHttpFileManager declaration
 ============================================================================
 */

#ifndef HTTPFILEMANAGER_H
#define HTTPFILEMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

// CLASS DECLARATION

/**
 *  CHttpFileManager
 * 
 */
class CHttpFileManager : public CBase
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CHttpFileManager();

	/**
	 * Two-phased constructor.
	 */
	static CHttpFileManager* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CHttpFileManager* NewLC();

	/**
	 * This function generates full path to new incomplete request (or response) file. It does not create any file on filesystem.
	 * This function also returns the name of the complete file. 
	 * After processing original file will be renamed with a new file name aka complete file name.
	 * 
	 * @param aFilePath - in/out parameter, will contain path to new incomplete file
	 * @param aCompleteFileName - in/out parameter, will contain the name of the _complete_ file.
	 * @param aIn - bool flag which described in which folder file will be placed IN or OUT (request or response file)
	 */
	void GetNewFile(TFileName& aFilePath, TFileName& aCompleteFileName, TBool aIn);
	
	/**
	 * Get oldest file in the request (or response) directory.
	 * 
	 * @param aFilePath - in/out parameter, will contain path to oldest file
	 * @param aIn - bool flag which describes in which folder the oldest file will be looking for, IN or OUT (request or response)
	 */
	void GetOldestFile( TFileName& aFilePath, TBool aIn );
	
	/**
	 * Creates new request file
	 * 
	 * @param aUrl - url for GET/POST request
	 * @param aBody - (optional) request body for POST 
	 */
	TBool CreateRequestFile( const TUint8* aUrl, TInt aUrlSize, const TUint8* aBody, TInt aBodySize);
	
	/**
	 * Delete file
	 */
	TBool DeleteFile( TFileName& aFilePath);
	
	/**
	 * Read response file
	 */
	char* ReadResponseFile( TFileName& aFilePath );
	
	/**
	 * @return the number of files in the directory
	 */
	TInt GetFilesCount( TBool aIn );
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHttpFileManager();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

private: //data
	RFs iFileServ;
	};

#endif // HTTPFILEMANAGER_H
