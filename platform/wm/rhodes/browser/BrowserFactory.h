#pragma once

#include "IBrowserEngine.h"

namespace rho
{

    enum EBrowserEngine
{
    eNone,
    eIE_CE,
    eIE_WM,
    eWebkit,
};
    
struct IBrowserFactory
{
    virtual IBrowserEngine* create(HWND hWnd) = 0;    
};
 
class BrowserFactory : public IBrowserFactory
{
private:
    static BrowserFactory* g_browserFactory;

public:
    //
    static IBrowserFactory* getInstance();
    //
    IBrowserEngine* create(HWND hWnd);
    //
    static EBrowserEngine getCurrentBrowserType();
};

}