package com.rhomobile.rhodes.kioskservices;

import java.util.Arrays;
import java.util.List;

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
import android.view.KeyEvent;

import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.webview.RhoInputListener;
import com.rhomobile.rhodes.Logger;


public class MyAccessibilityService extends AccessibilityService {

    private static final String TAG = MyAccessibilityService.class.getSimpleName();

    private CharSequence oldEvent = null;
    private BroadcastReceiver receiver = null;
    private static final int NOTIFICATION_ID = 1234;
    private boolean filter = true;
    private CharSequence powerPackageName;
    public static boolean isPowerProcessing = false;
    private static List<String> ignorePackets = null;
    private static boolean isEnabled = false;

    public static void setPowerProcessing(boolean value) {
        isPowerProcessing = value;
    }

    public static void setIgnoreEventsFromPackets(String packets){
        ignorePackets = Arrays.asList(packets.split(";"));
    }

    private boolean isIgnorPackages(CharSequence eventPackageName){
        if(ignorePackets != null && ignorePackets.size() > 0){
            if(ignorePackets.indexOf(eventPackageName.toString()) >= 0)
                return false;
        }
        return true;
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
        isEnabled = true;


        //Toast.makeText(this, "Accessibility Service connected", Toast.LENGTH_LONG).show();
    }

    public static boolean getStatus(){
        return isEnabled;
    }

    @Override
    public void onAccessibilityEvent(AccessibilityEvent event) {
        //Log.d("myLog", "Kiosk status: "+(KioskManager.getKioskModeStatus()?"true":"false")+"; event: "+ event.toString());
        Logger.T(this.TAG, "===================== onAccessibilityEvent ====================");
        Logger.T(this.TAG, event.toString());
        Logger.T(this.TAG, "filter: " + filter);
        Logger.T(this.TAG, "================================================================");

        if (filter){

            if( event.getEventType() == AccessibilityEvent.TYPE_WINDOW_CONTENT_CHANGED ||
                event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED)
            {

                if( KioskManager.getKioskModeStatus() &&
                    event.getPackageName() != null &&
                    !event.getPackageName().equals(getPackageName()) &&
                    isIgnorPackages(event.getPackageName()))
                {
                    Logger.T(this.TAG, "====================    GLOBAL_ACTION_HOME    =======================");
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

            Logger.T(this.TAG, "Package name = "+getPackageName());

            if( (event.getEventType() == AccessibilityEvent.TYPE_VIEW_CLICKED &&
                !event.getPackageName().equals(getPackageName()) && !event.getPackageName().equals(powerPackageName)) ||
                (event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED &&
                event.getPackageName() != powerPackageName ))
            {
                Logger.T(this.TAG, "FILTER ON");
                filter = true;
                powerPackageName = null;
            }
        }

        if( RhoConf.isExist("click_sound_with_accessibility_service") &&
            RhoConf.getBool("click_sound_with_accessibility_service")){
            Logger.T(this.TAG, "click_sound_with_accessibility_service");
            Logger.T(this.TAG, "event.getClassName() = "+event.getClassName());
            Logger.T(this.TAG, "Package name = "+getPackageName());
            if (event.getEventType() == AccessibilityEvent.TYPE_VIEW_TEXT_CHANGED &&
                event.getSource() != null &&
                event.getPackageName().equals(getPackageName()) &&
                event.getClassName().equals("android.widget.EditText")){
                    playSound(event);
            }
        }
    }

    protected boolean onKeyEvent (KeyEvent event) {
        Logger.E(this.TAG, "$$$## KEY CODE = ["+String.valueOf(event.getKeyCode())+"]");
        return super.onKeyEvent(event);
    }

    private void playSound(AccessibilityEvent event){
        RhoInputListener.IRhoInputListener listener = RhoInputListener.getListener();
        if (listener != null) {
            listener.onTextInput();
            listener.onAccessibitityEvent(event);
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
