package com.rho.clipboard;

import java.util.LinkedList;
import java.util.List;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.text.TextUtils;

import android.content.Context;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import com.rhomobile.rhodes.api.IMethodResult;

class ClipboardSingleton extends ClipboardSingletonBase implements IClipboardSingleton {


    protected static String TAG = ClipboardSingleton.class.getSimpleName();

    private static String sres = null;
    private static boolean sbool = false;


    public ClipboardSingleton(ClipboardFactory factory) {
        super();
    }

    private static Context getContext() {
        Context context = null;
        try {
            context = RhodesService.getContext();
        }
        catch (IllegalStateException e) {
            context = null;
            Logger.E(TAG, "Check Debuggable ERROR : Rhodes main Service (com.rhomobile.rhodes.RhodesService) is not run !!!");
        }
        return context;
    }

    private static boolean hasTextInClipboard() {
        try {
            ClipboardManager clipMan = (ClipboardManager)getContext().getSystemService(Context.CLIPBOARD_SERVICE);
            if (clipMan.hasPrimaryClip()) {
                Logger.T(TAG, "$$$ Clipboard.hasTextInClipboard() clipMan.hasPrimaryClip() == true");
                ClipData cp = clipMan.getPrimaryClip();
                if (cp.getItemCount() > 0) {
                    Logger.T(TAG, "$$$ Clipboard.hasTextInClipboard() cp.getItemCount() > 0");
                    ClipData.Item item = cp.getItemAt(0);
                    if (item.getText() != null) {
                        Logger.T(TAG, "$$$ Clipboard.hasTextInClipboard() item.getText() != null");
                        String text = item.getText().toString();
                        boolean has = !TextUtils.isEmpty(text);
                        if (has) {
                            Logger.T(TAG, "$$$ Clipboard.hasTextInClipboard() return true");
                            return true;
                        }
                    }
                }
            }
        }
        catch (Exception e) {
            Logger.E(TAG, "ERROR : Clipboard.hasTextInClipboard() !!!");
            e.printStackTrace();
        }
        Logger.T(TAG, "$$$ Clipboard.hasTextInClipboard() return false");
        return false;
    }

    public void getText(IMethodResult result) {
        sres = "";
        PerformOnUiThread.exec(new Runnable() {
            public void run() {
                try {
                    Logger.T(TAG, "$$$ Clipboard.getText()");
                    if (hasTextInClipboard()) {
                        ClipboardManager clipMan = (ClipboardManager)getContext().getSystemService(Context.CLIPBOARD_SERVICE);
                        ClipData cp = clipMan.getPrimaryClip();
                        if (cp.getItemCount() > 0) {
                            ClipData.Item item = cp.getItemAt(0);
                            String text = item.getText().toString();
                            sres = text;
                        }
                    }
                }
                catch (Exception e) {
                    Logger.E(TAG, "ERROR : Clipboard.getText() !!!");
                    e.printStackTrace();
                }
            }
        }, true);
        result.set(sres);
    }

    public void hasText(IMethodResult result) {
        sbool = false;
        PerformOnUiThread.exec(new Runnable() {
            public void run() {
                sbool = hasTextInClipboard();
            }
        }, true);
        result.set(sbool);
    }

    public void setText(String text, IMethodResult result) {
        final String stext = text;
        PerformOnUiThread.exec(new Runnable() {
            public void run() {
                try {
                    Logger.T(TAG, "$$$ Clipboard.setText("+stext+")");
                    ClipboardManager clipMan = (ClipboardManager)getContext().getSystemService(Context.CLIPBOARD_SERVICE);
                    ClipData clip = ClipData.newPlainText("text", stext);
                    clipMan.setPrimaryClip(clip);
                }
                catch (Exception e) {
                    Logger.E(TAG, "ERROR : Clipboard.setText() !!!");
                    e.printStackTrace();
                }
            }
        }, true);
    }


    public void clear(IMethodResult result) {
        PerformOnUiThread.exec(new Runnable() {
            public void run() {
                try {
                    Logger.T(TAG, "$$$ Clipboard.clear() ...");
                    ClipboardManager clipMan = (ClipboardManager)getContext().getSystemService(Context.CLIPBOARD_SERVICE);
                    ClipData clip = ClipData.newPlainText("", "");
                    clipMan.setPrimaryClip(clip);
                    // can be removed when Google remove deprecated API
                    //clipMan.setText(null);
                }
                catch (Exception e) {
                    Logger.E(TAG, "ERROR : Clipboard.clear() !!!");
                    e.printStackTrace();
                }
            }
        });
    }


}
