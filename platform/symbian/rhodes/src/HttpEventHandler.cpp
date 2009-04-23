/*
 ============================================================================
 Name		: HttpEventHandler.cpp
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
 Description : CHttpEventHandler implementation
 ============================================================================
 */

#include "HttpEventHandler.h"
#include <uri8.h>
#include <http.h>

#include <http\MHTTPDataSupplier.h>
#include <http\RHTTPSession.h>
#include <http\RHTTPHeaders.h>

#include "HttpConstants.h"
#include "HttpFileManager.h"
#include "tcmalloc/rhomem.h"
#include <stdlib.h>

// Supplied as the name of the test program
_LIT(KHttpClientTestName, "RhoHttpClient");


CHttpEventHandler::CHttpEventHandler()
	: iResBodyBufferPtr(0,0), iResBodyBuffer(NULL)
	{
	iVerbose = EFalse;
	iUsingFile = EFalse;
	iTest = NULL;
	}

CHttpEventHandler::~CHttpEventHandler()
	{
	iFileServ.Close();
	
	if ( iTest )
	{
		iTest->End();
		iTest->Close();
		delete iTest;
	}
	if ( iHttpFileManager )
		delete iHttpFileManager;

	}

CHttpEventHandler* CHttpEventHandler::NewLC()
	{
	CHttpEventHandler* self = new (ELeave)CHttpEventHandler();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CHttpEventHandler* CHttpEventHandler::NewL()
	{
	CHttpEventHandler* self=CHttpEventHandler::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CHttpEventHandler::ConstructL()
	{
	User::LeaveIfError(iFileServ.Connect());
	iHttpFileManager = CHttpFileManager::NewL();
	}

void CHttpEventHandler::SetVerbose(TBool aVerbose)
	{ 
	iVerbose = aVerbose; 
	iTest = new RTest(KHttpClientTestName);
	iTest->Start(KNullDesC);
	}


TInt CHttpEventHandler::MHFRunError(TInt aError, RHTTPTransaction /*aTransaction*/, const THTTPEvent& /*aEvent*/)
	{
	if (iVerbose)
		iTest->Console()->Printf(_L("MHFRunError fired with error code %d\n"), aError);

	return KErrNone;
	}

void CHttpEventHandler::MHFRunL(RHTTPTransaction aTransaction, const THTTPEvent& aEvent)
	{
	switch (aEvent.iStatus)
		{
		case THTTPEvent::EGotResponseHeaders:
			{
			// HTTP response headers have been received. We can determine now if there is
			// going to be a response body to save.
			RHTTPResponse resp = aTransaction.Response();
			TInt status = resp.StatusCode();
			
			if (iVerbose)
			{
				RStringF statusStr = resp.StatusText();
				TBuf<32> statusStr16;
				statusStr16.Copy(statusStr.DesC());
				iTest->Console()->Printf(_L("Status: %d (%S)\n"), status, &statusStr16);

				// Dump the headers if we're being verbose
				DumpRespHeadersL(aTransaction);
			}
			// Determine if the body will be saved to disk
			iSavingResponseBody = EFalse;
			if (resp.HasBody() && (status >= 200) && (status < 300) && (status != 204))
			{
				if (iVerbose)
				{
					TInt dataSize = resp.Body()->OverallDataSize();
					if (dataSize >= 0)
						iTest->Console()->Printf(_L("Response body size is %d\n"), dataSize);
					else
						iTest->Console()->Printf(_L("Response body size is unknown\n"));
				}
				iSavingResponseBody = ETrue;
			}
			else
			{
				if ( resp.HasBody() ){
					iResBodyBuffer = HBufC8::NewMaxL(1);
					iResBodyBufferPtr.Set(iResBodyBuffer->Des());
					iResBodyBufferPtr[0] = 0;
				}
					
				if (iVerbose)
					iTest->Console()->Printf(_L("Response status is bad\n"));
			}

			if ((status >= 200) && (status < 300) && (status != 204))
			{
				ParseCookieL(aTransaction);
			}
			
			if (iSavingResponseBody) // If we're saving, then open a file handle for the new file
			{
				if ( iUsingFile )
				{
					iHttpFileManager->GetNewFile(iRespBodyFilePath, iRespBodyFileName, EFalse);
					
					// Check it exists and open a file handle
					TInt valid = iFileServ.IsValidName(iRespBodyFilePath);
					if (!valid)
					{
						if (iVerbose)
							iTest->Console()->Printf(_L("The specified filename is not valid!.\n"));
						
						iSavingResponseBody = EFalse;
					}
					else
					{
						TInt err = iRespBodyFile.Create(iFileServ,
													  iRespBodyFilePath,
													  EFileWrite|EFileShareExclusive);
						if (err)
						{
							iSavingResponseBody = EFalse;
							User::Leave(err);
						}
					}
				}
				else
				{
					TInt dataSize = resp.Body()->OverallDataSize();
					
					if ( iResBodyBuffer )
						delete iResBodyBuffer;
						
					iResBodyBuffer = NULL;
					
					if ( dataSize > 50 * 1024) //skip large chunks of data
					{
						iSavingResponseBody = false;
						//try to stop current connection
						if (iVerbose)
							iTest->Console()->Printf(_L("Transaction Failed\n"));
						aTransaction.Close();
						CActiveScheduler::Stop();
					}
					else
					{
						iResBodyBuffer = HBufC8::NewMaxL(dataSize);
						iResBodyBufferPtr.Set(iResBodyBuffer->Des());
					}					
					iCurPos = 0;
				}
			}
			
		} break;
		case THTTPEvent::EGotResponseBodyData:
			{
			// Get the body data supplier
			iRespBody = aTransaction.Response().Body();

			// Some (more) body data has been received (in the HTTP response)
			if (iVerbose)
				DumpRespBody(aTransaction);
			
			// Append to the output file if we're saving responses
			if (iSavingResponseBody)
			{
				TPtrC8 bodyData;
				TBool lastChunk = iRespBody->GetNextDataPart(bodyData);
								
				if ( iUsingFile )
				{
					iRespBodyFile.Write(bodyData);
					if (lastChunk)
					{
						iRespBodyFile.Flush();
						iRespBodyFile.Rename(iRespBodyFileName);
						iRespBodyFile.Close();
					}
				}
				else
				{
					Mem::Copy((void*)(iResBodyBuffer->Ptr()+iCurPos), (void*)bodyData.Ptr(), bodyData.Size());
					iCurPos += bodyData.Size();
				}
			}

			// Done with that bit of body data
			iRespBody->ReleaseData();
			} break;
		case THTTPEvent::EResponseComplete:
			{
			// The transaction's response is complete
			if (iVerbose)
				iTest->Console()->Printf(_L("\nTransaction Complete\n"));
			} break;
		case THTTPEvent::ESucceeded:
			{
			if (iVerbose)
				iTest->Console()->Printf(_L("Transaction Successful\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::EFailed:
			{
			if (iVerbose)
				iTest->Console()->Printf(_L("Transaction Failed\n"));
			aTransaction.Close();
			CActiveScheduler::Stop();
			} break;
		case THTTPEvent::ERedirectedPermanently:
			{
			if (iVerbose)
				iTest->Console()->Printf(_L("Permanent Redirection\n"));
			} break;
		case THTTPEvent::ERedirectedTemporarily:
			{
			if (iVerbose)
				iTest->Console()->Printf(_L("Temporary Redirection\n"));
			} break;
		default:
			{
			if (iVerbose)
				iTest->Console()->Printf(_L("<unrecognised event: %d>\n"), aEvent.iStatus);
			// close off the transaction if it's an error
			if (aEvent.iStatus < 0)
				{
				aTransaction.Close();
				CActiveScheduler::Stop();
				}
			} break;
		}
	}

void CHttpEventHandler::DumpRespHeadersL(RHTTPTransaction& aTrans)
	{
	RHTTPResponse resp = aTrans.Response();
	RStringPool strP = aTrans.Session().StringPool();
	RHTTPHeaders hdr = resp.GetHeaderCollection();
	THTTPHdrFieldIter it = hdr.Fields();

	TBuf<CHttpConstants::KMaxHeaderNameLen>  fieldName16;
	TBuf<CHttpConstants::KMaxHeaderValueLen> fieldVal16;

	while (it.AtEnd() == EFalse)
		{
		RStringTokenF fieldName = it();
		RStringF fieldNameStr = strP.StringF(fieldName);
		THTTPHdrVal fieldVal;
		if (hdr.GetField(fieldNameStr,0,fieldVal) == KErrNone)
			{
			const TDesC8& fieldNameDesC = fieldNameStr.DesC();
			fieldName16.Copy(fieldNameDesC.Left(CHttpConstants::KMaxHeaderNameLen));
			switch (fieldVal.Type())
				{
			case THTTPHdrVal::KTIntVal:
				iTest->Console()->Printf(_L("%S: %d\n"), &fieldName16, fieldVal.Int());
				break;
			case THTTPHdrVal::KStrFVal:
				{
				RStringF fieldValStr = strP.StringF(fieldVal.StrF());
				const TDesC8& fieldValDesC = fieldValStr.DesC();
				fieldVal16.Copy(fieldValDesC.Left(CHttpConstants::KMaxHeaderValueLen));
				iTest->Console()->Printf(_L("%S: %S\n"), &fieldName16, &fieldVal16);
				}
				break;
			case THTTPHdrVal::KStrVal:
				{
				RString fieldValStr = strP.String(fieldVal.Str());
				const TDesC8& fieldValDesC = fieldValStr.DesC();
				fieldVal16.Copy(fieldValDesC.Left(CHttpConstants::KMaxHeaderValueLen));
				iTest->Console()->Printf(_L("%S: %S\n"), &fieldName16, &fieldVal16);
				}
				break;
			case THTTPHdrVal::KDateVal:
				{
				TDateTime date = fieldVal.DateTime();
				TBuf<40> dateTimeString;
				TTime t(date);
				t.FormatL(dateTimeString,CHttpConstants::KDateFormat);

				iTest->Console()->Printf(_L("%S: %S\n"), &fieldName16, &dateTimeString);
				} 
				break;
			default:
				iTest->Console()->Printf(_L("%S: <unrecognised value type>\n"), &fieldName16);
				break;
				}

			// Display realm for WWW-Authenticate header
			RStringF wwwAuth = strP.StringF(HTTP::EWWWAuthenticate,RHTTPSession::GetTable());
			if (fieldNameStr == wwwAuth)
				{
				// check the auth scheme is 'basic'
				RStringF basic = strP.StringF(HTTP::EBasic,RHTTPSession::GetTable());
				RStringF realm = strP.StringF(HTTP::ERealm,RHTTPSession::GetTable());
				THTTPHdrVal realmVal;
				if ((fieldVal.StrF() == basic) && 
					(!hdr.GetParam(wwwAuth, realm, realmVal)))
					{
					RStringF realmValStr = strP.StringF(realmVal.StrF());
					fieldVal16.Copy(realmValStr.DesC());
					iTest->Console()->Printf(_L("Realm is: %S\n"), &fieldVal16);
					}
				}
			}
		++it;
		}
	}

void CHttpEventHandler::DumpRespBody(RHTTPTransaction& aTrans)
	{
	MHTTPDataSupplier* body = aTrans.Response().Body();
	TPtrC8 dataChunk;
	TBool isLast = body->GetNextDataPart(dataChunk);
	DumpIt(dataChunk);
	if (isLast)
		iTest->Console()->Printf(_L("Got last data chunk.\n"));
	}

CConsoleBase* CHttpEventHandler::Console()
	{
		return iTest->Console();
	}

void CHttpEventHandler::DumpIt(const TDesC8& aData)
//Do a formatted dump of binary data
	{
	// Iterate the supplied block of data in blocks of cols=80 bytes
	const TInt cols=16;
	TInt pos = 0;
	TBuf<KMaxFileName - 2> logLine;
	TBuf<KMaxFileName - 2> anEntry;
	const TInt dataLength = aData.Length();
	while (pos < dataLength)
		{
		//start-line hexadecimal( a 4 digit number)
		anEntry.Format(TRefByValue<const TDesC>_L("%04x : "), pos);
		logLine.Append(anEntry);

		// Hex output
		TInt offset;
		for (offset = 0; offset < cols; ++offset)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				anEntry.Format(TRefByValue<const TDesC>_L("%02x "), nextByte);
				logLine.Append(anEntry);
				}
			else
				{
				//fill the remaining spaces with blanks untill the cols-th Hex number 
				anEntry.Format(TRefByValue<const TDesC>_L("   "));
				logLine.Append(anEntry);
				}
			}
			anEntry.Format(TRefByValue<const TDesC>_L(": "));
			logLine.Append(anEntry);

		// Char output
		for (offset = 0; offset < cols; ++offset)
			{
			if (pos + offset < aData.Length())
				{
				TInt nextByte = aData[pos + offset];
				if ((nextByte >= ' ') && (nextByte <= '~'))
					{
					anEntry.Format(TRefByValue<const TDesC>_L("%c"), nextByte);
					logLine.Append(anEntry);
					}
				else
					{
					anEntry.Format(TRefByValue<const TDesC>_L("."));
					logLine.Append(anEntry);
					}
				}
			else
				{
				anEntry.Format(TRefByValue<const TDesC>_L(" "));
				logLine.Append(anEntry);
				}
			}
		
		iTest->Console()->Printf(TRefByValue<const TDesC>_L("%S\n"), &logLine);	
		logLine.Zero();

		// Advance to next  byte segment (1 seg= cols)
		pos += cols;
		}
	}

char* CHttpEventHandler::GetResponse()
{
	char* str = NULL;
	
	if ( iResBodyBuffer && iResBodyBufferPtr.Length() > 0 )
	{
		TInt size = iResBodyBufferPtr.Length();
		str = (char*)malloc(size + 1);
	    Mem::Copy(str, iResBodyBuffer->Ptr(), size);
	    str[size] = '\0';
	}else if ( iResBodyBuffer )
		str = (char*)calloc(1,1);
	
	if ( iResBodyBuffer )
		delete iResBodyBuffer;
	
	iResBodyBuffer = NULL;
	
	return str;
}

void CHttpEventHandler::ParseCookieL(RHTTPTransaction& aTrans)
{
	RHTTPResponse response = aTrans.Response();
	RHTTPResponse resp = aTrans.Response();
	RStringPool pool = aTrans.Session().StringPool();
    RHTTPHeaders headers = resp.GetHeaderCollection();
    
	RStringF fieldName = pool.StringF(HTTP::ESetCookie,RHTTPSession::GetTable());
	
	_LIT(KSeparator,";");
	_LIT(KPathName,";path=");
	_LIT(KEqual,"=");
	
	THTTPHdrVal val;
	if (headers.GetField(fieldName, 0, val) != KErrNotFound)
	{
		RStringF cookieValueName = pool.StringF(HTTP::ECookieValue,RHTTPSession::GetTable());
		RStringF cookieNameName = pool.StringF(HTTP::ECookieName,RHTTPSession::GetTable());
		RStringF cookiePathName = pool.StringF(HTTP::EPath,RHTTPSession::GetTable());
	
		if (val.StrF() == pool.StringF(HTTP::ECookie, RHTTPSession::GetTable()))
		{
			THTTPHdrVal cookieValue;
			THTTPHdrVal cookieName;
			THTTPHdrVal cookiePath;
			
			TInt parts = headers.FieldPartsL(fieldName);
	
			Mem::Fill((void*)iCookies.Ptr(), 1024, 0);
			
			// Get all the cookies.
			for (TInt i = 0; i < parts; i++)
			{
				headers.GetParam(fieldName, cookieValueName, cookieValue, i);
				headers.GetParam(fieldName, cookieNameName, cookieName, i);
				headers.GetParam(fieldName, cookiePathName, cookiePath, i);
				
				if ( GetHdrVal( cookieName, pool) )
					iCookies.Append(KEqual);
					
				if ( GetHdrVal( cookieValue, pool) )
				{	
					iCookies.Append(KPathName);
					GetHdrVal( cookiePath, pool);
					iCookies.Append(KSeparator);
				}
			}
		}
	}
}

TBool CHttpEventHandler::GetHdrVal( THTTPHdrVal& hdrVal, RStringPool& pool)
{
	TBool retval = ETrue;
	TPtrC8 auth_token((const TUint8*)"auth_token");
	
	switch (hdrVal.Type())
	{
		case THTTPHdrVal::KStrFVal:
			{
				RStringF fieldNameStr = pool.StringF(hdrVal.StrF());
				const TDesC8& fieldNameDesC = fieldNameStr.DesC();
				
				if (iVerbose)
				{
					TBuf<CHttpConstants::KMaxHeaderValueLen>  value;
					value.Copy(fieldNameDesC.Left(CHttpConstants::KMaxHeaderValueLen));
					iTest->Console()->Printf(_L("%S:\n"), &value);
				}

				if ( fieldNameDesC.Length() > 0 && fieldNameDesC.Compare(auth_token) )
					iCookies.Append(fieldNameDesC);
				else
					retval = EFalse;
			}
			break;
		case THTTPHdrVal::KStrVal:
			{
				RString fieldNameStr = pool.String(hdrVal.Str());
				const TDesC8& fieldNameDesC = fieldNameStr.DesC();
				
				if (iVerbose)
				{
					TBuf<CHttpConstants::KMaxHeaderValueLen>  value;
					value.Copy(fieldNameDesC.Left(CHttpConstants::KMaxHeaderValueLen));
					iTest->Console()->Printf(_L("%S:\n"), &value);
				}
				
				if ( fieldNameDesC.Length() > 0 && fieldNameDesC.Compare(auth_token) )
					iCookies.Append(fieldNameDesC);
				else
					retval = EFalse;
			}
			break;
	}
	
	return retval;
}

char* CHttpEventHandler::GetCookie()
{
	if (iVerbose)
	{
		if ( iCookies.Length() > 0)
			iTest->Console()->Printf(_L("Have cookie"));
		else
			iTest->Console()->Printf(_L("No cookie"));
	}
	
	if ( iCookies.Length() > 0 )
		return (char *)iCookies.Ptr();
	else
		return NULL;
}

void CHttpEventHandler::ClearCookie()
{
	Mem::Fill((void*)iCookies.Ptr(), CHttpConstants::KMaxUrlSize, 0);
	iCookies.Zero();
	if (iVerbose)
		iTest->Console()->Printf(_L("CHttpEventHandler::ClearCookie"));
}
