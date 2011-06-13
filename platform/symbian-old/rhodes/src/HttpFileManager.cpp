/*
 ============================================================================
 Name		: HttpFileManager.cpp
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
 Description : CHttpFileManager implementation
 ============================================================================
 */

#include "HttpFileManager.h"
#include "HttpConstants.h"

#include <eikenv.h>
#include <e32math.h>  //Rand
#include "tcmalloc/rhomem.h"
#include <stdlib.h>

CHttpFileManager::CHttpFileManager()
	{
	// No implementation required
	}

CHttpFileManager::~CHttpFileManager()
	{
	iFileServ.Close();
	}

CHttpFileManager* CHttpFileManager::NewLC()
	{
	CHttpFileManager* self = new (ELeave)CHttpFileManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CHttpFileManager* CHttpFileManager::NewL()
	{
	CHttpFileManager* self=CHttpFileManager::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CHttpFileManager::ConstructL()
	{
	User::LeaveIfError(iFileServ.Connect());
	
	iFileServ.MkDirAll(CHttpConstants::KHttpOUT); //make file path
	iFileServ.MkDirAll(CHttpConstants::KHttpIN); //make file path

	}

TBool CHttpFileManager::DeleteFile( TFileName& aFilePath)
	{
	return iFileServ.Delete(aFilePath) == KErrNone;
	}

TBool CHttpFileManager::CreateRequestFile( const TUint8* aUrl, TInt aUrlSize, const TUint8* aBody, TInt aBodySize)
	{
		TBool isCreated = EFalse; 
		
		if ( aUrl )
		{
			TFileName filePath;
			TFileName completeFileName;
			
			GetNewFile(filePath, completeFileName, ETrue);
			
			TInt valid = iFileServ.IsValidName(filePath);
			if (valid)
			{
				RFile reqFile;
				TInt err = reqFile.Create(iFileServ,filePath,EFileWrite|EFileShareExclusive);
				
				if ( err == KErrNone )
				{
					//write command
					HBufC8 * url = HBufC8::NewMaxL(CHttpConstants::KMaxUrlSize);
					
					Mem::Fill((void*)url->Ptr(), CHttpConstants::KMaxUrlSize, 0);
					Mem::Copy((void*)url->Ptr(), (void*)aUrl, aUrlSize);
					
					//write url
					err = reqFile.Write(url->Des(), CHttpConstants::KMaxUrlSize);
					
					//write body
					if ( aBody && err == KErrNone )
					{
						TPtrC8	body(aBody, aBodySize);
						err = reqFile.Write(body, aBodySize);
					}
					
					delete url;
					
					if ( err != KErrNone )
					{
						//unable to write
						reqFile.Close();
						iFileServ.Delete(filePath);
					}
				}
			
				isCreated = err ? EFalse : ETrue;
				
				if ( isCreated )
					reqFile.Close();
			}
		}
		return isCreated;
	}

char* CHttpFileManager::ReadResponseFile( TFileName& aFilePath )
{
	char* str = NULL;
	RFile respFile;
	if (respFile.Open(iFileServ, aFilePath, EFileRead) == KErrNone)
	{
		TInt size;
		respFile.Size(size);
		
		if ( size > 0 )
		{
			HBufC8 * buffer = HBufC8::NewMaxL(size);
			TPtr8 des (buffer->Des());
			if ( respFile.Read(des, size) == KErrNone) 
			{
				str = (char*)malloc(size + 1);
			    Mem::Copy(str, buffer->Ptr(), size);
			    str[size] = '\0';
			}
			
			delete buffer;
		}
		
		respFile.Close();
	}
	
	return str;
}

void CHttpFileManager::GetNewFile(TFileName& aFilePath, TFileName& aCompleteFileName, TBool aIn)
	{
		aFilePath.Zero();
		aCompleteFileName.Zero();
		
		_LIT(KFileExtension,".dat");
		
		_LIT(KTimeFormat,"%F%D%M%H%1%T");
		TBuf<50> buf;
		TTime time;
		time.HomeTime();
		time.FormatL( buf, KTimeFormat);
		
		aCompleteFileName.Append(buf);

		TInt64 aSeed = time.Int64();
		TInt randNum = Math::Rand(aSeed) % 1000;
		aCompleteFileName.AppendNum(randNum);
		
		aCompleteFileName.Append(KFileExtension);
		
		if ( aIn )
			aFilePath.Append( CHttpConstants::KHttpIN );
		else
			aFilePath.Append( CHttpConstants::KHttpOUT );

		if ( !aIn )
			aFilePath.Append(CHttpConstants::KFileIncompletePrefix);

		aFilePath.Append(aCompleteFileName);
	}




void CHttpFileManager::GetOldestFile( TFileName& aFilePath, TBool aIn )
	{
	aFilePath.Zero();
	
	TFileName filteredFiles; 
	if ( aIn ) 
		filteredFiles = CHttpConstants::KHttpIN;
	else
		filteredFiles = CHttpConstants::KHttpOUT;
	
	// valid Directory in the file system tree
	CDirScan* scanner = CDirScan::NewL(iFileServ);
	CleanupStack::PushL(scanner);
	
	scanner->SetScanDataL(filteredFiles, KEntryAttNormal, ESortByDate, CDirScan::EScanDownTree); 
	 
	CDir* presentFolder = (CDir*)NULL;
	TRAPD(error, scanner->NextL(presentFolder));
	
	if(presentFolder && presentFolder->Count())
	{
		TInt nCount = presentFolder->Count();
		for(TInt index = 0; index < nCount; index++)
		{
			TEntry presentEntry = (*presentFolder)[index];
		
			if( !presentEntry.IsDir() )
			{
			/* presentEntry points to content or file inside the folder
			 * so do any operation */
				aFilePath.Zero();
				aFilePath.Copy(presentEntry.iName);
				
				if ( aIn )
				{
					if ( aFilePath.Find(CHttpConstants::KFileIncompletePrefix) == KErrNotFound )
						break;
				}
				else
				{
					break;
				}

			}
 
		}
		
		delete presentFolder;
	}
	
	CleanupStack::PopAndDestroy(scanner);
	}

TInt CHttpFileManager::GetFilesCount( TBool aIn )
	{
	TInt retval = 0;

	TFileName filteredFiles; 
	if ( aIn ) 
		filteredFiles = CHttpConstants::KHttpIN;
	else
		filteredFiles = CHttpConstants::KHttpOUT;

	//_LIT(KFileMask,"*.dat");
	//filteredFiles.Append(KFileMask);
	
	// valid Directory in the file system tree
	CDirScan* scanner = CDirScan::NewL(iFileServ);
	CleanupStack::PushL(scanner);
	
	scanner->SetScanDataL(filteredFiles, KEntryAttDir|KEntryAttMatchExclusive, ESortByDate | EDescending, CDirScan::EScanDownTree); 
	 
	CDir* presentFolder = (CDir*)NULL;
	TRAPD(error, scanner->NextL(presentFolder));
	
	if(presentFolder && presentFolder->Count())
	{
		for(TInt index(0); index < presentFolder->Count() ;index++)
		{
			TEntry presentEntry = (*presentFolder)[index];
	
			if( !presentEntry.IsDir() )
			{
				retval++;
			}
		}
		
		delete presentFolder;
	}
	
	CleanupStack::PopAndDestroy(scanner);
	
	return retval;
	}
