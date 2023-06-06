package com.rhomobile.rhodes.kioskservices;

import android.accessibilityservice.AccessibilityService;
import android.util.Log;
import android.view.accessibility.AccessibilityEvent;
import android.widget.Toast;
import android.app.Notification;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhoConf;



public class MyAccessibilityService extends AccessibilityService {
    private CharSequence oldEvent;
    private BroadcastReceiver receiver;
    private static final int NOTIFICATION_ID = 1234;
    private boolean filter = true;
    private CharSequence powerPackageName;
    public static boolean isPowerProcessing = false;

    public static void setPowerProcessing(boolean value) {
        isPowerProcessing = value;
    }


    @Override
    protected void onServiceConnected() {
        super.onServiceConnected();

        receiver = new BroadcastReceiver(){
            @Override
            public void onReceive(Context context, Intent intent){
                String action = intent.getAction();
                if( action != null && 
                    action.equals("com.rhobrowser.poweroff") &&
                    isPowerProcessing)
                {
                    powerOff();
                }
            }
        };
        IntentFilter filter = new IntentFilter("com.rhobrowser.poweroff");
        LocalBroadcastManager.getInstance(this).registerReceiver(receiver, filter);
        

        Toast.makeText(this, "Accessibility Service connected", Toast.LENGTH_LONG).show();
    }

    @Override
    public void onAccessibilityEvent(AccessibilityEvent event) {
        //Log.d("myLog", "Kiosk status: "+(KioskManager.getKioskModeStatus()?"true":"false")+"; event: "+ event.toString());
        Log.d("myLog1", event.toString());
        Log.d("myLog1", "filter: " + filter);
        Log.d("myLog1", "================================================================");

        if (filter){

            if( event.getEventType() == AccessibilityEvent.TYPE_WINDOW_CONTENT_CHANGED ||
                event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED)
            {
                        
                if( KioskManager.getKioskModeStatus() && 
                    event.getPackageName() != null && 
                    !event.getPackageName().equals(getPackageName()))
                {
                    performGlobalAction(GLOBAL_ACTION_HOME);
                    if(event.getPackageName().equals(oldEvent)) {
                        //Toast.makeText(this, "package: " + event.getPackageName(), Toast.LENGTH_SHORT).show();
                        oldEvent = event.getPackageName();
                    }
                }
            }

        } else {
            if (powerPackageName == null && event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED){
                powerPackageName = event.getPackageName();
            }

            Log.d("TESTETST", getPackageName());

            if( (event.getEventType() == AccessibilityEvent.TYPE_VIEW_CLICKED && 
                !event.getPackageName().equals("com.tau.taubrowser")) || 
                (event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED &&
                event.getPackageName() != powerPackageName ))
            {
                Log.d("myLog1", "FILTER ON");
                filter = true;
                powerPackageName = null;
            }
        }
    }

    private void powerOff(){
        Log.d("myLog1", "FILTER OFF");
        filter = false;
        performGlobalAction(GLOBAL_ACTION_POWER_DIALOG);
    }

    @Override
    public void onInterrupt() {

    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if(isPowerProcessing)
            LocalBroadcastManager.getInstance(this).unregisterReceiver(receiver);
    }
}
