package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.mapview.TouchHandler;

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

import android.widget.Toast;




public class RhoInputConnectionWrapper extends InputConnectionWrapper {

    private String TAG = RhoInputConnectionWrapper.class.getSimpleName();

    public RhoInputConnectionWrapper (InputConnection target){
        super(target, true);
    }

    @Override
    public boolean commitText(CharSequence text, int newCursorPosition) {
        Logger.T(this.TAG, "================================commitText================================");
        Logger.T(this.TAG, text.toString());
        Logger.T(this.TAG, "================================");

        playSound();

        return super.commitText(text, newCursorPosition);
    }

    @Override
    public boolean sendKeyEvent(KeyEvent event) {
        Logger.T(this.TAG, "================================sendKeyEvent================================");
        Logger.T(this.TAG, event.toString());
        Logger.T(this.TAG, "================================");
        playSound();

        return super.sendKeyEvent(event);
    }

    private void playSound(){
        RhoInputListener.IRhoInputListener listener = RhoInputListener.getListener();
        if (listener != null) {
            listener.onTextInput();
        }
    }


    //region compulsory methods

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
            Logger.E(this.TAG, e.toString());
        }
        return null;
    }

    @Override
    public boolean finishComposingText() {
        Logger.T(this.TAG, ".finishComposingText()");

        try{
            return super.finishComposingText();
        }  catch (Exception e){
            Logger.E(this.TAG, e.toString());
        }
        return false;
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
        Logger.T(this.TAG, "================================sendKeyEvent================================");
        Logger.T(this.TAG, Integer.toString(editorAction));
        Logger.T(this.TAG, "================================");
        return super.performEditorAction(editorAction);
    }

    @Override
    public boolean requestCursorUpdates(int cursorUpdateMode) {
        Logger.T(this.TAG, ".requestCursorUpdates()");
        try {
            return super.requestCursorUpdates(cursorUpdateMode);
        } catch(Exception e){
            Logger.E(this.TAG, e.toString());
        }
        return false;
    }
    //endregion


    @Override
    public boolean reportFullscreenMode(boolean enabled) {
        Logger.T(this.TAG, ".reportFullscreenMode()");
        boolean h = false;
        try {
            h = super.reportFullscreenMode(enabled);
        }
        catch (Exception ex) {
            Logger.E(this.TAG, ".reportFullscreenMode() ERROR = "+ex.getMessage());
        }
        return h;
    }

}
