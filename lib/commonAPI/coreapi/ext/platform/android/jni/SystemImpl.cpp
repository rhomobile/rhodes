#include "SystemImplBase.h"
#include "rhodes/JNIRhodes.h"


RHO_GLOBAL int rho_sysimpl_get_property(const char* szPropName, rho::apiGenerator::CMethodResult& result);
RHO_GLOBAL int rho_sys_set_sleeping(int sleeping);
RHO_GLOBAL void rho_sys_is_app_installed(const rho::String& appname, rho::apiGenerator::CMethodResult& result);
RHO_GLOBAL void rho_sys_app_install(const rho::String& url, rho::apiGenerator::CMethodResult& result);
RHO_GLOBAL void rho_sys_app_uninstall(const rho::String& appname, rho::apiGenerator::CMethodResult& result);
RHO_GLOBAL void rho_sys_run_app(const rho::String& appname, const rho::String& params, rho::apiGenerator::CMethodResult& result);
RHO_GLOBAL void rho_sys_bring_to_front();
RHO_GLOBAL void rho_sys_set_full_screen_mode(bool);
RHO_GLOBAL void rho_sys_get_full_screen_mode(rho::apiGenerator::CMethodResult& result);
RHO_GLOBAL void rho_sys_set_screen_auto_rotate_mode(bool enable);
RHO_GLOBAL void rho_sys_get_screen_auto_rotate_mode(rho::apiGenerator::CMethodResult& result);

namespace rho {

//----------------------------------------------------------------------------------------------------------------------

class CSystemImpl : public CSystemImplBase
{
    bool mScreenSleeping;
public:
    CSystemImpl() : CSystemImplBase(), mScreenSleeping(true) {}
    virtual ~CSystemImpl(){}

    virtual void getScreenWidth(rho::apiGenerator::CMethodResult& result);
    virtual void getScreenHeight(rho::apiGenerator::CMethodResult& result);
    virtual void getRealScreenWidth(rho::apiGenerator::CMethodResult& result);
    virtual void getRealScreenHeight(rho::apiGenerator::CMethodResult& result);
    virtual void getScreenOrientation(rho::apiGenerator::CMethodResult& result);
    virtual void getPpiX(rho::apiGenerator::CMethodResult& result);
    virtual void getPpiY(rho::apiGenerator::CMethodResult& result);
    virtual void getPhoneId(rho::apiGenerator::CMethodResult& result);
    virtual void getDeviceName(rho::apiGenerator::CMethodResult& result);
    virtual void getLocale(rho::apiGenerator::CMethodResult& result);
    virtual void getCountry(rho::apiGenerator::CMethodResult& result);
    virtual void getIsEmulator(rho::apiGenerator::CMethodResult& result);
    virtual void getHasCalendar(rho::apiGenerator::CMethodResult& result);
    virtual void getOemInfo(rho::apiGenerator::CMethodResult& result);
    virtual void getUuid(rho::apiGenerator::CMethodResult& result);
    virtual void getLockWindowSize(rho::apiGenerator::CMethodResult& result);
    virtual void setLockWindowSize(bool, rho::apiGenerator::CMethodResult& result);
    virtual void getKeyboardState(rho::apiGenerator::CMethodResult& result);
    virtual void setKeyboardState(const rho::String&, rho::apiGenerator::CMethodResult& result);
    virtual void getFullScreen(rho::apiGenerator::CMethodResult& result);
    virtual void setFullScreen(bool, rho::apiGenerator::CMethodResult& result);
    virtual void getScreenAutoRotate(rho::apiGenerator::CMethodResult& result);
    virtual void setScreenAutoRotate(bool, rho::apiGenerator::CMethodResult& result);
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& result);
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& result);
    virtual void setScreenSleeping(bool, rho::apiGenerator::CMethodResult& result);
    virtual void applicationInstall(const rho::String&, rho::apiGenerator::CMethodResult& result);
    virtual void isApplicationInstalled(const rho::String&, rho::apiGenerator::CMethodResult& result);
    virtual void applicationUninstall(const rho::String&, rho::apiGenerator::CMethodResult& result);
    virtual void openUrl(const rho::String&, rho::apiGenerator::CMethodResult& result);
    virtual void setWindowFrame(int, int, int, int, rho::apiGenerator::CMethodResult& result);
    virtual void setWindowPosition(int, int, rho::apiGenerator::CMethodResult& result);
    virtual void setWindowSize(int, int, rho::apiGenerator::CMethodResult& result);
    virtual void bringToFront(rho::apiGenerator::CMethodResult& result);
    virtual void runApplication(const rho::String&, const rho::String&, bool, rho::apiGenerator::CMethodResult& result);
    virtual void getHasCamera(rho::apiGenerator::CMethodResult& result);
    virtual void getPhoneNumber(rho::apiGenerator::CMethodResult& result);
    virtual void getHasNetwork(rho::apiGenerator::CMethodResult& result);
    virtual void getHasWifiNetwork(rho::apiGenerator::CMethodResult& result);
    virtual void getHasCellNetwork(rho::apiGenerator::CMethodResult& result);
    virtual void getDeviceOwnerName(rho::apiGenerator::CMethodResult& result);
    virtual void getDeviceOwnerEmail(rho::apiGenerator::CMethodResult& result);
    virtual void getOsVersion(rho::apiGenerator::CMethodResult& result);
    virtual void getIsSymbolDevice(rho::apiGenerator::CMethodResult& result);
    virtual void hideSplashScreen(rho::apiGenerator::CMethodResult& oResult);
    virtual void getSystemInfo(rho::apiGenerator::CMethodResult& oResult);


};
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SystemImpl"

//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getScreenWidth(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("screen_width", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getScreenHeight(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("screen_height", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getRealScreenWidth(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("real_screen_width", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getRealScreenHeight(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("real_screen_height", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getScreenOrientation(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("screen_orientation", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getPpiX(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("ppi_x", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getPpiY(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("ppi_y", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getPhoneId(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("phone_id", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getDeviceName(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("device_name", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getLocale(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("locale", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getCountry(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("country", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getIsEmulator(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("is_emulator", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getHasCalendar(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("has_calendar", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getOemInfo(rho::apiGenerator::CMethodResult& result)
{
	rho_sysimpl_get_property("oem_info", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getUuid(rho::apiGenerator::CMethodResult& result)
{
	rho_sysimpl_get_property("uuid", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getLockWindowSize(rho::apiGenerator::CMethodResult& result)
{
//    result.setError("not implemented at Android platform");
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setLockWindowSize(bool, rho::apiGenerator::CMethodResult& result)
{
//    result.setError("not implemented at Android platform");
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getKeyboardState(rho::apiGenerator::CMethodResult& result)
{
//    result.setError("not implemented at Android platform");
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setKeyboardState(const rho::String&, rho::apiGenerator::CMethodResult& result)
{
//    result.setError("not implemented at Android platform");
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getFullScreen(rho::apiGenerator::CMethodResult& result)
{
    rho_sys_get_full_screen_mode(result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setFullScreen(bool fullmode, rho::apiGenerator::CMethodResult& result)
{
    rho_sys_set_full_screen_mode(fullmode);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getScreenAutoRotate(rho::apiGenerator::CMethodResult& result)
{
    rho_sys_get_screen_auto_rotate_mode(result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setScreenAutoRotate(bool mode, rho::apiGenerator::CMethodResult& result)
{
    rho_sys_set_screen_auto_rotate_mode(mode);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getWebviewFramework(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("webview_framework", result);

}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getScreenSleeping(rho::apiGenerator::CMethodResult& result)
{
    result.set(mScreenSleeping);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setScreenSleeping(bool flag, rho::apiGenerator::CMethodResult& result)
{
    mScreenSleeping = flag;
    rho_sys_set_sleeping(flag?1:0);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::applicationInstall(const rho::String& url, rho::apiGenerator::CMethodResult& result)
{
    rho_sys_app_install(url, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::isApplicationInstalled(const rho::String& appname, rho::apiGenerator::CMethodResult& result)
{
    rho_sys_is_app_installed(appname, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::applicationUninstall(const rho::String& appname, rho::apiGenerator::CMethodResult& result)
{
    rho_sys_app_uninstall(appname, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::openUrl(const rho::String& url, rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass clsRhodesService = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    jmethodID midOpenExternalUrl = getJNIClassStaticMethod(env, clsRhodesService, "openExternalUrl", "(Ljava/lang/String;)V");
    JNI_EXCEPTION_CHECK(env, result);

    jhstring jhUrl = rho_cast<jstring>(env, url);
    env->CallStaticVoidMethod(clsRhodesService, midOpenExternalUrl, jhUrl.get());

    JNI_EXCEPTION_CHECK(env, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setWindowFrame(int, int, int, int, rho::apiGenerator::CMethodResult& result)
{
//    result.setError("not implemented at Android platform");
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setWindowPosition(int, int, rho::apiGenerator::CMethodResult& result)
{
//    result.setError("not implemented at Android platform");
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::setWindowSize(int, int, rho::apiGenerator::CMethodResult& result)
{
//    result.setError("not implemented at Android platform");
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::bringToFront(rho::apiGenerator::CMethodResult& result)
{
    rho_sys_bring_to_front();
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::runApplication(const rho::String& app, const rho::String& params, bool blocking, rho::apiGenerator::CMethodResult& result)
{
    rho_sys_run_app(app, params, result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getHasCamera(CMethodResult& result)
{
    rho_sysimpl_get_property("has_camera", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getPhoneNumber(CMethodResult& result)
{
    rho_sysimpl_get_property("phone_number", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getHasNetwork(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("has_network", result);
}
//----------------------------------------------------------------------------------------------------------------------
void CSystemImpl::getHasWifiNetwork(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("has_wifi_network", result);
}
//----------------------------------------------------------------------------------------------------------------------
void CSystemImpl::getHasCellNetwork(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("has_cell_network", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getDeviceOwnerName(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("device_owner_name", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getDeviceOwnerEmail(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("device_owner_email", result);
}
//----------------------------------------------------------------------------------------------------------------------

void CSystemImpl::getOsVersion(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("os_version", result);
}

//----------------------------------------------------------------------------------------------------------------------
void CSystemImpl::getIsSymbolDevice(rho::apiGenerator::CMethodResult& result)
{
    rho_sysimpl_get_property("is_symbol_device", result);
}
//----------------------------------------------------------------------------------------------------------------------
void CSystemImpl::hideSplashScreen(rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass clsRhodesService = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    jmethodID mid = getJNIClassStaticMethod(env, clsRhodesService, "removeSplashScreen", "()V");
    JNI_EXCEPTION_CHECK(env, result);

    env->CallStaticVoidMethod(clsRhodesService, mid);

    JNI_EXCEPTION_CHECK(env, result);
}

//----------------------------------------------------------------------------------------------------------------------
void CSystemImpl::getSystemInfo(rho::apiGenerator::CMethodResult& result)
{

    JNIEnv *env = jnienv();
    jclass clsRhodesService = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    jmethodID mid = getJNIClassStaticMethod(env, clsRhodesService, "getSystemInfo", "()Ljava/util/Map;");
    JNI_EXCEPTION_CHECK(env, result);

    jhobject jh = env->CallStaticObjectMethod(clsRhodesService, mid);
    HStringMap hres = rho_cast<HStringMap>(jh);
    rho::Hashtable<rho::String, rho::String> res;

    for ( auto const &it: *hres.get() )
    {
        res.put( it.first, it.second );
    }


    result.set(res);

    JNI_EXCEPTION_CHECK(env, result);
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

class CSystemFactory: public CSystemFactoryBase
{
public:
    ~CSystemFactory(){}

    ISystemSingleton* createModuleSingleton(){ return new CSystemImpl(); }
};

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

extern "C" void Init_System()
{
    CSystemFactory::setInstance( new CSystemFactory() );
    Init_System_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoSystemApi");
}
//----------------------------------------------------------------------------------------------------------------------

}
