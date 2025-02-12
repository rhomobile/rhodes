package com.rhomobile.rhodes.deviceowner;

import android.app.admin.DeviceAdminReceiver;
import android.app.admin.DevicePolicyManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.kioskservices.PermissionManager;

import android.content.IntentFilter;
import android.provider.Settings;

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

    public static void setHomeLauncher(boolean status, Activity activity, String packageName){
        DevicePolicyManager dpm = (DevicePolicyManager)activity.getSystemService(Context.DEVICE_POLICY_SERVICE);
        IntentFilter filter = new IntentFilter(Intent.ACTION_MAIN);
        filter.addCategory(Intent.CATEGORY_HOME);
        filter.addCategory(Intent.CATEGORY_DEFAULT);
        if (status) {
            dpm.addPersistentPreferredActivity (
                new ComponentName((Context) activity, RhoDeviceAdminReceiver.class), 
                filter, 
                new ComponentName((Context) activity, RhodesActivity.class.getName())
            );
        } else {
            dpm.clearPackagePersistentPreferredActivities(new ComponentName((Context) activity, RhoDeviceAdminReceiver.class), packageName);
        }
    }

    // private static void disabledKeyguard(DevicePolicyManager dpm, ComponentName adminComponentName){
    //     dpm.setKeyguardDisabled(adminComponentName, true);
    // }

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
