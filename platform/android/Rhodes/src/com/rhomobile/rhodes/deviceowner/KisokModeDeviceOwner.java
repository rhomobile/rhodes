package com.rhomobile.rhodes.deviceowner;

import com.rhomobile.rhodes.RhodesActivity;

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
        setUserRestriction(UserManager.DISALLOW_SAFE_BOOT, disallow);               // ограничение на возможность безопасной загрузки (Safe Boot)
        setUserRestriction(UserManager.DISALLOW_FACTORY_RESET, disallow);           // ограничение на возможность сброса устройства к заводским настройкам
        setUserRestriction(UserManager.DISALLOW_ADD_USER, disallow);                // ограничение на возможность добавления новых пользователей
        //setUserRestriction(UserManager.DISALLOW_MOUNT_PHYSICAL_MEDIA, disallow);    // ограничение на возможность монтирования физических носителей данных
        //setUserRestriction(UserManager.DISALLOW_ADJUST_VOLUME, disallow);           // ограничение на возможность регулировки громкости устройства
        mDevicePolicyManager.setStatusBarDisabled(mAdminComponentName, disallow);   // отображение статусной строки на устройстве
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
        if(start){
            String[] pack = new String[] {mActivity.getPackageName()};
            mDevicePolicyManager.setLockTaskPackages(mAdminComponentName, pack);
            mActivity.startLockTask();
        }else {
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
