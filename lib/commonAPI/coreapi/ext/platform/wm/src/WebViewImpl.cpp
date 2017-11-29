#include "../../../shared/generated/cpp/WebViewBase.h"
#include "../../../shared/System.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "rubyext/WebView.h"

//extern "C" HWND getMainWnd();
extern "C" const wchar_t* rho_wmimpl_getNavTimeOutVal();

namespace rho {

using namespace apiGenerator;
using namespace common;

class CWebViewImpl: public CWebViewSingletonBase
{
    int m_nNavigationTimeout;
    double m_dZoomPage;
    int m_nTextZoom;
public:

    CWebViewImpl(): m_nNavigationTimeout(45000), m_dZoomPage(1.0), m_nTextZoom(1), CWebViewSingletonBase()
    {
        //TODO TAU
		//convertFromStringW( rho_wmimpl_getNavTimeOutVal(), m_nNavigationTimeout );
		if(m_nNavigationTimeout<=0)
		{
			LOG(WARNING)+" NavigationTimeout  value  from config.xml not correct "+m_nNavigationTimeout;
			m_nNavigationTimeout=45000;
		}
    }

    virtual void getFramework(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(System::getWebviewFramework());
    }

    virtual void getFullScreen(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(rho_webview_get_full_screen() != 0 ? true : false );
    }

    virtual void setFullScreen( bool value, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_webview_full_screen_mode(value ? 1 : 0);
    }

    //Android only
    virtual void getEnableZoom(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(true);
    }

    virtual void setEnableZoom( bool value, rho::apiGenerator::CMethodResult& oResult){}

    virtual void getEnableMediaPlaybackWithoutGesture(rho::apiGenerator::CMethodResult& oResult) {
        oResult.set(false);        
    }

    virtual void getEnablePageLoadingIndication(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(false);
        /*int nValue = 0;
        convertFromStringW( rho_wmimpl_sharedconfig_getvalue( L"GUI\\HourglassEnabled" ), nValue );
        oResult.set( nValue ? true : false );*/
    }

    virtual void setEnablePageLoadingIndication( bool value, rho::apiGenerator::CMethodResult& oResult)
    {
        //Do nothing. It can be set only in config.xml
    }

    virtual void getEnableWebPlugins(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(true);
    }

    virtual void setEnableWebPlugins( bool value, rho::apiGenerator::CMethodResult& oResult){}
    //

    virtual void getNavigationTimeout(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(m_nNavigationTimeout);
    }

    virtual void setNavigationTimeout( int value, rho::apiGenerator::CMethodResult& oResult)
    {
        m_nNavigationTimeout = value;
        rho_webview_setNavigationTimeout(m_nNavigationTimeout);
    }

    virtual void getScrollTechnique(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("WebView.scrollTechnique") );
        //oResult.set( convertToStringA( rho_wmimpl_sharedconfig_getvalue( L"Scrolling\\ScrollTechnique" ) ) );
    }

    virtual void getFontFamily(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("Webview.fontFamily") );
        //oResult.set( convertToStringA( rho_wmimpl_sharedconfig_getvalue( L"HTMLStyles\\FontFamily" ) ) );
    }

    virtual void getUserAgent(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getString("WebView.userAgent") );
        //oResult.set( convertToStringA( rho_wmimpl_sharedconfig_getvalue( L"Navigation\\UserAgent" ) ) );
    }

    virtual void getViewportEnabled(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getBool("WebView.viewportEnabled") );
        /*int nValue = 0;
        convertFromStringW( rho_wmimpl_sharedconfig_getvalue( L"Navigation\\ViewportEnabled" ), nValue );
        oResult.set( nValue ? true : false );*/
    }

    virtual void getViewportWidth(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( RHOCONF().getInt("WebView.viewportWidth") );
        /*int nValue = 0;
        convertFromStringW( rho_wmimpl_sharedconfig_getvalue( L"Navigation\\ViewportWidth" ), nValue );
        oResult.set( nValue );*/
    }

    virtual void getCacheSize(rho::apiGenerator::CMethodResult& oResult)
    {
        /*int nValue = 0;
        convertFromStringW( rho_wmimpl_sharedconfig_getvalue( L"Navigation\\Cache" ), nValue );
        oResult.set( nValue );*/

        oResult.set( RHOCONF().getInt("WebView.cacheSize") );
    }

    //TODO: EnableCache - does it supported by Webkit ?
    virtual void getEnableCache(rho::apiGenerator::CMethodResult& oResult){}
    virtual void setEnableCache( bool value, rho::apiGenerator::CMethodResult& oResult){}

    //TODO: AcceptLanguage - does it supported by Webkit ?
    virtual void getAcceptLanguage(rho::apiGenerator::CMethodResult& oResult){}
    virtual void setAcceptLanguage( const rho::String& value, rho::apiGenerator::CMethodResult& oResult){}

    virtual void getZoomPage(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(m_dZoomPage);
    }

    virtual void setZoomPage( double value, rho::apiGenerator::CMethodResult& oResult)
    {
        m_dZoomPage = value;
        RHODESAPP().getExtManager().zoomPage( (float)m_dZoomPage);
    }

    virtual void getTextZoomLevel(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( m_nTextZoom );
    }

    virtual void setTextZoomLevel( int value, rho::apiGenerator::CMethodResult& oResult)
    {
        m_nTextZoom = value;
        RHODESAPP().getExtManager().zoomText( m_nTextZoom );
    }

    virtual void refresh( int tabIndex, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_webview_refresh(tabIndex);
    }

    virtual void navigate( const rho::String& url,  int tabIndex, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_webview_navigate(url.c_str(), tabIndex);
    }

    virtual void navigateBack( int tabIndex, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_webview_navigate_back_with_tab(tabIndex);
    }

    virtual void currentLocation( int tabIndex, rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( rho_webview_current_location(tabIndex) );
    }

    //iOS, Android only
    virtual void currentURL( int tabIndex, rho::apiGenerator::CMethodResult& oResult){}

    virtual void executeJavascript( const rho::String& javascriptText,  int tabIndex, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_webview_execute_js( javascriptText.c_str(), tabIndex );
    }

    virtual void active_tab(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( rho_webview_active_tab() );
    }

    virtual void getActiveTab(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set( rho_webview_active_tab() );
    }

    virtual void full_screen_mode( bool enable, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_webview_full_screen_mode(enable ? 1 : 0);
    }

    virtual void setCookie( const rho::String& url,  const rho::String& cookie, rho::apiGenerator::CMethodResult& oResult)
    {
        rho_webview_set_cookie( url.c_str(), cookie.c_str() );
    }

    virtual void getCookies( const rho::String& url, rho::apiGenerator::CMethodResult& oResult)
    {

    }

    virtual void removeCookie( const rho::String& url,  const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
    {

    }

    virtual void removeAllCookies(rho::apiGenerator::CMethodResult& oResult)
    {
      
    }

    //Android only
    virtual void save( const rho::String& format,  const rho::String& path,  int tabIndex, rho::apiGenerator::CMethodResult& oResult){}
    //

	void getKeyboardDisplayRequiresUserAction(rho::apiGenerator::CMethodResult& oResult){}
    void setKeyboardDisplayRequiresUserAction( bool keyboardDisplayRequiresUserAction, rho::apiGenerator::CMethodResult& oResult){}

    void getEnableDragAndDrop(rho::apiGenerator::CMethodResult& oResult){}
    void setEnableDragAndDrop( bool enableDragAndDrop, rho::apiGenerator::CMethodResult& oResult){}

};

////////////////////////////////////////////////////////////////////////

class CWebViewFactory: public CWebViewFactoryBase
{
public:
    ~CWebViewFactory(){}

    IWebViewSingleton* createModuleSingleton()
    {
        return new CWebViewImpl();
    }
};

}

extern "C" void Init_WebView()
{
    rho::CWebViewFactory::setInstance( new rho::CWebViewFactory() );
    rho::Init_WebView_API();

    RHODESAPP().getExtManager().requireRubyFile("RhoWebViewApi");

}
