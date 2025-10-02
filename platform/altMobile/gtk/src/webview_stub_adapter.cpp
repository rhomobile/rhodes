#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#include "generated/cpp/WebViewBase.h"
#include "rubyext/WebView.h"

namespace rho {

using rho::apiGenerator::CMethodResult;

namespace {

inline void set_bool(CMethodResult& result, bool value = false) {
    result.set(value);
}

inline void set_int(CMethodResult& result, int value = 0) {
    result.set(value);
}

inline void set_double(CMethodResult& result, double value = 0.0) {
    result.set(value);
}

inline void set_string(CMethodResult& result, const rho::String& value = rho::String()) {
    result.set(value);
}

inline void set_hash(CMethodResult& result) {
    rho::Hashtable<rho::String, rho::String> empty;
    result.set(empty);
}

class CWebViewSingletonGtk : public CWebViewSingletonBase {
public:
    void getFramework(CMethodResult& oResult) override { set_string(oResult, "GTK"); }
    void getFullScreen(CMethodResult& oResult) override { set_bool(oResult); }
    void setFullScreen(bool, CMethodResult& oResult) override { set_bool(oResult); }
    void getEnableZoom(CMethodResult& oResult) override { set_bool(oResult); }
    void getEnableMediaPlaybackWithoutGesture(CMethodResult& oResult) override { set_bool(oResult); }
    void getEnablePageLoadingIndication(CMethodResult& oResult) override { set_bool(oResult); }
    void getEnableWebPlugins(CMethodResult& oResult) override { set_bool(oResult); }
    void getKeyboardDisplayRequiresUserAction(CMethodResult& oResult) override { set_bool(oResult); }
    void setKeyboardDisplayRequiresUserAction(bool, CMethodResult& oResult) override { set_bool(oResult); }
    void getNavigationTimeout(CMethodResult& oResult) override { set_int(oResult); }
    void setNavigationTimeout(int, CMethodResult& oResult) override { set_bool(oResult); }
    void getScrollTechnique(CMethodResult& oResult) override { set_string(oResult); }
    void getFontFamily(CMethodResult& oResult) override { set_string(oResult); }
    void getUserAgent(CMethodResult& oResult) override { set_string(oResult); }
    void getViewportEnabled(CMethodResult& oResult) override { set_bool(oResult); }
    void getViewportWidth(CMethodResult& oResult) override { set_int(oResult); }
    void getCacheSize(CMethodResult& oResult) override { set_int(oResult); }
    void getEnableCache(CMethodResult& oResult) override { set_bool(oResult); }
    void getAcceptLanguage(CMethodResult& oResult) override { set_string(oResult); }
    void setAcceptLanguage(const rho::String&, CMethodResult& oResult) override { set_bool(oResult); }
    void getZoomPage(CMethodResult& oResult) override { set_double(oResult, 1.0); }
    void setZoomPage(double, CMethodResult& oResult) override { set_bool(oResult); }
    void getTextZoomLevel(CMethodResult& oResult) override { set_int(oResult, 100); }
    void setTextZoomLevel(int, CMethodResult& oResult) override { set_bool(oResult); }
    void getActiveTab(CMethodResult& oResult) override { set_int(oResult); }
    void getEnableDragAndDrop(CMethodResult& oResult) override { set_bool(oResult); }
    void setEnableDragAndDrop(bool, CMethodResult& oResult) override { set_bool(oResult); }
    void refresh(int tabIndex, CMethodResult& oResult) override {
        rho_webview_refresh(tabIndex);
        set_bool(oResult, true);
    }
    void navigate(const rho::String& url, int tabIndex, CMethodResult& oResult) override {
        rho_webview_navigate(url.c_str(), tabIndex);
        set_bool(oResult, true);
    }
    void navigateBack(int tabIndex, CMethodResult& oResult) override {
        rho_webview_navigate_back_with_tab(tabIndex);
        set_bool(oResult, true);
    }
    void currentLocation(int tabIndex, CMethodResult& oResult) override {
        const char* location = rho_webview_current_location(tabIndex);
        rho::String value = location ? rho::String(location) : rho::String();
        set_string(oResult, value);
    }
    void currentURL(int tabIndex, CMethodResult& oResult) override {
        const char* location = rho_webview_current_location(tabIndex);
        rho::String value = location ? rho::String(location) : rho::String();
        set_string(oResult, value);
    }
    void executeJavascript(const rho::String& js, int tabIndex, CMethodResult& oResult) override {
        rho_webview_execute_js(js.c_str(), tabIndex);
        set_bool(oResult, true);
    }
    void active_tab(CMethodResult& oResult) override {
        set_int(oResult, rho_webview_active_tab());
    }
    void full_screen_mode(bool enable, CMethodResult& oResult) override {
        rho_webview_full_screen_mode(enable ? 1 : 0);
        set_bool(oResult, true);
    }
    void setCookie(const rho::String&, const rho::String&, CMethodResult& oResult) override { set_bool(oResult); }
    void getCookies(const rho::String&, CMethodResult& oResult) override { set_hash(oResult); }
    void removeCookie(const rho::String&, const rho::String&, CMethodResult& oResult) override { set_bool(oResult); }
    void removeAllCookies(CMethodResult& oResult) override { set_bool(oResult); }
    void save(const rho::String&, const rho::String&, int, CMethodResult& oResult) override { set_bool(oResult); }

    void addCommandToQueue(rho::common::CInstanceClassFunctorBase<CMethodResult>* pFunctor) override {
        if (pFunctor) {
            pFunctor->runObject();
            delete pFunctor;
        }
    }

    void callCommandInThread(rho::common::IRhoRunnable* pFunctor) override {
        if (pFunctor) {
            pFunctor->runObject();
            delete pFunctor;
        }
    }
};

class CWebViewFactoryGtk : public CWebViewFactoryBase {
public:
    IWebViewSingleton* createModuleSingleton() override {
        static CWebViewSingletonGtk singleton;
        return &singleton;
    }
};

} // namespace

} // namespace rho

extern "C" void Init_WebView()
{
    rho::CWebViewFactoryGtk::setInstance(new rho::CWebViewFactoryGtk());
    rho::Init_WebView_API();
    RHODESAPP().getExtManager().requireRubyFile("RhoWebViewApi");
}
