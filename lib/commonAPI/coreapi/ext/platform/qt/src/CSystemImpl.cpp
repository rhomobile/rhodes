#include "../../../shared/SystemImplBase.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "../../platform/shared/qt/rhodes/impl/MainWindowImpl.h"
#include "../../platform/shared/qt/rhodes/RhoSimulator.h"

#undef null

// commented for OSX build, but headers seems to unused
//#include <qwebkitglobal.h>
//#if QT_VERSION >= 0x050000
//#include <QtWebKit/qtwebkitversion.h>
//#endif
//#include <qtwebkitversion.h>
//#include <qwebkitglobal.h>

#include <QLocale>
#include <QDesktopServices>
#include <QUrl>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"



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
    //virtual void getKeyboardState(CMethodResult& oResult);
    //virtual void setKeyboardState( const rho::String& value, CMethodResult& oResult);
    virtual void getFullScreen(CMethodResult& oResult);
    virtual void setFullScreen( bool value, CMethodResult& oResult);
    virtual void getScreenAutoRotate(CMethodResult& oResult);
    virtual void setScreenAutoRotate( bool value, CMethodResult& oResult);
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult);
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& oResult);
    virtual void setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult);

    virtual void applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult);
    virtual void isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult);
    virtual void applicationUninstall( const rho::String& applicationName, CMethodResult& oResult);
    virtual void openUrl( const rho::String& url, CMethodResult& oResult);
    //virtual void setRegistrySetting( int hive,  int type,  const rho::String& subKey,  const rho::String& setting,  const rho::String& value, rho::apiGenerator::CMethodResult& oResult);
    //virtual void getRegistrySetting( int hive,  const rho::String& subKey,  const rho::String& setting, rho::apiGenerator::CMethodResult& oResult);
    virtual void setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult);
    virtual void setWindowPosition( int x,  int y, CMethodResult& oResult);
    virtual void setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult);
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& oResult);
    virtual void bringToFront(rho::apiGenerator::CMethodResult& oResult);
    virtual void runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult);
    virtual void getMain_window_closed(rho::apiGenerator::CMethodResult& oResult);

    virtual void set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult);
    virtual void unset_http_proxy(rho::apiGenerator::CMethodResult& oResult);
};

void CSystemImpl::getOsVersion(CMethodResult& oResult)
{
    #ifndef OS_LINUX
    oResult.set(String(RHOSIMULATOR_NAME " v" RHOSIMULATOR_VERSION));
    #endif
}

void CSystemImpl::getIsEmulator(CMethodResult& oResult)
{
    oResult.set(true);
}

void CSystemImpl::getScreenWidth(CMethodResult& oResult)
{
    oResult.set(CMainWindow::getScreenWidth());
}

void CSystemImpl::getScreenHeight(CMethodResult& oResult)
{
    oResult.set(CMainWindow::getScreenHeight());
}

void CSystemImpl::getScreenOrientation(CMethodResult& oResult)
{
    oResult.set(StringW(CMainWindow::getScreenWidth() <= CMainWindow::getScreenHeight() ? L"portrait" : L"landscape"));
}

void CSystemImpl::getPpiX(CMethodResult& oResult)
{
    oResult.set(CMainWindow::getInstance()->getLogicalDpiX());
}

void CSystemImpl::getPpiY(CMethodResult& oResult)
{
    oResult.set(CMainWindow::getInstance()->getLogicalDpiY());
}

void CSystemImpl::getPhoneId(CMethodResult& oResult)
{
    //oResult.set(...);
    getOsVersion(oResult);
}

void CSystemImpl::getDeviceName(CMethodResult& oResult)
{
#ifdef RHODES_EMULATOR
    oResult.set(String("RhoSimulator"));
#else
    oResult.set(String("Qt"));
#endif
}

void CSystemImpl::getLocale(CMethodResult& oResult)
{
    oResult.set(String(QLocale::system().name().left(2).toStdString().c_str()));
}

void CSystemImpl::getCountry(CMethodResult& oResult)
{
    oResult.set(String(QLocale::system().name().right(2).toStdString().c_str()));
}

void CSystemImpl::getHasCalendar(CMethodResult& oResult)
{
	oResult.set(true);
}

void CSystemImpl::getOemInfo(CMethodResult& oResult)
{
    //oResult.set(...);
}

void CSystemImpl::getUuid(CMethodResult& oResult)
{
    //oResult.set(...);
}

void CSystemImpl::getLockWindowSize(CMethodResult& oResult){}
void CSystemImpl::setLockWindowSize( bool value, CMethodResult& oResult){}
//void CSystemImpl::getKeyboardState(CMethodResult& oResult){}
//void CSystemImpl::setKeyboardState( const rho::String& value, CMethodResult& oResult){}
void CSystemImpl::getFullScreen(CMethodResult& oResult)
{
	//all apps working in full screen mode on WP8
    oResult.set(CMainWindow::getInstance()->getFullScreen());
}
void CSystemImpl::setFullScreen(bool value, CMethodResult& oResult)
{
    CMainWindow::getInstance()->fullscreenCommand(value);
}

void CSystemImpl::getScreenAutoRotate(CMethodResult& oResult)
{
    oResult.set(false);
}

void CSystemImpl::setScreenAutoRotate( bool value, CMethodResult& oResult)
{
    // TODO: impolement auto rotate
}

void CSystemImpl::applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult)
{
}

void CSystemImpl::isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult)
{
	oResult.set(false);
}

void CSystemImpl::applicationUninstall( const rho::String& applicationName, CMethodResult& oResult)
{
}

void CSystemImpl::openUrl( const rho::String& url, CMethodResult& oResult)
{
    QString sUrl = QString::fromStdString(url);
    if (sUrl.startsWith("/"))
        sUrl.prepend("file://");
    QDesktopServices::openUrl(QUrl(sUrl));
}

void CSystemImpl::getMain_window_closed(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(CMainWindow::mainWindowClosed);
}

void CSystemImpl::runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, CMethodResult& oResult)
{
    //unsupported
}

/* use default implementation from SystemImplBase.cpp
void CSystemImpl::setRegistrySetting( int hive,  int type,  const rho::String& subKey,  const rho::String& setting,  const rho::String& value, rho::apiGenerator::CMethodResult& oResult)
{
	//unsupported
}

void CSystemImpl::getRegistrySetting( int hive,  const rho::String& subKey,  const rho::String& setting, rho::apiGenerator::CMethodResult& oResult)
{
	//unsupported
}
*/

void CSystemImpl::setWindowFrame(int x,  int y,  int width,  int height, CMethodResult& oResult)
{
    CMainWindow::getInstance()->setFrame(x, y, width, height);
}
void CSystemImpl::setWindowPosition( int x,  int y, CMethodResult& oResult)
{
    CMainWindow::getInstance()->setPosition(x, y);
}

extern "C" const char* rho_sys_qt_getWebviewFramework();
void CSystemImpl::getWebviewFramework(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(String(rho_sys_qt_getWebviewFramework()));
}

void CSystemImpl::bringToFront(rho::apiGenerator::CMethodResult& oResult)
{
    CMainWindow::getInstance()->bringToFront();
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
    oResult.set(false);
}

void CSystemImpl::setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult)
{
    //unsupported
}

void CSystemImpl::setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult)
{
    CMainWindow::getInstance()->setSize(width, height);
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

    RHODESAPP().getExtManager().requireRubyFile("RhoSystemApi");
}

}
