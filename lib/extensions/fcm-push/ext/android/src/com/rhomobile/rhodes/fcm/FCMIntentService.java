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
import androidx.core.app.NotificationCompat;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;
import com.google.firebase.messaging.Constants;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;
import com.rhomobile.rhodes.R;
import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;
import android.util.Log;
import java.util.HashMap;
import java.util.Map;
import org.json.JSONException;
import org.json.JSONObject;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.TimeUnit;
import android.os.Bundle;
import android.os.Handler;
import java.util.Queue;
import java.util.LinkedList;


public class FCMIntentService extends FirebaseMessagingService {

    private static final String TAG = FCMIntentService.class.getSimpleName();
    public static final FCMListener listener = FCMListener.getInstance();
        
    public static Queue<String> messagesQueue = new LinkedList<String>();
    

    public FCMIntentService(){
        super();
        Logger.I(TAG, "FCM: creating FCMIntentService");
    }
  

    @Override
    public void handleIntent(Intent intent) {
        Logger.W(TAG, "FCM: onHandleIntent()");
        extractDataFromIntent(intent);
        super.handleIntent(intent);
    }


    void extractDataFromIntent(Intent intent){

        Bundle bundle = intent.getExtras();
        if (bundle != null) {
            try{

                RemoteMessage remoteMessage = new RemoteMessage(bundle);

                if (remoteMessage.getMessageId() == null) return;

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

            
                JSONObject jsonObject = new JSONObject(params);

                try {
                    jsonObject.put("data", new JSONObject(remoteMessage.getData()));
                    Logger.I(TAG, "FCM: data - " + remoteMessage.getData());
                } catch (JSONException e) {
                    Logger.I(TAG, "FCM: jsonObject generation error");
                    e.printStackTrace();
                }

                Logger.W(TAG, "FCM: push message in JSON: " + jsonObject.toString());

                messagesQueue.add(jsonObject.toString());

            }catch(Exception e){
                Logger.W(TAG, "FCM: exception on extracting data from intent");
                e.printStackTrace();
            }

            handleMessagesQueue();
        }

    }

    public static void handleMessagesQueue(){
        try{
            while(messagesQueue.size() != 0){
                Logger.W(TAG, "FCM: tryToHandleIntent() - trying to handle messages queue");
                PushContract.handleMessage(ContextFactory.getContext(), messagesQueue.peek(), FCMFacade.FCM_PUSH_CLIENT);
                messagesQueue.remove();
            }
        }catch(Exception e){
            Logger.E(TAG, "FCM: can't handle the messages queue");
            e.printStackTrace();
        }
    }

    static public void resume()
    {
        Timer timerObj = new Timer();
        TimerTask timerTaskObj = new TimerTask() {
            public void run() {
                handleMessagesQueue();
            }
        };
        timerObj.schedule(timerTaskObj, 1000);
    }

    @Override
    public void onNewToken(String token){
        super.onNewToken(token);
        Logger.W(TAG, "FCM: onNewToken: " + token);
        ContextFactory.getContext().getSharedPreferences("FireBase", MODE_PRIVATE).edit().putString("token", token).apply();
        FCMFacade.refreshToken();        
    }



}
