package com.rho.webview;

import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.IRhoWebView;

public class WebViewSingleton extends WebViewSingletonBase implements
        IWebViewSingleton {

//    public WebViewSingleton() {
//        RhoExtManager.getInstance().requireRubyFile("WebView");
//    }
    
    @Override
    public void getFramework(IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().getWebView(-1).getEngineId());

    }

    @Override
    public void getFullScreen(IMethodResult result) {
        result.set(BaseActivity.getFullScreenMode());
    }

    @Override
    public void setFullScreen(boolean value, IMethodResult result) {
        BaseActivity.setFullScreenMode(value);
    }

    @Override
    public void getEnableZoom(IMethodResult result) {
        //RhodesActivity.safeGetInstance().getMainView().getWebView(-1).getZoom());

    }

    @Override
    public void setEnableZoom(boolean value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getEnablePageLoadingIndication(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setEnablePageLoadingIndication(boolean value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getEnableWebPlugins(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setEnableWebPlugins(boolean value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getNavigationTimeout(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setNavigationTimeout(int value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getScrollTechnique(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getFontFamily(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getUserAgent(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getViewportEnabled(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getViewportWidth(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getCacheSize(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getEnableCache(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setEnableCache(boolean value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getAcceptLanguage(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setAcceptLanguage(String value, IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void getZoomPage(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setZoomPage(double value, IMethodResult result) {
        RhodesActivity.safeGetInstance().getMainView().getWebView(-1).setZoom(value);

    }

    @Override
    public void getTextZoomLevel(IMethodResult result) {
        // TODO Auto-generated method stub

    }

    @Override
    public void setTextZoomLevel(int value, IMethodResult result) {
		IRhoWebView.TextZoom textZoom;
		switch (value)
		{
		case 0:
			textZoom = IRhoWebView.TextZoom.SMALLEST;
			break;
		case 1:
			textZoom = IRhoWebView.TextZoom.SMALLER;
			break;
		case 2:
			textZoom = IRhoWebView.TextZoom.NORMAL;
			break;
		case 3:
			textZoom = IRhoWebView.TextZoom.LARGER;
			break;
		case 4:
			textZoom = IRhoWebView.TextZoom.LARGEST;
			break;
		default:
			textZoom = IRhoWebView.TextZoom.NORMAL;
			break;
		}
        RhodesActivity.safeGetInstance().getMainView().getWebView(-1).setTextZoom(textZoom);
    }

    @Override
    public void refresh(int tabIndex, IMethodResult result) {
        RhodesActivity.safeGetInstance().getMainView().reload(tabIndex);
    }

    @Override
    public void navigate(String url, int tabIndex, IMethodResult result) {
        WebView.navigate(RhodesService.getInstance().normalizeUrl(url), tabIndex);
    }

    @Override
    public void navigateBack(int tabIndex, IMethodResult result) {
        WebView.navigateBack(tabIndex);
    }

    @Override
    public void currentLocation(int tabIndex, IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().currentLocation(tabIndex));
    }

    @Override
    public void currentURL(int tabIndex, IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().get_current_url(tabIndex));
    }

    @Override
    public void executeJavascript(String javascriptText, int tabIndex, IMethodResult result) {
        WebView.executeJs(javascriptText, tabIndex);
    }

    @Override
    public void active_tab(IMethodResult result) {
        result.set(RhodesActivity.safeGetInstance().getMainView().activeTab());
    }

    @Override
    public void full_screen_mode(boolean enable, IMethodResult result) {
        setFullScreen(enable, result);
    }

    @Override
    public void setCookie(String url, String cookie, IMethodResult result) {
        WebView.setCookie(url, cookie);
    }

    @Override
    public void save(String format, String path, int tabIndex, IMethodResult result) {
        WebView.saveCurrentPage(format, path, tabIndex);
    }

    @Override
    public void getActiveTab(IMethodResult result) {
        result.set(WebView.activeTab());
    }

}
