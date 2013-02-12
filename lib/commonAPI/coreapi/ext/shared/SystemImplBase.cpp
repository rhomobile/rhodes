#include "SystemImplBase.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "common/RhodesApp.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "unzip/zip.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

namespace rho {

using namespace apiGenerator;
using namespace common;

void CSystemImplBase::getPlatform(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    rho::String strPlatform = RHOSIMCONF().getString("platform");

    if ( strPlatform.compare("bb") == 0 || strPlatform.compare("bb6") == 0 )
        oResult.set( L"Blackberry" );
    else if ( strPlatform.compare("wm") == 0 )
        oResult.set( L"WINDOWS" );
    else if ( strPlatform.compare("win32") == 0 )
        oResult.set( L"WINDOWS_DESKTOP" );
    else if ( strPlatform.compare("wp") == 0 )
        oResult.set( L"WP7" );
    else if ( strPlatform.compare("wp8") == 0 )
        oResult.set( L"WP8");
    else if ( strPlatform.compare("android") == 0 )
        oResult.set( L"ANDROID" );
    else if ( strPlatform.compare("iphone") == 0 )
        oResult.set( L"APPLE" );
    else if ( strPlatform.compare("symbian") == 0 )
        oResult.set( L"SYMBIAN" );
    else
        oResult.set( L"UNKNOWN" );

#else //!RHODES_EMULATOR

#if defined(OS_MACOSX)
	oResult.set( PLATFORM_IOS );
#elif defined(OS_WINDOWS_DESKTOP)
	oResult.set( PLATFORM_WINDOWS_DESKTOP );
#elif defined(WINDOWS_PLATFORM)
#if defined(OS_WP8)
	oResult.set( PLATFORM_WP8 );
#else
	oResult.set( PLATFORM_WM_CE );
#endif
#elif defined(OS_SYMBIAN)
	oResult.set( L"SYMBIAN" );
#elif defined(OS_ANDROID)
    oResult.set( PLATFORM_ANDROID );
#elif defined(OS_LINUX)
    oResult.set( L"LINUX" );
#else
	oResult.set( L"UNKNOWN" );
#endif			

#endif
}

void CSystemImplBase::getHasCamera(CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImplBase::getDevicePushId(CMethodResult& oResult)
{
    rho::String strDeviceID;
	if ( rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) 
		strDeviceID = rho::sync::RhoconnectClientManager::clientRegisterGetDevicePin();

    oResult.set( convertToStringW(strDeviceID) );
}

void CSystemImplBase::getOsVersion(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    oResult.set( RHOSIMCONF().getString("os_version") );
#endif
}

void CSystemImplBase::getIsMotorolaDevice(CMethodResult& oResult)
{
    oResult.set(false);
}

void CSystemImplBase::getLocalServerPort(CMethodResult& oResult)
{
    oResult.set( (int64)atoi(RHODESAPP().getFreeListeningPort()) );
}

void CSystemImplBase::setLocalServerPort( __int64 value, CMethodResult& oResult)
{
    //Local port can be set only in confuguration file
}

void CSystemImplBase::getFreeServerPort(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set( (int64)RHODESAPP().determineFreeListeningPort() );
}

void CSystemImplBase::getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImplBase::getSecurityTokenNotPassed(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set( RHODESAPP().isSecurityTokenNotPassed() );
}

void CSystemImplBase::getStartParams(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set( convertToStringW(RHODESAPP().getStartParameters()) );
}

//TODO: move rho_sys_unzip_file here
int rho_sys_unzip_file(const char* szZipPath, const char* psw);
void CSystemImplBase::unzipFile( const rho::StringW& localPathToZip, const rho::StringW& password, rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set( rho_sys_unzip_file( convertToStringA(localPathToZip).c_str(), convertToStringA(password).c_str()) );
}

void CSystemImplBase::zipFile( const rho::StringW& localPathToZip,  const rho::StringW& localPathToFile,  const rho::StringW& password, CMethodResult& oResult)
{
    ZRESULT res = -1;
    CFilePath oPath( convertToStringA(localPathToFile));

#if defined(UNICODE) || defined(_UNICODE)
    StringW strFileNameW;
    convertToStringW(oPath.getBaseName(), strFileNameW);

    HZIP hz = CreateZip(localPathToZip.c_str(), convertToStringA(password).c_str());
    if ( hz )
    {
        res = ZipAdd( hz, strFileNameW.c_str(), localPathToFile.c_str() );
        res = CloseZip(hz);
    }
#else
    HZIP hz = CreateZip( convertToStringA(localPathToZip).c_str(), convertToStringA(password).c_str());
    if ( hz )
    {
        res = ZipAdd( hz, oPath.getBaseName(), convertToStringA(localPathToFile).c_str() );
        res = CloseZip(hz);
    }
#endif
    oResult.set(res);
}

int rho_sys_zip_files_with_path_array_ptr(const char* szZipFilePath, const char *base_path, void* ptrFilesArray, const char* psw)
{
	ZRESULT res;
	HZIP hz = 0;
	
#if defined(UNICODE) || defined(_UNICODE)
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
		
#if defined(UNICODE) || defined(_UNICODE)
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

void CSystemImplBase::zipFiles( const rho::StringW& localPathToZip,  const rho::StringW& basePath,  const rho::Vector<rho::StringW>& filePathsToZip,  const rho::StringW& password, CMethodResult& oResult)
{
	rho::Vector<rho::String> arFiles;
    for( int i = 0; i < (int)filePathsToZip.size(); i++)
        arFiles.addElement( convertToStringA(filePathsToZip[i]) );

    int nRes = rho_sys_zip_files_with_path_array_ptr( convertToStringA(localPathToZip).c_str(), convertToStringA(basePath).c_str(), &arFiles, convertToStringA(password).c_str() );
	oResult.set( nRes );
}

}