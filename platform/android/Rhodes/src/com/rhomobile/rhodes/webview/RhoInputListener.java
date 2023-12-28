package com.rhomobile.rhodes.webview;

import android.view.accessibility.AccessibilityEvent;

public class RhoInputListener {
    public interface IRhoInputListener{
        public void onTextInput();
        public void onAccessibitityEvent(AccessibilityEvent event);
        public void onRestartBrowser();
    }

    private static IRhoInputListener mListener;

    public static IRhoInputListener getListener() {
        return mListener;
    }

    public static void setListener(IRhoInputListener listener){
        mListener = listener;
    }
}
