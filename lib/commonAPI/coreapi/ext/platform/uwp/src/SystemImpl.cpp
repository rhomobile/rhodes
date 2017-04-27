#include "../../../shared/SystemImplBase.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"

#include <windows.h>
#include "rhoruntime/rhoruntime.h"
#include "rhoruntime/common/RhodesHelperWP8.h"
#include <windows.system.profile.h>
#include <windows.system.h>
#include <windows.storage.streams.h>

using namespace Windows::Graphics::Display;
using namespace Windows::Devices::Input;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation::Collections;
using namespace Windows::System::Profile;
using namespace Windows::System;



Platform::String^ A2PS(char* str)
{
	std::string s_str = std::string(str);
	std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
	const wchar_t* w_char = wid_str.c_str();
	return ref new Platform::String(w_char);
}

namespace rho {

using namespace apiGenerator;

class CSystemImpl: public CSystemImplBase
{
public:
    CSystemImpl(): CSystemImplBase(){}

    virtual void getScreenWidth(CMethodResult& oResult);
    virtual void getScreenHeight(CMethodResult& oResult);
    virtual void getScreenOrientation(CMethodResult& oResult);
    virtual void getPpiX(CMethodResult& oResult);
    virtual void getPpiY(CMethodResult& oResult);
    virtual void getPhoneId(CMethodResult& oResult);
    virtual void getDeviceName(CMethodResult& oResult);
    virtual void getOsVersion(CMethodResult& oResult);
    virtual void getLocale(CMethodResult& oResult);
    virtual void getCountry(CMethodResult& oResult);
    virtual void getIsEmulator(CMethodResult& oResult);
    virtual void getHasCalendar(CMethodResult& oResult);
    virtual void getOemInfo(CMethodResult& oResult);
    virtual void getUuid(CMethodResult& oResult);
    virtual void getHttpProxyURI(CMethodResult& oResult);
    virtual void setHttpProxyURI( const rho::String& value, CMethodResult& oResult);
    virtual void getLockWindowSize(CMethodResult& oResult);
    virtual void setLockWindowSize( bool value, CMethodResult& oResult);
    virtual void getKeyboardState(CMethodResult& oResult);
    virtual void setKeyboardState( const rho::String& value, CMethodResult& oResult);
    virtual void getFullScreen(CMethodResult& oResult);
    virtual void setFullScreen( bool value, CMethodResult& oResult);
    virtual void getScreenAutoRotate(CMethodResult& oResult);
    virtual void setScreenAutoRotate( bool value, CMethodResult& oResult);
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult);
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& oResult);
    virtual void setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult);
	virtual void getShowKeyboard(rho::apiGenerator::CMethodResult& oResult);
    virtual void setShowKeyboard( bool showKeyboard, rho::apiGenerator::CMethodResult& oResult);
	virtual void setRegistrySetting( int hive,  int type,  const rho::String& subKey,  const rho::String& setting,  const rho::String& value, rho::apiGenerator::CMethodResult& oResult);
    virtual void getRegistrySetting( int hive,  const rho::String& subKey,  const rho::String& setting, rho::apiGenerator::CMethodResult& oResult);

    virtual void applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult);
    virtual void isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult);
    virtual void applicationUninstall( const rho::String& applicationName, CMethodResult& oResult);
    virtual void openUrl( const rho::String& url, CMethodResult& oResult);
    virtual void setRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void getRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void deleteRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult);
    virtual void setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult);
    virtual void setWindowPosition( int x,  int y, CMethodResult& oResult);
    virtual void setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult);
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& oResult);
    virtual void bringToFront(rho::apiGenerator::CMethodResult& oResult);
    virtual void runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult);

    virtual void set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult);
    virtual void unset_http_proxy(rho::apiGenerator::CMethodResult& oResult);
};

void CSystemImpl::getOsVersion(CMethodResult& oResult)
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (!GetVersionEx(&osv))
    {
        LOG(ERROR) + "GetVersionEx failed.";
		oResult.set(L"0");
    }
    else
    {
	    wchar_t buf[50];
        memset(buf, 0, sizeof(buf));
	    _snwprintf(buf, sizeof(buf) - 1, L"%u.%u.%u", (unsigned)osv.dwMajorVersion,
		    (unsigned)osv.dwMinorVersion, (unsigned)osv.dwBuildNumber);

        oResult.set(StringW(buf));
    }
}

void CSystemImpl::getShowKeyboard(rho::apiGenerator::CMethodResult& oResult)
{
}

void CSystemImpl::setShowKeyboard( bool showKeyboard, rho::apiGenerator::CMethodResult& oResult)
{
}

void CSystemImpl::setRegistrySetting( int hive,  int type,  const rho::String& subKey,  const rho::String& setting,  const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{
}

void CSystemImpl::getRegistrySetting( int hive,  const rho::String& subKey,  const rho::String& setting, rho::apiGenerator::CMethodResult& oResult)
{
}

void CSystemImpl::getIsEmulator(CMethodResult& oResult)
{
	oResult.set(rhoruntime::CRhoRuntime::getInstance()->getMainPage()->isEmulator());
}

void CSystemImpl::getScreenWidth(CMethodResult& oResult)
{
	oResult.set(rhoruntime::CRhoRuntime::getInstance()->getMainPage()->getScreenWidth());
}

void CSystemImpl::getScreenHeight(CMethodResult& oResult)
{
	oResult.set(rhoruntime::CRhoRuntime::getInstance()->getMainPage()->getScreenHeight());
}

void CSystemImpl::getScreenOrientation(CMethodResult& oResult)
{
	oResult.set(rhoruntime::CRhoRuntime::getInstance()->getMainPage()->getScreenOrientation()->Data());
}

void CSystemImpl::getPpiX(CMethodResult& oResult)
{
	oResult.set(rhoruntime::CRhoRuntime::getInstance()->getMainPage()->getLogicalDpiX());
}

void CSystemImpl::getPpiY(CMethodResult& oResult)
{
	oResult.set(rhoruntime::CRhoRuntime::getInstance()->getMainPage()->getLogicalDpiY());
}

void CSystemImpl::getPhoneId(CMethodResult& oResult)
{
	try {
		Windows::System::Profile::HardwareToken ^ token = Windows::System::Profile::HardwareIdentification::GetPackageSpecificToken(nullptr);
		Windows::Storage::Streams::IBuffer ^ hardwareId = token->Id;
		Windows::Storage::Streams::DataReader ^ dataReader = Windows::Storage::Streams::DataReader::FromBuffer(hardwareId);
	
		Platform::String ^ platformStr = dataReader->ReadString(hardwareId->Length);
		std::wstring fooW(platformStr->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		oResult.set(fooA);
	}
	catch(std::exception e){

	}
}

void CSystemImpl::getDeviceName(CMethodResult& oResult)
{
	oResult.set(Windows::Networking::Proximity::PeerFinder::DisplayName->Data());
}

void CSystemImpl::getLocale(CMethodResult& oResult)
{
	wchar_t szLang[20];
	int nRes = GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,LOCALE_SABBREVLANGNAME , szLang, sizeof(szLang)/sizeof(szLang[0]));
	szLang[2] = 0;
	wcslwr(szLang);

	oResult.set(StringW(szLang));
}
void CSystemImpl::getCountry(CMethodResult& oResult)
{
	wchar_t szCountry[20];
	int nRes = GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT,LOCALE_SISO3166CTRYNAME , szCountry, sizeof(szCountry)/sizeof(szCountry[0]));
	szCountry[2] = 0;

	oResult.set(StringW(szCountry));
}

void CSystemImpl::getHasCalendar(CMethodResult& oResult)
{
	oResult.set(true);
}

void CSystemImpl::getOemInfo(CMethodResult& oResult)
{

}

void CSystemImpl::getUuid(CMethodResult& oResult)
{
	getPhoneId(oResult);
	//oResult.set(Windows::Phone::System::Analytics::HostInformation::PublisherHostId->Data());
}

void CSystemImpl::getLockWindowSize(CMethodResult& oResult){}
void CSystemImpl::setLockWindowSize( bool value, CMethodResult& oResult){}
void CSystemImpl::getKeyboardState(CMethodResult& oResult)
{
	//seems it's unsupported
}
void CSystemImpl::setKeyboardState( const rho::String& value, CMethodResult& oResult)
{
	//seems it's unsupported
	//to show SIP on WP8 we firstly have to focus text box element
}
void CSystemImpl::getFullScreen(CMethodResult& oResult)
{
	//all apps working in full screen mode on WP8
	oResult.set(true);
}
void CSystemImpl::setFullScreen( bool value, CMethodResult& oResult)
{
	//all apps working in full screen mode on WP8
}

void CSystemImpl::getScreenAutoRotate(CMethodResult& oResult)
{
	//not supported in XAML apps
	/*if(DisplayProperties::AutoRotationPreferences == Windows::Graphics::Display::DisplayOrientations::None)
		oResult.set(false);
	else */
	oResult.set(true);
}

void CSystemImpl::setScreenAutoRotate( bool value, CMethodResult& oResult)
{
	if(value)
		DisplayProperties::AutoRotationPreferences = Windows::Graphics::Display::DisplayOrientations::Portrait |
													 Windows::Graphics::Display::DisplayOrientations::PortraitFlipped;
	else
		DisplayProperties::AutoRotationPreferences = Windows::Graphics::Display::DisplayOrientations::None;
}

void CSystemImpl::applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult)
{
}

void CSystemImpl::isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult)
{
	/*
	IIterable<Package^>^ packages = InstallationManager::FindPackagesForCurrentPublisher();
	IIterator<Package^>^ it = packages->First();
	do
    {
		if (it->Current->Id->Name == A2PS(const_cast<char*>(applicationName.c_str())))
        {
            oResult.set(true);
			return;
        
        }
	}while(it->MoveNext());

	oResult.set(false);

	Not supported
	*/
}

void CSystemImpl::applicationUninstall( const rho::String& applicationName, CMethodResult& oResult)
{

}

void CSystemImpl::openUrl( const rho::String& url, CMethodResult& oResult)
{
	auto uri = ref new Windows::Foundation::Uri(A2PS(const_cast<char*>(url.c_str())));
	Windows::System::Launcher::LaunchUriAsync(uri);
}

void CSystemImpl::runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, CMethodResult& oResult)
{
	/*
	IIterable<Package^>^ packages = InstallationManager::FindPackagesForCurrentPublisher();
	IIterator<Package^>^ it = packages->First();
	do
    {
		if (it->Current->Id->Name == A2PS(const_cast<char*>(appName.c_str())))
        {
            CMethodResult res;
			it->Current->Launch(A2PS(const_cast<char*>(params.c_str())));
			return;
        
        }
	}while(it->MoveNext());

	Not supported
	
	*/
}

void CSystemImpl::setRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
	//unsupported
}

void CSystemImpl::getRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
	//unsupported
}

void CSystemImpl::deleteRegistrySetting( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult)
{
	//unsupported
}

void CSystemImpl::setWindowFrame(int x,  int y,  int width,  int height, CMethodResult& oResult)
{
	//seems it doesn't make sence on WP8
}
void CSystemImpl::setWindowPosition( int x,  int y, CMethodResult& oResult)
{
	//seems it doesn't make sence on WP8
}

void CSystemImpl::getWebviewFramework(rho::apiGenerator::CMethodResult& oResult)
{
	oResult.set(rhoruntime::CRhoRuntime::getInstance()->getMainPage()->getWebviewFramework()->Data());
}

void CSystemImpl::bringToFront(rho::apiGenerator::CMethodResult& oResult)
{
	rhoruntime::CRhoRuntime::getInstance()->getMainPage()->bringToFront();
}

extern "C" const char* rho_sys_get_http_proxy_url();
extern "C" void rho_sys_set_http_proxy_url(const char* url);
extern "C" void rho_sys_unset_http_proxy();
void CSystemImpl::getHttpProxyURI(CMethodResult& oResult)
{
	oResult.set(rho_sys_get_http_proxy_url());
}

void CSystemImpl::setHttpProxyURI( const rho::String& value, CMethodResult& oResult)
{
	if ( value.length() )
        rho_sys_set_http_proxy_url( value.c_str() );
    else
        rho_sys_unset_http_proxy();
}

void CSystemImpl::getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult)
{
	oResult.set(true);
}

void CSystemImpl::getScreenSleeping(rho::apiGenerator::CMethodResult& oResult)
{
	//Windows::System::Display::DisplayRequest().RequestActive;
	//Windows::System::Profile::AnalyticsInfo info
	//
	//oResult.set(Windows::Phone::System::SystemProtection::ScreenLocked);
}

void CSystemImpl::setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult)
{
	//http://msdn.microsoft.com/en-us/library/windowsphone/develop/jj206968(v=vs.105).aspx


}

void CSystemImpl::setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult)
{
}

void CSystemImpl::set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult)
{
	rho_sys_set_http_proxy_url( proxyURI.c_str() );
}

void CSystemImpl::unset_http_proxy(rho::apiGenerator::CMethodResult& oResult)
{
	rho_sys_unset_http_proxy();
}

////////////////////////////////////////////////////////////////////////
class CSystemSingleton: public CModuleSingletonBase<ISystemSingleton>
{
public:
    ~CSystemSingleton(){}
    rho::StringW getInitialDefaultID(){return L"1";}
	rho::StringW getDefaultID(){return L"1";}
	void setDefaultID(const rho::StringW& strI){}
};

class CSystemFactory: public CSystemFactoryBase
{
public:
    ~CSystemFactory(){}

    ISystemSingleton* createModuleSingleton(){ return new CSystemImpl(); }
};

extern "C" void Init_System()
{
    CSystemFactory::setInstance( new CSystemFactory() );
    Init_System_API();
}

}
