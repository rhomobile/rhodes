#include "generated/cpp/ApplicationBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "rubyext/WebView.h"

#ifdef RHODES_EMULATOR
#define RHO_APPS_DIR ""
#else
#define RHO_APPS_DIR "apps/"
#endif

extern "C" void rho_sys_app_exit();

namespace rho {

#ifdef OS_ANDROID

void rho_impl_setNativeMenu(const Vector<String>& menu);
String rho_impl_getNativeMenu();

#endif

using namespace apiGenerator;
using namespace common;

class CApplicationImpl: public CApplicationSingletonBase
{
public:

    CApplicationImpl(): CApplicationSingletonBase(){}

    virtual void getAppBundleFolder(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( CFilePath::join( rho_native_rhopath(), RHO_APPS_DIR"app/") );
    }

    virtual void getAppsBundleFolder(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( CFilePath::join( rho_native_rhopath(), RHO_APPS_DIR) );
    }

    virtual void getUserFolder(rho::apiGenerator::CMethodResult& oResult)
    {
#ifdef OS_MACOSX
        oResult.set( CFilePath::join( rho_native_rhouserpath(), RHO_APPS_DIR) );
#else
        oResult.set( CFilePath::join( rho_native_rhopath(), RHO_APPS_DIR) );
#endif
    }

    virtual void getConfigPath(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getConfFilePath() );
    }

    virtual void getModelsManifestPath(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( CFilePath::join( rho_native_rhopath(), RHO_APPS_DIR"app_manifest.txt") );
    }

    virtual void getDatabaseBlobFolder(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( CFilePath::join( rho_native_rhodbpath(), RHO_EMULATOR_DIR"/db/db-files") );
    }

    virtual void getPublicFolder(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( CFilePath::join( rho_native_rhopath(), RHO_APPS_DIR"public") );
    }

    virtual void getStartURI(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("start_path") );
    }

    virtual void setStartURI( const rho::String& startURI, rho::apiGenerator::CMethodResult& oResult)
    {
        RHOCONF().setString("start_path", startURI, false );
    }

    virtual void getSettingsPageURI(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("options_path") );
    }

    virtual void setSettingsPageURI( const rho::String& settingsPageURI, rho::apiGenerator::CMethodResult& oResult)
    {
        RHOCONF().setString("options_path", settingsPageURI, false );
    }

    virtual void getSplash(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("splash_screen") );
    }

    virtual void getVersion(rho::apiGenerator::CMethodResult& oResult)
    {
       oResult.set( RHOCONF().getString("app_version") );
    }

    virtual void getTitle(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("title_text") );  
    }

    virtual void setTitle( const rho::String& title, rho::apiGenerator::CMethodResult& oResult)
    {
        RHOCONF().setString("title_text", title, false);
    }

    virtual void getName(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHODESAPP().getAppName() );
    }

    virtual void getNativeMenu(rho::apiGenerator::CMethodResult& oResult)
    {
#ifdef OS_ANDROID
        oResult.setJSON(rho_impl_getNativeMenu());
#else
        rho::Vector< Hashtable<String, String> > arRes;
        RHODESAPP().getAppMenu().getMenuItemsEx(arRes);

        oResult.set(arRes);
#endif
    }

    virtual void setNativeMenu( const rho::Vector<rho::String>& value, rho::apiGenerator::CMethodResult& oResult)
    {
#ifdef OS_ANDROID
        rho_impl_setNativeMenu(value);
#else
        RHODESAPP().getAppMenu().setAppMenuJSONItems(value);
        
#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
       rho_webview_update_menu(0);
#endif

#endif
    }

    virtual void getNativeMenuLeft(rho::apiGenerator::CMethodResult& oResult)
    {
        rho::Vector< Hashtable<String, String> > arRes;
        RHODESAPP().getAppMenu().getMenuItemsEx(arRes, true);

        oResult.set(arRes);
    }

    virtual void setNativeMenuLeft( const rho::Vector<rho::String>& nativeMenuLeft, rho::apiGenerator::CMethodResult& oResult)
    {
        RHODESAPP().getAppMenu().setAppMenuJSONItems(nativeMenuLeft, true);
    }

    //////////////////////////////////////////////////////////////////////////

    virtual void getEnableMenuButtonRight(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(RHODESAPP().getAppMenu().getRightItem().m_isEnable);
    }

    virtual void setEnableMenuButtonRight( bool enableMenuButtonRight, rho::apiGenerator::CMethodResult& oResult)
    {
        RHODESAPP().getAppMenu().setEnableRightItem(enableMenuButtonRight);

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
        rho_webview_update_menu(1);
#endif
    }
    
    virtual void getEnableMenuButtonLeft(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(RHODESAPP().getAppMenu().getLeftItem().m_isEnable);
    }
    
    virtual void setEnableMenuButtonLeft( bool enableMenuButtonLeft, rho::apiGenerator::CMethodResult& oResult)
    {
        RHODESAPP().getAppMenu().setEnableRightItem(enableMenuButtonLeft);

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
        rho_webview_update_menu(1);
#endif
    }

    virtual void getEnableMenuItemRight(rho::apiGenerator::CMethodResult& oResult)
    {
        //TODO
    }

    virtual void setEnableMenuItemRight( const rho::Hashtable<rho::String, rho::String>& enableMenuItemRight, rho::apiGenerator::CMethodResult& oResult)
    {
        const rho::Hashtable<rho::String, rho::String>::const_iterator it = enableMenuItemRight.begin();
        if ( it != enableMenuItemRight.end() )
        {
            RHODESAPP().getAppMenu().setEnableMenuItem(it->first, it->second == "true" ? true : false);
        }

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
        rho_webview_update_menu(1);
#endif
    }

    virtual void getEnableMenuItemLeft(rho::apiGenerator::CMethodResult& oResult)
    {
        //TODO
    }

    virtual void setEnableMenuItemLeft( const rho::Hashtable<rho::String, rho::String>& enableMenuItemLeft, rho::apiGenerator::CMethodResult& oResult)
    {
        const rho::Hashtable<rho::String, rho::String>::const_iterator it = enableMenuItemLeft.begin();
        if ( it != enableMenuItemLeft.end() )
        {
            RHODESAPP().getAppMenu().setEnableMenuItem(it->first, it->second == "true" ? true : false, true);
        }

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
        rho_webview_update_menu(1);
#endif
    }

    //////////////////////////////////////////////////////////////////////////
    
    virtual void getNativeMenuItemRight(rho::apiGenerator::CMethodResult& oResult)
    {
        Hashtable<String, String> hashRes;
        RHODESAPP().getAppMenu().getMenuItemEx(hashRes);
        oResult.set(hashRes);
    }

    virtual void setNativeMenuItemRight( const rho::Hashtable<rho::String, rho::String>& nativeMenuItemRight, rho::apiGenerator::CMethodResult& oResult)
    {
        const rho::Hashtable<rho::String, rho::String>::const_iterator it = nativeMenuItemRight.begin();
        if ( it != nativeMenuItemRight.end() )
        {
            RHODESAPP().getAppMenu().setRightItem(it->first, it->second);

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
            rho_webview_update_menu(1);
#endif
        }
    }

    virtual void getNativeMenuItemLeft(rho::apiGenerator::CMethodResult& oResult)
    {
        Hashtable<String, String> hashRes;
        RHODESAPP().getAppMenu().getMenuItemEx(hashRes, true);
        oResult.set(hashRes);
    }

    virtual void setNativeMenuItemLeft( const rho::Hashtable<rho::String, rho::String>& nativeMenuItemLeft, rho::apiGenerator::CMethodResult& oResult)
    {
        const rho::Hashtable<rho::String, rho::String>::const_iterator it = nativeMenuItemLeft.begin();
        if ( it != nativeMenuItemLeft.end() )
        {
            RHODESAPP().getAppMenu().setLeftItem(it->first, it->second);

#if defined (_WIN32_WCE) && !defined (OS_PLATFORM_MOTCE)
            rho_webview_update_menu(1);
#endif
        }
    }

    virtual void getBadLinkURI(rho::apiGenerator::CMethodResult& oResult)
    {
        //TODO: getBadLinkURI
    }

    virtual void setBadLinkURI( const rho::String& badLinkURI, rho::apiGenerator::CMethodResult& oResult)
    {
        //TODO: setBadLinkURI
    }

    virtual void modelFolderPath( const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( CFilePath::join( rho_native_rhopath(), RHO_APPS_DIR"app/" + name + "/") );
    }

    virtual void databaseFilePath( const rho::String& partitionName, rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( CFilePath::join( rho_native_rhodbpath(), RHO_EMULATOR_DIR"/db/syncdb" + partitionName + ".sqlite") );
    }

    virtual void expandDatabaseBlobFilePath( const rho::String& relativePath, rho::apiGenerator::CMethodResult& oResult)
    {
        if ( String_startsWith(relativePath, "file://") )
            oResult.set(relativePath);
        else
        {
            String dbRootFolder = CFilePath::join( rho_native_rhodbpath(), RHO_EMULATOR_DIR);
            if ( !String_startsWith(relativePath, dbRootFolder) )
                oResult.set( CFilePath::join( dbRootFolder, relativePath ) );
        }
    }

    virtual void relativeDatabaseBlobFilePath( const rho::String& absolutePath, rho::apiGenerator::CMethodResult& oResult)
    {
        String dbRootFolder = CFilePath::join( rho_native_rhodbpath(), RHO_EMULATOR_DIR);

        if ( String_startsWith(absolutePath, "file://") )
            dbRootFolder = "file://" + dbRootFolder;

        if ( String_startsWith(absolutePath, dbRootFolder) )
            oResult.set( absolutePath.substr( dbRootFolder.length(), absolutePath.length()-dbRootFolder.length()) );
        else
            oResult.set( absolutePath );
    }

    virtual void quit(rho::apiGenerator::CMethodResult& oResult)
    {
        rho_sys_app_exit();
    }

    virtual void minimize(rho::apiGenerator::CMethodResult& oResult)
    {
        RHODESAPP().getExtManager().minimizeApp();
    }

    virtual void restore(rho::apiGenerator::CMethodResult& oResult)
    {
        RHODESAPP().getExtManager().restoreApp();
    }

    virtual void getSecurityTokenNotPassed(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHODESAPP().isSecurityTokenNotPassed() );
    }

    virtual void getInvalidSecurityTokenStartPath(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("invalid_security_token_start_path") );
    }

    virtual void setInvalidSecurityTokenStartPath( const rho::String& invalidSecurityTokenStartPath, rho::apiGenerator::CMethodResult& oResult)
    {
        RHOCONF().setString("invalid_security_token_start_path", invalidSecurityTokenStartPath, false );
    }

    virtual void setApplicationNotify(rho::apiGenerator::CMethodResult& oResult)
    {
        RHODESAPP().setApplicationEventHandler(oResult);
    }

    virtual void getRhoPlatformVersion(rho::apiGenerator::CMethodResult& oResult)
    {
        //TODO: getRhoPlatformVersion
    }
};

////////////////////////////////////////////////////////////////////////

class CApplicationFactory: public CApplicationFactoryBase
{
public:
    ~CApplicationFactory(){}

    IApplicationSingleton* createModuleSingleton()
    { 
        return new CApplicationImpl(); 
    }
};

}

extern "C" void Init_Application()
{
    rho::CApplicationFactory::setInstance( new rho::CApplicationFactory() );
    rho::Init_Application_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoApplicationApi");
}
