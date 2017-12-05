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
    public class WebView : WebViewBase
    {
        public WebView(string id) : base(id)
        {
        }
    }

    public class WebViewSingleton : WebViewSingletonBase
    {
        public WebViewSingleton()
        {
        }

        static private MainPage getMainPage()
        {
            MainPage mainPage = MainPage.getInstance();
            return mainPage;
        }

        public override void getFramework(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getWebviewFramework() : "");
        }

        public override void getFullScreen(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.isFullscreen() : false);
        }

        public override void setFullScreen(bool fullScreen, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.fullscreenCommand(fullScreen ? 1 : 0);
        }

        public override void getKeyboardDisplayRequiresUserAction(IMethodResult oResult)
        {
            oResult.set(true);
        }

        public override void setKeyboardDisplayRequiresUserAction(bool keyboardDisplayRequiresUserAction, IMethodResult oResult)
        {
        }

        public override void getEnableDragAndDrop(IMethodResult oResult)
        {
            oResult.set(true);
        }

        public override void setEnableDragAndDrop(bool enableDragAndDrop, IMethodResult oResult)
        {
        }


        public override void getEnableZoom(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(true);
        }

        public override void getEnableMediaPlaybackWithoutGesture(IMethodResult oResult)
        {
            oResult.set(false);
        }

        public override void getEnablePageLoadingIndication(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(false);
        }

        public override void getEnableWebPlugins(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(true);
        }

        public override void getNavigationTimeout(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(0);
        }

        public override void setNavigationTimeout(int navigationTimeout, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getScrollTechnique(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set("natural");
        }

        public override void getFontFamily(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set("Arial");
        }

        public override void getUserAgent(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getUserAgent() : "");
        }

        public override void getViewportEnabled(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(true);
        }

        public override void getViewportWidth(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getScreenWidth() : 0);
        }

        public override void getCacheSize(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(0);
        }

        public override void getEnableCache(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(false);
        }

        public override void getAcceptLanguage(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set("en");
        }

        public override void setAcceptLanguage(string acceptLanguage, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getZoomPage(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(1.0);
        }

        public override void setZoomPage(double zoomPage, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getTextZoomLevel(IMethodResult oResult)
        {
            // implement this method in C# here
            oResult.set(1.0);
        }

        public override void setTextZoomLevel(int textZoomLevel, IMethodResult oResult)
        {
            // implement this method in C# here
        }

        public override void getActiveTab(IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.tabbarGetCurrent() : 0);
        }

        public override void refresh(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.Refresh(tabIndex);
        }

        public override void navigate(string url, int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
            {
                //waitForBrowserInitialized(tabIndex);
                mp.navigate(CRhoRuntime.getInstance().canonicalizeRhoUrl(url), tabIndex);
            }
        }

        public override void navigateBack(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.GoBack(tabIndex);
        }

        public override void currentLocation(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getCurrentURL(tabIndex) : "");
        }

        public override void currentURL(int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            oResult.set(mp != null ? mp.getCurrentURL(tabIndex) : "");
        }

        public override void executeJavascript(string javascriptText, int tabIndex, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.executeScript(javascriptText, tabIndex);
        }

        public override void active_tab(IMethodResult oResult)
        {
            getActiveTab(oResult);
        }

        public override void full_screen_mode(bool enable, IMethodResult oResult)
        {
            setFullScreen(enable, oResult);
        }

        public override void setCookie(string url, string cookie, IMethodResult oResult)
        {
            MainPage mp = getMainPage();
            if (mp != null)
                mp.setCookie(CRhoRuntime.getInstance().canonicalizeRhoUrl(url), cookie);
        }

        public override void save(string format, string path, int tabIndex, IMethodResult oResult)
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

    public class WebViewFactory : WebViewFactoryBase
    {
    }
}

}
