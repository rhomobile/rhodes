package com.rhomobile.rhodes.deviceowner;

import com.rhomobile.rhodes.RhodesActivity;

import java.util.Arrays;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.app.admin.DevicePolicyManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.UserManager;
import android.view.View;
import android.view.Window;


public class KisokModeDeviceOwner {
    private DevicePolicyManager mDevicePolicyManager;
    private ComponentName mAdminComponentName;
    private Activity mActivity;

    private static String[] packageNameForSetDeviceOwner = null;

    public static void SetIgnorePackageForDeviceOwner(Activity activity, String packages){
        List<String> listPackage = new ArrayList<>();
        listPackage.add(activity.getPackageName());
        listPackage.addAll(Arrays.asList(packages.split(";")));
        packageNameForSetDeviceOwner = listPackage.toArray(new String[0]);
    }

    public KisokModeDeviceOwner(Activity mActivity){
        this.mDevicePolicyManager= (DevicePolicyManager) mActivity.getSystemService(Context.DEVICE_POLICY_SERVICE);
        this.mAdminComponentName = RhoDeviceAdminReceiver.getComponentName(mActivity);
        this.mActivity = mActivity;
    }

    public void setKioskMode(boolean enable){
        setRestrictions(enable);
        setAsHomeApp(enable);
        setKeyGuardEnabled(enable);
        setLockTask(enable);
        setImmersiveMode(enable);
    }

    public void rebootDevice(){
        mDevicePolicyManager.reboot(mAdminComponentName);
    }

    // region restrictions 

    private void setRestrictions(Boolean disallow) {
        setUserRestriction(UserManager.DISALLOW_SAFE_BOOT, disallow);              	 			// secure download restriction (Safe Boot)
        setUserRestriction(UserManager.DISALLOW_FACTORY_RESET, disallow);           			// restriction on the ability to reset the device to factory settings
        setUserRestriction(UserManager.DISALLOW_ADD_USER, disallow);                				// restriction on the ability to add new users
        //setUserRestriction(UserManager.DISALLOW_MOUNT_PHYSICAL_MEDIA, disallow);   // restriction on the ability to mount physical storage media
        //setUserRestriction(UserManager.DISALLOW_ADJUST_VOLUME, disallow);          	 	// restriction on the ability to adjust the volume of the device
        mDevicePolicyManager.setStatusBarDisabled(mAdminComponentName, disallow);   // displaying the status bar on the device
    }

    private void setUserRestriction(String restriction, Boolean disallow){
        if (disallow) {
            mDevicePolicyManager.addUserRestriction(mAdminComponentName, restriction);
        } else {
            mDevicePolicyManager.clearUserRestriction(mAdminComponentName, restriction);
        }
    }

    // endregion

    private void setLockTask(Boolean start) {
        if (start){
            if (packageNameForSetDeviceOwner == null){
                packageNameForSetDeviceOwner = new String[] {mActivity.getPackageName()};
            }
            mDevicePolicyManager.setLockTaskPackages(mAdminComponentName, packageNameForSetDeviceOwner);
            mActivity.startLockTask();
        } else {
            mDevicePolicyManager.setLockTaskPackages(mAdminComponentName, new String[]{});
            mActivity.stopLockTask();
        }
    }

    
    private void setAsHomeApp(Boolean enable) {
        if (enable) {
            IntentFilter intentFilter = new IntentFilter(Intent.ACTION_MAIN);
            intentFilter.addCategory(Intent.CATEGORY_HOME);
            intentFilter.addCategory(Intent.CATEGORY_DEFAULT);

            mDevicePolicyManager.addPersistentPreferredActivity(
                    mAdminComponentName, intentFilter, new ComponentName(mActivity.getPackageName(), RhodesActivity.class.toString())
            );
        } else {
            mDevicePolicyManager.clearPackagePersistentPreferredActivities(
                    mAdminComponentName, mActivity.getPackageName()
            );
        }
    }

    private void setKeyGuardEnabled(Boolean enable) {
        mDevicePolicyManager.setKeyguardDisabled(mAdminComponentName, !enable);
    }

    private void setImmersiveMode(Boolean enable) {
        Window window = mActivity.getWindow();
        View decorView = window.getDecorView();
        if (enable) {
            int flags = (View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_FULLSCREEN
                    | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
            decorView.setSystemUiVisibility(flags);
        } else {
            int flags = (View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                    | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                    | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
            decorView.setSystemUiVisibility(flags);
        }
    }
        
}
