package com.rhomobile.rhodes.kioskservices;

import android.app.Service;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.IBinder;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.FrameLayout;
import android.util.Log;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.mainview.MainView;

import android.view.Gravity;
import java.util.Vector;

import com.rhomobile.rhodes.Logger;

import androidx.annotation.Nullable;

import com.rhomobile.rhodes.R;

public class MyOverlayService extends Service {

    private final static String TAG = "MyOverlayService";

    private WindowManager wm;
    private LinearLayout overlayLayout;
    private FrameLayout mFrameLayout = null;

    private MainView mMainView = null;

    private static MyOverlayService ourInstance = null;

    private static boolean ourIsOverlayModeEnabled = false;



    public interface MyOverlayServiceListener {
        void onOverlayStart(MyOverlayService overlayService, FrameLayout mainLayout);
        void onOverlayStop(MyOverlayService overlayService, FrameLayout mainLayout);
    }

    private static Vector<MyOverlayServiceListener> ourListeners = new Vector<MyOverlayServiceListener>(1);

    public static MyOverlayService getInstance() {
        return ourInstance;
    }

    public static void addListener(MyOverlayServiceListener listener) {
        ourListeners.addElement(listener);
    }

    private void callListsnersOnStart() {
        for(int i=0; i < ourListeners.size(); ++i) {
            ourListeners.get(i).onOverlayStart(this, mFrameLayout);
        }
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }


    public static boolean isOverlayMode() {
        return ourIsOverlayModeEnabled;
    }



    public void stopOverlayMode() {
        ourInstance = null;
        ourIsOverlayModeEnabled = false;
        if (mMainView != null) {
            mFrameLayout.removeAllViews();
            overlayLayout.removeAllViews();
            RhodesActivity.safeGetInstance().setMainView(mMainView);
        }
        stopSelf();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Logger.T(TAG, "$$$ onStartCommand() ");
        ourInstance = this;
        ourIsOverlayModeEnabled = true;

        wm = (WindowManager) getSystemService(WINDOW_SERVICE);

        overlayLayout = new LinearLayout(this);
        overlayLayout.setOrientation(LinearLayout.VERTICAL);
        overlayLayout.setBackgroundColor(0xFFFFFFFF);

        // Button btnClose = new Button(this);
        // btnClose.setText("STOP OVERLAY");
        // btnClose.setOnClickListener(new View.OnClickListener() {
        //     @Override
        //     public void onClick(View v){
        //         stopOverlayMode();
        //     }
        // });

        LinearLayout.LayoutParams lparams = new LinearLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT
        );
        lparams.topMargin = 8;
        lparams.bottomMargin = 8;
        lparams.leftMargin = 8;
        lparams.rightMargin = 8;

        // overlayLayout.addView(btnClose, lparams);

/*
        |
                WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED |
                WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD 
*/

        WindowManager.LayoutParams params = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.MATCH_PARENT,
            WindowManager.LayoutParams.MATCH_PARENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            PixelFormat.TRANSLUCENT
        );

        mFrameLayout = new FrameLayout(this);


        FrameLayout.LayoutParams wvlparams = new FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.MATCH_PARENT,
            FrameLayout.LayoutParams.MATCH_PARENT
        );

        mMainView = RhodesActivity.extractMainView();
        if(mMainView != null){
            if(mMainView != null){
                Logger.T(TAG, "$$$ mMainView != NULL ! class = "+mMainView.getClass().getSimpleName());
            }else{
                Logger.T(TAG, "$$$ mMainView == NULL ! ");
            }

            mFrameLayout.addView(mMainView.getView(), wvlparams);
        }

        callListsnersOnStart();

        overlayLayout.addView(RhodesActivity.onOverlayStarted(mFrameLayout), lparams);


        wm.addView(overlayLayout, params);

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        Log.d("myLog", "Overlay stop");
        Logger.T(TAG, "OverLay Stop");
        ourIsOverlayModeEnabled = false;
        super.onDestroy();
        wm.removeView(overlayLayout);
        overlayLayout = null;
    }
}
