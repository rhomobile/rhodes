#include "generated/cpp/ApplicationBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "json/JSONIterator.h"
#include "rubyext/WebView.h"

#ifdef RHODES_EMULATOR
#define RHO_APPS_DIR ""
#include "common/RhoSimConf.h"
#else
#define RHO_APPS_DIR "apps/"
#endif

extern "C" void rho_sys_app_exit();
extern "C" void rho_title_change(const int tabIndex, const char* strTitle);

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
    virtual void getBundleFolder(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( rho_native_rhopath() );
    }

    virtual void getUserFolder(rho::apiGenerator::CMethodResult& oResult)
    {
#if defined(OS_MACOSX) || defined (OS_LINUX)
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

#if defined(OS_WINCE) || defined (OS_WINDOWS_DESKTOP) || defined(RHODES_EMULATOR) || defined(OS_LINUX)
        rho_title_change(0, title.c_str());
#endif
    }

    virtual void getAppName(rho::apiGenerator::CMethodResult& oResult)
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
        RHODESAPP().getAppMenu().setAppBackUrlWithJSONItems(value);
#else
        RHODESAPP().getAppMenu().setAppMenuJSONItemsEx(value);
#if defined (_WIN32_WCE)
       rho_webview_update_menu(1);
#endif

#endif
    }

    virtual void getBadLinkURI(rho::apiGenerator::CMethodResult& oResult)
    {
#ifdef OS_WINCE
        oResult.set( RHOCONF().getString("Application.BadLinkURI") );
#endif
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
        else if ( relativePath.find("://") != String::npos )
            oResult.set(relativePath);
        else
        {
            String dbRootFolder = CFilePath::join( rho_native_rhodbpath(), RHO_EMULATOR_DIR);
            if ( !String_startsWith(relativePath, dbRootFolder) )
                oResult.set( CFilePath::join( dbRootFolder, relativePath ) );
            else {
                oResult.set(relativePath);
            }
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
        oResult.set(RHOCONF().getString("invalid_security_token_start_path"));
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
#ifndef RHODES_EMULATOR
        oResult.set(RHOCONF().getString("rhodes_gem_version"));
#else
        oResult.set(RHOSIMCONF().getString("rhodes_gem_version"));
#endif
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
