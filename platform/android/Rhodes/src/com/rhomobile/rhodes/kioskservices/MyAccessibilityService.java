package com.rhomobile.rhodes.kioskservices;

import android.accessibilityservice.AccessibilityService;
import android.util.Log;
import android.view.accessibility.AccessibilityEvent;
import android.widget.Toast;
import android.app.Notification;
import com.rhomobile.rhodes.R;


public class MyAccessibilityService extends AccessibilityService {
    private CharSequence oldEvent;
    private static final int NOTIFICATION_ID = 1234;


    @Override
    protected void onServiceConnected() {
        super.onServiceConnected();


        Toast.makeText(this, "Accessibility Service connected", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onAccessibilityEvent(AccessibilityEvent event) {
        Log.d("myLog", "Kiosk status: "+(KioskManager.getKioskModeStatus()?"true":"false")+"; event: "+ event.toString());
        if(event.getEventType() == AccessibilityEvent.TYPE_WINDOW_CONTENT_CHANGED ||
                event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED){
            if(KioskManager.getKioskModeStatus()) {
                if (!event.getPackageName().equals(getPackageName())) {
                    performGlobalAction(GLOBAL_ACTION_HOME);
                    if(event.getPackageName().equals(oldEvent)) {
                        Toast.makeText(this, "package: " + event.getPackageName(), Toast.LENGTH_SHORT).show();
                        oldEvent = event.getPackageName();
                    }
                }
            }
            
        }
    }

    @Override
    public void onInterrupt() {

    }
}
