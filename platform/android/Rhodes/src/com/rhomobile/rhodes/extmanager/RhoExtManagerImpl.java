package com.rhomobile.rhodes.extmanager;

import java.util.Hashtable;

import android.view.View;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.WebView;

public class RhoExtManagerImpl implements IRhoExtManager {
    private static final String TAG = RhoExtManagerImpl.class.getSimpleName();

	private Hashtable<String, IRhoExtension> mExtensions;

    private IRhoExtData makeDefExtData(View view) {
        return new RhoExtDataImpl(view, RhodesActivity.safeGetInstance().getMainView().activeTab());
    }
    
    private static native void nativeRequireRubyFile(String path);

	public RhoExtManagerImpl() {
		mExtensions = new Hashtable<String, IRhoExtension>();
	}
	
	@Override
	public IRhoExtension getExtByName(String strName) {
	    synchronized (mExtensions) {
	        return mExtensions.get(strName);
	    }
	}

    public void registerExtension(String strName, IRhoExtension ext) {
        Logger.T(TAG, "Registering extension: " + strName);
        synchronized (mExtensions) {
            if (mExtensions.containsKey(strName)) {
                mExtensions.remove(strName);
            }

            mExtensions.put(strName, ext);
        }
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
        IRhoExtension ext = null;
        synchronized (mExtensions) {
            ext = mExtensions.get(extName);
        }
        if (ext != null) {
            ext.onSetProperty(this, name, value, extData);
        }
    }

    @Override
    public void executeRubyCallback(String url, String body, String data, boolean waitForResponse) {
        RhodesService.doRequestEx(url, body, data, waitForResponse);
    }

    @Override
    public void executeRubyCallbackWithJsonBody(String url, String json, String data, boolean waitForResponse) {
        RhodesService.doRequestJson(url, json, data, waitForResponse);
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

    public void onSetPropertiesData(View view,String propId, String data, int position, int total) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onSetPropertiesData(this, propId, data, position, total, makeDefExtData(view));
            }
        }
    }

    public void onBeforeNavigate(View view, String url) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onBeforeNavigate(this, url, makeDefExtData(view));
            }
        }
    }

    public void onNavigateComplete(View view, String url) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onNavigateComplete(this, url, makeDefExtData(view));
            }
        }
    }

    public void onAppActivate(boolean isActivate) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onAppActivate(this, isActivate);
            }
        }
    }

    public void onAlert(View view, String msg) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onAlert(this, msg, makeDefExtData(view));
            }
        }
    }

    public void onConfirm(View view, String msg) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onConfirm(this, msg, makeDefExtData(view));
            }
        }
    }

    public void onConsole(View view, String msg) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onConsole(this, msg, makeDefExtData(view));
            }
        }
    }

    public void onInputMethod(View view, boolean enabled) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onInputMethod(this, enabled, makeDefExtData(view));
            }
        }
    }

    public void onLoadEnd(View view, String url, long arg2, long arg3) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onNavigateComplete(this, url, makeDefExtData(view));
            }
        }
    }

    public void onLoadError(View view) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onNavigateError(this, "", makeDefExtData(view));
            }
        }
    }

    public void onLoadProgress(View view, int val, int total) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onNavigateProgress(this, "", val, total, makeDefExtData(view));
            }
        }
    }

    public void onMetaEnd(View view) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onSetPropertiesDataEnd(this, makeDefExtData(view));
            }
        }
    }

    public void onPrompt(View view, String prompt, String arg2) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onConfirm(this, prompt, makeDefExtData(view));
            }
        }
    }

    public void onSelect(View view, String[] lines, int pos) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onSelect(this, lines, pos, makeDefExtData(view));
            }
        }
    }

    public void onStatus(View view, String msg) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onStatus(this, msg, makeDefExtData(view));
            }
        }
    }

    public void onTitle(View view, String title) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onStatus(this, title, makeDefExtData(view));
            }
        }
    }
}

