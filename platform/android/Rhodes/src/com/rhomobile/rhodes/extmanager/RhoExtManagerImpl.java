package com.rhomobile.rhodes.extmanager;

import java.util.Hashtable;

import android.view.View;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.WebView;

public class RhoExtManagerImpl extends Object implements IRhoExtManager {
    private static final String TAG = RhoExtManagerImpl.class.getSimpleName();

	private Hashtable<String, IRhoExtension> mExtensions;

    private IRhoExtData makeDefExtData() {
        return new RhoExtDataImpl(RhodesActivity.safeGetInstance().getMainView().activeTab());
    }
    
    private static native void nativeRequireRubyFile(String path);

	public RhoExtManagerImpl() {
		mExtensions = new Hashtable<String, IRhoExtension>();
	}
	
	@Override
	public IRhoExtension getExtByName(String strName) {
		return mExtensions.get(strName);
	}

	public void registerExtension(String strName, IRhoExtension ext) {
		if (mExtensions.containsKey(strName)) {
			mExtensions.remove(strName);
		}
		mExtensions.put(strName, ext);
	}

    @Override
	public View getWebView() {
        return RhodesActivity.safeGetInstance().getMainView().getWebView(WebView.activeTab()).getView();
    }

    @Override
    public View getTopView() {
        return RhodesActivity.safeGetInstance().getMainView().getView();
    }

    @Override
    public void onUnhandledProperty(String extName, String name, String value, IRhoExtData extData) {
        IRhoExtension ext = mExtensions.get(extName);
        if (ext != null) {
            ext.onSetProperty(name, value, extData);
        }
    }

    @Override
    public void executeRubyCallback(String url, String body, String data, boolean waitForResponse) {
        RhodesService.doRequestEx(url, body, data, waitForResponse);
    }

    @Override
    public void requireRubyFile(String filePath) {
        Logger.I(TAG, "Require ruby file: " + filePath);
        nativeRequireRubyFile(filePath);
    }

    @Override
    public void navigate(String url) {
        WebView.navigate(url, WebView.activeTab());
    }

    @Override
    public void executeJavascript(String jsFunction) {
        WebView.executeJs(jsFunction, WebView.activeTab());
    }

//    @Override
//    public void rhoLog(int nSeverity, String szModule, String szMsg, String szFile, int nLine) {
//    }

    @Override
    public String getCurrentUrl() {
        return RhodesActivity.safeGetInstance().getMainView().currentLocation(RhodesActivity.safeGetInstance().getMainView().activeTab());
    }

    @Override
    public void stopNavigate() {
        WebView.stopNavigate();
    }

    @Override
    public void historyForward() {
        WebView.navigateForward();
    }

    @Override
    public void historyBack() {
        WebView.navigateBack();
    }

    @Override
    public void refreshPage(boolean bFromCache) {
        WebView.refresh(WebView.activeTab());
    }

    @Override
    public void quitApp() {
        RhodesService.exit();
    }

    @Override
    public void minimizeApp() {
        // TODO Auto-generated method stub
    }

    @Override
    public void restoreApp() {
        // TODO Auto-generated method stub
    }

    @Override
    public void zoomPage(float fZoom) {
        // TODO Auto-generated method stub
    }

    @Override
    public void zoomText(int nZoom) {
        // TODO Auto-generated method stub
    }

    @Override
    public String getBuildConfigItem(String name) {
        return RhodesService.getBuildConfig(name);
    }


    //-----------------------------------------------------------------------------------------------------------------
    // Rhodes implementation related methods are below

    public void onSetPropertiesData(String propId, String data, int position, int total) {
        for (IRhoExtension ext : mExtensions.values()) {
            ext.onSetPropertiesData(propId, data, position, total, makeDefExtData());
        }
    }

    public void onBeforeNavigate(String url) {
        for (IRhoExtension ext : mExtensions.values()) {
            ext.onBeforeNavigate(url, makeDefExtData());
        }
    }

    public void onNavigateComplete(String url) {
        for (IRhoExtension ext : mExtensions.values()) {
            ext.onNavigateComplete(url, makeDefExtData());
        }
    }

    public void onAppActivate(boolean isActivate) {
        for (IRhoExtension ext : mExtensions.values()) {
            ext.onAppActivate(isActivate, makeDefExtData());
        }
    }
    
}