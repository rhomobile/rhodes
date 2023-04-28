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



import androidx.annotation.Nullable;

import com.rhomobile.rhodes.R;

public class MyOverlayService extends Service {
    private WindowManager wm;
    private LinearLayout overlayLayout;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        wm = (WindowManager) getSystemService(WINDOW_SERVICE);

        overlayLayout = new LinearLayout(this);
        overlayLayout.setOrientation(LinearLayout.VERTICAL);
        overlayLayout.setBackgroundColor(0xFFFFFFFF);

        Button btnClose = new Button(this);
        btnClose.setText("STOP OVERLAY");
        btnClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopSelf();
            }
        });

        LinearLayout.LayoutParams lparams = new LinearLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT
        );
        lparams.topMargin = 8;
        lparams.bottomMargin = 8;
        lparams.leftMargin = 8;
        lparams.rightMargin = 8;

        overlayLayout.addView(btnClose, lparams);


        WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.MATCH_PARENT,
                WindowManager.LayoutParams.MATCH_PARENT,
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
                WindowManager.LayoutParams.FLAG_FULLSCREEN |
                        WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED |
                        WindowManager.LayoutParams.FLAG_DISMISS_KEYGUARD,
                PixelFormat.TRANSLUCENT
        );
        wm.addView(overlayLayout, params);

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        wm.removeView(overlayLayout);
        overlayLayout = null;
    }
}
