using System;
using System.Collections.Generic;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using rhoruntime;
using rhodes;

namespace rho {

namespace WebViewImpl
{
    public class WebView : IWebViewImpl
    {
        private long _nativeImpl = 0;
        WebViewRuntimeComponent _runtime;

        public WebView()
        {
            _runtime = new WebViewRuntimeComponent(this);
        }

        public long getNativeImpl()
        {
            return _nativeImpl;
        }

        public void setNativeImpl(long native)
        {
            _nativeImpl = native;
        }
    }

    public class WebViewSingleton : IWebViewSingletonImpl
    {
        public WebViewSingleton()
        {
            WebViewSingletonComponent _runtime = new WebViewSingletonComponent(this);
        }

        static private MainPage getMainPage()
        {
            MainPage mainPage = MainPage.getInstance();
            return mainPage;
        }

        public void getFramework(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getWebviewFramework() : "");
        }

        public void getFullScreen(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.isFullscreen() : false);
        }

        public void setFullScreen(bool fullScreen, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.fullscreenCommand(fullScreen ? 1 : 0);
        }

        public void getEnableZoom(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(true);
        }

        public void setEnableZoom(bool enableZoom, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getEnablePageLoadingIndication(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(false);
        }

        public void setEnablePageLoadingIndication(bool enablePageLoadingIndication, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getEnableWebPlugins(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(true);
        }

        public void setEnableWebPlugins(bool enableWebPlugins, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getNavigationTimeout(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(0);
        }

        public void setNavigationTimeout(int navigationTimeout, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getScrollTechnique(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set("natural");
        }

        public void getFontFamily(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set("Arial");
        }

        public void getUserAgent(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getUserAgent() : "");
        }

        public void getViewportEnabled(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(true);
        }

        public void getViewportWidth(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getScreenWidth() : 0);
        }

        public void getCacheSize(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(0);
        }

        public void getEnableCache(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(false);
        }

        public void setEnableCache(bool enableCache, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getAcceptLanguage(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set("en");
        }

        public void setAcceptLanguage(string acceptLanguage, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getZoomPage(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(1.0);
        }

        public void setZoomPage(double zoomPage, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getTextZoomLevel(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(1.0);
        }

        public void setTextZoomLevel(int textZoomLevel, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getActiveTab(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.tabbarGetCurrent() : 0);
        }

        public void refresh(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.Refresh(tabIndex);
        }

        public void navigate(string url, int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                //waitForBrowserInitialized(tabIndex);
                mp.navigate(CRhoRuntime.getInstance().canonicalizeRhoUrl(url), tabIndex);
            }
        }

        public void navigateBack(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.GoBack(tabIndex);
        }

        public void currentLocation(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getCurrentURL(tabIndex) : "");
        }

        public void currentURL(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getCurrentURL(tabIndex) : "");
        }

        public void executeJavascript(string javascriptText, int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.executeScript(javascriptText, tabIndex);
        }

        public void active_tab(IMethodResult oResult)
        {
            getActiveTab(oResult);
        }

        public void full_screen_mode(bool enable, IMethodResult oResult)
        {
            setFullScreen(enable, oResult);
        }

        public void setCookie(string url, string cookie, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.setCookie(CRhoRuntime.getInstance().canonicalizeRhoUrl(url), cookie);
        }

        public void save(string format, string path, int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        private void waitForBrowserInitialized(int index)
        {
	        while(true)
	        {
		
	        	if(getMainPage().isBrowserInitialized(index))
			        return;
		        else
                    System.Threading.Thread.Sleep(100);
	        }
        }
    }

    public class WebViewFactory : IWebViewFactoryImpl
    {
        public IWebViewImpl getImpl() {
            return new WebView();
        }
        public IWebViewSingletonImpl getSingletonImpl() {
            return new WebViewSingleton();
        }
    }
}

}
