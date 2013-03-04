package com.rho.javascriptvm;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

import android.os.Handler;
import android.view.View;
import android.webkit.WebView;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.util.ContextFactory;

// partially based on https://github.com/lolboxen/AndroidNIWebView/blob/master/NIWebView.java

class JavascriptVMSingleton extends JavascriptVMSingletonBase implements IJavascriptVMSingleton {
    private static final boolean DEBUG = true;

    private static final int SCRIPT_COMPILE_TIMEOUT_IN_MILLISECS = 1 * 1000;
    private static final int SCRIPT_EXECUTION_TIMEOUT_IN_MILLISECS = 10 * 1000;
    private static final String JS_HANDLER_NAME = "JSHandler";

    private List<String> mIds = null;
    private WebView mWebView = null;

    private WebViewResultCallback syncInterface = null;

    public JavascriptVMSingleton(JavascriptVMFactory factory) {
        if (DEBUG) {
            Logger.I(JavascriptVM.JS_TRACE_TAG,"JavascriptVMSingleton");
        }

        List<String> mIds = new LinkedList<String>();
        mIds.add("ScriptEngine");

        Logger.I(JavascriptVM.JS_TRACE_TAG,"Create Handler");

        Handler handler = new Handler();

        handler.post(new Runnable() {
            public void run() {
                if (DEBUG)
                    Logger.I(JavascriptVM.JS_TRACE_TAG,"creating webView");
                mWebView = new WebView(ContextFactory.getUiContext());
                mWebView.setVisibility( View.INVISIBLE );
                mWebView.getSettings().setJavaScriptEnabled(true);

                if (DEBUG)
                    Logger.I(JavascriptVM.JS_TRACE_TAG,"webView :" + mWebView.toString());

                syncInterface = new WebViewResultCallback();
                mWebView.addJavascriptInterface(syncInterface, JS_HANDLER_NAME);
            }
        });
    }

    @Override
    public void executeScript(String script, IMethodResult result) {
        if (DEBUG)
            Logger.I(JavascriptVM.JS_TRACE_TAG,"executeScript");

        String escapedJavascript = script.replaceAll("\\\\", "\\\\\\\\").replaceAll("\\\"", "\\\\\"");
        final String finalCode =
                "javascript:try { " +
                        JS_HANDLER_NAME + ".didCompile();" +
                        JS_HANDLER_NAME + ".setReturnValue(eval(\"" + escapedJavascript + "\"));" +
                        "} catch(err) {" +
                        JS_HANDLER_NAME + ".setReturnValue('');" +
                        "}";
        if (DEBUG)
            Logger.I(JavascriptVM.JS_TRACE_TAG, finalCode);

        Handler handler = new Handler();

        handler.post(new Runnable() {
            public void run() {
                mWebView.loadUrl(finalCode);
            }
        });

        String jsResult = syncInterface.getReturnValue();

        if (DEBUG)
            Logger.I(JavascriptVM.JS_TRACE_TAG,"Result "+ jsResult);

        result.set(jsResult);
    }

    @Override
    public void executeScriptAsync(String script, IMethodResult result) {
        if (DEBUG)
            Logger.I(JavascriptVM.JS_TRACE_TAG,"executeScriptAsync");

        String escapedJavascript = script.replaceAll("\\\\", "\\\\\\\\").replaceAll("\\\"", "\\\\\"");
        final String finalCode =
                "javascript:{" + escapedJavascript + "}";
        if (DEBUG)
            Logger.I(JavascriptVM.JS_TRACE_TAG, finalCode);

        Handler handler = new Handler();

        handler.post(new Runnable() {
            public void run() {
                mWebView.loadUrl(finalCode);
            }
        });
    }
    private class WebViewResultCallback
    {
        private CountDownLatch compileLatch;
        private CountDownLatch finishLatch;
        private String returnValue;

        public String getReturnValue()
        {
            returnValue = "";

            compileLatch = new CountDownLatch(1);
            finishLatch = new CountDownLatch(1);

            try {
                if (!compileLatch.await(SCRIPT_COMPILE_TIMEOUT_IN_MILLISECS, TimeUnit.MILLISECONDS))
                {
                    Logger.E(JavascriptVM.JS_TRACE_TAG, "script did not compile");
                    return returnValue;
                }
            } catch (InterruptedException e) {
                return returnValue;
            }

            try {
                finishLatch.await(SCRIPT_EXECUTION_TIMEOUT_IN_MILLISECS, TimeUnit.MILLISECONDS);
                return returnValue;
            } catch (InterruptedException e) {
                Logger.E(JavascriptVM.JS_TRACE_TAG, "Timed out waiting for JS response");
                throw new RuntimeException("Timed out waiting for JS response");
            }
        }

        @SuppressWarnings("unused")
        public void setReturnValue(String returnValue)
        {
            this.returnValue = returnValue;
            finishLatch.countDown();
        }

        @SuppressWarnings("unused")
        public void didCompile()
        {
            compileLatch.countDown();
        }
    }

    public List<String> getIDs() {
        return mIds;
    }

    protected String getInitialDefaultID() {
        return getIDs().get(0);
    }

    public void enumerate(IMethodResult res) {
        res.set(getIDs());
    }
}