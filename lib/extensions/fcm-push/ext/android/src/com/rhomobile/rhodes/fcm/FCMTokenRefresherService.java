package com.rhomobile.rhodes.fcm;
import android.support.v4.app.NotificationCompat;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.Logger;
import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;
import android.util.Log;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.FirebaseInstanceIdService;
import com.rhomobile.rhodes.fcm.FCMFacade;


public class FCMTokenRefresherService extends FirebaseInstanceIdService {

    private static final String TAG = FCMTokenRefresherService.class.getSimpleName();
    
    @Override
    public void onTokenRefresh()
    {
        Logger.W(TAG, "FCM: onTokenRefresh()");
        FCMFacade.refreshToken();
    }
    
}
