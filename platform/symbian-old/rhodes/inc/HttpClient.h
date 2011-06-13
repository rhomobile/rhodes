/*
 ============================================================================
 Name		 : HttpClient.h
 Author	     : Anton Antonov
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
 Description : CHttpClient declaration
 ============================================================================
 */

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

// INCLUDES
#include <e32std.h>
#include <f32file.h>

//#include <COEMAIN.H>
#include <http\MHTTPAuthenticationCallback.h>
#include <http\MHTTPDataSupplier.h>

// Maximum size of buffer to hold content-type data
//
const TInt KMaxContentTypeSize = 64;

//FORWARD DECLARATION
class CConnectionManager;
class CHttpEventHandler;
class CHttpFileManager;
// CLASS DECLARATION

/**
 *  CHttpClient
 * 
 *  The http client.
 *  
 *	It is derived from MHTTPAuthenticationCallback to allow access to servers that require a user to submit a username and
 *	password to access protected sections of that server (for future use).
 * 
 */

class CHttpClient : MHTTPDataSupplier, MHTTPAuthenticationCallback
	{
public:
	// Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CHttpClient* NewL();

	/**
	 * Two-phased constructor.
	 */
	static CHttpClient* NewLC();

	
	/**
	 * Destructor.
	 */
	virtual ~CHttpClient();

	enum THttpClientPanics
		{
		EReqBodySumitBufferNotAllocated,
		KBodyWithInvalidSize,
		KCouldntNotifyBodyDataPart,
		KCouldntReadUrlPart
		};

	// methods inherited from MHTTPDataSupplier
	virtual TBool GetNextDataPart(TPtrC8& aDataPart);
	virtual void  ReleaseData();
	virtual TInt  OverallDataSize();
	virtual TInt  Reset();

	// methods inherited from MHTTPAuthenticationCallback
	virtual TBool GetCredentialsL(const TUriC8& aURI, RString aRealm, 
								 RStringF aAuthenticationType,
								 RString& aUsername, 
								 RString& aPassword);

	
	//Invoke http method
	void InvokeHttpMethodL(TInt aCommand, char* szContentType);

	//Invoke http method
	void InvokeHttpMethodL(TInt aCommand, const TUint8* aUrl, TInt aUrlSize, const TUint8* aBody, TInt aBodySize, char* szContentType);

	//Toggle verbose mode
	void ToggleVerbose();
	
	//get response body (client must free allocated memory)
	char* GetResponse();
	
	//get cookie
	char* GetCookie();
	
	void SetCookie( char* cookie );
private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CHttpClient();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL();
	
protected:
	//read body from body request file
	void GetRequestBodyL();

	//set header
	void SetHeaderL(RHTTPHeaders& aHeaders, TInt aHdrField, const TDesC8& aHdrValue);
	
private: //data
	
	TPtr8 iReqBodySubmitBufferPtr;
	HBufC8* iReqBodySubmitBuffer;
	
	TPtr8 iUriPtr;
	HBufC8* iUri;
	
	TInt iDataChunkCount;
	
	CConnectionManager* iConnectionManager;
	RHTTPTransaction iTrans;

	TBool iHasARequestBody;
	TBool iNotUsingFile;
	
	RFs iFileServ;
	RFile iReqBodyFile;
	TFileName iReqBodyFileName;
	TFileName iReqBodyFilePath;

	//TBuf<KMaxContentTypeSize> iReqBodyContentType;
	CHttpEventHandler* iTransObs;
	
	CHttpFileManager* iHttpFileManager; 
	
	TBuf8<1024> iCookie;
	};

#endif // HTTPCLIENT_H
