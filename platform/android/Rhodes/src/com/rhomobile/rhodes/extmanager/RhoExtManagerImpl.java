package com.rhomobile.rhodes.extmanager;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.util.Hashtable;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class RhoExtManagerImpl implements IRhoExtManager {
    private static final String TAG = RhoExtManagerImpl.class.getSimpleName();

    private Hashtable<String, IRhoExtension> mExtensions;
    private Object mLicense;
    private boolean mFirstNavigate = true;

    private IRhoExtData makeDefExtData(View view) {
        return new RhoExtDataImpl(view, RhodesActivity.safeGetInstance().getMainView().activeTab());
    }
    
    private static native void nativeRequireRubyFile(String path);

//    private static Class<?> getSubclass(Class<?> clazz, String subclassName) throws ClassNotFoundException {
//        Class<?> subClasses [] = clazz.getDeclaredClasses();
//        for(Class<?> subClass: subClasses) {
//            if(subClass.getSimpleName)
//        }
//        throw new ClassNotFoundException(subclassName);
//    }
    
    static int getResId(String className, String idName) {
        className = R.class.getCanonicalName() + "$" + className;
        try {
            Class<?> rClass = Class.forName(className);
            Field field = rClass.getDeclaredField(idName);
            return field.getInt(null);
        } catch (Throwable e) {
            throw new IllegalArgumentException("Cannot get " + className + "." + idName, e);
        }
    }

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
        MainView mainView = RhodesActivity.safeGetInstance().getMainView();
        return mainView != null ? mainView.getWebView(mainView.activeTab()).getView() : null;
    }

    @Override
    public View getTopView() {
        MainView mainView = RhodesActivity.safeGetInstance().getMainView();
        return mainView != null ? mainView.getView() : null;
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
        MainView mainView = RhodesActivity.safeGetInstance().getMainView();
        return mainView != null ? mainView.currentLocation(mainView.activeTab()) : "";
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
    public void setFullScreen(boolean fullScreen) {
        
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
        WebView.setZoom((int)(fZoom * 100));
    }

    @Override
    public void zoomText(int nZoom) {
        WebView.setTextZoom(nZoom);
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
        if(mFirstNavigate) {
            if(Capabilities.MOTOROLA_ENABLED) {
                PerformOnUiThread.exec(new Runnable() {
                    @SuppressWarnings("deprecation")
                    @Override
                    public void run() {
                        Logger.I(TAG, "Init license now");
                        try {
                            RhodesActivity activity = RhodesActivity.safeGetInstance();
                            LayoutInflater inflater = activity.getLayoutInflater();
                            View licenseView = inflater.inflate(getResId("layout", "license"), null);  
                            activity.addContentView(licenseView, new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT));
                            Class<?> licenseClass = Class.forName("com.motorolasolutions.rhoelements.License");
                            Constructor<?> licenseCtor = licenseClass.getConstructor();
                            mLicense = licenseCtor.newInstance();
                        } catch(Throwable e) {
                            Logger.E(TAG, e);
                            Logger.T(TAG, "No motorola runtime licensing");
                        }
                    }
                });
            }
            mFirstNavigate = false;
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

    public void onPrompt(View view, String prompt, String defaultResponse) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onPrompt(this, prompt, defaultResponse, makeDefExtData(view));
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

    public void startLocationUpdates(View view, boolean val) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.startLocationUpdates(this, val, makeDefExtData(view));
            }
        }
    }

    public void stopLocationUpdates(View view) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.stopLocationUpdates(this, makeDefExtData(view));
            }
        }
    }
    
    // RE shared runtime detect
    public boolean onStartNewConfig() {
    	return Capabilities.SHARED_RUNTIME_ENABLED;
    }

}

