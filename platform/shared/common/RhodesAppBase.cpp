#include "RhodesAppBase.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "unzip/unzip.h"

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CRhodesAppBase,"RhodesApp");
CRhodesAppBase* CRhodesAppBase::m_pInstance = 0;

/*static*/ CRhodesAppBase* CRhodesAppBase::Create(const String& strRootPath)
{
    if ( m_pInstance != null) 
        return m_pInstance;

    m_pInstance = new CRhodesAppBase(strRootPath);
    return m_pInstance;
}

/*static*/void CRhodesAppBase::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CRhodesAppBase::CRhodesAppBase(const String& strRootPath) : CRhoThread()
{
    m_strRhoRootPath = strRootPath;

    initAppUrls();
}
	
void CRhodesAppBase::initAppUrls() 
{
#ifndef RHODES_EMULATOR
    m_strBlobsDirPath = getRhoRootPath() + "db/db-files";
	m_strDBDirPath = getRhoRootPath() + "db";

    m_strAppRootPath = getRhoRootPath() + "apps";
    m_strRhodesPath = "";
#else
    m_strBlobsDirPath = getRhoRootPath() + RHO_EMULATOR_DIR"/db/db-files";
	m_strDBDirPath = getRhoRootPath() + RHO_EMULATOR_DIR"/db";

    m_strAppRootPath += getRhoRootPath();
#endif
}

String CRhodesAppBase::resolveDBFilesPath(const String& strFilePath)
{
    if ( String_startsWith(strFilePath, getRhoRootPath()) )
        return strFilePath;

    return CFilePath::join(getRhoRootPath(), strFilePath);
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

} //namespace common
} //namespace rho

extern "C" {

int rho_unzip_file(const char* szZipPath)
{
#ifdef  UNICODE
    rho::StringW strZipPathW;
    rho::common::convertToStringW(szZipPath, strZipPathW);
    HZIP hz = OpenZipFile(strZipPathW.c_str(), "");
    if ( !hz )
        return 0;

	// Set base for unziping
    SetUnzipBaseDir(hz, rho::common::convertToStringW(RHODESAPPBASE().getDBDirPath()).c_str() );
#else
    HZIP hz = OpenZipFile(szZipPath, "");
    if ( !hz )
        return 0;

	// Set base for unziping
    SetUnzipBaseDir(hz, RHODESAPPBASE().getDBDirPath().c_str() );
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

    return res == ZR_OK ? 1 : 0;
}

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

const char* rho_rhodesapp_getrhodespath()
{
    return RHODESAPPBASE().getRhodesPath().c_str();
}

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
