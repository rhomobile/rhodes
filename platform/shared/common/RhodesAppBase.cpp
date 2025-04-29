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
#include "unzip/gunzip.h"
#include "sync/RhoconnectClientManager.h"
#include "net/INetRequest.h"

#ifdef OS_ANDROID
#include "minizip-ng/mz.h"
#include "minizip-ng/mz_strm.h"
#include "minizip-ng/mz_strm_os.h"
#include "minizip-ng/mz_zip.h"
#endif

extern "C" void rho_net_request_with_data(const char *url, const char *str_body);
extern "C" int  rho_ruby_is_started();
extern "C" const char* rho_rhodesapp_getapprootpath();

namespace rho {
namespace common{

IMPLEMENT_LOGCLASS(CRhodesAppBase,"RhodesApp");
CRhodesAppBase* CRhodesAppBase::m_pInstance = 0;
#if defined(OS_WP8) || defined(OS_UWP)
	String CRhodesAppBase::m_strHomeUrl = "";
#endif

/*static*/ CRhodesAppBase* CRhodesAppBase::Create(const String& strRootPath, const String& strUserPath, const String& strRuntimePath)
{
    if ( m_pInstance != NULL) 
        return m_pInstance;

    m_pInstance = new CRhodesAppBase(strRootPath, strUserPath, strRuntimePath);
    return m_pInstance;
}

/*static*/void CRhodesAppBase::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CRhodesAppBase::CRhodesAppBase(const String& strRootPath, const String& strUserPath, const String& strRuntimePath) : CRhoThread(), m_appPushMgr(*this)
{
    m_strRhoRootPath = strRootPath;
    m_strAppUserPath = strUserPath;
    m_strRuntimePath = strRuntimePath;
    m_bSendingLog = false;

#ifdef RHO_NO_RUBY
    m_bJSApplication   = true;        
#else
    m_bJSApplication   = false;
#endif

    const char* nodejs_app = get_app_build_config_item("nodejs_application");
    m_bNodeJSApplication = nodejs_app && (strcmp(nodejs_app,"true") == 0);

    const char* rubynodejs_app = get_app_build_config_item("rubynodejs_application");
    m_bRubyNodeJSApplication = rubynodejs_app && (strcmp(rubynodejs_app,"true") == 0);

    initAppUrls();
}
	
void CRhodesAppBase::initAppUrls() 
{
#ifndef RHODES_EMULATOR
    
    String db_dir = rho_native_rhodbpath();
    
    m_strBlobsDirPath = db_dir + "db/db-files";
	m_strDBDirPath = db_dir + "db";

//#ifndef OS_WP8
    m_strAppRootPath = getRhoRootPath() + "apps";
//#else
//    m_strAppRootPath = getRhoRootPath();
//#endif
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
    String strDbFileRoot = rho_native_rhodbpath();//getRhoRootPath();
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
    String strDbFileRoot = rho_native_rhodbpath();//getRhoRootPath();
#else
    String strDbFileRoot = getRhoRootPath() + RHO_EMULATOR_DIR;
#endif

    if ( strFilePath.length() == 0 || String_startsWith(strFilePath, strDbFileRoot) || String_startsWith(strFilePath, "file://"))
        return strFilePath;

    return CFilePath::join(strDbFileRoot, strFilePath);
}
	
String CRhodesAppBase::getDBFileRoot()
{
#ifndef RHODES_EMULATOR
		String strDbFileRoot = rho_native_rhodbpath();//getRhoRootPath();
#else
		String strDbFileRoot = getRhoRootPath() + RHO_EMULATOR_DIR;
#endif
	return strDbFileRoot;
}

String CRhodesAppBase::canonicalizeRhoUrl(const String& strUrl) const
{
    if (strUrl.length() == 0 )
    {
        return m_strHomeUrl;
    }

    if (String_startsWith(strUrl, "javascript:"))
    {
        return strUrl;
    }
	//Don't append localhost if URL starts with "www." in WINCE device
#ifdef OS_WINCE
    if (String_startsWith(strUrl, "www."))
    {
        return strUrl;
    }
#endif
    if (String_startsWith(strUrl, "file:"))
    {
        const rho::String appRootTag = "%APP_PATH%";

        String::size_type findIt = strUrl.find(appRootTag);
        String retPath = strUrl;

        if (findIt != String::npos)
        {
            retPath.erase(findIt, appRootTag.size());
            retPath.insert(findIt, getAppRootPath());
        }
#if defined( RHODES_SIMULATOR ) || defined (OS_WINDOWS_DESKTOP)
			if ( !String_startsWith(retPath, "file:///")) 
				retPath.insert( 5, "/");

			return retPath;
#elif defined(OS_WP8) || defined(OS_UWP)
            return retPath.substr(7);
#else
            return retPath;
#endif

    }

    size_t pos = strUrl.find_first_of(":#");
    if((pos == String::npos) || (strUrl.at(pos) == '#'))
        return CFilePath::join(m_strHomeUrl,strUrl);

    return strUrl;
}

boolean CRhodesAppBase::isBaseUrl(const String& strUrl)
{
	return String_startsWith(strUrl, m_strHomeUrl);
}
    
void rho_do_send_log(rho::apiGenerator::CMethodResult& oResult)
{
    String strDevicePin, strClientID;
    net::IRhoSession* pSession = 0;
	if ( sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) 
    {
	    strDevicePin = rho::sync::RhoconnectClientManager::clientRegisterGetDevicePin();
	    strClientID = rho::sync::RhoconnectClientManager::syncEnineReadClientID();
        pSession = rho::sync::RhoconnectClientManager::getRhoSession();
    }

    String strQuery;
    String strLogUrl = RHOCONF().getString("Log.destinationURI");
    if ( strLogUrl.length() == 0 )
    {
        strLogUrl = RHOCONF().getPath("logserver");
        if ( strLogUrl.length() == 0 )
            strLogUrl = RHOCONF().getPath("syncserver");
        
        strQuery = strLogUrl + "client_log?" +
            "client_id=" + strClientID + "&device_pin=" + strDevicePin + "&log_name=" + RHOCONF().getString("logname");
    }else
    {
        String strSign = "?";
        if ( strrchr(strLogUrl.c_str(), '?') )
            strSign = "&";

        strQuery = strLogUrl + strSign + "client_id=" + strClientID + "&device_pin=" + strDevicePin;
    }

    net::CMultipartItem oItem;
    oItem.m_strFilePath = LOGCONF().getLogFilePath();
    oItem.m_strContentType = "application/octet-stream";
    
    boolean bOldSaveToFile = LOGCONF().isLogToFile();
    LOGCONF().setLogToFile(false);
    NetRequest oNetRequest;
    oNetRequest.setSslVerifyPeer(false);
    
	NetResponse resp = getNetRequest(&oNetRequest).pushMultipartData( strQuery, oItem, pSession, NULL );

    LOGCONF().setLogToFile(bOldSaveToFile);
    
    boolean isOK = true;
    
    if ( !resp.isOK() )
    {
        LOG(ERROR) + "send_log failed : network error - " + resp.getRespCode() + "; Body - " + resp.getCharData();
        isOK = false;
    }
    
    Hashtable<String, String> hashRes;
    hashRes["status"] = isOK ? "ok" : "error";
    oResult.set(hashRes);
    
    RHODESAPPBASE().setSendingLog(false);
}


class CRhoSendLogCall
{
    rho::apiGenerator::CMethodResult m_oResult;
public:
    CRhoSendLogCall(rho::apiGenerator::CMethodResult& oResult): m_oResult(oResult){}
    
    void run(common::CRhoThread &)
    {
        rho_do_send_log(m_oResult);
    }
};

boolean CRhodesAppBase::sendLog( rho::apiGenerator::CMethodResult& oResult ) 
{
    if ( m_bSendingLog )
        return true;
    
    m_bSendingLog = true;
    rho_rhodesapp_call_in_thread( new CRhoSendLogCall(oResult) );
    return true;
}

boolean CRhodesAppBase::sendLogInSameThread() 
{
    if ( m_bSendingLog )
        return true;
    
    m_bSendingLog = true;
    rho::apiGenerator::CMethodResult oResult;
    rho_do_send_log(oResult);
    return true;
}
	
	// Deprecated
	boolean CRhodesAppBase::callPushCallback(const String& strData) const
	{
		synchronized(m_mxPushCallback)
		{
			if ( m_strPushCallback.length() == 0 )
				return false;
			
			String strBody = strData + "&rho_callback=1";
			if ( m_strPushCallbackParams.length() > 0 )
				strBody += "&" + m_strPushCallbackParams;
			
			NetResponse resp = getNetRequest().pushData( m_strPushCallback, strBody, NULL );
			if (!resp.isOK())
				LOG(ERROR) + "Push notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData();
			else
			{
				const char* szData = resp.getCharData();
				return !(szData && strcmp(szData,"rho_push") == 0);
			}
		}
		
		return false;
	}


} //namespace common
} //namespace rho

extern "C" {

#define RHO_ZIP_FORMAT_ZIP      1
#define RHO_ZIP_FORMAT_GZIP     2
#define RHO_ZIP_FORMAT_INVALID  -1

static int rho_get_zip_format(const char* szZipPath)
{
  rho::common::CRhoFile f;
  f.open(szZipPath, rho::common::CRhoFile::OpenReadOnly);
  if ( f.isOpened() )
  {
    unsigned char buf[4];
    f.readData(buf, 0, 4);
    
    if (buf[0]=='P' && buf[1]=='K' && buf[2]==3 && buf[3]==4) {
      return RHO_ZIP_FORMAT_ZIP;
    } else if ( buf[0]==0x1f && buf[1]==0x8b && buf[2]==0x08 ) {
      return RHO_ZIP_FORMAT_GZIP;
    }
  }
  return RHO_ZIP_FORMAT_INVALID;
}

static int rho_internal_unzip_zip(const char* szZipPath, const char* psw);
static int rho_internal_unzip_zipfile_with_minizip(const char* szZipPath, const char* psw);
static int rho_internal_unzip_gzip(const char* szZipPath, const char* outputFilename);
    
//TODO: use System.unzip_file
int rho_sys_unzip_file(const char* szZipPath, const char* psw, const char* outputFilename )
{
  switch( rho_get_zip_format(szZipPath) )
  {
    case RHO_ZIP_FORMAT_ZIP:  
#ifdef OS_ANDROID
        return rho_internal_unzip_zipfile_with_minizip(szZipPath, psw);
#else
        return rho_internal_unzip_zip(szZipPath, psw);
#endif
    case RHO_ZIP_FORMAT_GZIP: 
        return rho_internal_unzip_gzip(szZipPath, outputFilename);
  }

  return -1;
}

static int rho_internal_unzip_zipfile_with_minizip(const char* szZipPath, const char* psw)
{
    //log
    LOG(TRACE) + "Unzip : " + szZipPath;

    rho::common::CFilePath oPath(szZipPath);
    rho::String strBaseDir = oPath.getFolderName();

    //trace
    LOG(TRACE) + "Unzip : " + szZipPath + "; Base dir : " + strBaseDir;

    void *zip_handle = NULL;
    void *stream = NULL;
    int err = MZ_OK;

    //trace
    LOG(TRACE) + "Minizip: creating stream for zip file: " + oPath.getPath();

    stream = mz_stream_os_create();
    if (stream == NULL) {
        LOG(ERROR) + ("Minizip: failed to create OS stream");
        return -1;
    }

    //trace
    LOG(TRACE) + "Minizip: opening stream for zip file: " + oPath.getPath();

    err = mz_stream_open(stream, oPath.getPath(), MZ_OPEN_MODE_READ);
    if (err != MZ_OK) {
        LOG(ERROR) + ("Minizip: failed to open stream");
        mz_stream_os_delete(&stream);
        return -1;
    }

    //trace
    LOG(TRACE) + "Minizip: creating zip handle";

    zip_handle = mz_zip_create();
    if (zip_handle == NULL) {
        LOG(ERROR) + ("Minizip: failed to create zip handle");
        mz_stream_close(stream);
        mz_stream_os_delete(&stream);
        return -1;
    }

    //trace
    LOG(TRACE) + "Minizip: opening zip file: " + oPath.getPath();

    err = mz_zip_open(zip_handle, stream, MZ_OPEN_MODE_READ);
    if (err != MZ_OK) {
        LOG(ERROR) + ("Minizip: failed to open zip file");
        mz_zip_delete(&zip_handle);
        mz_stream_close(stream);
        mz_stream_os_delete(&stream);
        return -1;
    }

    //trace
    LOG(TRACE) + "Minizip: going to first entry in zip file";

    err = mz_zip_goto_first_entry(zip_handle);

    uint32_t buflen = 1024 * 4096;
    std::unique_ptr<char[]> buffer(new char[buflen]); // 4MB buffer

    while (err == MZ_OK) {

        //trace
        LOG(TRACE) + "Minizip: going to next entry in zip file";

        mz_zip_file *file_info = NULL;
        err = mz_zip_entry_get_info(zip_handle, &file_info);
        if (err != MZ_OK) {
            LOG(ERROR) + ("Minizip: failed to get file info");
            break;
        }

        //trace
        LOG(TRACE) + "Minizip: opening zip entry: " + file_info->filename;

        err = mz_zip_entry_read_open(zip_handle,0,psw);
        if (err != MZ_OK) {
            LOG(ERROR) + "Minizip: failed to open zip entry: " + err;
            break;
        }

        //trace
        LOG(TRACE) + "Minizip: reading zip entry: " + file_info->filename;

        //get uncompressed size and crc32
        uint32_t crc32 = file_info->crc;
        int64_t compressed_size = file_info->compressed_size;
        int64_t uncompressed_size = file_info->uncompressed_size;
        
        LOG(INFO) + "Unzip : " + file_info->filename + "; Compressed size : " + compressed_size +
            "; Uncompressed size : " + uncompressed_size + "; CRC32 : " + crc32;

        //create output file path
        
        rho::String strFilePath = oPath.join(strBaseDir,file_info->filename);
        rho::common::CRhoFile oFile;

        //trace
        LOG(TRACE) + "Minizip: creating output file: " + strFilePath;

        if ( !oFile.open(strFilePath.c_str(), rho::common::CRhoFile::OpenForWrite) )
        {
            LOG(ERROR) + "Minizip: failed to open output file: " + strFilePath;
            mz_zip_entry_close(zip_handle);
            break;
        }

        //trace
        LOG(TRACE) + "Minizip: writing to output file: " + strFilePath;
   
        int64_t bytes_to_read = uncompressed_size;
        int64_t bytes_read = 0;

        //trace
        LOG(TRACE) + "Minizip: reading zip entry data: " + file_info->filename;

        while (bytes_to_read > 0) {

            //trace
            LOG(TRACE) + "Minizip: reading zip entry data: " + file_info->filename + "; bytes to read: " + bytes_to_read;

            int64_t bytes_to_read_now = (bytes_to_read > buflen) ? buflen : bytes_to_read;
            err = mz_zip_entry_read(zip_handle, buffer.get(), (int32_t)bytes_to_read_now);
            if (err < 0) {
                LOG(ERROR) + ("Minizip: failed to read zip entry");
                break;
            }

            //trace
            LOG(TRACE) + "Minizip: writing to output file: " + strFilePath + "; bytes read: " + err;

            oFile.write(buffer.get(), (unsigned int)err);
            bytes_read += err;
            bytes_to_read -= err;
        }
        //trace
        LOG(TRACE) + "Minizip: closing output file: " + strFilePath;

        oFile.flush();            

        mz_zip_entry_close(zip_handle);

        err = mz_zip_goto_next_entry(zip_handle);
    }

    //trace
    LOG(TRACE) + "Minizip: closing zip file: " + oPath.getPath();

    mz_zip_close(zip_handle);
    mz_zip_delete(&zip_handle);
    mz_stream_close(stream);
    mz_stream_os_delete(&stream);

    //trace
    LOG(TRACE) + "Minizip: finished unzipping zip file: " + oPath.getPath();

    return err;
}

static int rho_internal_unzip_zip(const char* szZipPath, const char* psw)
{
    rho::common::CFilePath oPath(szZipPath);
    rho::String strBaseDir = oPath.getFolderName();
#if defined(UNICODE) && defined(WIN32) && !defined(OS_WP8) && !defined(OS_UWP)
    rho::StringW strZipPathW;
    rho::common::convertToStringW(szZipPath, strZipPathW);
    HZIP hz = OpenZipFile(strZipPathW.c_str(), psw);
    if ( !hz )
        return -1;
	// Set base for unziping
    SetUnzipBaseDir(hz, rho::common::convertToStringW(strBaseDir).c_str());
#else
    HZIP hz = OpenZipFile((TCHAR*)szZipPath, psw);
    
    if ( !hz )
        return -1;

	// Set base for unziping
    SetUnzipBaseDir(hz,(TCHAR*)strBaseDir.c_str() );
#endif

	ZIPENTRY ze;
    ZRESULT res = 0;
	// Get info about the zip
	// -1 gives overall information about the zipfile
	res = GetZipItem(hz,-1,&ze);
	int numitems = ze.index;

    LOG(INFO) + "Unzip : " + szZipPath + "; Number of items : " + numitems;
	// Iterate through items and unzip them
	for (int zi = 0; zi<numitems; zi++)
	{ 
		// fetch individual details, e.g. the item's name.
		res = GetZipItem(hz,zi,&ze); 
        if ( res == ZR_OK )
        {
    		res = UnzipItem(hz, zi, ze.name);
            if ( res != 0 )
                LOG(ERROR) + "Unzip item failed: " + res + "; " +
				#if defined(OS_WP8) || defined(OS_UWP)
				(char*)
				#endif
				ze.name;
        }
        else
            LOG(ERROR) + "Get unzip item failed: " + res + "; " + zi;
	}

	CloseZip(hz);

    return res;
}

static int rho_internal_unzip_gzip(const char* inputFilename, const char* outputFilename)
{
    return gunzip::UnzipGzip(inputFilename, outputFilename);
}


//TODO_REMOVE : rho_rhodesapp_getplatform
#ifdef RHODES_EMULATOR

const char* rho_rhodesapp_getplatform()
{
    rho::String strPlatform = RHOSIMCONF().getString("platform");

    if ( strPlatform.compare("bb") == 0 || strPlatform.compare("bb6") == 0 )
        return "Blackberry";
    
    if ( strPlatform.compare("wm") == 0 )
        return "WINDOWS";

    if ( strPlatform.compare("win32") == 0 )
        return "WINDOWS_DESKTOP";

    if ( strPlatform.compare("wp") == 0 )
        return "WP7";

    if ( strPlatform.compare("wp8") == 0 )
        return "WP8";
    if ( strPlatform.compare("uwp") == 0 )
        return "UWP";
    if ( strPlatform.compare("android") == 0 )
        return "ANDROID";

    if ( strPlatform.compare("iphone") == 0 )
        return "APPLE";

    return "UNKNOWN";
}    
#else
const char* rho_rhodesapp_getplatform()
{
#if defined(OS_MACOSX)
	return "APPLE";
#elif defined(OS_WINDOWS_DESKTOP)
	return "WINDOWS_DESKTOP";
#elif defined(WINDOWS_PLATFORM)
#if defined(OS_WP8)
	return "WP8";
#elif defined(OS_UWP)
    return "UWP";
#else
	return "WINDOWS";
#endif
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
    
    int rho_conf_send_log(const char* callback_url)
    {
        rho::String s_callback_url = "";
        if (callback_url != NULL) {
            s_callback_url = callback_url;
        }
        rho::apiGenerator::CMethodResult oResult;
        oResult.setRubyCallback(s_callback_url);
        return RHODESAPPBASE().sendLog(oResult);
    }
    
    int rho_conf_send_log_in_same_thread()
    {
        return RHODESAPPBASE().sendLogInSameThread();
    }
	
	void rho_net_request_with_data(const char *url, const char *str_body)
	{
		rho::String strCallbackUrl = RHODESAPPBASE().canonicalizeRhoUrl(url);
		getNetRequest().pushData(strCallbackUrl.c_str(), str_body, NULL);
	}
    
    
    int rho_app_canonicalize_rho_url(const char* url, char* result, int max_len) {
        rho::String s_res = RHODESAPPBASE().canonicalizeRhoUrl(url);
        if (result != NULL) {
            strncpy(result, s_res.c_str(), max_len);
        }
        return (int)s_res.length();
    }

} //extern "C"

#if !defined(OS_ANDROID)
rho::String rho_sysimpl_get_phone_id()
{
    return "";
}
#endif

#if !defined(OS_MACOSX) || defined(RHODES_EMULATOR)
const char* rho_native_rhouserpath()
{
    return rho_native_rhopath();
}

const char* rho_native_rhodbpath()
{
    return rho_native_rhopath();
}


int rho_sys_set_do_not_bakup_attribute(const char* path, int value)
{
    // nothing
    return 1;
}


#endif

extern "C" int tau_decrypt_file(const char* filebuf, int filebuf_len, char* decrypted_buf, int maxlen, const char* key);

extern "C" int rho_decrypt_file(const char* filebuf, int filebuf_len, char* decrypted_buf, int maxlen ) {
    // execute function from extension "decryptstub"(empty implementstion) or "decrypt"(used openssl - binary from openssl.so)
#if (defined(OS_ANDROID) || defined(OS_MACOSX)) && !defined(RHODES_EMULATOR)
    const char* key = get_app_build_config_item("encrypt_files_key");
    return tau_decrypt_file(filebuf, filebuf_len, decrypted_buf, maxlen, key);
#else
    //memcpy(decrypted_buf, filebuf, filebuf_len);
    return filebuf_len;
#endif
}
