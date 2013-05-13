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
using rhoruntime;

namespace rho {

namespace WebViewImpl
{
    public class WebView : IWebViewImpl
    {
        public WebView()
        {
            var _runtime = new WebViewRuntimeComponent(this);
        }
    }

    public class WebViewSingleton : IWebViewSingletonImpl
    {
        public WebViewSingleton()
        {
            var _runtime = new WebViewSingletonComponent(this);
        }

        public void getFramework(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getFullScreen(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setFullScreen(bool fullScreen, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getEnableZoom(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setEnableZoom(bool enableZoom, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getEnablePageLoadingIndication(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setEnablePageLoadingIndication(bool enablePageLoadingIndication, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getEnableWebPlugins(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setEnableWebPlugins(bool enableWebPlugins, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getNavigationTimeout(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setNavigationTimeout(int navigationTimeout, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getScrollTechnique(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getFontFamily(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getUserAgent(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getViewportEnabled(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getViewportWidth(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getCacheSize(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getEnableCache(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setEnableCache(bool enableCache, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getAcceptLanguage(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setAcceptLanguage(string acceptLanguage, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getZoomPage(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setZoomPage(double zoomPage, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getTextZoomLevel(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setTextZoomLevel(int textZoomLevel, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void getActiveTab(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void refresh(int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void navigate(string url, int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void navigateBack(int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void currentLocation(int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void currentURL(int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void executeJavascript(string javascriptText, int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void active_tab(IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void full_screen_mode(bool enable, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void setCookie(string url, string cookie, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public void save(string format, string path, int tabIndex, IMethodResult oResult)
        {
            // implement this method in C# here
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
