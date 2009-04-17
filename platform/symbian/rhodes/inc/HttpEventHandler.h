/*
 ============================================================================
 Name		: HttpEventHandler.h
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
 Description : CHttpEventHandler declaration
 ============================================================================
 */

#ifndef HTTPEVENTHANDLER_H
#define HTTPEVENTHANDLER_H

// INCLUDES
#include <e32std.h>
#include <e32test.h>
#include <f32file.h>

#include <http\mhttptransactioncallback.h>

//FORWARD DECLARATION 
class CHttpFileManager;

// CLASS DECLARATION

/**
 *  CHttpEventHandler
 * 
 *  Handles all events for the active transaction
 */
class CHttpEventHandler : public MHTTPTransactionCallback
	{
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CHttpEventHandler();

	/**
	 * Two-phased constructor.
	 */
	static CHttpEventHandler* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CHttpEventHandler* NewLC();

	void SetVerbose(TBool aVerbose);
	TBool Verbose() const { return iVerbose; }
	
	void SetUsingFile(TBool aUsingFile) { iUsingFile = aUsingFile; } 
	
	//
	// methods from MHTTPTransactionCallback
	//
	virtual void MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	virtual TInt MHFRunError(TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent);
	
	char* GetResponse();

	char* GetCookie();
	
	void ClearCookie();
	
	CConsoleBase* Console();
	
protected:
	//get file name for response body
	void GetResponseBodyFile(TDes& aResBodyFileName);
	
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHttpEventHandler();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();

	void DumpRespHeadersL(RHTTPTransaction& aTrans);
	void DumpRespBody(RHTTPTransaction& aTrans);
	void DumpIt(const TDesC8& aData);
	
	void ParseCookieL(RHTTPTransaction& aTrans);
	TBool GetHdrVal( THTTPHdrVal& hdrVal, RStringPool& pool);
	
private: //data
	TBool iVerbose;

	TBool iSavingResponseBody;
	MHTTPDataSupplier* iRespBody;

	TFileName iRespBodyFileName;
	RFile iRespBodyFile;
	
	RTest* iTest;
	RFs iFileServ;
	TFileName iRespBodyFilePath;
	
	CHttpFileManager* iHttpFileManager;
	
	TBool iUsingFile;
	
	TPtr8 iResBodyBufferPtr;
	HBufC8* iResBodyBuffer;
	TInt iCurPos;
	
	TBuf8<1024> iCookies;
	};

#endif // HTTPEVENTHANDLER_H
