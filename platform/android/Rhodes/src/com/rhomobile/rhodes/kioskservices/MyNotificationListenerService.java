package com.rhomobile.rhodes.kioskservices;

import android.service.notification.NotificationListenerService;
import android.service.notification.StatusBarNotification;
import android.widget.Toast;

public class MyNotificationListenerService extends NotificationListenerService {
    @Override
    public void onNotificationPosted(StatusBarNotification sbn) {
        if(KioskManager.getKioskModeStatus()){
            //Toast.makeText(this,"kiosk mode enable", Toast.LENGTH_LONG).show();
            cancelNotification(sbn.getKey());
        }else
            super.onNotificationPosted(sbn);
    }
}
