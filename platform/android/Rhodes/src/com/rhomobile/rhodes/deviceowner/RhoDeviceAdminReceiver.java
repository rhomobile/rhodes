package com.rhomobile.rhodes.deviceowner;

import android.app.admin.DeviceAdminReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

import androidx.annotation.NonNull;

public class RhoDeviceAdminReceiver extends DeviceAdminReceiver {
    
    public static ComponentName getComponentName(Context context){
        return new ComponentName(context.getApplicationContext(), RhoDeviceAdminReceiver.class);
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
