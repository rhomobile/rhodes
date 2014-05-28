#pragma once

#include "common/RhoStd.h"
#include "IBrowserEngine.h"

namespace rho
{

enum EBrowserEngineType
{
    eNone,
    eIE,
    eWebkit,
};
    
struct IBrowserFactory
{
    virtual void checkLicense(HWND hParent, HINSTANCE hLicenseInstance) = 0;    

    virtual IBrowserEngine* create(HWND hWnd) = 0;    
};
 
class BrowserFactory : public IBrowserFactory
{
public:
    static const char* IETag;
    static const char* webkitTag;

private:
    EBrowserEngineType m_selBrowserType;
    bool               m_bLicenseChecked;

    static BrowserFactory* g_browserFactory;

private:
    BrowserFactory() : m_selBrowserType(eNone), m_bLicenseChecked(false) {}
    //
    IBrowserEngine* createWebkit(HWND hwndParent);
    //
    IBrowserEngine* createIE(HWND hwndParent);
    //
    EBrowserEngineType convertBrowserType(rho::String browserType);
    //
    EBrowserEngineType getBrowserType() const;

public:
    //
    static IBrowserFactory* getInstance();
    //
    static EBrowserEngineType getCurrentBrowserType();
    //
    IBrowserEngine* create(HWND hWnd);
    //
    void checkLicense(HWND hParent, HINSTANCE hLicenseInstance);
};

}