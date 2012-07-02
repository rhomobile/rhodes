//
//  Zip.cpp
//  RhoConnectClient
//
//  Created by Alex Epifanoff on 02.07.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "unzip/zip.h"
#include "common/RhoStd.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoSystem"

using namespace rho;
using namespace rho::common;

extern "C"
{		
	int rho_sys_zip_files_with_path_array_ptr(const char* szZipFilePath, const char *base_path, void* ptrFilesArray, const char* psw)
	{
		ZRESULT res;
		HZIP hz = 0;
		
#if defined(UNICODE) && defined(WIN32)
		hz = CreateZip( convertToStringW(String(szZipFilePath)).c_str(), psw);
#else
		hz = CreateZip(szZipFilePath, psw);
#endif
		
		if ( !hz )
			return -1;
		
		rho::Vector<rho::String>& arFiles = *reinterpret_cast<rho::Vector<rho::String>*>(ptrFilesArray);
		
		for ( int i = 0; i < (int)arFiles.size(); i++ )
		{
			rho::String filePath = arFiles.elementAt(i);
			bool isDir = CRhoFile::isDirectory(filePath.c_str());
			rho::String zipPath = base_path ? filePath.substr(strlen(base_path)) : filePath;
			
#if defined(UNICODE) && defined(WIN32)
			if ( isDir )
				res = ZipAddFolder( hz, convertToStringW(zipPath).c_str(), convertToStringW(filePath).c_str() );
			else
				res = ZipAdd( hz, convertToStringW(zipPath).c_str(), convertToStringW(filePath).c_str() );
#else
			if ( isDir )
				res = ZipAddFolder( hz, zipPath.c_str(), filePath.c_str() );
			else
				res = ZipAdd( hz, zipPath.c_str(), filePath.c_str() );
#endif
			
			if (res != 0)
				LOG(ERROR) + "Zip file failed: " + res + "; " + filePath;
		}
		
		res = CloseZip(hz);
		
		return res;
	}
}