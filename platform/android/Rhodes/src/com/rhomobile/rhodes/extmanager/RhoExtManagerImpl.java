package com.rhomobile.rhodes.extmanager;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.Hashtable;

import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Rect;
import android.view.View;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.WebView;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.Utils;

public class RhoExtManagerImpl implements IRhoExtManager {
    private static final String TAG = RhoExtManagerImpl.class.getSimpleName();

    private Hashtable<String, IRhoExtension> mExtensions = new Hashtable<String, IRhoExtension>();
    private ArrayList<IRhoListener> mListeners = new ArrayList<IRhoListener>();
    private boolean mLogError = false;
    private boolean mLogWarning = false;
    private boolean mLogInfo = false;
    private boolean mLogUser = false;
    private boolean mLogDebug = false;

    private IRhoExtData makeDefExtData(View view) {
        return new RhoExtDataImpl(view, RhodesActivity.safeGetInstance().getMainView().activeTab());
    }
    
    private static native void nativeRequireRubyFile(String path);

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
    public void addRhoListener(IRhoListener listener) {
        if (!mListeners.contains(listener)) {
            mListeners.add(listener);
        }
    }
    
    @Override
    public Context getContext() {
        return ContextFactory.getUiContext();
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
    public void stopNavigate(final IRhoExtension.LoadErrorReason reason) {
        
        //TODO: cannot do stopNavigate due to EkiohView bug
        //WebView.stopNavigate();
        
        final RhodesActivity activity = RhodesActivity.safeGetInstance();
        final MainView mainView = activity.getMainView();
        
        // In case stopNavigate is called from onBeforeNavigate
        // we must be aware that all other parties (extensions) finish to proceed current event
        activity.post(new Runnable() {
            @Override public void run() {
                onLoadError(mainView.getWebView(mainView.activeTab()).getView(), reason);
            }
        });
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

    /** 
     * @return is extension allowed to navigate to its start page
     */
    @Override
    public boolean onStartNewConfig() {
        return Capabilities.SHARED_RUNTIME_ENABLED;
    }

    /**
     * @param name - parameter name
     * @param value - parameter value
     * @return is parameter accepted by rhodes platform
     */
    @Override
    public boolean onNewConfigValue(String name, String value) {
        return false;
    }
    //-----------------------------------------------------------------------------------------------------------------
    // Rhodes implementation related methods are below

    public IRhoWebView createWebView(int tabIndex) {
        IRhoWebView res = null;
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                IRhoWebView view = ext.onCreateWebView(this, tabIndex);
                if (view != null) {
                    if (res != null) {
                        Logger.W(TAG, "WebView has already created by another extension, overlapping it");
                    }
                    res = view;
                }
            }
        }
        return res;
    }
    
    public void enableLogLevelError(boolean enabled) {
        Logger.I(TAG, "RE Error log: " + enabled);
        mLogError = enabled;
    }
    public void enableLogLevelWarning(boolean enabled) { 
        Logger.I(TAG, "RE Warning log: " + enabled);
        mLogWarning = enabled; 
    }
    public void enableLogLevelInfo(boolean enabled) { 
        Logger.I(TAG, "RE Info log: " + enabled);
        mLogInfo = enabled;
    }
    public void enableLogLevelUser(boolean enabled) { 
        Logger.I(TAG, "RE User log: " + enabled);
        mLogUser = enabled;
    }
    public void enableLogLevelDebug(boolean enabled) { 
        Logger.I(TAG, "RE Debug log: " + enabled);
        mLogDebug = enabled;
    }

    void logT(String tag, String msg) {
        if (mLogDebug)
            Logger.I(tag, msg);
    }

    void logI(String tag, String msg) {
        if (mLogInfo)
            Logger.I(tag, msg);
    }

    void logU(String tag, String msg) {
        if (mLogUser)
            Logger.I(tag, msg);
    }

    void logW(String tag, String msg) {
        if (mLogWarning)
            Logger.W(tag, msg);
    }

    void logE(String tag, String msg) {
        if (mLogError)
            Logger.E(tag, msg);
    }

    
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
        Logger.T(TAG, "onAppActivate: " + isActivate);
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                Logger.T(TAG, "onAppActivate: " + ext.getClass().getSimpleName());
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

    public void onInputMethod(View view, boolean enabled, String type, Rect area) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onInputMethod(this, enabled, type, area, makeDefExtData(view));
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

    public void onLoadError(View view, IRhoExtension.LoadErrorReason reason) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onNavigateError(this, "", reason, makeDefExtData(view));
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
    
    public void onAuthRequired(View view, String type, String url, String realm) {
        synchronized (mExtensions) {
            for (IRhoExtension ext : mExtensions.values()) {
                ext.onAuthRequired(this, type, url, realm, makeDefExtData(view));
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

    public void createRhoListeners() {
        for (String classname: RhodesStartupListeners.ourRunnableList) {
            if (classname.length() == 0) continue;
            
            Class<? extends IRhoListener> klass = null;
            try {
                klass = Class.forName(classname).asSubclass(IRhoListener.class);
            } catch (ClassNotFoundException e) {
                Utils.platformLog("RhodesActivity", "processStartupListeners() : ClassNotFoundException for ["+classname+"]");
                e.printStackTrace();
            }
            IRhoListener listener = null;
            try {
                if (klass != null) {
                    listener = klass.newInstance();
                }
            } catch (InstantiationException e) {
                Utils.platformLog("RhodesActivity", "processStartupListeners() : InstantiationException for ["+classname+"]");
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                Utils.platformLog("RhodesActivity", "processStartupListeners() : IllegalAccessException for ["+classname+"]");
                e.printStackTrace();
            }
            if (listener != null) {
                listener.onCreateApplication(this);
            }
        }
    }

    public void onCreateActivity(RhodesActivity activity, Intent intent) {
        for (IRhoListener listener: mListeners) {
            listener.onCreate(activity, intent);
        }
    }
    public void onStartActivity(RhodesActivity activity) {
        for (IRhoListener listener: mListeners) {
            listener.onStart(activity);
        }
    }
    public void onResumeActivity(RhodesActivity activity) {
        for (IRhoListener listener: mListeners) {
            listener.onResume(activity);
        }
    }
    public void onPauseActivity(RhodesActivity activity) {
        for (IRhoListener listener: mListeners) {
            listener.onPause(activity);
        }
    }
    public void onStopActivity(RhodesActivity activity) {
        for (IRhoListener listener: mListeners) {
            listener.onStop(activity);
        }
    }
    public void onDestroyActivity(RhodesActivity activity) {
        for (IRhoListener listener: mListeners) {
            listener.onDestroy(activity);
        }
    }
    public void onNewIntent(RhodesActivity activity, Intent intent) {
        for (IRhoListener listener: mListeners) {
            listener.onNewIntent(activity, intent);
        }
    }
    public Dialog onCreateDialog(RhodesActivity activity, int id/*, Bundle args*/) {
        Dialog res = null;
        for (IRhoListener listener: mListeners) {
            res = listener.onCreateDialog(activity, id);
            if (res != null)
                break;
        }
        return res;
    }

}

