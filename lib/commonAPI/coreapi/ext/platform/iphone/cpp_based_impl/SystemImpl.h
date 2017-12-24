
#include "SystemImplBase.h"

#import "IAppMessageReceiver.h"

namespace rho
{
    class SystemImplIphone;
}

@interface AppMessageReceiverHolder : NSObject<IAppMessageReceiver>
{
    rho::SystemImplIphone* mReceiver;
}
    - (id) init : (rho::SystemImplIphone*)receiver;
@end

namespace rho {
    
    using namespace apiGenerator;

class SystemImplIphone: public CSystemImplBase {

    AppMessageReceiverHolder* m_pAppMessageReceiverHolder;

public:
    SystemImplIphone();

private:
    virtual void getHasCamera(CMethodResult& oResult);
    virtual void getPhoneNumber(CMethodResult& oResult);
    virtual void getOsVersion(CMethodResult& oResult);
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult);
    virtual void getHasSqlite(rho::apiGenerator::CMethodResult& oResult);
    virtual void getRealScreenWidth(CMethodResult& oResult);
    virtual void getRealScreenHeight(CMethodResult& oResult);
    virtual void getDeviceOwnerEmail(CMethodResult& oResult);
    virtual void getDeviceOwnerName(CMethodResult& oResult);
    virtual void getApplicationIconBadge(CMethodResult& oResult);
    virtual void setApplicationIconBadge( int value, CMethodResult& oResult);

    virtual void setDoNotBackupAttribute( const rho::String& pathToFile,  bool doNotBackup, rho::apiGenerator::CMethodResult& oResult);
    //virtual void setPushNotification( const rho::String& url,  const rho::String& url_params,  const rho::String& push_client, rho::apiGenerator::CMethodResult& oResult);
    
    virtual void getHasNetwork(rho::apiGenerator::CMethodResult& oResult);
    virtual void getHasWifiNetwork(rho::apiGenerator::CMethodResult& oResult);
    virtual void getHasCellNetwork(rho::apiGenerator::CMethodResult& oResult);
    
    
    virtual void getScreenWidth(CMethodResult& oResult);
    virtual void getScreenHeight(CMethodResult& oResult);
    virtual void getScreenOrientation(CMethodResult& oResult);
    virtual void getPpiX(CMethodResult& oResult);
    virtual void getPpiY(CMethodResult& oResult);
    virtual void getPhoneId(CMethodResult& oResult);
    virtual void getDeviceName(CMethodResult& oResult);
    virtual void getLocale(CMethodResult& oResult);
    virtual void getCountry(CMethodResult& oResult);
    virtual void getIsEmulator(CMethodResult& oResult);
    virtual void getHasCalendar(CMethodResult& oResult);
    virtual void getOemInfo(CMethodResult& oResult);
    virtual void getUuid(CMethodResult& oResult);
    virtual void getLockWindowSize(CMethodResult& oResult);
    virtual void setLockWindowSize( bool value, CMethodResult& oResult);
    virtual void getKeyboardState(CMethodResult& oResult);
    virtual void setKeyboardState( const rho::String&, CMethodResult& oResult);
    virtual void getScreenAutoRotate(CMethodResult& oResult);
    virtual void setScreenAutoRotate( bool value, CMethodResult& oResult);
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& oResult);
    virtual void setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult);
    
    virtual void applicationInstall( const rho::String& applicationUrl, CMethodResult& oResult);
    virtual void isApplicationInstalled( const rho::String& applicationName, CMethodResult& oResult);
    virtual void applicationUninstall( const rho::String& applicationName, CMethodResult& oResult);
    virtual void openUrl( const rho::String& url, CMethodResult& oResult);
    virtual void setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult);
    virtual void setWindowPosition( int x,  int y, CMethodResult& oResult);
    virtual void setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult);
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& oResult);
    virtual void bringToFront(rho::apiGenerator::CMethodResult& oResult);
    virtual void runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult);
    virtual void getSystemInfo(rho::apiGenerator::CMethodResult& oResult);
    
    //virtual void getFullScreen(rho::apiGenerator::CMethodResult& result);
    //virtual void setFullScreen(bool, rho::apiGenerator::CMethodResult& result);
    
    void getIphoneProperty(const char* property_name, CMethodResult& oResult);
    
};
    
    
}
