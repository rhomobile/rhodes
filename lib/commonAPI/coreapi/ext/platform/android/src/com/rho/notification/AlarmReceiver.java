package com.rho.notification;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import com.rhomobile.rhodes.RhodesActivity;

public class AlarmReceiver extends BroadcastReceiver {

    String TAG = "AlarmReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction() != null && context != null) {
            if (intent.getAction().equalsIgnoreCase(Intent.ACTION_BOOT_COMPLETED)) {
                NotificationScheduler.setReminder(context, AlarmReceiver.class);
                return;
            }
        }

        Log.d(TAG, "onAlarmReceive: ");
        NotificationScheduler.showNotification(context, RhodesActivity.class);

    }
}