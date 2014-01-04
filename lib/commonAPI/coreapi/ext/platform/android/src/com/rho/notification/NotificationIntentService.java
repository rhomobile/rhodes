package com.rho.notification;

import com.rhomobile.rhodes.Logger;

import android.app.IntentService;
import android.content.Intent;

public class NotificationIntentService extends IntentService {

    private static final String TAG = NotificationIntentService.class.getSimpleName();
    
    public NotificationIntentService() {
        super(TAG);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        //Intent.
        Logger.T(TAG, "onHandleIntent");
        int notificationIdx = intent.getIntExtra(Notification.NOTIFICATION_ID, -1);
        int actionIdx = intent.getIntExtra(INotificationSingleton.HK_BUTTON_INDEX, -1);
        if (notificationIdx == -1 || actionIdx == -1) {
            Logger.E(TAG, "Notification Intent has no mandatory values.");
        }
        
        ((NotificationSingleton)(NotificationFactorySingleton.getInstance().getApiSingleton())).onAction(notificationIdx, actionIdx);
    }

}
