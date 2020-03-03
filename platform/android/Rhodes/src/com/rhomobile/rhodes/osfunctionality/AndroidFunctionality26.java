/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
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

package com.rhomobile.rhodes.osfunctionality;

import android.support.annotation.RequiresApi;
import android.app.Notification.Builder;
import android.content.Context;
import android.app.NotificationManager;
import com.rhomobile.rhodes.Logger;
import android.os.Build.VERSION;

import android.app.Activity;
import android.content.Intent;
import android.content.ComponentName;


@RequiresApi(26)
class AndroidFunctionality26 extends AndroidFunctionality11 implements AndroidFunctionality {

    private static final String TAG = AndroidFunctionality26.class.getSimpleName();
    private static final int IMPORTANCE_HIGH = 0x00000004;
    private static final int PRIORITY_HIGH = 0x00000001;

    @Override
    public Builder getNotificationBuilder( Context ctx, String channelID, String channelName ) {

        Builder builder = null;

        try { 
            NotificationManager notificationManager = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);

            Class ncClass = Class.forName("android.app.NotificationChannel");
            java.lang.reflect.Constructor ncCtor = ncClass.getConstructor( String.class, CharSequence.class, int.class );

            java.lang.reflect.Method getNCMethod = notificationManager.getClass().getMethod("getNotificationChannel", String.class);
            java.lang.reflect.Method createNCMethod = notificationManager.getClass().getMethod("createNotificationChannel", ncClass);

            java.lang.reflect.Method enableVibrationMethod = ncClass.getMethod("enableVibration", boolean.class);
            java.lang.reflect.Method setVibrationMethod = ncClass.getMethod("setVibrationPattern", long[].class);

            Object channel = getNCMethod.invoke(notificationManager,channelID);

            if (channel == null) {
                channel = ncCtor.newInstance( channelID, channelName, 4 );

                enableVibrationMethod.invoke(channel,true);
                setVibrationMethod.invoke(channel,new long[]{100, 200, 300, 400, 500, 400, 300, 200, 400});
              
                createNCMethod.invoke(notificationManager,channel);
            }

            java.lang.reflect.Constructor nbCtor = android.os.Build.VERSION.SDK_INT >= 26 ? 
                Builder.class.getConstructor( Context.class, String.class ) :
                Builder.class.getConstructor( Context.class );

            builder = android.os.Build.VERSION.SDK_INT >= 26 ? 
                 (Builder)nbCtor.newInstance( ctx, channelID ) : 
                 (Builder)nbCtor.newInstance( ctx );

            java.lang.reflect.Method setPriorityMethod = ncClass.getMethod("setPriority", Integer.class);

            if(android.os.Build.VERSION.SDK_INT >= 26)
            {
                java.lang.reflect.Method setShowBadgeMethod = ncClass.getMethod("setShowBadge", boolean.class);
                setShowBadgeMethod.invoke(channel, true);
                setPriorityMethod.invoke(builder, IMPORTANCE_HIGH);
            }
            else
            {
                setPriorityMethod.invoke(builder, PRIORITY_HIGH);
                //builder.setVisibility(android.app.Notification.VISIBILITY_PUBLIC);
            }

        } catch( Exception e ) {
            Logger.E( TAG, "Error initializing notification channel. Will create default builder" );
            Logger.E( TAG, e.toString() );
        }

        if ( null == builder ) {
            builder = super.getNotificationBuilder( ctx, channelID, channelName );
        }

        return builder;
    }

    @Override
	public ComponentName startForegroundService(Activity activity, Intent service) {
        java.lang.reflect.Method methodStartForegroundService = null;
        try {
            java.lang.reflect.Method methodStartForegroundService = activity.getClass().getMethod("startForegroundService", Intent.class);
        } catch( Exception e ) {
            Logger.E( TAG, "Error: Activity class do not has startForegroundService() method !" );
            Logger.E( TAG, e.toString() );
        }
        if (methodStartForegroundService!= null) {
            return (ComponentName)methodStartForegroundService.invoke(activity, service);
        }
        else {
		          return activity.startService(service);
        }
	}


}
