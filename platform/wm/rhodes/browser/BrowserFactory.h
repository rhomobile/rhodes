#pragma once

#include "IBrowserEngine.h"

namespace rho
{
    
struct IBrowserFactory
{
    enum EBrowserEngine
    {
        eNone,
        eIE_CE,
        eIE_WM,
        eWebkit,
    };

    virtual IBrowserEngine* create(HWND hWnd) = 0;
    //
    virtual EBrowserEngine getCurrentBrowserType() = 0;
};
 
class BrowserFactory : public IBrowserFactory
{
private:
    static BrowserFactory* g_BrowserFactory;

public:
    //
    static IBrowserFactory* getInstance();
    //
    IBrowserEngine* create(HWND hWnd);
    //
    EBrowserEngine getCurrentBrowserType();
};

}