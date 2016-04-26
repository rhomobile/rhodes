package com.rhomobile.rhoelements.ans;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import com.symbol.rhoconnect.pushservice.ANSConstants;
import com.rhomobile.rhodes.Logger;

public class ANSManager {

    private static final String TAG = "ANSManager";
    private static final boolean DEBUG = false;

    // Routines to register and unregister from the ANS service
    public static synchronized void register(
            Context context,
            String appName,
            String userName,
            String userPassword,
            String session,
            String notificationServer) {

        Logger.T(TAG, "### register() ###");

        Intent intent = new Intent(ANSConstants.ANS_REGISTER_ACTION);
        // The pending intent provides a spoof-proof way to pass the
        // package-name string
        intent.putExtra(ANSConstants.ANS_EXTRA_APP, PendingIntent.getBroadcast(context, 0, new Intent(), 0));

        // send the sender ID to the reg service
        intent.putExtra(ANSConstants.ANS_EXTRA_APP_NAME, appName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_NAME, userName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_PASSWORD, userPassword);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_SESSION, session);
        intent.putExtra(ANSConstants.ANS_EXTRA_SERVER_URL, notificationServer);

        if (DEBUG) {
            Logger.D(TAG, "#####     - registering with Intent - " + intent.toString() + " *****");
            for(String key: intent.getExtras().keySet()) {
                Logger.D(TAG, key + "=" + intent.getExtras().getByte(key));
            }
        }
        
        context.startService(intent);
    }

    public static void checkRegister(
            Context context,
            String appName,
            String userName,
            String userPassword,
            String session) {

        Logger.T(TAG, "### checkRegister() ###");

        Intent intent = new Intent(
                ANSConstants.ANS_CHECK_REGISTRATION_ACTION);
        // The pending intent provides a spoof-proof way to pass the
        // package-name string
        intent.putExtra(ANSConstants.ANS_EXTRA_APP,
                PendingIntent.getBroadcast(context, 0, new Intent(), 0));
        // send the sender ID to the reg service
        intent.putExtra(ANSConstants.ANS_EXTRA_APP_NAME, appName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_NAME, userName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_PASSWORD, userPassword);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_SESSION, session);
        Logger.D(TAG, "#####     - checking registration with Intent - " + intent.toString() + " *****");
        context.startService(intent);
    }

    public static void unregister(
            Context context,
            String appName,
            String userName,
            String userPassword,
            String session,
            String notificationServer) {

        Logger.T(TAG, "### unregister() ###");

        Intent intent = new Intent(ANSConstants.ANS_UNREGISTER_ACTION);
        // The pending intent provides a spoof-proof way to pass the
        // package-name string
        intent.putExtra("app", PendingIntent.getBroadcast(context, 0, new Intent(), 0));
        // The sender ID needs to be passed along for identification purposes
        intent.putExtra(ANSConstants.ANS_EXTRA_APP_NAME, appName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_NAME, userName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_PASSWORD, userPassword);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_SESSION, session);
        intent.putExtra(ANSConstants.ANS_EXTRA_SERVER_URL, notificationServer);
        Logger.D(TAG, "#####     - unregistering with Intent - " + intent.toString() + " *****");
        context.startService(intent);
    }
}
