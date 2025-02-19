package com.rhomobile.rhodes.osfunctionality;

import androidx.annotation.RequiresApi;
import android.app.Notification.Builder;
import android.content.Context;
import android.app.NotificationManager;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;

import android.os.Build.VERSION;

import android.app.Activity;
import android.content.Intent;
import android.content.ComponentName;

@RequiresApi(28)
public class AndroidFunctionality28 extends AndroidFunctionality26 implements AndroidFunctionality {

    private static final String TAG = AndroidFunctionality28.class.getSimpleName();

    @Override
	public ComponentName startForegroundService(Activity activity, Intent service) {
        Logger.D( TAG, "startForegroundService() START" );
        java.lang.reflect.Method methodStartForegroundService = null;
        ComponentName resultOfInvoke = null;

        if (RhoConf.isExist("using_start_foreground_service") && RhoConf.getInt("using_start_foreground_service") == 1){
            try {
                methodStartForegroundService = activity.getClass().getMethod("startForegroundService", Intent.class);
            } catch( Exception e ) {
                Logger.E( TAG, "Error: Activity class do not has startForegroundService() method !" );
                Logger.E( TAG, e.toString() );
            }
            if (methodStartForegroundService!= null) {
                try {
                    Logger.D( TAG, "invoke startForegroundService()" );
                    resultOfInvoke = (ComponentName)methodStartForegroundService.invoke(activity, service);
                } catch( Exception e ) {
                    Logger.E( TAG, "Error: some error during invoke startForegroundService() method !" );
                    Logger.E( TAG, e.toString() );
                }
                Logger.D( TAG, "startForegroundService() FINISH" );
                return resultOfInvoke;        
            }
        }

        Logger.D( TAG, "use old startService()" );
        resultOfInvoke = activity.startService(service);
        Logger.D( TAG, "startForegroundService() FINISH" );
        return resultOfInvoke;

        // try {
        //     methodStartForegroundService = activity.getClass().getMethod("startForegroundService", Intent.class);
        // } catch( Exception e ) {
        //     Logger.E( TAG, "Error: Activity class do not has startForegroundService() method !" );
        //     Logger.E( TAG, e.toString() );
        // }
        // if (methodStartForegroundService!= null) {
        //     try {
        //         Logger.D( TAG, "invoke startForegroundService()" );
        //         resultOfInvoke = (ComponentName)methodStartForegroundService.invoke(activity, service);
        //     } catch( Exception e ) {
        //         Logger.E( TAG, "Error: some error during invoke startForegroundService() method !" );
        //         Logger.E( TAG, e.toString() );
        //     }
        // }
        // else {
        //     Logger.D( TAG, "use old startService()" );
        //     resultOfInvoke = activity.startService(service);
        // }
        // Logger.D( TAG, "startForegroundService() FINISH" );
        // return resultOfInvoke;
	}

}
