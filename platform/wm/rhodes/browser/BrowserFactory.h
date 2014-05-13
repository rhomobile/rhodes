#pragma once

#include "IBrowserEngine.h"

namespace rho
{
    
struct IBrowserFactory
{
    virtual IBrowserEngine* create(HWND hWnd) = 0;
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
};

}