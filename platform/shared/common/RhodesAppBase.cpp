/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "RhodesAppBase.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "common/RhoConf.h"
#include "unzip/unzip.h"

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CRhodesAppBase,"RhodesApp");
CRhodesAppBase* CRhodesAppBase::m_pInstance = 0;

/*static*/ CRhodesAppBase* CRhodesAppBase::Create(const String& strRootPath, const String& strUserPath)
{
    if ( m_pInstance != null) 
        return m_pInstance;

    m_pInstance = new CRhodesAppBase(strRootPath, strUserPath);
    return m_pInstance;
}

/*static*/void CRhodesAppBase::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CRhodesAppBase::CRhodesAppBase(const String& strRootPath, const String& strUserPath) : CRhoThread()
{
    m_strRhoRootPath = strRootPath;
    m_strAppUserPath = strUserPath;

    initAppUrls();
}
	
void CRhodesAppBase::initAppUrls() 
{
#ifndef RHODES_EMULATOR
    m_strBlobsDirPath = getRhoUserPath() + "db/db-files";
	m_strDBDirPath = getRhoUserPath() + "db";

    m_strAppRootPath = getRhoRootPath() + "apps";
    //m_strRhodesPath = "";
#else
    m_strBlobsDirPath = getRhoUserPath() + RHO_EMULATOR_DIR"/db/db-files";
    m_strDBDirPath = getRhoUserPath() + RHO_EMULATOR_DIR"/db";

    m_strAppRootPath = getRhoRootPath();
#endif
}

String CRhodesAppBase::getRelativeDBFilesPath(const String& strFilePath)
{
#ifndef RHODES_EMULATOR
    String strDbFileRoot = getRhoUserPath();//getRhoRootPath();
#else
    String strDbFileRoot = getRhoRootPath() + RHO_EMULATOR_DIR;
#endif

    if ( !String_startsWith(strFilePath, strDbFileRoot) )
        return strFilePath;

    return strFilePath.substr(strDbFileRoot.length());
}

String CRhodesAppBase::resolveDBFilesPath(const String& strFilePath)
{
#ifndef RHODES_EMULATOR
    String strDbFileRoot = getRhoUserPath();//getRhoRootPath();
#else
    String strDbFileRoot = getRhoRootPath() + RHO_EMULATOR_DIR;
#endif

    if ( strFilePath.length() == 0 || String_startsWith(strFilePath, strDbFileRoot) )
        return strFilePath;

    return CFilePath::join(strDbFileRoot, strFilePath);
}

String CRhodesAppBase::canonicalizeRhoUrl(const String& strUrl) 
{
    if (strUrl.length() == 0 )
        return m_strHomeUrl;

    size_t pos = strUrl.find_first_of(":#");
    if((pos == String::npos) || (strUrl.at(pos) == '#'))
        return CFilePath::join(m_strHomeUrl,strUrl);

    return strUrl;
}

boolean CRhodesAppBase::isBaseUrl(const String& strUrl)
{
    return String_startsWith(strUrl, m_strHomeUrl);
}


} //namespace common
} //namespace rho

extern "C" {

int rho_sys_unzip_file(const char* szZipPath, const char* psw)
{
    rho::common::CFilePath oPath(szZipPath);
    rho::String strBaseDir = oPath.getFolderName();
#if defined(UNICODE) && defined(WIN32)
    rho::StringW strZipPathW;
    rho::common::convertToStringW(szZipPath, strZipPathW);
    HZIP hz = OpenZipFile(strZipPathW.c_str(), psw);
    if ( !hz )
        return -1;

	// Set base for unziping
    SetUnzipBaseDir(hz, rho::common::convertToStringW(strBaseDir).c_str());
#else
    HZIP hz = OpenZipFile(szZipPath, psw);
    if ( !hz )
        return -1;

	// Set base for unziping
    SetUnzipBaseDir(hz,strBaseDir.c_str() );
#endif

    ZIPENTRY ze;
    ZRESULT res = 0;
	// Get info about the zip
	// -1 gives overall information about the zipfile
	res = GetZipItem(hz,-1,&ze);
	int numitems = ze.index;

	// Iterate through items and unzip them
	for (int zi = 0; zi<numitems; zi++)
	{ 
		// fetch individual details, e.g. the item's name.
		res = GetZipItem(hz,zi,&ze); 
        if ( res == ZR_OK )
    		res = UnzipItem(hz, zi, ze.name);         
	}

	CloseZip(hz);

    return res;
}

#ifdef RHODES_EMULATOR

const char* rho_rhodesapp_getplatform()
{
    rho::String strPlatform = RHOSIMCONF().getString("platform");

    if ( strPlatform.compare("bb") == 0 || strPlatform.compare("bb6") == 0 )
        return "Blackberry";
    
    if ( strPlatform.compare("wm") == 0 )
        return "WINDOWS";

    if ( strPlatform.compare("wp") == 0 )
        return "WP7";

    if ( strPlatform.compare("android") == 0 )
        return "ANDROID";

    if ( strPlatform.compare("iphone") == 0 )
        return "APPLE";

    if ( strPlatform.compare("symbian") == 0 )
        return "SYMBIAN";

    return "UNKNOWN";
}    
#else

const char* rho_rhodesapp_getplatform()
{
#if defined(OS_MACOSX)
	return "APPLE";
#elif defined(OS_WINDOWS) || defined (OS_WINCE)
	return "WINDOWS";
#elif defined(OS_SYMBIAN)
	return "SYMBIAN";
#elif defined(OS_ANDROID)
    return "ANDROID";
#elif defined(OS_LINUX)
    return "LINUX";
#else
	return "UNKNOWN";
#endif			
}
#endif  //!RHODES_EMULATOR

/*
const char* rho_rhodesapp_getrhodespath()
{
    return RHODESAPPBASE().getRhodesPath().c_str();
}*/

void rho_file_recursive_createdir(const char* szPath, const char* szBasePath)
{
    rho::common::CRhoFile::recursiveCreateDir(szPath, szBasePath);
}

static const char table64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int rho_base64_encode(const char *src, int srclen, char *dst)
{
	if (srclen < 0)
		srclen = strlen(src);
	if (!dst)
		return (srclen/3)*4 + (srclen%3 ? 4 : 0) + 1;
	
	int out = 0;
	for(int in = 0; in < srclen; in += 3, out += 4) {
		
		unsigned x = 0;
		int actual = 0;
		for (int i = 0; i < 3; ++i) {
			char c;
			if (in + i >= srclen)
				c = 0;
			else {
				c = src[in + i];
				actual += 8;
			}
			x = (x << 8) + (unsigned char)c;
		}
		
		for (int i = 0; i < 4; ++i) {
			if (actual <= 0) {
				dst[out + i] = '=';
			}
			else {
				dst[out + i] = table64[(x >> 18) & 0x3F];
				x <<= 6;
				actual -= 6;
			}
		}
	}
	
	dst[out++] = '\0';
	return out;
}

int rho_base64_decode(const char *src, int srclen, char *dst)
{
	if (srclen < 0)
		srclen = strlen(src);
	// Do not decode in case if srclen can not be divided by 4
	if (srclen%4)
		return 0;
	if (!dst)
		return srclen*3/4 + 1;
	
	char *found;
	int out = 0;
	for (int in = 0; in < srclen; in += 4, out += 3) {
		unsigned x = 0;
		for (int i = 0; i < 4; ++i) {
			if ((found = strchr(const_cast<char*>(table64), src[in + i])) != NULL)
				x = (x << 6) + (unsigned int)(found - table64);
			else if (src[in + i] == '=')
				x <<= 6;
		}
		
		for (int i = 0; i < 3; ++i) {
			dst[out + i] = (unsigned char)((x >> 16) & 0xFF);
			x <<= 8;
		}
	}
	dst[out++] = '\0';
	return out;
}
	
} //extern "C"

#if !defined(OS_ANDROID)
rho::String rho_sysimpl_get_phone_id()
{
    return "";
}
#endif

#if !defined(OS_MACOSX)
const char* rho_native_rhouserpath()
{
    return rho_native_rhopath();
}
#endif
