
#import <UIKit/UIKit.h>

#include "SystemImpl.h"

#import "api_generator/iphone/CRubyConverter.h"
#import "api_generator/iphone/CMethodResult.h"
#import "Rhodes.h"
#import "sys/utsname.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "common/RhodesApp.h"
#include "sync/RhoconnectClientManager.h"
#include "common/RhoFilePath.h"
#include "common/RhoFile.h"
#include "unzip/zip.h"
#include "db/DBAdapter.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"




extern "C" int rho_sysimpl_get_property_iphone(char* szPropName, NSObject** resValue);
extern "C" void rho_sys_set_application_icon_badge(int badge_number);
extern "C" int rho_sys_set_do_not_bakup_attribute(const char* path, int value);
extern "C" BOOL rho_main_is_rotation_locked();
extern "C" void rho_main_set_rotation_locked(BOOL locked);
extern "C" int rho_sys_set_sleeping(int sleeping);

extern "C" int rho_sys_is_app_installed(const char *appname);
extern "C" void rho_sys_app_uninstall(const char *appname);
extern "C" void rho_sys_open_url(const char* url);
extern "C" BOOL rho_sys_app_install(const char *url);
extern "C" BOOL rho_sys_run_app_iphone(const char* appname, char* params);


@implementation AppMessageReceiverHolder
    rho::SystemImplIphone *mReceiver = nil;

    - (id) init : (rho::SystemImplIphone*)receiver
    {
        self = [super init];
        if (self)
        {
            mReceiver = receiver;
            if ([Rhodes sharedInstance])
            {
                //[[Rhodes sharedInstance] setAppMessageReceiver:self];
            }
        }
        return self;
    }


@end




using namespace rho::common;

namespace rho {
    
    using namespace apiGenerator;
    
    SystemImplIphone::SystemImplIphone()
    {
        m_pAppMessageReceiverHolder = [[AppMessageReceiverHolder alloc]init:this];
    }
    
    void SystemImplIphone::getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(true);
    }
    
    
    void SystemImplIphone::getHasSqlite(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(true);
    }
    
    void SystemImplIphone::getRealScreenWidth(CMethodResult& oResult)
    {
        getIphoneProperty("real_screen_width", oResult);
    }
    
    void SystemImplIphone::getRealScreenHeight(CMethodResult& oResult)
    {
        getIphoneProperty("real_screen_height", oResult);
    }
    
    void SystemImplIphone::getApplicationIconBadge(CMethodResult& oResult)
    {
        //oResult.setError("supported only set IconBadge !");
        int r = (int)([UIApplication sharedApplication].applicationIconBadgeNumber);
        //NSLog(@"badge number is %d", r);
        RAWLOG_INFO1("badge number is %d", r);
        oResult.set(r);
    }
    
    void SystemImplIphone::setApplicationIconBadge( int value, CMethodResult& oResult)
    {
        ::rho_sys_set_application_icon_badge(value);
    }
    
    
    
    void SystemImplIphone::setDoNotBackupAttribute( const rho::String& pathToFile,  bool doNotBackup, rho::apiGenerator::CMethodResult& oResult)
    {
        ::rho_sys_set_do_not_bakup_attribute(pathToFile.c_str(), (int)doNotBackup);
    }
    
    void SystemImplIphone::getScreenWidth(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("screen_width", result);
    }
    
    void SystemImplIphone::getScreenHeight(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("screen_height", result);
    }
    
    void SystemImplIphone::getScreenOrientation(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("screen_orientation", result);
    }
    
    void SystemImplIphone::getPpiX(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("ppi_x", result);
    }
    
    void SystemImplIphone::getPpiY(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("ppi_y", result);
    }
    
    void SystemImplIphone::getPhoneId(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("phone_id", result);
    }
    
    void SystemImplIphone::getDeviceName(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("device_name", result);
    }
    
    void SystemImplIphone::getLocale(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("locale", result);
    }
    
    void SystemImplIphone::getCountry(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("country", result);
    }
    
    void SystemImplIphone::getIsEmulator(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("is_emulator", result);
    }
    
    void SystemImplIphone::getHasCalendar(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("has_calendar", result);
    }
    
    void SystemImplIphone::getOemInfo(rho::apiGenerator::CMethodResult& result)
    {
        result.set("");
        //result.setError("not implemented at iOS platform");
    }
    
    void SystemImplIphone::getUuid(rho::apiGenerator::CMethodResult& result)
    {
        result.set("");
        //result.setError("not implemented at iOS platform");
    }
    
    
    void SystemImplIphone::getLockWindowSize(rho::apiGenerator::CMethodResult& result)
    {
        result.set(false);
        //result.setError("not implemented at iOS platform");
    }
    
    void SystemImplIphone::setLockWindowSize(bool, rho::apiGenerator::CMethodResult& result)
    {
        //result.setError("not implemented at iOS platform");
    }
    
    void SystemImplIphone::getKeyboardState(rho::apiGenerator::CMethodResult& result)
    {
        result.set("");
        //result.setError("not implemented at iOS platform");
    }
    
    void SystemImplIphone::setKeyboardState(const rho::String&, rho::apiGenerator::CMethodResult& result)
    {
        //result.setError("not implemented at iOS platform");
    }
    
    void SystemImplIphone::getScreenAutoRotate(rho::apiGenerator::CMethodResult& result)
    {
        //rho_sys_get_screen_auto_rotate_mode(result);
        bool  rotLocked = rho_main_is_rotation_locked()?true:false;
        result.set(!rotLocked);
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::setScreenAutoRotate(bool mode, rho::apiGenerator::CMethodResult& result)
    {
        //rho_sys_set_screen_auto_rotate_mode(mode);
        rho_main_set_rotation_locked(!mode);
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getWebviewFramework(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("webview_framework", result);
        
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getScreenSleeping(rho::apiGenerator::CMethodResult& result)
    {
        
        boolean ret = [[UIApplication sharedApplication] isIdleTimerDisabled] ? false : true;
        result.set(ret);
        //result.setError("not implemented at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::setScreenSleeping(bool flag, rho::apiGenerator::CMethodResult& result)
    {
        [[UIApplication sharedApplication] setIdleTimerDisabled: (!flag ? YES : NO)];
        //rho_sys_set_sleeping(flag?1:0);
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::applicationInstall(const rho::String& url, rho::apiGenerator::CMethodResult& result)
    {
        if ( !::rho_sys_app_install(url.c_str()) ) {
            result.setError("System.applicationInstall failed for: " + url);
        }
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::isApplicationInstalled(const rho::String& appname, rho::apiGenerator::CMethodResult& result)
    {
        int res = ::rho_sys_is_app_installed(appname.c_str());
        result.set(res != 0);
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::applicationUninstall(const rho::String& appname, rho::apiGenerator::CMethodResult& result)
    {
        ::rho_sys_app_uninstall(appname.c_str());
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::openUrl(const rho::String& url, rho::apiGenerator::CMethodResult& result)
    {
        ::rho_sys_open_url(url.c_str());
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::setWindowFrame(int, int, int, int, rho::apiGenerator::CMethodResult& result)
    {
        //result.setError("not implemented at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::setWindowPosition(int, int, rho::apiGenerator::CMethodResult& result)
    {
        //result.setError("not implemented at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::setWindowSize(int, int, rho::apiGenerator::CMethodResult& result)
    {
        //result.setError("not implemented at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::bringToFront(rho::apiGenerator::CMethodResult& result)
    {
        //result.setError("unsupported at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::runApplication(const rho::String& app, const rho::String& params, bool blocking, rho::apiGenerator::CMethodResult& result)
    {
        if ( !::rho_sys_run_app_iphone(app.c_str(), (char*)params.c_str()) ) {
            result.setError("System.runApplication failed for: " + app);
        }
        //rho_sys_run_app(app, params, result);
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getHasCamera(CMethodResult& result)
    {
        getIphoneProperty("has_camera", result);
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getPhoneNumber(CMethodResult& result)
    {
        result.set("");
        //result.setError("unsupported at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getHasNetwork(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("has_network", result);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SystemImplIphone::getHasWifiNetwork(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("has_wifi_network", result);
    }
    //----------------------------------------------------------------------------------------------------------------------
    void SystemImplIphone::getHasCellNetwork(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("has_cell_network", result);
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getDeviceOwnerName(rho::apiGenerator::CMethodResult& result)
    {
        result.set("");
        //result.setError("unsupported at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getDeviceOwnerEmail(rho::apiGenerator::CMethodResult& result)
    {
        result.set("");
        //result.setError("unsupported at iOS platform");
    }
    //----------------------------------------------------------------------------------------------------------------------
    
    void SystemImplIphone::getOsVersion(rho::apiGenerator::CMethodResult& result)
    {
        getIphoneProperty("os_version", result);
    }
    
    
    
    void SystemImplIphone::getIphoneProperty(const char* property_name, CMethodResult& oResult) {
        NSObject* result = nil;
        if (::rho_sysimpl_get_property_iphone((char*)property_name, &result)) {
            if ([result isKindOfClass:[NSString class]]) {
                // string
                NSString* objString = (NSString*)result;
                String s = [objString UTF8String];
                oResult.set(s);
            }
            else if ([result isKindOfClass:[NSNumber class]]) {
                // int, bool or float
                NSNumber* objNumber = (NSNumber*)result;
                if ([::CMethodResult isBoolInsideNumber:objNumber]) {
                    bool b = [objNumber boolValue];
                    oResult.set(b);
                }
                else if ([::CMethodResult isFloatInsideNumber:objNumber]) {
                    double d = [objNumber doubleValue];
                    oResult.set(d);
                }
                else {
                    int i = [objNumber intValue];
                    oResult.set(i);
                }
            }
        }
    }
    
    
    void SystemImplIphone::getSystemInfo(rho::apiGenerator::CMethodResult& oResult)
    {
        rho::Hashtable<rho::String, rho::String> retVal;

//       retVal.put(key,value);

        NSString *value = [[UIDevice currentDevice] localizedModel];       
        retVal.put("Device Localized Model", std::string([value UTF8String]));

        struct utsname systemInfo;
        uname(&systemInfo);
        retVal.put("Device", systemInfo.machine);

        value = [[UIDevice currentDevice] model];       
        retVal.put("Device Model", std::string([value UTF8String]));

        value = [[UIDevice currentDevice] name];
        retVal.put("Device Name", std::string([value UTF8String]));

        NSString *osName = [[UIDevice currentDevice] systemName];       
        NSString *osVersion = [[UIDevice currentDevice] systemVersion];
        value = [@[osName, @", v.", osVersion] componentsJoinedByString:@""];
        retVal.put("Device OS", std::string([value UTF8String]));

        oResult.set(retVal);
    }

    
    
    
    
    class CSystemImplIphoneAccessFactory: public CSystemFactoryBase
    {
    public:
        CSystemImplIphoneAccessFactory(){}
        
        ISystemSingleton* createModuleSingleton()
        {
            return new SystemImplIphone();
        }
    };
    
    
}





extern "C" void Init_System_API();

extern "C" void Init_System()
{
    rho::CSystemFactoryBase::setInstance( new rho::CSystemImplIphoneAccessFactory() );
    Init_System_API();
    
#ifndef RHO_NO_RUBY_API
    RHODESAPP().getExtManager().requireRubyFile("RhoSystemApi");
#endif
}





