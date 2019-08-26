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
import android.graphics.BitmapFactory;
import android.media.RingtoneManager;
import android.net.Uri;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.support.v4.app.NotificationCompat;

import android.app.PendingIntent;
import android.net.Uri;
import android.util.Log;
import android.os.Build;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.R;


import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;
import android.util.Log;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.HashSet;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.TimeUnit;
import android.os.Handler;
import com.rhomobile.rhodes.fcm.FCMFacade;
import com.google.firebase.iid.FirebaseInstanceId;


public class FCMIntentService extends FirebaseMessagingService {

    private static final String TAG = FCMIntentService.class.getSimpleName();
    public static final FCMListener listener = FCMListener.getInstance();
    
    private static final String CHANNEL_ID = "default";
    private static Map<String, JSONObject> savedMessages = new HashMap<String, JSONObject>();
    
    @Override
    public void onNewToken(String token) {
       Logger.I(TAG, "FCM: on new token: " + token);
       super.onNewToken(token);
       FCMFacade.refreshToken();
    }

    @Override
    public void onCreate() {
        super.onCreate();
        if (Build.VERSION.SDK_INT >= 26) {
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, getPackageName(), NotificationManager.IMPORTANCE_DEFAULT);
            NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
            notificationManager.createNotificationChannel(channel);
        }
    }


    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        //savedIntents.remove(remoteMessage.getMessageId());

        Logger.I(TAG, "FCM: onMessageReceived()");
        Map<String, String> params = new HashMap<String, String>();

        if (remoteMessage.getMessageId() != null) params.put("id", remoteMessage.getMessageId());
        Logger.I(TAG, "FCM: id - " + remoteMessage.getMessageId());

        if (remoteMessage.getFrom() != null) params.put("from", remoteMessage.getFrom());
        Logger.I(TAG, "FCM: from - " + remoteMessage.getFrom());

        if (remoteMessage.getTo() != null) params.put("to", remoteMessage.getTo());
        Logger.I(TAG, "FCM: to - " + remoteMessage.getTo());

        if (remoteMessage.getNotification() != null){
            if (remoteMessage.getNotification().getBody() != null)  params.put("body", remoteMessage.getNotification().getBody());
            Logger.I(TAG, "FCM: body - " + remoteMessage.getNotification().getBody());

            if (remoteMessage.getNotification().getTitle() != null) params.put("title", remoteMessage.getNotification().getTitle());
            Logger.I(TAG, "FCM: title - " + remoteMessage.getNotification().getTitle());

            if (remoteMessage.getNotification().getTag() != null) params.put("tag", remoteMessage.getNotification().getTag());
            Logger.I(TAG, "FCM: tag - " + remoteMessage.getNotification().getTag());
        }

        String title = null;
        String text = null;

        try{
            JSONObject jsonObject = new JSONObject(params);

            try {
                jsonObject.put("data", new JSONObject(remoteMessage.getData()));
                Logger.I(TAG, "FCM: data - " + remoteMessage.getData());
                title = remoteMessage.getData().get("notification_title");
                text = remoteMessage.getData().get("notification_text");
            } catch (JSONException e) {
                Logger.I(TAG, "FCM: jsonObject generation error");
                e.printStackTrace();
            }

            Logger.W(TAG, "FCM: push message in JSON: " + jsonObject.toString());
            try{
                PushContract.handleMessage(ContextFactory.getContext(), jsonObject.toString(), FCMFacade.FCM_PUSH_CLIENT);
            }catch(Exception e){
                if (params.containsKey("id")){
                    savedMessages.put(params.get("id"), jsonObject);
                }
            }
        }catch(Exception e){
            Logger.E(TAG, "FCM: can't create object for handleMessage");
            e.printStackTrace();
        }

        if (title != null && text != null){
            Intent resultIntent = new Intent(this, RhodesActivity.class);
            PendingIntent resultPendingIntent = PendingIntent.getActivity(this, 0, resultIntent,
                           PendingIntent.FLAG_UPDATE_CURRENT);
        

            NotificationCompat.Builder notificationBuilder = null;
            if (Build.VERSION.SDK_INT >= 26) {
                notificationBuilder = new NotificationCompat.Builder(this, "default");
            }else{
                notificationBuilder = new NotificationCompat.Builder(this);
                           
            }
            notificationBuilder.setSmallIcon(R.mipmap.icon)
                           .setContentTitle(title)
                           .setContentText(text)
                           .setAutoCancel(true)
                           .setContentIntent(resultPendingIntent);

            Notification notification = notificationBuilder.build();
             
            NotificationManager notificationManager =
                   (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

            notificationManager.notify(1, notification);
        }


    }

    public static boolean tryToHandleMessage(JSONObject jsonObject){
        try{
            Logger.W(TAG, "FCM: tryToHandleMessage() - trying to handle saved JSONObject");
            PushContract.handleMessage(ContextFactory.getContext(), jsonObject.toString(), FCMFacade.FCM_PUSH_CLIENT);
            return true;
        }catch(Exception e){
            Logger.W(TAG, "FCM: tryToHandleMessage() - can't handle JSONObject");
        }
        return false;
    }


    static public void resume()
    {
        if (savedMessages.size() > 0){

            Timer timerObj = new Timer();
            TimerTask timerTaskObj = new TimerTask() {
                public void run() {
                    Set<String> keySet = new HashSet<String>(savedMessages.keySet());
                    for (String key : keySet){
                        try{
                            if (tryToHandleMessage(savedMessages.get(key))){
                                savedMessages.remove(key);
                            }
                        }catch(Exception e){
                            Logger.W(TAG, "FCM: can't handle saved message " + key);
                        }
                    }
                }
            };
            timerObj.schedule(timerTaskObj, 2000);
        }
    }

}
