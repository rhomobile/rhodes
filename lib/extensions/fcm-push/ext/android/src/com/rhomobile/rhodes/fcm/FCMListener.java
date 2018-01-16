/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2017 Rhomobile, Inc.
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
import com.google.firebase.iid.FirebaseInstanceId;
import com.rhomobile.rhodes.R;
import java.lang.Throwable;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.extmanager.RhoExtManagerImpl;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;


public class FCMListener extends AbstractRhoListener{
	private static final String TAG = FCMListener.class.getSimpleName();

	private static FCMListener listener = null;
	public static FCMListener getInstance(){
		if (listener == null){
			listener = new FCMListener();
		}
		RhoExtManager.getImplementationInstance().addRhoListener(listener);
		return listener;
	}

	public FCMListener(){
		super();
		Logger.W("fcm", "FCM: FCMListener created");
	}

    @Override
    public void onResume(RhodesActivity activity) { 
         Logger.W("fcm", "FCM: onResume");
         FCMIntentService.resume();
         
    }
    @Override
    public void onNewIntent(RhodesActivity activity, Intent intent) { 
        Logger.W("fcm", "FCM: onNewIntent");
        if ((intent != null)&&(intent.getExtras() != null)) {
        	try{
	            for (String key : intent.getExtras().keySet()) {
	                Object value = intent.getExtras().get(key);
	                Logger.W(TAG, "Key: " + key + " Value: " + value);
	                if (key.equals("google.message_id")){
	                    FCMIntentService.tryToHandleIntent((String)value);
	                }
	            }
	        }catch(Exception e){

	        }
        }else{
            Logger.W(TAG, "New intent is null");
        }
        
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager) {
        Logger.W("fcm", "FCM: onCreateApplication");
		//extManager.addRhoListener(this);
    }
}