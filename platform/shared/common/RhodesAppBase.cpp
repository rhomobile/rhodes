#include "RhodesAppBase.h"
#include "common/RhoFilePath.h"
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

CRhodesAppBase::CRhodesAppBase(const String& strRootPath) : CRhoThread(rho_impl_createClassFactory())
{
    m_strRhoRootPath = strRootPath;

    initAppUrls();
}
	
void CRhodesAppBase::initAppUrls() 
{
    m_strBlobsDirPath = getRhoRootPath() + "db/db-files";
	m_strDBDirPath = getRhoRootPath() + "db";
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

    if ( strncmp("http://", strUrl.c_str(), 7 ) == 0 ||
        strncmp("https://", strUrl.c_str(), 8 ) == 0 ||
        strncmp("javascript:", strUrl.c_str(), 11 ) == 0 ||
        strncmp("mailto:", strUrl.c_str(), 7) == 0 ||
        strncmp("tel:", strUrl.c_str(), 4) == 0 ||
        strncmp("wtai:", strUrl.c_str(), 5) == 0
        )
        return strUrl;

    return CFilePath::join(m_strHomeUrl,strUrl);
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


} //extern "C"
