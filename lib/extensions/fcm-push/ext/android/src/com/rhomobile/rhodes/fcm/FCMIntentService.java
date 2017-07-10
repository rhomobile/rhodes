/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2012 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.fcm;
import android.content.Context;
import android.content.Intent;
import android.app.NotificationManager;
import android.graphics.BitmapFactory;
import android.media.RingtoneManager;
import android.net.Uri;
import android.support.v4.app.NotificationCompat;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;
import com.rhomobile.rhodes.R;
import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;
import android.util.Log;

public class FCMIntentService extends FirebaseMessagingService {

    private static final String TAG = FCMIntentService.class.getSimpleName();

    protected void onRegistered(Context context, String pin) {
        Logger.W(TAG, "FCM: push registation: " + pin);
        PushContract.handleRegistration(context, pin, FCMFacade.FCM_PUSH_CLIENT);
    }

    protected void onUnregistered(Context context, String pin) {
        Logger.W(TAG, "FCM: push unregistation: " + pin);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Logger.W(TAG, "FCM: Creating FireBaseClient intent sevice");
        Log.d(TAG, "onCreate() FCMIntentService");
        onRegistered(ContextFactory.getContext(), "FireBaseClient");
    }

    @Override
    protected void finalize ( ) {
        onUnregistered(ContextFactory.getContext(), "FireBaseClient");
    }

    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        //remoteMessage.getNotification().getBody();
        Log.d(TAG, "FCMIntentService reseived message: " + remoteMessage.getNotification().getBody());
        Logger.W(TAG, "FCM: push message: " + remoteMessage.getNotification().getBody());
        PushContract.handleMessage(ContextFactory.getContext(), remoteMessage.getNotification().getBody(), FCMFacade.FCM_PUSH_CLIENT);
    }
}
