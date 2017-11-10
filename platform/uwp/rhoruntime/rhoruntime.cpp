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

#include "pch_rhoruntime.h"
#include "rhoruntime.h"

//#include "../../shared/sqlite/sqlite3.h"
//#include "logging/RhoLogConf.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"
#include "common/RhoConvertWP8.h"
#include "common/RhodesHelperWP8.h"
#include "api_generator\js_helpers.h"
#include <collection.h>

using namespace rhoruntime;
using namespace Platform;
using namespace Windows::Foundation::Collections;

CRhoRuntime^ CRhoRuntime::m_instance = nullptr;

CRhoRuntime::CRhoRuntime(IMainPage^ mainPage, ::Platform::String^ storagePath):
	m_MainPage(mainPage)
{
	localRhoPath = "rho";
	setLocalStoragePath(storagePath);
}

CRhoRuntime^ CRhoRuntime::getInstance(IMainPage^ mainPage, ::Platform::String^ storagePath)
{
	if (m_instance == nullptr)
		m_instance = ref new CRhoRuntime(mainPage, storagePath);
    return m_instance;
}

CRhoRuntime^ CRhoRuntime::getInstance()
{
    return m_instance;
}

IMainPage^ CRhoRuntime::getMainPage()
{
	return m_MainPage;
}

// rhodes executed in a separate thread
void CRhoRuntime::Execute()
{
	rho::String m_strRootPath = rho_native_rhopath(), m_logPort, m_strRuntimePath;
    rho_logconf_Init(m_strRootPath.c_str(), m_strRootPath.c_str(), m_logPort.c_str());

    rho::common::CRhodesApp::Create(m_strRootPath, m_strRootPath, m_strRuntimePath);
    //RHODESAPP().setExtManager( &m_oExtManager );

	//Create Main window
	//createCustomMenu();

	RHODESAPP().startApp();
}


// *** CALLBACKS from MainPage object ***

void CRhoRuntime::updateSizeProperties(int width, int height)
{
	// TODO: implement
}

void CRhoRuntime::onActivate(int active)
{
	while(!RHODESAPP().getInstance())
		Sleep(50);

	RHODESAPP().callUiCreatedCallback();
	rho_rhodesapp_callAppActiveCallback(1);
}
void CRhoRuntime::setLocalStoragePath(::Platform::String^ storage) {
	localStorage = rho::common::convertStringWFromWP8(storage);
	localNullFile = rho::common::convertStringWFromWP8(storage + "\\nullfile");
	localRhoPath = rho::common::convertStringAFromWP8(storage + "\\rho\\");
	setLocalStorage(localStorage.c_str());
	setLocalNullFile(localNullFile.c_str());
	setLocalRhoFolder(localRhoPath.c_str());
}

void CRhoRuntime::setOsVersion(int major, int minor, int build) {
	cSetOSVersion(major, minor, build);
}


void CRhoRuntime::logEvent(::Platform::String^ message)
{
	LOG(INFO) + rho::common::convertStringWFromWP8(message);
}

void CRhoRuntime::createCustomMenu()
{
	IMainPage^ mainPage = CRhoRuntime::getInstance()->getMainPage();
	rho::Vector<rho::common::CAppMenuItem> m_arAppMenuItems;
    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);

    //update UI with custom menu items
	mainPage->menuClear();
	//RHODESAPP().setAppBackUrl("");
    for ( unsigned int i = 0; i < m_arAppMenuItems.size(); i++)
    {
        rho::common::CAppMenuItem& oItem = m_arAppMenuItems.elementAt(i);
        if (oItem.m_eType == rho::common::CAppMenuItem::emtSeparator) 
            mainPage->menuAddSeparator();
        else {
			//if ((oItem.m_eType == rho::common::CAppMenuItem::emtBack) && (oItem.m_strLink.compare("back") != 0))
			//	RHODESAPP().setAppBackUrl(oItem.m_strLink);
			mainPage->menuAddAction((oItem.m_eType == rho::common::CAppMenuItem::emtClose ? "Exit" : rho::common::convertStringToWP8(oItem.m_strLabel)), i);
        }
    }
}

void CRhoRuntime::onCustomMenuItemCommand(int nItemPos)
{
	rho::Vector<rho::common::CAppMenuItem> m_arAppMenuItems;
    RHODESAPP().getAppMenu().copyMenuItems(m_arAppMenuItems);

	if ( nItemPos < 0 || nItemPos >= (int)m_arAppMenuItems.size() )
        return;

    rho::common::CAppMenuItem& oMenuItem = m_arAppMenuItems.elementAt(nItemPos);
    oMenuItem.processCommand();
}

void CRhoRuntime::onToolbarAction(::Platform::String^ action)
{
	if ( action->Equals("forward") )
		rho_webview_navigate_forward();
	else
		RHODESAPP().loadUrl(rho::common::convertStringAFromWP8(action));
}

void CRhoRuntime::onTabbarCurrentChanged(int index, ::Platform::String^ action)
{
	// TODO: complete implementation

	//if (tbrp["on_change_tab_callback"].toString().length() > 0) {
	//	QString body = QString("&rho_callback=1&tab_index=") + QVariant(index).toString();
	//	rho::String* cbStr = new rho::String(tbrp["on_change_tab_callback"].toString().toStdString());
	//	rho::String* bStr = new rho::String(body.toStdString());
	//	const char* b = bStr->c_str();
	//	rho_net_request_with_data(RHODESAPP().canonicalizeRhoUrl(*cbStr).c_str(), b);
	//}

    //if (tbrp["reload"].toBool() || (ui->webView && (ui->webView->history()->count()==0))) {
	RHODESAPP().loadUrl(rho::common::convertStringAFromWP8(action), index);
    //}
}


void CRhoRuntime::onWindowClose(void)
{
	// TODO: implement
}

void CRhoRuntime::onWebViewUrlChanged(::Platform::String^ url)
{
	RHODESAPP().keepLastVisitedUrl(rho::common::convertStringAFromWP8(url));
}

bool CRhoRuntime::onBackKeyPress()
{
	// TODO: implement check for history size (return false if there's no history)
	RHODESAPP().navigateBack();
	return true;
}

::Platform::String^ CRhoRuntime::onJSInvoke(::Platform::String^ inJSON)
{
	rho::String ret = rho::apiGenerator::js_entry_point(rho::common::convertToStringA(inJSON->Data()).c_str());
	return rho::common::convertStringToWP8(ret);
}

void CRhoRuntime::executeRhoRunnable(int64 native)
{
	((rho::common::IRhoRunnable*)native)->runObject();
}

// *** PUBLIC METHODS ***

bool CRhoRuntime::Initialize(::Platform::String^ title)
{
	return true;
}

void CRhoRuntime::DestroyUi(void)
{
	rho_rhodesapp_callUiDestroyedCallback();
	rho_rhodesapp_callAppActiveCallback(0);
}

void CRhoRuntime::setCryptoEngine(ICryptoEngine^ cryptoEngine)
{
	m_CryptoEngine = cryptoEngine;
}

// *** proxy methods needed for C# implementation of CoreAPI ***

::Platform::String^ CRhoRuntime::canonicalizeRhoUrl(::Platform::String^ url)
{
	rho::String fullUrl = RHODESAPPBASE().canonicalizeRhoUrl(rho::common::convertStringAFromWP8(url));
	return rho::common::convertStringToWP8(fullUrl);
}

::Platform::String^ CRhoRuntime::getRootPath(::Platform::String^ path)
{
	return rho::common::convertStringToWP8( rho::common::CFilePath::join( RHODESAPP().getRhoRootPath(), rho::common::convertStringAFromWP8(path) ) );
}

::Platform::String^ CRhoRuntime::getAppRootPath(::Platform::String^ path)
{
	return rho::common::convertStringToWP8( rho::common::CFilePath::join( RHODESAPP().getAppRootPath(), rho::common::convertStringAFromWP8(path) ) );
}

::Platform::String^ CRhoRuntime::getRERuntimePath(::Platform::String^ path)
{
	return rho::common::convertStringToWP8( rho::common::CFilePath::join( rho_native_reruntimepath(), rho::common::convertStringAFromWP8(path)) );
}

// JSON parsing

CJSONEntryProxy::CJSONEntryProxy(::Platform::String^ data)
{
	m_Entry = new rho::json::CJSONEntry(rho::common::convertStringAFromWP8(data).c_str());
}

bool CJSONEntryProxy::isEmpty()
{
	return m_Entry->isEmpty();
}

bool CJSONEntryProxy::hasName(::Platform::String^ name)
{
	return m_Entry->hasName(rho::common::convertStringAFromWP8(name));
}

bool CJSONEntryProxy::isString()
{
	return m_Entry->isString();
}

bool CJSONEntryProxy::isArray()
{
	return m_Entry->isArray();
}

bool CJSONEntryProxy::isObject()
{
	return m_Entry->isObject();
}

bool CJSONEntryProxy::isNull()
{
	return m_Entry->isNull();
}

bool CJSONEntryProxy::isInteger()
{
	return m_Entry->isInteger();
}

bool CJSONEntryProxy::isFloat()
{
	return m_Entry->isFloat();
}

bool CJSONEntryProxy::isBoolean()
{
	return m_Entry->isBoolean();
}

int CJSONEntryProxy::getInt(::Platform::String^ name)
{
	return m_Entry->getInt(rho::common::convertStringAFromWP8(name).c_str());
}

uint64 CJSONEntryProxy::getUInt64(::Platform::String^ name)
{
	return m_Entry->getUInt64(rho::common::convertStringAFromWP8(name).c_str());
}

double CJSONEntryProxy::getDouble(::Platform::String^ name)
{
	return m_Entry->getDouble(rho::common::convertStringAFromWP8(name).c_str());
}

int CJSONEntryProxy::getInt()
{
	return m_Entry->getInt();
}

uint64 CJSONEntryProxy::getUInt64()
{
	return m_Entry->getUInt64();
}

double CJSONEntryProxy::getDouble()
{
	return m_Entry->getDouble();
}

bool CJSONEntryProxy::getBoolean()
{
	return m_Entry->getBoolean();
}

::Platform::String^ CJSONEntryProxy::getString(::Platform::String^ name)
{
	return rho::common::convertStringCToWP8(m_Entry->getString(rho::common::convertStringAFromWP8(name).c_str()));
}

::Platform::String^ CJSONEntryProxy::getString(::Platform::String^ name, ::Platform::String^ szDefValue)
{
	return rho::common::convertStringCToWP8(m_Entry->getString(rho::common::convertStringAFromWP8(name).c_str(), rho::common::convertStringAFromWP8(szDefValue).c_str()));
}

::Platform::String^ CJSONEntryProxy::getString()
{
	return rho::common::convertStringCToWP8(m_Entry->getString());
}


extern "C" void createMenu()
{
	CRhoRuntime::getInstance()->createCustomMenu();
}

// old API functions

extern "C" void rho_sys_app_exit()
{
	CRhoRuntime::getInstance()->getMainPage()->exitCommand();
	rho_rhodesapp_callUiDestroyedCallback();

	//TODO: fix issue with stop thread - stopWait does not wakeup sleeping thread
	//rho::common::CRhodesApp::Destroy();

	
}

#ifdef OS_UWP
extern "C" void rho_net_impl_network_indicator(int active)
{
	//TODO: rho_net_ping_network
    //RAWLOGC_INFO("rhoruntime.cpp", "rho_net_impl_network_indicator() has no implementation on WP8.");
}

extern "C" int rho_net_ping_network(const char* szHost)
{
    //TODO: rho_net_ping_network
    //RAWLOGC_INFO("rhoruntime.cpp", "rho_net_ping_network() has no implementation on WP8.");
    return 1;
}
#endif

extern "C" void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask)
{
	//CRhoRuntime::getInstance()->getMainPage()->performOnUiThread((int64)pTask);
	pTask->runObject();
}

extern "C" void rho_title_change(const int tabIndex, const char* strTitle)
{
}

extern "C" const char* rho_native_rhopath()
{
	return getLocalRhoFolder(); //rho/apps?
}


void rho_webview_navigate_back_with_tab(int index)
{
	CRhoRuntime::getInstance()->getMainPage()->GoBack(index);
}

extern "C" const char* rho_native_get_appname()
{
    return "Rhodes";
}

RHO_GLOBAL const char* rho_native_reruntimepath()
{
    return rho_native_rhopath();
}

/// <summary>Automatically closes a search handle upon destruction</summary>
class SearchHandleScope {
 
  /// <summary>Initializes a new search handle closer</summary>
  /// <param name="searchHandle">Search handle that will be closed on destruction</param>
  public: SearchHandleScope(HANDLE searchHandle) :
    searchHandle(searchHandle) {}
 
  /// <summary>Closes the search handle</summary>
  public: ~SearchHandleScope() {
    ::FindClose(this->searchHandle);
  }
 
  /// <summary>Search handle that will be closed when the instance is destroyed</summary>
  private: HANDLE searchHandle;
 
};
 
/// <summary>Recursively deletes the specified directory and all its contents</summary>
/// <param name="path">Absolute path of the directory that will be deleted</param>
/// <remarks>
///   The path must not be terminated with a path separator.
/// </remarks>
extern "C" void recursiveDeleteDirectory(const std::wstring &path) {
  static const std::wstring allFilesMask(L"\\*");
 
  WIN32_FIND_DATAW findData;
 
  // First, delete the contents of the directory, recursively for subdirectories
  std::wstring searchMask = path + allFilesMask;
  HANDLE searchHandle = ::FindFirstFileExW(
    searchMask.c_str(), FindExInfoBasic, &findData, FindExSearchNameMatch, nullptr, 0
  );
  if(searchHandle == INVALID_HANDLE_VALUE) {
    DWORD lastError = ::GetLastError();
    if(lastError != ERROR_FILE_NOT_FOUND) { // or ERROR_NO_MORE_FILES, ERROR_NOT_FOUND?
    //  throw std::runtime_error("Could not start directory enumeration");
		return;
    }
  }
 
  // Did this directory have any contents? If so, delete them first
  if(searchHandle != INVALID_HANDLE_VALUE) {
    SearchHandleScope scope(searchHandle);
    for(;;) {
 
      // Do not process the obligatory '.' and '..' directories
      if(findData.cFileName[0] != '.') {
        bool isDirectory = 
          ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
          ((findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);
 
        // Subdirectories need to be handled by deleting their contents first
        std::wstring filePath = path + L'/' + findData.cFileName;
        if(isDirectory) {
          recursiveDeleteDirectory(filePath);
        } else {
          BOOL result = ::DeleteFileW(filePath.c_str());
          if(result == FALSE) {
            //throw std::runtime_error("Could not delete file");
			  return;
          }
        }
      }
 
      // Advance to the next file in the directory
      BOOL result = ::FindNextFileW(searchHandle, &findData);
      if(result == FALSE) {
        DWORD lastError = ::GetLastError();
        if(lastError != ERROR_NO_MORE_FILES) {
        //  throw std::runtime_error("Error enumerating directory");
			return;
        }
        break; // All directory contents enumerated and deleted
      }
 
    } // for
  }
 
  // The directory is empty, we can now safely remove it
  BOOL result = ::RemoveDirectory(path.c_str());
  if(result == FALSE) {
    //throw std::runtime_error("Could not remove directory");
	  return;
  }
}