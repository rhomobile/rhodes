#ifndef MAINWINDOWCALLBACK_H
#define MAINWINDOWCALLBACK_H

#include <string>

typedef struct _TNavigateData {
    int index;
    wchar_t* url;
} TNavigateData;

class IMainWindowCallback
{
public:
    virtual void updateSizeProperties(int width, int height) = 0;
    virtual void onActivate(int active) = 0;
    virtual void logEvent(const ::std::string& message) = 0;
    virtual void createCustomMenu(void) = 0;
    virtual void onCustomMenuItemCommand(int nItemPos) = 0;
    virtual void onWindowClose(void) = 0;
    virtual void onWebViewUrlChanged(const ::std::string& url) = 0;
protected:
    virtual ~IMainWindowCallback() {};
};

#endif // MAINWINDOWCALLBACK_H
