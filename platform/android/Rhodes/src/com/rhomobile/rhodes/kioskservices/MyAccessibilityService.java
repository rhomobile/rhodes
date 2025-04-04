package com.rhomobile.rhodes.kioskservices;

import java.util.Arrays;
import java.util.List;

import android.accessibilityservice.AccessibilityService;
import android.util.Log;
import android.view.accessibility.AccessibilityEvent;
import android.widget.Toast;
import android.app.Notification;
import android.app.ActivityManager;
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
import com.rhomobile.rhodes.RhodesService;

import com.rhomobile.rhodes.kioskservices.PermissionManager;

//import com.rho.zebrakiosk.ZebraHomeLauncherTool;
import java.lang.reflect.Method;


public class MyAccessibilityService extends AccessibilityService {

    private static final String TAG = MyAccessibilityService.class.getSimpleName();

    private CharSequence oldEvent = null;
    private BroadcastReceiver receiver = null;
    private static final int NOTIFICATION_ID = 1234;
    private boolean filter = true;
    private CharSequence powerPackageName;

    private boolean kioskEnabledWithStart = false;

    public static boolean isPowerProcessing = false;
    private static List<String> ignorePackets = null;
    private static boolean isEnabled = false;

    private static boolean isCheckLauncherAfterAccessibilityEvent = true;
    private static boolean isUseTaskManagerBlockOtherTasks = false;


    private static int ourRhodesActivityID = 0;


    public static void setRhodesActivityID(int id) {
        ourRhodesActivityID = id;
    }

    public static void setUseTaskManagerForBlockOtherTasks(boolean use_flag) {
        isUseTaskManagerBlockOtherTasks = use_flag;
    }

    private void doReturnToRhodesActivity() {
        try {
            //performGlobalAction(GLOBAL_ACTION_HOME);
            if (PermissionManager.isIgnoreHomeLauncherCheckPermission() || isUseTaskManagerBlockOtherTasks) {
                if (ourRhodesActivityID != 0) {
                    ActivityManager am = (ActivityManager)getSystemService( Context.ACTIVITY_SERVICE );
                    am.moveTaskToFront( ourRhodesActivityID, 0, null );
                }
                Intent intent = new Intent(this, com.rhomobile.rhodes.RhodesActivity.class);
                startActivity(intent);
            }
            else {
                performGlobalAction(GLOBAL_ACTION_HOME);
            }
        } catch (Exception ignored) {

            Logger.E(TAG, "Error during get zebrakiosk class = "+ignored.getMessage());
        }
    }

    private void forceHomeLauncher() {
        try {
            Class<?> c = Class.forName("com.rho.zebrakiosk.ZebraHomeLauncherTool");
            Method h = c.getMethod("setHomeLauncher", String.class, Context.class);
            h.invoke(c, getPackageName(), getApplicationContext());

            //ZebraHomeLauncherTool.setHomeLauncher("com.tau.taubrowser", getApplicationContext());
        } catch (Exception ignored) {

            Logger.E(TAG, "Error during get zebrakiosk class = "+ignored.getMessage());
        }
    }

    public static void setPowerProcessing(boolean value) {
        isPowerProcessing = value;
    }

    public static void disabledCheckLauncher(){
        isCheckLauncherAfterAccessibilityEvent = false;
    }

    public static void setIgnoreEventsFromPackets(String packets){
        try {
            ignorePackets = Arrays.asList(packets.split(";"));
        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }
    }

    private boolean isIgnorPackages(CharSequence eventPackageName){
        try {
            if(ignorePackets != null && ignorePackets.size() > 0){
                if(ignorePackets.indexOf(eventPackageName.toString()) >= 0)
                    return false;
            }
        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }

        return true;
    }

    @Override
    public void onCreate() {
        Log.d(TAG, "Accessibility service created");


        try {
            kioskEnabledWithStart = KioskManager.isKioskModeOnStartForAccessebility(getApplicationContext());
            KioskManager.decreaseKioskModeOnStartForAccessebility(getApplicationContext());


        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }
        Log.d(TAG, "kioskEnabledWithStart: " + ( kioskEnabledWithStart ? "true" : "false" ));

        if (kioskEnabledWithStart) {
            forceHomeLauncher();
        }

        super.onCreate();
    }

    @Override
    protected void onServiceConnected() {
        try {
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

        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }

        isEnabled = true;


        //Toast.makeText(this, "Accessibility Service connected", Toast.LENGTH_LONG).show();

        Log.d(TAG, "Accessibility service connected");



        if (kioskEnabledWithStart) {
            forceHomeLauncher();
            //RhodesService.beep();
            //performGlobalAction(GLOBAL_ACTION_HOME);
        }

    }

    public static boolean getStatus(){
        return isEnabled;
    }

    @Override
    public void onAccessibilityEvent(AccessibilityEvent event) {
        try {


            //RhodesService.beep_mini();

            if (event == null) {
                Logger.T(this.TAG, "===================== onAccessibilityEvent ====================");
                Logger.T(this.TAG, "event == null !");
                Logger.T(this.TAG, "filter: " + filter);
                Logger.T(this.TAG, "================================================================");
                return;
            }

            if (isCheckLauncherAfterAccessibilityEvent) {
                if (!PermissionManager.isMyLauncherDefault(getApplicationContext())){
                    Logger.T(this.TAG, "===================== onAccessibilityEvent ====================");
                    Logger.T(this.TAG, "============  Tau Browser is not Home Launcher !!! ============");
                    Logger.T(this.TAG, "===================== onAccessibilityEvent ====================");
                    if (kioskEnabledWithStart || KioskManager.getKioskModeStatus()) {
                        forceHomeLauncher();
                    }
                    //return;
                }
            }

            //Log.d("myLog", "Kiosk status: "+(KioskManager.getKioskModeStatus()?"true":"false")+"; event: "+ event.toString());
            Logger.T(this.TAG, "===================== onAccessibilityEvent ====================");
            Logger.T(this.TAG, event.toString());
            Logger.T(this.TAG, "filter: " + filter);
            Logger.T(this.TAG, "================================================================");

            if (filter){

                if( event.getEventType() == AccessibilityEvent.TYPE_WINDOW_CONTENT_CHANGED ||
                    event.getEventType() == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED)
                {

                    //kioskEnabledWithStart = KioskManager.GetKioskModeEnabledFilteringEventsOnStart(getApplicationContext());

                    if( ( kioskEnabledWithStart || KioskManager.getKioskModeStatus() ) &&
                        event.getPackageName() != null &&
                        !event.getPackageName().equals(getPackageName()) &&
                        isIgnorPackages(event.getPackageName()))
                    {
                        Logger.T(this.TAG, "====================    GLOBAL_ACTION_HOME    =======================");
                        //RhodesService.beep_mini();
                        doReturnToRhodesActivity();
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
        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }

    }

    protected boolean onKeyEvent (KeyEvent event) {
        try {
            if (event != null) {
                Logger.E(this.TAG, "$$$## KEY CODE = ["+String.valueOf(event.getKeyCode())+"]");
            }
            else {
                Logger.E(this.TAG, "$$$## KEY CODE = [event is null !]");
            }
            return super.onKeyEvent(event);
        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }
        return false;
    }

    private void playSound(AccessibilityEvent event){
        try {
            RhoInputListener.IRhoInputListener listener = RhoInputListener.getListener();
            if (listener != null) {
                listener.onTextInput();
                listener.onAccessibitityEvent(event);
            }
        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }
    }

    private void powerOff(){
        try {
            Log.d("myLog1", "FILTER OFF");
            filter = false;
            performGlobalAction(GLOBAL_ACTION_POWER_DIALOG);
        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }
    }

    @Override
    public void onInterrupt() {

    }

    @Override
    public void onDestroy() {
        try {
            super.onDestroy();
            if(isPowerProcessing)
                LocalBroadcastManager.getInstance(this).unregisterReceiver(receiver);
        } catch(Exception e) {
            Logger.E(TAG, "Exception in AccessibilityService : " + e.getMessage() + "\n");
            e.printStackTrace();
        }
    }
}
