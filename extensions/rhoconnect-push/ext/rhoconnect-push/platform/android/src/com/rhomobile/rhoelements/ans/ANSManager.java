package com.rhomobile.rhoelements.ans;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.ComponentName;

import java.util.List;

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

        Intent intent = createExplicitFromImplicitIntent( context, new Intent(ANSConstants.ANS_REGISTER_ACTION));
        // The pending intent provides a spoof-proof way to pass the
        // package-name string
        intent.putExtra(ANSConstants.ANS_EXTRA_APP, PendingIntent.getBroadcast(context, 0, new Intent(), PendingIntent.FLAG_IMMUTABLE));

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

        try {
            context.startService(intent);
        } catch (IllegalArgumentException e) {
            Logger.E( TAG, "Can't start service to register for ANS push. Probably wasn't able to resolve target for intent" );
        }
    }

    public static void checkRegister(
            Context context,
            String appName,
            String userName,
            String userPassword,
            String session) {

        Logger.T(TAG, "### checkRegister() ###");

        Intent intent = createExplicitFromImplicitIntent( context, new Intent( ANSConstants.ANS_CHECK_REGISTRATION_ACTION) );
        // The pending intent provides a spoof-proof way to pass the
        // package-name string
        intent.putExtra(ANSConstants.ANS_EXTRA_APP, PendingIntent.getBroadcast(context, 0, new Intent(), PendingIntent.FLAG_IMMUTABLE));

        // send the sender ID to the reg service
        intent.putExtra(ANSConstants.ANS_EXTRA_APP_NAME, appName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_NAME, userName);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_PASSWORD, userPassword);
        intent.putExtra(ANSConstants.ANS_EXTRA_USER_SESSION, session);
        Logger.D(TAG, "#####     - checking registration with Intent - " + intent.toString() + " *****");

        try {
            context.startService(intent);
        } catch (IllegalArgumentException e) {
            Logger.E( TAG, "Can't start service to register for ANS push. Probably wasn't able to resolve target for intent" );
        }
    }

    public static void unregister(
            Context context,
            String appName,
            String userName,
            String userPassword,
            String session,
            String notificationServer) {

        Logger.T(TAG, "### unregister() ###");

        Intent intent = createExplicitFromImplicitIntent( context, new Intent(ANSConstants.ANS_UNREGISTER_ACTION) );        

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
        
        try {
            context.startService(intent);
        } catch (IllegalArgumentException e) {
            Logger.E( TAG, "Can't start service to register for ANS push. Probably wasn't able to resolve target for intent" );
        }
    }

    private static Intent createExplicitFromImplicitIntent(Context context, Intent implicitIntent) {
        // Retrieve all services that can match the given intent
        PackageManager pm = context.getPackageManager();
        List<ResolveInfo> resolveInfo = pm.queryIntentServices(implicitIntent, 0);

        // Make sure only one match was found
        if (resolveInfo == null || resolveInfo.size() != 1) {
            Logger.E( TAG, "Can't resolve target service. Check for ANS installed.");
            return implicitIntent;
        }

        // Get component info and create ComponentName
        ResolveInfo serviceInfo = resolveInfo.get(0);
        String packageName = serviceInfo.serviceInfo.packageName;
        String className = serviceInfo.serviceInfo.name;
        ComponentName component = new ComponentName(packageName, className);

        // Create a new intent. Use the old one for extras and such reuse
        Intent explicitIntent = new Intent(implicitIntent);

        // Set the component to be explicit
        explicitIntent.setComponent(component);

        return explicitIntent;
    }
}
