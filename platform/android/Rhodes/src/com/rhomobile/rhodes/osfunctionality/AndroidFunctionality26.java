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

import androidx.annotation.RequiresApi;
import android.app.Notification.Builder;
import android.content.Context;
import android.app.NotificationManager;
import com.rhomobile.rhodes.Logger;
import android.os.Build.VERSION;

import android.app.Activity;
import android.content.Intent;
import android.content.ComponentName;

//Android 8.0
@RequiresApi(26)
class AndroidFunctionality26 extends AndroidFunctionality11 implements AndroidFunctionality {

    private static final String TAG = AndroidFunctionality26.class.getSimpleName();
    private static final int IMPORTANCE_HIGH = 0x00000004;
    private static final int IMPORTANCE_NONE = 0x00000000;
    private static final int IMPORTANCE_UNSPECIFIED = 0xfffffc18;
    private static final int PRIORITY_HIGH = 0x00000001;

    private Builder getNotificationBuilderEx( Context ctx, String channelID, String channelName, int priority, boolean showBadge )
    {
        Logger.D( TAG, "getNotificationBuilderEx() START" );
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
                Logger.D( TAG, "getNotificationBuilderEx() chanell==null" );
                channel = ncCtor.newInstance( channelID, channelName, 4 );
              
                createNCMethod.invoke(notificationManager,channel);
            }

            java.lang.reflect.Constructor nbCtor = android.os.Build.VERSION.SDK_INT >= 26 ? 
                Builder.class.getConstructor( Context.class, String.class ) :
                Builder.class.getConstructor( Context.class );

            builder = android.os.Build.VERSION.SDK_INT >= 26 ? 
                 (Builder)nbCtor.newInstance( ctx, channelID ) : 
                 (Builder)nbCtor.newInstance( ctx );

            if(android.os.Build.VERSION.SDK_INT >= 26)
            {
                java.lang.reflect.Method setShowBadgeMethod = ncClass.getMethod("setShowBadge", boolean.class);
                setShowBadgeMethod.invoke(channel, showBadge);        
                java.lang.reflect.Method setPriorityMethod = ncClass.getMethod("setImportance", int.class);
                setPriorityMethod.invoke(channel, priority);
                //setPriorityMethod.invoke(channel, IMPORTANCE_HIGH);
                //setPriorityMethod.invoke(channel, IMPORTANCE_UNSPECIFIED);
            }
            else
            {		
                java.lang.reflect.Method setPriorityMethod = Builder.class.getMethod("setPriority", int.class);
                setPriorityMethod.invoke(builder, PRIORITY_HIGH);
                //builder.setVisibility(android.app.Notification.VISIBILITY_PUBLIC);
            }

        } catch( Exception e ) {
            Logger.E( TAG, "Error initializing notification channel. Will create default builder" );
            Logger.E( TAG, e.toString() );
        }

        if ( null == builder ) {
            Logger.D( TAG, "getNotificationBuilderEx() builder == null" );
            builder = super.getNotificationBuilder( ctx, channelID, channelName );
        }
        Logger.D( TAG, "getNotificationBuilderEx() FINISH" );

        return builder;
    }


    @Override
    public Builder getNotificationBuilder( Context ctx, String channelID, String channelName ) {
        return getNotificationBuilderEx(ctx, channelID, channelName, IMPORTANCE_HIGH, true);
    }

    public Builder getNotificationBuilderForService( Context ctx, String channelID, String channelName ) {
        return getNotificationBuilderEx(ctx, channelID, channelName, IMPORTANCE_UNSPECIFIED, false);
    }

    // @Override
	// public ComponentName startForegroundService(Activity activity, Intent service) {
    //     Logger.D( TAG, "startForegroundService() START" );
    //     java.lang.reflect.Method methodStartForegroundService = null;
    //     ComponentName resultOfInvoke = null;
    //     try {
    //         methodStartForegroundService = activity.getClass().getMethod("startForegroundService", Intent.class);
    //     } catch( Exception e ) {
    //         Logger.E( TAG, "Error: Activity class do not has startForegroundService() method !" );
    //         Logger.E( TAG, e.toString() );
    //     }
    //     if (methodStartForegroundService!= null) {
    //         try {
    //             Logger.D( TAG, "invoke startForegroundService()" );
    //             resultOfInvoke = (ComponentName)methodStartForegroundService.invoke(activity, service);
    //         } catch( Exception e ) {
    //             Logger.E( TAG, "Error: some error during invoke startForegroundService() method !" );
    //             Logger.E( TAG, e.toString() );
    //         }
    //     }
    //     else {
    //         Logger.D( TAG, "use old startService()" );
    //         resultOfInvoke = activity.startService(service);
    //     }
    //     Logger.D( TAG, "startForegroundService() FINISH" );
    //     return resultOfInvoke;
	// }


}
