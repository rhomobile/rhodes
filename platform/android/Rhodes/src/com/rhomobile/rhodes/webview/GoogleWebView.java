package com.rhomobile.rhodes.webview;

import java.io.File;
import java.io.FileOutputStream;
import java.util.Timer;
import java.util.TimerTask;
import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.LocalFileProvider;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.deviceowner.RhoDeviceAdminReceiver;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.osfunctionality.AndroidFunctionalityManager;
import com.rhomobile.rhodes.osfunctionality.OsVersionManager;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.RhoConf;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Picture;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.webkit.URLUtil;
import android.webkit.WebBackForwardList;
import android.webkit.WebHistoryItem;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebViewClient;
import android.webkit.CookieManager;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;

import android.widget.EditText;
import android.widget.TextView;
import android.text.InputType;

import android.view.inputmethod.InputMethodManager;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputConnectionWrapper;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.ExtractedTextRequest;
import android.view.inputmethod.ExtractedText;
import android.view.inputmethod.CompletionInfo;
import android.view.inputmethod.CorrectionInfo;
import android.view.inputmethod.InputContentInfo;

import android.app.admin.DevicePolicyManager;
import android.content.ComponentName;


import android.content.Context;
import android.view.View;
import android.view.WindowManager;
import android.view.Display;
import android.view.MotionEvent;

import android.content.SharedPreferences;
import android.provider.Settings;

import android.os.Looper;
import android.os.Handler;
import android.util.TypedValue;
import android.os.Bundle;
import android.view.KeyEvent;

//import android.view.inputmethod.SurroundingText;

//import android.annotation.IntRange;
//import android.annotation.NonNull;
//import android.annotation.Nullable;

//import android.os.LocaleList;
//import 	java.util.Locale;


public class GoogleWebView implements IRhoWebView {
    private static final String TAG = GoogleWebView.class.getSimpleName();

    private static String tauSipValue = "com.tau.taubrowser/com.tau.TauKeyboard";

    public static String mSavedEnabledInputMethods = null;
    public static String mSavedDefaultInputMethod = null;

    private boolean mIsCanRunAgain = true;

    private boolean mIsShouldKillKeyboardMethodUse = true;

    Timer mKillKeyboardTimer = null;

    private static boolean ourShouldDisableKeyboard = false;

    private OnGlobalLayoutListener mOnGlobalLayoutListener = null;




    /*
    public class RhoInputConnectionWrapper extends InputConnectionWrapper {
        private String TAG = RhoInputConnectionWrapper.class.getSimpleName();

        public RhoInputConnectionWrapper(InputConnection connection) {
            super(connection, true);
        }


        @Override
        public CharSequence getTextBeforeCursor(int n, int flags) {
            Logger.I(this.TAG, ".getTextBeforeCursor()");
            hideAnyKeyboard();
            return super.getTextBeforeCursor(n, flags);
        }

        @Override
        public CharSequence getTextAfterCursor(int n, int flags) {
            Logger.T(this.TAG, ".getTextAfterCursor()");
            hideAnyKeyboard();
            return super.getTextAfterCursor(n, flags);
        }

        @Override
        public CharSequence getSelectedText(int flags) {
            Logger.T(this.TAG, ".getSelectedText()");
            hideAnyKeyboard();
            return super.getSelectedText(flags);
        }

        @Override
        public SurroundingText getSurroundingText(int beforeLength, int afterLength, int flags) {
            Logger.T(this.TAG, ".getSurroundingText()");
            hideAnyKeyboard();
            return super.getSurroundingText(beforeLength, afterLength, flags);
        }

        @Override
        public int getCursorCapsMode(int reqModes) {
            Logger.T(this.TAG, ".getCursorCapsMode()");
            hideAnyKeyboard();
            return super.getCursorCapsMode(reqModes);
        }

        @Override
        public ExtractedText getExtractedText(ExtractedTextRequest request, int flags) {
            Logger.T(this.TAG, ".getExtractedText()");
            hideAnyKeyboard();
            return super.getExtractedText(request, flags);
        }

        @Override
        public boolean setComposingText(CharSequence text, int newCursorPosition) {
            Logger.T(this.TAG, ".setComposingText()");
            hideAnyKeyboard();
            return super.setComposingText(text, newCursorPosition);
        }

        //@Override
        //public boolean setComposingText(@NonNull CharSequence text,
        //            int newCursorPosition, @Nullable TextAttribute textAttribute) {
        //    hideAnyKeyboard();
        //    return super.setComposingText(text, newCursorPosition, textAttribute);
        //}

        @Override
        public boolean setComposingRegion(int start, int end) {
            Logger.T(this.TAG, ".setComposingRegion()");
            hideAnyKeyboard();
            return super.setComposingRegion(start, end);
        }

        //@Override
        //public boolean setComposingRegion(int start, int end, @Nullable TextAttribute textAttribute) {
        //    hideAnyKeyboard();
        //    return super.setComposingRegion(start, end, textAttribute);
        //}

        @Override
        public boolean finishComposingText() {
            Logger.T(this.TAG, ".finishComposingText()");
            hideAnyKeyboard();
            return super.finishComposingText();
        }

        @Override
        public boolean commitText(CharSequence text, int newCursorPosition) {
            Logger.T(this.TAG, ".commitText()");
            hideAnyKeyboard();
            return super.commitText(text, newCursorPosition);
        }

        //@Override
        //public boolean commitText(@NonNull CharSequence text, int newCursorPosition,
        //                            @Nullable TextAttribute textAttribute) {
        //    hideAnyKeyboard();
        //    return super.commitText(text, newCursorPosition, textAttribute);
        //}

        @Override
        public boolean commitCompletion(CompletionInfo text) {
            Logger.T(this.TAG, ".commitCompletion()");
            hideAnyKeyboard();
            return super.commitCompletion(text);
        }

        @Override
        public boolean commitCorrection(CorrectionInfo correctionInfo) {
            Logger.T(this.TAG, ".commitContent()");
            hideAnyKeyboard();
            return super.commitCorrection(correctionInfo);
        }

        @Override
        public boolean setSelection(int start, int end) {
            Logger.T(this.TAG, ".setSelection()");
            hideAnyKeyboard();
            return super.setSelection(start, end);
        }

        @Override
        public boolean performEditorAction(int editorAction) {
            Logger.T(this.TAG, ".performEditorAction()");
            hideAnyKeyboard();
            return super.performEditorAction(editorAction);
        }

        @Override
        public boolean performContextMenuAction(int id) {
            Logger.T(this.TAG, ".performContextMenuAction()");
            hideAnyKeyboard();
            return super.performContextMenuAction(id);
        }

        @Override
        public boolean beginBatchEdit() {
            Logger.T(this.TAG, ".beginBatchEdit()");
            hideAnyKeyboard();
            return super.beginBatchEdit();
        }

        @Override
        public boolean endBatchEdit() {
            Logger.T(this.TAG, ".endBatchEdit()");
            hideAnyKeyboard();
            return super.endBatchEdit();
        }

        @Override
        public boolean sendKeyEvent(KeyEvent event) {
            Logger.T(this.TAG, ".sendKeyEvent()");
            hideAnyKeyboard();
            return super.sendKeyEvent(event);
        }

        @Override
        public boolean clearMetaKeyStates(int states) {
            Logger.T(this.TAG, ".clearMetaKeyStates()");
            hideAnyKeyboard();
            return super.clearMetaKeyStates(states);
        }

        @Override
        public boolean reportFullscreenMode(boolean enabled) {
            Logger.T(this.TAG, ".reportFullscreenMode()");
            //hideAnyKeyboard();
            boolean h = false;
            try {
                h = super.reportFullscreenMode(enabled);
            }
            catch (Exception ex) {
                Logger.E(this.TAG, ".reportFullscreenMode() ERROR = "+ex.getMessage());
            }
            return h;
        }

        @Override
        public boolean performSpellCheck() {
            Logger.T(this.TAG, ".performSpellCheck()");
            hideAnyKeyboard();
            return super.performSpellCheck();
        }

        @Override
        public boolean performPrivateCommand(String action, Bundle data) {
            Logger.T(this.TAG, ".performPrivateCommand()");
            hideAnyKeyboard();
            return super.performPrivateCommand(action, data);
        }

        @Override
        public boolean requestCursorUpdates(int cursorUpdateMode) {
            Logger.T(this.TAG, ".requestCursorUpdates()");
            hideAnyKeyboard();
            return super.requestCursorUpdates(cursorUpdateMode);
        }

        @Override
        public Handler getHandler() {
            Logger.T(this.TAG, ".getHandler()");
            //hideAnyKeyboard();
            Handler h = null;
            try {
                h = super.getHandler();
            }
            catch (Exception ex) {
                Logger.E(this.TAG, ".getHandler() ERROR = "+ex.getMessage());
            }
            if (h == null) {
                h = new Handler(Looper.getMainLooper());
            }
            return h;
        }

        @Override
        public void closeConnection() {
            Logger.T(this.TAG, ".closeConnection()");
            //hideAnyKeyboard();
            try {
                super.closeConnection();
            }
            catch (Exception ex) {
                Logger.E(this.TAG, ".closeConnection() ERROR = "+ex.getMessage());
            }
        }

        @Override
        public boolean commitContent(InputContentInfo inputContentInfo, int flags, Bundle opts) {
            Logger.T(this.TAG, ".commitContent()");
            hideAnyKeyboard();
            return super.commitContent(inputContentInfo, flags, opts);
        }

        @Override
        public boolean setImeConsumesInput(boolean imeConsumesInput) {
            Logger.T(this.TAG, ".setImeConsumesInput()");
            hideAnyKeyboard();
            return super.setImeConsumesInput(imeConsumesInput);
        }

        private void hideAnyKeyboard() {
            if (ourShouldDisableKeyboard) {
                if (mIsShouldKillKeyboardMethodUse) {
                    hideKeyboard();
                }
            }
        }

    }
    */


    public class TauWebView extends android.webkit.WebView {

        private String TAG = TauWebView.class.getSimpleName();


        public void setDisableKeyboard(boolean disable) {
            ourShouldDisableKeyboard = disable;
            if (disable) {
                setupOurTauKeyboard();
            }
            else {
                restoreKeyboardSettings();
            }
        }

        public TauWebView(Activity activity) {
            super(activity);

            //if (ourShouldDisableKeyboard) {
                setupOurTauKeyboard();
                RhodesActivity.safeGetInstance().getWindow().setSoftInputMode(android.view.WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);

                //if (mIsShouldKillKeyboardMethodUse ) {
                    final TauWebView me = this;
                    setOnTouchListener(new View.OnTouchListener() {

                        private int mCurrentMax = 0;

                        @Override
                        public boolean onTouch(View v, MotionEvent event) {
                            Logger.I(TAG, "TauWebView.onTouch");
                            if ((event.getActionMasked() == MotionEvent.ACTION_UP) || (event.getActionMasked() == MotionEvent.ACTION_POINTER_UP)) {
                                if ((event.getPointerCount() == 3) && (mCurrentMax == 3)) {
                                    RhodesActivity.onTripleTap();
                                }
                                if ((event.getPointerCount() == 4) && (mCurrentMax == 4)) {
                                    RhodesActivity.onQuadroTap();
                                }
                                if (event.getPointerCount() == 1) {
                                    mCurrentMax = 0;
                                }
                            }
                            else if (event.getAction() == MotionEvent.ACTION_CANCEL) {
                                mCurrentMax = 0;
                            }
                            else if ((event.getActionMasked() == MotionEvent.ACTION_DOWN) || (event.getActionMasked() == MotionEvent.ACTION_POINTER_DOWN)) {
                                Logger.I(TAG, "TauWebView.onTouch COUNT = " + String.valueOf(event.getPointerCount()));
                                if (event.getPointerCount() > mCurrentMax) {
                                    mCurrentMax = event.getPointerCount();
                                }
                            }
                            //ViewGroup activityRootView = ((ViewGroup) RhodesActivity.safeGetInstance().findViewById(android.R.id.content));
                            //recursiveLoopChildrenForTextEdit(activityRootView);
                            //RhodesActivity.safeGetInstance().getWindow().setFlags(  WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM,
                            //                                                        WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM);
                            if ( ourShouldDisableKeyboard && mIsShouldKillKeyboardMethodUse) {
                                hideKeyboard();
                            }
                            return false;
                        }
                    });
                //}
            //}
        }


        private boolean isAutorizationDisable = false;

        public void setDisableHTTPAuthorisation(boolean sa) {
            isAutorizationDisable = sa;
        }



        public void loadUrl(String url) {
            Logger.I(this.TAG, "TauWebView.loadUrl url = " + url);
            ///*
            if (isAutorizationDisable) {
                HashMap<String, String> extraHeaders = new HashMap<String, String>();
                extraHeaders.put("Authorization", "");
                //isAutorizationDisable = false;
                Logger.I(this.TAG, "TauWebView.loadUrl disable Authorization !");
                loadUrl(url, extraHeaders);
                return;
            }
            //*/
            super.loadUrl(url);
        }

        public void loadUrl (String url, Map<String, String> additionalHttpHeaders) {
            Logger.I(this.TAG, "TauWebView.loadUrl url,additionalHttpHeaders  = " + url);
            /*
            if (isAutorizationDisable) {
                HashMap<String, String> extraHeaders = new HashMap<String, String>();
                extraHeaders.put("Authorization", "");
                isAutorizationDisable = false;
                super.loadUrl(url, extraHeaders);
                return;
            }
            */
            super.loadUrl(url, additionalHttpHeaders);
        }



        public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
            if (ourShouldDisableKeyboard) {
                outAttrs.privateImeOptions = "tau browser!";
                if (mIsShouldKillKeyboardMethodUse) {
                    //recursiveLoopChildrenForTextEdit(this);
                    hideKeyboard();
                    outAttrs.imeOptions = EditorInfo.IME_ACTION_DONE;
                    outAttrs.inputType = EditorInfo.TYPE_NULL;
                    //outAttrs.hintLocales = new LocaleList(new Locale("en", "US"));
                }
            }
            InputConnection super_connection = super.onCreateInputConnection(outAttrs);
            InputConnection connection = super_connection;
            if (ourShouldDisableKeyboard && mIsShouldKillKeyboardMethodUse) {
                //connection = new RhoInputConnectionWrapper(super_connection);
            }

            if (ourShouldDisableKeyboard) {
                if (mIsShouldKillKeyboardMethodUse) {
                    //outAttrs.hintLocales = new LocaleList(new Locale("en", "US"));
                    outAttrs.imeOptions = EditorInfo.IME_ACTION_DONE;
                    outAttrs.inputType = EditorInfo.TYPE_NULL;
                    hideKeyboard();
                    //ViewGroup activityRootView = ((ViewGroup) RhodesActivity.safeGetInstance().findViewById(android.R.id.content));
                    //recursiveLoopChildrenForTextEdit(activityRootView);
                }
            }
            return connection;
        }

        public void recursiveLoopChildrenForTextEdit(ViewGroup parent) {
        for (int i = 0; i < parent.getChildCount(); i++) {
            final View child = parent.getChildAt(i);

            //child.setBackgroundColor(0xFF00FF00);
            if (child instanceof TextView) {
                TextView et = (TextView)child;
                et.setRawInputType(InputType.TYPE_CLASS_TEXT);
                et.setTextIsSelectable(true);
                et.setBackgroundColor(0xFF00FF00);
                //et.setInputType(InputType.TYPE_CLASS_TEXT);
            }

            if (child instanceof ViewGroup) {
                recursiveLoopChildrenForTextEdit((ViewGroup) child);
            } else {
                if (child != null) {
                }
            }
        }
    }
    }


    private void setupOurTauKeyboard() {
        if (ourShouldDisableKeyboard) {
            Logger.I(this.TAG, "TAU KEYBOARD setupOurTauKeyboard() ourShouldDisableKeyboard == true");
            try {
                mIsShouldKillKeyboardMethodUse = true;
                RhodesActivity.safeGetInstance();
                Context context = RhodesActivity.getContext();
                String enabledInputmethods = Settings.Secure.getString(context.getContentResolver(), "enabled_input_methods");
                Logger.I(this.TAG, "TAU KEYBOARD enabled_input_methods = " + enabledInputmethods);

                String default_input_method_local = Settings.Secure.getString(context.getContentResolver(), "default_input_method");
                Logger.I(this.TAG, "TAU KEYBOARD default_input_method = " + default_input_method_local);

                boolean hasError = false;
                if (!enabledInputmethods.contains(tauSipValue)) {
                    // should be added !
                    boolean res_added = Settings.Secure.putString(context.getContentResolver(), "enabled_input_methods", enabledInputmethods+":"+tauSipValue);
                    Logger.I(this.TAG, "TAU KEYBOARD add our keyboard to enabled_input_methods res_added = " + res_added);
                    if (res_added) {
                        mSavedEnabledInputMethods = enabledInputmethods;
                    }
                    else {
                        hasError = true;
                    }
                }
                else {
                    Logger.I(this.TAG, "TAU KEYBOARD enabled_input_methods already has tau keyboard");
                }

                if (!hasError) {
                    boolean res_def = Settings.Secure.putString(context.getContentResolver(), "default_input_method", tauSipValue);
                    Logger.I(this.TAG, "TAU KEYBOARD set our keyboard to default_input_method res_def = " + res_def);
                    if (res_def) {
                        mSavedDefaultInputMethod = default_input_method_local;
                        //if (mSavedDefaultInputMethod == null) {
                            mSavedDefaultInputMethod = "com.google.android.inputmethod.latin/com.android.inputmethod.latin.LatinIME";
                        //}
                    }
                    else {
                        hasError = true;
                    }
                }
                if (!hasError) {
                    mIsShouldKillKeyboardMethodUse = false;
                }

            } catch (Exception e) {
                Logger.E(this.TAG, "TAU KEYBOARD Exception = " + e.getMessage());
            }
            
            if(mIsShouldKillKeyboardMethodUse && RhoDeviceAdminReceiver.isDeviceOwner(RhodesActivity.safeGetInstance())){
                try{
                    RhodesActivity.safeGetInstance();
                    Context context = RhodesActivity.getContext();
                    String default_input_method_local = Settings.Secure.getString(context.getContentResolver(), "default_input_method");

                    if (!default_input_method_local.contains(tauSipValue)){
                        DevicePolicyManager devicePolicyManager = (DevicePolicyManager)context.getSystemService(Context.DEVICE_POLICY_SERVICE);
                        ComponentName adminComponentName = RhoDeviceAdminReceiver.getComponentName(context);
                        devicePolicyManager.setSecureSetting(adminComponentName, Settings.Secure.DEFAULT_INPUT_METHOD,tauSipValue);

                        mSavedDefaultInputMethod = default_input_method_local;

                        mIsShouldKillKeyboardMethodUse = false;
                    }
                } catch (Exception e){
                    Logger.E(this.TAG, "TAU KEYBOARD Exception = " + e.getMessage());
                }
            }

            if (mIsShouldKillKeyboardMethodUse) {
                Logger.E(this.TAG, "TAU KEYBOARD no android.permission.WRITE_SECURE_SETTINGS permission - use alternative method !");

                if (mKillKeyboardTimer != null) {
                    mKillKeyboardTimer.cancel();
                }

                mKillKeyboardTimer = new Timer();

                //Update the elapsed time every second
                mKillKeyboardTimer.schedule(new TimerTask() {
                    @Override
                    public void run() {
                        //hideKeyboardNow();
                        ///*
                        if (mIsCanRunAgain) {
                            mIsCanRunAgain = false;
                            new Handler(Looper.getMainLooper()).post(new Runnable() {
                                @Override
                                public void run() {
                                    hideKeyboardNow();
                                    mIsCanRunAgain = true;
                                }
                            });
                        }
                        //*/
                    }
                }, 100, 5);
            }
            setKeyboardListener();
        }
    }


    private void restoreKeyboardSettings() {
        Logger.I(this.TAG, "TAU KEYBOARD restoreKeyboardSettings()");
        if (mKillKeyboardTimer != null) {
            mKillKeyboardTimer.cancel();
            mKillKeyboardTimer = null;
        }
        if (mSavedEnabledInputMethods != null) {
            String to_restore = mSavedEnabledInputMethods;
            mSavedEnabledInputMethods = null;
            Logger.I(this.TAG, "TAU KEYBOARD restore saved enabled_input_methods");
            try {
                RhodesActivity.safeGetInstance();
                Context context = RhodesActivity.getContext();
                Settings.Secure.putString(context.getContentResolver(), "enabled_input_methods", to_restore);
            } catch (Exception e) {
                Logger.E(this.TAG, "TAU KEYBOARD Exception = " + e.getMessage());
            }
        }
        if (mSavedDefaultInputMethod != null) {
            String to_restore = mSavedDefaultInputMethod;
            mSavedDefaultInputMethod = null;
            Logger.I(this.TAG, "TAU KEYBOARD restore saved default_input_method");
            boolean isDefault = false;
            try {
                RhodesActivity.safeGetInstance();
                Context context = RhodesActivity.getContext();
                Settings.Secure.putString(context.getContentResolver(), "default_input_method", to_restore);
                isDefault = true;
            } catch (Exception e) {
                Logger.E(this.TAG, "TAU KEYBOARD Exception = " + e.getMessage());
            }

            if(!isDefault){
                try{
                    RhodesActivity.safeGetInstance();
                    Context context = RhodesActivity.getContext();

                    DevicePolicyManager devicePolicyManager = (DevicePolicyManager)context.getSystemService(Context.DEVICE_POLICY_SERVICE);
                    ComponentName adminComponentName = RhoDeviceAdminReceiver.getComponentName(context);
                    devicePolicyManager.setSecureSetting(adminComponentName, Settings.Secure.DEFAULT_INPUT_METHOD, to_restore);
                    isDefault = true;
                } catch (Exception e) {
                    Logger.E(this.TAG, "TAU KEYBOARD Exception = " + e.getMessage());
                }
            }
        }
        removeKeyboardListener();
    }


    public final void setKeyboardListener() {
        final View activityRootView = ((ViewGroup) RhodesActivity.safeGetInstance().findViewById(android.R.id.content)).getChildAt(0);
        if (mOnGlobalLayoutListener != null) {
            activityRootView.getViewTreeObserver().addOnGlobalLayoutListener(mOnGlobalLayoutListener);
        }
    }

    public final void removeKeyboardListener() {
        final View activityRootView = ((ViewGroup) RhodesActivity.safeGetInstance().findViewById(android.R.id.content)).getChildAt(0);

        if (mOnGlobalLayoutListener != null) {
            activityRootView.getViewTreeObserver().removeOnGlobalLayoutListener(mOnGlobalLayoutListener);
        }
    }



    private WebChromeClient mChromeClient;
    private WebViewClient mWebViewClient;
    private static Boolean mInitialized = false;

    private android.webkit.WebView mWebView;
    private ViewGroup mContainerView;
    private TextZoom mTextZoom = TextZoom.NORMAL;
    private IRhoConfig mConfig;

    private boolean mIsBackDisabled = false;

    public GoogleWebView(Activity activity) {
        mWebView = new TauWebView(activity);
        mWebView.getSettings().setLoadWithOverviewMode(true);
        mWebView.getSettings().setUseWideViewPort(true);

        tauSipValue = activity.getApplicationContext().getPackageName()+"/com.tau.TauKeyboard";

        //if (ourShouldDisableKeyboard) {
            mOnGlobalLayoutListener = new OnGlobalLayoutListener() {

                @Override
                public void onGlobalLayout() {
                    if (ourShouldDisableKeyboard) {
                        hideKeyboard();
                    }
                }

            };


            mWebView.setOnTouchListener(new View.OnTouchListener() {
                public boolean onTouch(View v, MotionEvent event) {
                    if (ourShouldDisableKeyboard) {
                        hideKeyboard();
                    }
                    return false;
                }
            });


            if (mIsShouldKillKeyboardMethodUse) {
                setKeyboardListener();
            }
        //}

        synchronized(mInitialized) {
            if (!mInitialized) {
                mWebView.clearCache(true);
                initWebStuff(activity);
            }
        }
        mWebViewClient = new RhoWebViewClient(this);
        mChromeClient = new RhoWebChromeClient(activity, this);
    }

    private void hideKeyboardNow() {
        try {
            Activity ctx = RhodesActivity.safeGetInstance();
            if (ctx != null) {
                InputMethodManager imm = (InputMethodManager) ctx.getSystemService(Context.INPUT_METHOD_SERVICE);
                if (imm != null) {
                    imm.hideSoftInputFromWindow(getView().getApplicationWindowToken(), 0);
                    if (ctx.getCurrentFocus() != null) {
                        imm.hideSoftInputFromWindow(ctx.getCurrentFocus().getWindowToken(), 0);
                    }
                }
            }
        }
        catch (Throwable e) {
            Logger.E(TAG, e);
        }
    }

    private void hideKeyboard() {
       if(Looper.getMainLooper().getThread() == Thread.currentThread()) {
           hideKeyboardNow();
       }
       else {
           PerformOnUiThread.exec(new Runnable() {
                @Override
                public void run() {
                    hideKeyboardNow();
                }
           });
       }
       PerformOnUiThread.exec(new Runnable() {
            @Override
            public void run() {
                hideKeyboardNow();
            }
       }, 10);
    }

    public void setBackDisabled(boolean value) {
        mIsBackDisabled = value;
    }

    private static void initWebStuff(Activity activity) {

        Logger.I(TAG, "Initialize Google WEbView staff");

        OsVersionManager.registerSelector(IWebSettingsProvider.class, WebSettingsProviderBase.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.ECLAIR, IWebSettingsProvider.class, WebSettingsProviderEclair.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.ECLAIR_MR1, IWebSettingsProvider.class, WebSettingsProviderEclairMR1.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.FROYO, IWebSettingsProvider.class, WebSettingsProviderFroyo.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.HONEYCOMB_MR1, IWebSettingsProvider.class, WebSettingsProviderHoneycombMR1.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.JELLY_BEAN, IWebSettingsProvider.class, WebSettingsProviderJellyBean.class.getCanonicalName());
        OsVersionManager.registerSelector(Build.VERSION_CODES.KITKAT, IWebSettingsProvider.class, WebSettingsProviderKitKat.class.getCanonicalName());

        mInitialized = true;
    }

    private void applyWebSettings() {
        Logger.I(TAG, "applyWebSettings");
//        PerformOnUiThread.exec(new Runnable() {
//            @Override
//            public void run() {
                Logger.I(TAG, "Web settings is applying now  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

                double z = getConfig().getDouble(WebViewConfig.PAGE_ZOOM);
                mWebView.setInitialScale((int)(z * 100));
                mWebView.setVerticalScrollBarEnabled(true);
                mWebView.setHorizontalScrollBarEnabled(true);
                mWebView.setVerticalScrollbarOverlay(true);
                mWebView.setHorizontalScrollbarOverlay(true);
                mWebView.setFocusableInTouchMode(true);
                int sdkVersion = Build.VERSION.SDK_INT;

                if ( sdkVersion >= 21 ) {
                    //Logger.I(TAG, "Enabling third-party cookies");
                    //CookieManager cm = CookieManager.getInstance();
                    //try {
                        //java.lang.reflect.Method mtdAllow3Party = cm.getClass().getMethod("setAcceptThirdPartyCookies", android.webkit.WebView.class, boolean.class);
                        //mtdAllow3Party.invoke(cm,mWebView,true);
                        //Logger.I(TAG, "Enabling third-party cookies done");
                    //} catch (SecurityException e) {
                        //Logger.W(TAG, "Can't allow third-party cookies explicily due to SecurityException");
                    //} catch (NoSuchMethodException e) {
                        //Logger.W(TAG, "Can't allow third-party cookies explicily due to NoSuchMethodException");
                    //} catch ( Exception e ) {
                        //Logger.E(TAG, "Can't allow third-party cookies explicily due to exception: " + e.toString() );
                    //}
                }

    		  /*  if(sdkVersion <19){
    		    	  mWebView.getSettings().setLoadWithOverviewMode(true);
    		    	  mWebView.getSettings().setUseWideViewPort(true);
    		    }*/

                IWebSettingsProvider provider = OsVersionManager.getFeature(IWebSettingsProvider.class);
                provider.fillSettings(mWebView.getSettings(), mConfig);

                RhodesActivity.safeGetInstance().notifyUiCreated();
//            }
//        });
    }

    @Override
    public void setWebClient() {
        PerformOnUiThread.exec(new Runnable() {
            @Override
            public void run() {
                Logger.I(TAG, "Setting RhoWebChromeClient and RhoWebViewClient");
                mWebView.setWebChromeClient(mChromeClient);
                mWebView.setWebViewClient(mWebViewClient);
            }
        });
    }

    @Override
    public void setConfig(IRhoConfig config) {
        mConfig = config;
        applyWebSettings();
    }

    public IRhoConfig getConfig() {
        return mConfig;
    }

    @Override
    public View getView() {
        return mWebView;
    }

    @Override
    public void setContainerView(ViewGroup view) {
        mContainerView = view;
    }

    @Override
    public ViewGroup getContainerView() {
        return mContainerView;
    }

    @Override
    public boolean canGoBack() {
        return mWebView.canGoBack();
    }

    @Override
    public void goBack() {
        if (mIsBackDisabled) {
            return;
        }
        try {
            WebBackForwardList history = mWebView.copyBackForwardList();
            int current_index = history.getCurrentIndex();
            if (current_index > 0) {
                // can go back
                WebHistoryItem item = history.getItemAtIndex(current_index-1);
                if (item != null) {
                    String url = item.getUrl();
                    if (url != null) {
                        // inform about back with this url
                        RhoExtManager.getImplementationInstance().onGoBack(mWebView, getUrl(), url);
                    }
                }
            }
        }
        catch (Throwable e) {
            Logger.E(TAG, e);
        }
        mWebView.goBack();
    }

    @Override
    public void goForward() {
        try {
            WebBackForwardList history = mWebView.copyBackForwardList();
            int current_index = history.getCurrentIndex();
            if (current_index > 0) {
                // can go back
                WebHistoryItem item = history.getItemAtIndex(current_index-1);
                if (item != null) {
                    String url = item.getUrl();
                    if (url != null) {
                        // inform about back with this url
                        RhoExtManager.getImplementationInstance().onGoForward(mWebView, getUrl(), url);
                    }
                }
            }
        }
        catch (Throwable e) {
            Logger.E(TAG, e);
        }
        mWebView.goForward();
    }

    @Override
    public void reload() {
        mWebView.reload();
    }

    @Override
    public void clear() {
        mWebView.clearView();
        mWebView.clearCache(true);
        mWebView.invalidate();
        mWebView.loadData("", "", "");
    }

    @Override
    public String getUrl() {
        return mWebView.getUrl();
    }

    @Override
    public void loadUrl(String url) {
        Logger.profStart("BROWSER_PAGE");

        RhoExtManager.getImplementationInstance().onBeforeNavigate(mWebView, url);

        Uri uri = LocalFileProvider.overrideUri(Uri.parse(url));
        if (uri != null) {
            url = Uri.decode(uri.toString());
            Logger.T(TAG, "Override URL: " + url);
        }
        mWebView.loadUrl(url);
    }

    @Override
    public void loadData(String data, String mime, String encoding) {
        mWebView.loadData(data, mime, encoding);
    }

    @Override
    public void loadDataWithBaseURL(String baseUrl, String data, String mimeType, String encoding, String historyUrl) {
        mWebView.loadDataWithBaseURL(baseUrl, data, mimeType, encoding, historyUrl);
    }

    @Override
    public void stopLoad() {
        mWebView.stopLoading();
    }

    @Override
    public void setZoom(double scale) {
        mWebView.setInitialScale((int)(scale * 100));
    }

    @Override
    public void setTextZoom(TextZoom zoom) {
        WebSettings.TextSize googleTextZoom = WebSettings.TextSize.NORMAL;
        mTextZoom = TextZoom.NORMAL;
        if (zoom == TextZoom.SMALLEST)
        {
            googleTextZoom = WebSettings.TextSize.SMALLEST;
            mTextZoom = TextZoom.SMALLEST;
        }
        else if (zoom == TextZoom.SMALLER)
        {
            googleTextZoom = WebSettings.TextSize.SMALLER;
            mTextZoom = TextZoom.SMALLER;
        }
        else if (zoom == IRhoWebView.TextZoom.NORMAL)
        {
            googleTextZoom = WebSettings.TextSize.NORMAL;
            mTextZoom = TextZoom.NORMAL;
        }
        else if (zoom == IRhoWebView.TextZoom.LARGER)
        {
            googleTextZoom = WebSettings.TextSize.LARGER;
            mTextZoom = TextZoom.LARGER;
        }
        else if (zoom == IRhoWebView.TextZoom.LARGEST)
        {
            googleTextZoom = WebSettings.TextSize.LARGEST;
            mTextZoom = TextZoom.LARGEST;
        }
        mWebView.getSettings().setTextSize(googleTextZoom);
    }

    @Override
    public TextZoom getTextZoom()
    {
	return mTextZoom;
    }

    @Override
    public String getEngineId() {
        return "WEBKIT/GOOGLE/" + Build.VERSION.RELEASE;
    }

    @Override
    public void onPause() {
        // lost foreground - restore keyboard settings !
        restoreKeyboardSettings();


        //AndroidFunctionalityManager.getAndroidFunctionality().pauseWebView(mWebView,true);
    }

    @Override
    public void onResume() {
        setupOurTauKeyboard();

        //AndroidFunctionalityManager.getAndroidFunctionality().pauseWebView(mWebView,false);
    }

    @Override
    public void destroy() {
        restoreKeyboardSettings();
        mWebView.destroy();
    }

    @Override
    public WebBackForwardList copyBackForwardList() {
    	return mWebView.copyBackForwardList();
    }

    @Override
    public void capture(CaptureFormat format, String path) {
        switch (format) {
        /*case CAPTURE_FORMAT_HTML:
            Logger.T(TAG, "Capturing current page as HTML archive: " + path);
            mWebView.saveWebArchive(path);
            break;*/
        case CAPTURE_FORMAT_JPEG:
            Logger.T(TAG, "Capturing current page as JPEG image: " + path);
            saveJpeg(path);
            break;
        default:
            //Should never happen
            Logger.E(TAG, "Wrong capture format.");
            break;
        }
    }

    private void saveJpeg(String path) {
        Picture picture = mWebView.capturePicture();

        if ( ( picture == null ) || ( picture.getWidth() == 0 ) || picture.getHeight() == 0 ) {
            Logger.E(TAG, "Can't capture picture from WebView.");
            return;
        }

        Bitmap bitmap = Bitmap.createBitmap( picture.getWidth(), picture.getHeight(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        picture.draw(canvas);
        FileOutputStream fos = null;
        try {
            fos = new FileOutputStream(path);
            if ( fos != null )
            {
                bitmap.compress(Bitmap.CompressFormat.JPEG, 90, fos );
                fos.close();
            }
        }
        catch (Throwable e){
            Logger.E(TAG, e);
        }
    }

    @Override
    public void addJSInterface(Object obj, String name) {
        Logger.I(TAG, "Adding new JS interface: " + name);
        mWebView.addJavascriptInterface(obj, name);
    }
}
