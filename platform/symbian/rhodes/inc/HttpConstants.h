/*
 ============================================================================
 Name		: HttpClient.cpp
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   :   Copyright (C) 2008 Rhomobile. All rights reserved.

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

#ifndef HTTPCONSTANTS_H_
#define HTTPCONSTANTS_H_

#include <e32std.h>

#define _DECLARE_LIT(name,s) const static TLitC<sizeof(L##s)/2> name
#define _DEFINE_LIT(class_name,name,s) const TLitC<sizeof(L##s)/2> class_name::name={sizeof(L##s)/2-1,L##s}

class CHttpConstants
	{
public:
	
	enum TCommands
		{
		EGet,
		EPost
		};

	//Commands
	static const TInt KMaxCommandLength = 5; //5 for future needs
	
	static const char* KGetCommand;
	static const char* KPostCommand;
	
	// Size of buffer used when submitting request bodies
	static const TInt KMaxSubmitSize = 1024;
	static const TInt KMaxUrlSize = 1024;
	static const TInt KMaxHeaderNameLen = 32;
	static const TInt KMaxHeaderValueLen = 128;
	static const TInt KMaxContentTypeSize = 64;
	
	//Prefix 
	_DECLARE_LIT(KFileIncompletePrefix, "___");
	
	//folder for request body files
	_DECLARE_LIT(KHttpIN, "\\data\\rho\\http\\in\\");
	
	//folder for response body files
	_DECLARE_LIT(KHttpOUT, "\\data\\rho\\http\\out\\");

	// File system root
	_DECLARE_LIT(KFileSystemRoot,"\\");

	// format for output of data/time values
	_DECLARE_LIT(KDateFormat,"%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S.%C%:3");

	_DECLARE_LIT(KImagesDir, "\\data\\rho\\images\\");
	};

#endif /*HTTPCONSTANTS_H_*/
