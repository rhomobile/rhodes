package com.rhomobile.rhodes.webview;

public class RhoInputListener {
    public interface IRhoInputListener{
        public void onTextInput();
    }
    
    private static IRhoInputListener mListener;

    public static IRhoInputListener getListener() {
        return mListener;
    }

    public static void setListener(IRhoInputListener listener){
        mListener = listener;
    }
}
