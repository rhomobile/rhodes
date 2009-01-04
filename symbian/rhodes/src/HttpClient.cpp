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
 Description : CHttpClient implementation
 ============================================================================
 */

#include "HttpClient.h"

#include <uri8.h>
#include <http.h>

#include "HttpEventHandler.h"
#include "ConnectionManager.h"

#include "HttpConstants.h"
#include "HttpFileManager.h"

// Standard headers used by default
_LIT8(KUserAgent, "rhodes-sym (1.0)");
_LIT8(KAccept, "*/*");

_LIT(KHttpClientPanic, "HTTP-EC");

const TDesC8& my_cookie_name=_L8("Cookie");

//Uid for singleton
//const TInt KUidHttpClient = 12334534;

CHttpClient::CHttpClient()
	: /*CCoeStatic( TUid::Uid(KUidHttpClient) ),*/ 
	iReqBodySubmitBufferPtr(0,0), iUriPtr(0,0), iUri(NULL), iReqBodySubmitBuffer(NULL), iCookie(NULL)
	{
	iDataChunkCount = 0;
	}

CHttpClient::~CHttpClient()
	{
		if ( iConnectionManager )
		{
			iConnectionManager->Release();
			delete iConnectionManager;
		}
		
		iFileServ.Close();

		delete iReqBodySubmitBuffer;
		delete iUri;
		delete iTransObs;
		
		if ( iHttpFileManager )
			delete iHttpFileManager;
	}

// -------------------------------------------------------------------------
// CHttpClient::InstanceL
// Returns an instance of this class. When called for the first time,
// a new instance is created and returned. After that, calling
// InstanceL returns the same instance that was created earlier.
// Note that the UID passed to CCoeEnv::Static needs to be unique.
// -------------------------------------------------------------------------
/*CHttpClient* CHttpClient::InstanceL()
	{
		CHttpClient* instance = static_cast<CHttpClient*>( CCoeEnv::Static( TUid::Uid(KUidHttpClient) ) );
		
		if ( !instance )
		{
			instance = new ( ELeave ) CHttpClient;
			CleanupStack::PushL( instance );
			instance->ConstructL();
			CleanupStack::Pop();
		}
		return instance;
	}
*/

CHttpClient* CHttpClient::NewLC()
	{
	CHttpClient* self = new (ELeave)CHttpClient();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CHttpClient* CHttpClient::NewL()
	{
	CHttpClient* self=CHttpClient::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}


void CHttpClient::ConstructL()
	{
		iConnectionManager = CConnectionManager::NewL();
		
		User::LeaveIfError(iFileServ.Connect());
		iTransObs = CHttpEventHandler::NewL();
		
		iHttpFileManager = CHttpFileManager::NewL();
		
		iNotUsingFile = EFalse;
	}

void CHttpClient::InvokeHttpMethodL(TInt aCommand, const TUint8* aUrl, TInt aUrlSize, const TUint8* aBody, TInt aBodySize)
{
	iNotUsingFile = ETrue;
	
	if( iUri )
		delete iUri;
	
	iUri = NULL;
	iUri = HBufC8::NewMaxL(aUrlSize+1);
						
	Mem::Fill((void*)iUri->Ptr(), aUrlSize+1, 0);
	Mem::Copy((void*)iUri->Ptr(), (void*)aUrl, aUrlSize);
	
	iUriPtr.Set(iUri->Des());
		
	if ( aBody )
	{
		if ( iReqBodySubmitBuffer )
			delete iReqBodySubmitBuffer;
		
		iReqBodySubmitBuffer = NULL;
		iReqBodySubmitBuffer = HBufC8::NewMaxL(aBodySize);
		
		Mem::Fill((void*)iReqBodySubmitBuffer->Ptr(), aBodySize, 0);
		Mem::Copy((void*)iReqBodySubmitBuffer->Ptr(), (void*)aBody, aBodySize);
			
		iReqBodySubmitBufferPtr.Set(iReqBodySubmitBuffer->Des());
	}
	
	iTransObs->SetUsingFile(EFalse); 
	
	InvokeHttpMethodL(aCommand);
	
	iTransObs->SetUsingFile(ETrue); //enable by default
}

//Invoke http method
void CHttpClient::InvokeHttpMethodL(TInt aCommand)
	{
		RStringF method;
		iHasARequestBody = EFalse;
		
		if ( iConnectionManager && !iConnectionManager->IsOfflineMode())
			{
				iConnectionManager->SetupConnection();
				
				RStringPool strP = iConnectionManager->GetHTTPSession().StringPool();
		
				switch (aCommand)
						{
					case CHttpConstants::EGet: 
						method = strP.StringF(HTTP::EGET,RHTTPSession::GetTable());
						break;
					case CHttpConstants::EPost:
						method = strP.StringF(HTTP::EPOST,RHTTPSession::GetTable());
						iHasARequestBody = ETrue; 
						break;
					default:
						return;//other commands not supported
						}
				
				//read body and url
				if ( !iNotUsingFile )
				GetRequestBodyL();
				
				TInt realSize = 0;
				for ( TInt i = 0; i < CHttpConstants::KMaxUrlSize && iUriPtr[i]; i++)
				{
				realSize++;
				}
				
				TBuf8<CHttpConstants::KMaxUrlSize> tmp;
				tmp.Copy(iUriPtr.Ptr(), realSize);
				tmp.ZeroTerminate();
				
				TUriParser8 uri; 
				uri.Parse(tmp);
					
				iTrans = iConnectionManager->GetHTTPSession().OpenTransactionL(uri, *iTransObs, method);
				RHTTPHeaders hdr = iTrans.Request().GetHeaderCollection();
		
				// Add headers appropriate to all methods
				SetHeaderL(hdr, HTTP::EUserAgent, KUserAgent);
				SetHeaderL(hdr, HTTP::EAccept, KAccept);
		
				if ( iCookie )
					WriteCookie(hdr);
				
				// Add headers and body data for methods that use request bodies
				if (iHasARequestBody)
					{
					// Content type header
					TBuf8<CHttpConstants::KMaxContentTypeSize> contTypeBuf;
					contTypeBuf.Copy(_L("text/html"));
					RStringF contTypeStr = iConnectionManager->GetHTTPSession().StringPool().OpenFStringL(contTypeBuf);
					THTTPHdrVal contType(contTypeStr);
					hdr.SetFieldL(iConnectionManager->GetHTTPSession().StringPool().StringF(HTTP::EContentType,RHTTPSession::GetTable()), contType);
					contTypeStr.Close();
					
					MHTTPDataSupplier* dataSupplier = this;
					iTrans.Request().SetBody(*dataSupplier);
					}
	
				// submit the transaction
				iTrans.SubmitL();
	
				// Start the scheduler, once the transaction completes or is cancelled on an error the scheduler will be
				// stopped in the event handler
				CActiveScheduler::Start();

				//close and delete request file
				if ( !iNotUsingFile )
				{
					iReqBodyFile.Close();
					iFileServ.Delete(iReqBodyFilePath);
				}
			}	
	}

TBool CHttpClient::GetNextDataPart(TPtrC8& aDataPart)
	{
	__ASSERT_DEBUG(iReqBodySubmitBuffer, User::Panic(KHttpClientPanic, EReqBodySumitBufferNotAllocated));
	
	TBool retVal = EFalse;
			
	if ( !iNotUsingFile )
	{
		// Read from the request body file
	TInt err = iReqBodyFile.Read(iReqBodySubmitBufferPtr);
	if (err == KErrNone)
		{
		aDataPart.Set(iReqBodySubmitBufferPtr);
		++iDataChunkCount;
		retVal = (iReqBodySubmitBufferPtr.Length() == 0);
		}
	}
	else
	{
		//read from memory
		
		aDataPart.Set(iReqBodySubmitBufferPtr);
	
		++iDataChunkCount;
		retVal = ETrue;
	}
	return retVal;
	}

void CHttpClient::ReleaseData()
	{
	iDataChunkCount = 0;
	// Clear out the submit buffer
	TPtr8 buff = iReqBodySubmitBuffer->Des();
	buff.Zero();
	
	// Notify HTTP of more data available immediately, since it's being read from file
	//TRAPD(err, iTrans.NotifyNewRequestBodyPartL());
	
	//if (err != KErrNone)
	//	User::Panic(KHttpClientPanic, KCouldntNotifyBodyDataPart);
	
}	

TInt CHttpClient::OverallDataSize()
	{
	TInt size = 0;

	if ( !iNotUsingFile )
	{
	TInt err = iReqBodyFile.Size(size);	
	if (err < 0)
		User::Panic(KHttpClientPanic,KBodyWithInvalidSize);
	}
	else
	{
		if ( iReqBodySubmitBuffer )
			size = iReqBodySubmitBuffer->Length();
	}
	return size;
	}

TInt CHttpClient::Reset()
	{
	return KErrNotSupported;
	}

void CHttpClient::SetHeaderL(RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue)
	{
	RStringF valStr = iConnectionManager->GetHTTPSession().StringPool().OpenFStringL(aHdrValue);
	THTTPHdrVal val(valStr);
	aHeaders.SetFieldL(iConnectionManager->GetHTTPSession().StringPool().StringF(aHdrField,RHTTPSession::GetTable()), val);
	valStr.Close();
	}


// -------------------------------------------------------------------------
// Called when a authenticated page is requested
// Asks the user for a username and password that would be appropriate for the url that was
// supplied.
//
// -------------------------------------------------------------------------
TBool CHttpClient::GetCredentialsL(const TUriC8& /*aURI*/, RString /*aRealm*/, 
								   RStringF /*aAuthenticationType*/,
								   RString& /*aUsername*/, 
								   RString& /*aPassword*/)

	{
		//TODO: Currently not supported. Reserved for future needs 
		return EFalse;
	}

void CHttpClient::ToggleVerbose()
	{
		TBool verbose = iTransObs->Verbose();
		iTransObs->SetVerbose(!verbose);
	}

void CHttpClient::GetRequestBodyL()
	{
	// get post data from a file
	iHttpFileManager->GetOldestFile(iReqBodyFileName, ETrue);
	
	iReqBodyFilePath.Zero();
	iReqBodyFilePath.Append(CHttpConstants::KHttpIN);
	iReqBodyFilePath.Append(iReqBodyFileName);

	// Check it exists and open a file handle
	if (iFileServ.IsValidName(iReqBodyFileName))
	{
		if ( iReqBodyFile.Open(iFileServ, iReqBodyFilePath, EFileRead) == KErrNone )
		{
			if( iUri )
				delete iUri;
			
			iUri = NULL;
			iUri = HBufC8::NewMaxL(CHttpConstants::KMaxUrlSize);
			iUriPtr.Set(iUri->Des());
			
			TInt err = iReqBodyFile.Read(iUriPtr, CHttpConstants::KMaxUrlSize);
			if (err != KErrNone)
				User::Panic(KHttpClientPanic, KCouldntReadUrlPart);
			}
			TInt fileSize = 0;
			iReqBodyFile.Size(fileSize);
			
			iHasARequestBody = fileSize > CHttpConstants::KMaxUrlSize;
			// Get request body
			if (iHasARequestBody)
			{
				if ( iReqBodySubmitBuffer )
					delete iReqBodySubmitBuffer;
				
				iReqBodySubmitBuffer = NULL;
				iReqBodySubmitBuffer = HBufC8::NewMaxL(CHttpConstants::KMaxSubmitSize);
				iReqBodySubmitBufferPtr.Set(iReqBodySubmitBuffer->Des());
			}
			
		}
	}

char* CHttpClient::GetResponse()
{
	return iTransObs->GetResponse();
}

void CHttpClient::SetCookie( char* cookie, TInt size )
{
	if( iCookie )
		delete iCookie;
	
	iCookie = NULL;
	
	if ( cookie && size > 0 )
	{
		iCookie = HBufC8::NewMaxL(size+1);
							
		Mem::Fill((void*)iCookie->Ptr(), size+1, 0);
		Mem::Copy((void*)iCookie->Ptr(), (void*)cookie, size);
	}
}
void CHttpClient::WriteCookie(RHTTPHeaders headers)
{
	RStringPool string_pool = iConnectionManager->GetHTTPSession().StringPool();
	
	RStringF cookie_rsf = string_pool.StringF(HTTP::ECookie,RHTTPSession::GetTable());
	THTTPHdrVal field_value;
	field_value.SetStrF (cookie_rsf);
	headers.SetFieldL(cookie_rsf, field_value);
	
	THTTPHdrVal cookie_hdr;
	RStringF cookie_name_rsf = string_pool.StringF(HTTP::ECookieName,RHTTPSession::GetTable());
	
	RString name = string_pool.OpenStringL (my_cookie_name);
	cookie_hdr.SetStr (name);
	headers.SetParamL (cookie_rsf, cookie_name_rsf, cookie_hdr, 0);
	name.Close();
	RStringF cookie_value_rsf = string_pool.StringF(HTTP::ECookieValue,RHTTPSession::GetTable());
	RString value = string_pool.OpenStringL (iCookie->Des());
	cookie_hdr.SetStr (value);
	headers.SetParamL (cookie_rsf, cookie_value_rsf, cookie_hdr, 0);
	value.Close();
}