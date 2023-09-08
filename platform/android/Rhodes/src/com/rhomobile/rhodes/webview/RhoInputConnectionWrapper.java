package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.inputmethodservice.InputMethodService;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputConnectionWrapper;
import android.os.Looper;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.ExtractedTextRequest;
import android.view.inputmethod.ExtractedText;




public class RhoInputConnectionWrapper extends InputConnectionWrapper {

    private String TAG = RhoInputConnectionWrapper.class.getSimpleName();

    private IRhoInputConnectionWrapperListener mListener = null;

    public void setListener(IRhoInputConnectionWrapperListener listener) {
        mListener = listener;
        Log.d("myLog", "setRhoInputConnectionWrapperListener ==> rhoInputConnectionWrapper != null ==> setListener");
    }

    public interface IRhoInputConnectionWrapperListener{
        public void onTextInput();
    }

    public RhoInputConnectionWrapper (InputConnection target){
        super(target, true);
    }

    @Override
    public Handler getHandler() {
        Logger.T(this.TAG, ".getHandler()");
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
    public ExtractedText getExtractedText(ExtractedTextRequest request, int flags) {
        Logger.T(this.TAG, ".getExtractedText()");
        try {
            return super.getExtractedText(request, flags);
        } catch (Exception e){
            Log.d("myLog", e.toString());
        }
        return null;
    }

    @Override
    public boolean finishComposingText() {
        Logger.T(this.TAG, ".finishComposingText()");
        
        try{
            return super.finishComposingText();
        }  catch (Exception e){
            Log.d("myLog", e.toString());
        }
        return false;
    }

    @Override
    public boolean commitText(CharSequence text, int newCursorPosition) {
        Log.d("myLog", "================================commitText================================");        
        Log.d("myLog", text.toString());
        Log.d("myLog", "================================");

        if(mListener != null) {
            mListener.onTextInput();
        }
        return super.commitText(text, newCursorPosition);
    }

    @Override
    public boolean sendKeyEvent(KeyEvent event) {
        Log.d("myLog", "================================sendKeyEvent================================");        
        Log.d("myLog", event.toString());
        Log.d("myLog", "================================");
        if(mListener != null) {
            mListener.onTextInput();
        }

        return super.sendKeyEvent(event);
    }



    @Override
    public void closeConnection() {
        Logger.T(this.TAG, ".closeConnection()");
        try {
            super.closeConnection();
        }
        catch (Exception ex) {
            Logger.E(this.TAG, ".closeConnection() ERROR = "+ex.getMessage());
        }
    }

    @Override
        public boolean performEditorAction(int editorAction) {
        Log.d("myLog", "================================sendKeyEvent================================");        
        Log.d("myLog", Integer.toString(editorAction));
        Log.d("myLog", "================================");

        if(mListener != null) {
            mListener.onTextInput();
        }

        return super.performEditorAction(editorAction);
    }

    @Override
    public boolean requestCursorUpdates(int cursorUpdateMode) {
        Logger.T(this.TAG, ".requestCursorUpdates()");
        try {
            return super.requestCursorUpdates(cursorUpdateMode);
        } catch(Exception e){
            Log.d("myLog", e.toString());
        }
        return false;
    }

}