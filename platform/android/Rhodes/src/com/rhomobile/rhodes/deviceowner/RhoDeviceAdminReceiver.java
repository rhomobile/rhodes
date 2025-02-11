package com.rhomobile.rhodes.deviceowner;

import android.app.admin.DeviceAdminReceiver;
import android.app.admin.DevicePolicyManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

import com.rhomobile.rhodes.RhodesActivity;
import android.content.IntentFilter;
import android.app.Activity;


import androidx.annotation.NonNull;

public class RhoDeviceAdminReceiver extends DeviceAdminReceiver {

    public static ComponentName getComponentName(Context context){
        return new ComponentName(context.getApplicationContext(), RhoDeviceAdminReceiver.class);
    }
    public static boolean isDeviceOwner(Activity activity){
        DevicePolicyManager mDevicePolicyManager = (DevicePolicyManager)activity.getSystemService(Context.DEVICE_POLICY_SERVICE);
        return mDevicePolicyManager.isDeviceOwnerApp(activity.getPackageName());
    }

    public static void cleanDeviceOwner(Activity activity){
        DevicePolicyManager mDevicePolicyManager = (DevicePolicyManager)activity.getSystemService(Context.DEVICE_POLICY_SERVICE);
        mDevicePolicyManager.clearDeviceOwnerApp(activity.getPackageName());
    }

    public static void setHomeLauncher(Activity activity, String packageName){
        DevicePolicyManager dpm = (DevicePolicyManager)activity.getSystemService(Context.DEVICE_POLICY_SERVICE);
        Context context = (Context) activity;
        ComponentName adminComponentName = new ComponentName(context, RhoDeviceAdminReceiver.class);
        dpm.clearPackagePersistentPreferredActivities(adminComponentName, packageName);
        IntentFilter filter = new IntentFilter(Intent.ACTION_MAIN);
        filter.addCategory(Intent.CATEGORY_HOME);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        ComponentName rhodesActivity = new ComponentName(context.getPackageName(), RhodesActivity.class.getName());
        dpm.addPersistentPreferredActivity(adminComponentName, filter, rhodesActivity);

        disabledKeyguard(dpm, adminComponentName);
    }

    private static void disabledKeyguard(DevicePolicyManager dpm, ComponentName adminComponentName){
        dpm.setKeyguardDisabledFeatures(
            adminComponentName,
            DevicePolicyManager.KEYGUARD_DISABLE_FEATURES_ALL
        );
    }

    @Override
    public void onEnabled(@NonNull Context context, @NonNull Intent intent) {
        super.onEnabled(context, intent);
        Toast.makeText(context, "device isAdmin", Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onDisabled(@NonNull Context context, @NonNull Intent intent) {
        super.onDisabled(context, intent);
        Toast.makeText(context, "device is NOT Admin", Toast.LENGTH_SHORT).show();
    }
}
