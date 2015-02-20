/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.webview;

import android.app.Activity;
import android.view.Window;
import android.webkit.GeolocationPermissions;
import android.webkit.JsPromptResult;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebStorage;
import android.webkit.WebView;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.RhoExtManager;


public class RhoWebChromeClient extends WebChromeClient {
    private static final String TAG = RhoWebChromeClient.class.getSimpleName();
    
    private class AlertResult implements IRhoExtension.IAlertResult {
        private boolean mPending = false;
        private JsResult mResult;
        public AlertResult(JsResult result) {
            mResult = result;
        }
        @Override
        public void setPending() {
            mPending = true;
        }
        @Override
        public void confirm() {
            mResult.confirm();
        }
        @Override
        public void cancel() {
            mResult.cancel();
        }
        public boolean isPending() {
            return mPending;
        }
    }

    private class PromptResult implements IRhoExtension.IPromptResult {
        private boolean mPending = false;
        private JsPromptResult mResult;
        public PromptResult(JsPromptResult result) {
            mResult = result;
        }
        @Override
        public void setPending() {
            mPending = true;
        }
        @Override
        public void confirm(String message) {
            mResult.confirm(message);
        }
        @Override
        public void cancel() {
            mResult.cancel();
        }
        public boolean isPending() {
            return mPending;
        }
    }

    private Activity mActivity;
    private GoogleWebView mWebView;

    public RhoWebChromeClient(Activity activity, GoogleWebView webView) {
        mActivity = activity;
        mWebView = webView;
    }
    
    public void onConsoleMessage(String message, int lineNumber, String sourceID) {
        Logger.I(TAG, message + " -- From line " + lineNumber + " of " + sourceID);
    }

    public void onExceededDatabaseQuota(String url, String databaseIdentifier, long currentQuota, long estimatedSize,
            long totalUsedQuota, WebStorage.QuotaUpdater quotaUpdater) {
            quotaUpdater.updateQuota(estimatedSize * 2);
    }

//    public void onShowCustomView(View view, CustomViewCallback callback) {
//        Logger.I(TAG, "onShowCustomView");
//        if(mMainView == null) {
//            Logger.E(TAG, "Cannot handle custom view, SimpleMainView still uninitialized");
//            return;
//        }
//
//        if (view instanceof FrameLayout){
//            Logger.I(TAG, "FrameLayout is detected");
//            FrameLayout frame = (FrameLayout) view;
//            
//            //frame.
//            
//            if (frame.getFocusedChild() instanceof VideoView) {
//                Logger.I(TAG, "VideoView is detected");
//                RhoVideoView rhoVideoView = new RhoVideoView(frame, callback);
//                mMainView.setCustomView(rhoVideoView);
//            }
//        }
//    }

    public void onProgressChanged(WebView view, int newProgress) {
        if (mWebView.getConfig() != null && mWebView.getConfig().getBool(WebViewConfig.ENABLE_PAGE_LOADING_INDICATION)) {
            newProgress *= 100;
            if (newProgress < 0)
                newProgress = 0;
            if (newProgress > RhodesActivity.MAX_PROGRESS)
                newProgress = RhodesActivity.MAX_PROGRESS;
            mActivity.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, newProgress);
        } else {
            newProgress = RhodesActivity.MAX_PROGRESS;
        }
        super.onProgressChanged(view, newProgress);
    }
    
    public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
        AlertResult alertResult = new AlertResult(result);
        RhoExtManager.getImplementationInstance().onAlert(view, message, alertResult);
        return alertResult.isPending();
    }
    
    public boolean onJsConfirm(WebView view, String url, String message, JsResult result) {
        AlertResult alertResult = new AlertResult(result);
        RhoExtManager.getImplementationInstance().onConfirm(view, message, alertResult);
        return alertResult.isPending();
    }

    public boolean onJsPrompt (WebView view, String url, String message, String defaultValue, final JsPromptResult result) {
        PromptResult promptResult = new PromptResult(result);
        RhoExtManager.getImplementationInstance().onPrompt(view, message, defaultValue, promptResult);
        Logger.D(TAG, "JS Prompt is processing by rhodes: " + promptResult.isPending());
        return promptResult.isPending();
    }

    public void onGeolocationPermissionsShowPrompt(String origin, GeolocationPermissions.Callback callback) {
        callback.invoke(origin, true, false);
    }

}


