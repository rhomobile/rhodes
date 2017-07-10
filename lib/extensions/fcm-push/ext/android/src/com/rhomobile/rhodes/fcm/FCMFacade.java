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

import com.rhomobile.rhodes.Push;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.fcm.FCMIntentService;
import android.content.Intent;
import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;
import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;
import com.google.firebase.messaging.FirebaseMessaging;
import com.rhomobile.rhodes.R;

public final class FCMFacade {
    private static final String TAG = FCMFacade.class.getSimpleName();
    
    static final String FCM_PUSH_CLIENT = "FireBaseClient";

    private static String gStr(int key){
        return ContextFactory.getContext().getResources().getString(key);
    }

    public static void Register(String senderId) {
        //ContextFactory.getContext().startService(new Intent(ContextFactory.getContext(), FCMIntentService.class));

        Logger.W(TAG, "FCM: Send FCM push register req");
      
        try{
            FirebaseApp.getInstance();
            Logger.W(TAG, "FCM: Firebase inited");
        }catch(Exception exc){
            FirebaseOptions options = new FirebaseOptions.Builder()
            .setApplicationId(gStr(R.string.google_app_id))
            .setApiKey(gStr(R.string.google_api_key))
            .setDatabaseUrl(gStr(R.string.firebase_database_url))
            .setStorageBucket(gStr(R.string.google_storage_bucket))
            .setGcmSenderId(gStr(R.string.gcm_defaultSenderId))
            .build();

            try{
                FirebaseApp.initializeApp(ContextFactory.getContext(), options);
                Logger.W(TAG, "FCM: initialization of application");
            }catch(Exception e){
                Logger.W(TAG, "FCM: poblems on initialization app: " + e);
            }
            try{
                FirebaseApp.getInstance();
                Logger.W(TAG, "FCM: Firebase Inited");
            }catch(Exception e){
                Logger.W(TAG, "FCM: poblems on getting instance: " + e);
            }
        }     


        if (senderId == null || senderId.length() == 0) {
            Logger.W(TAG, "Trying to read FCM push sender app id from standart settings");
            senderId = Push.SENDER;
        }

        
        Logger.W(TAG, "FCM: Already has FCM push registeration");
        PushContract.handleRegistration(ContextFactory.getContext(),
                                        FCM_PUSH_CLIENT,
                                        FCMFacade.FCM_PUSH_CLIENT);       

    }

    public static void Unregister() {
        Logger.T(TAG, "FCM: push unregister req");
    }



}
