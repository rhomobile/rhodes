package com.rho.notification;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;

public class AlarmReceiver extends BroadcastReceiver {

    String TAG = "AlarmReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        if (intent.getAction() != null && context != null) {
            if (intent.getAction().equalsIgnoreCase(Intent.ACTION_BOOT_COMPLETED)) {
                NotificationScheduler.getSchedulerSettings(context);
                NotificationScheduler.setReminder(context, AlarmReceiver.class);
                return;
            }
        }

        Logger.D(TAG, "onAlarmReceive: ");
        NotificationScheduler.showNotification(context, RhodesActivity.class);

    }
}