package com.motorolasolutions.rho.notification.led;

import com.rho.notification.led.ILed;
import com.rho.notification.led.ILedFactory;
import com.rho.notification.led.ILedSingleton;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.Logger;
import android.app.NotificationManager;
import android.app.Notification;
import android.content.Context;

import java.util.Map;
import java.util.List;

import com.rhomobile.rhodes.util.ContextFactory;
import android.os.Handler;

import java.lang.System;


public class LedImpl implements ILed
{
    protected static String TAG = "LedImpl";
    private String mName;
    private int mColor;
    private int mNotificationId;
    
    private Handler mCancelHandler = new Handler();
    
    public LedImpl( String name, int color/*, int notificationId*/ )
    {
        mName = name;
        mColor = color;
        //mNotificationId = notificationId;
    }

    public void getName(IMethodResult result)
    {
        result.set(mName);
    }
    
    public String getId()
    {
        return mName;
    }
    
    synchronized public void illuminate(IMethodResult result)
    {
        Logger.T(TAG, "illuminate " + mName);
        
        Context ctx = ContextFactory.getContext();
        String ns = ctx.NOTIFICATION_SERVICE;
        
        NotificationManager nm = ( NotificationManager ) ctx.getSystemService( ns );
        Notification notif = new Notification();
        notif.ledARGB = mColor;
        notif.flags = Notification.FLAG_SHOW_LIGHTS;
        notif.ledOnMS = 1;
        notif.ledOffMS = 0;
        mNotificationId = (int)System.currentTimeMillis();
        nm.notify(mNotificationId, notif);
    }
    
    synchronized public void extinguish(IMethodResult result)
    {
        Logger.T(TAG, "extinguish " + mName);
        
        Context ctx = ContextFactory.getContext();
        String ns = ctx.NOTIFICATION_SERVICE;
        
        NotificationManager nm = ( NotificationManager ) ctx.getSystemService( ns );
        nm.cancel(mNotificationId);
        /*Notification notif = new Notification();
        notif.ledARGB = 0x00FFFFFF & mColor;
        notif.flags = Notification.FLAG_SHOW_LIGHTS;
        notif.ledOnMS = 0;
        notif.ledOffMS = 0;
        nm.notify(mNotificationId, notif);*/
    }
    
    synchronized public void flash(Map<String, Integer> propertyMap, IMethodResult result)
    {
        Logger.T(TAG, "flash " + mName);

        int cycles = propertyMap.get(ILedSingleton.HK_NUMBER_OF_CYCLES);
        int msOn = propertyMap.get(ILedSingleton.HK_ON_DURATION);
        int msOff = propertyMap.get(ILedSingleton.HK_OFF_DURATION);
        
        int totalDuration = (msOn+msOff)*cycles;
        
        Context ctx = ContextFactory.getContext();
        String ns = ctx.NOTIFICATION_SERVICE;
        
        NotificationManager nm = ( NotificationManager ) ctx.getSystemService( ns );
        Notification notif = new Notification();
        notif.ledARGB = mColor;
        notif.flags = Notification.FLAG_SHOW_LIGHTS;
        notif.ledOnMS = msOn;
        notif.ledOffMS = msOff;
        mNotificationId = (int)System.currentTimeMillis();
        nm.notify(mNotificationId, notif);
        
        mCancelHandler.postDelayed(
            new Runnable() {
                public void run() {
                    extinguish(null);
                }
            },
            totalDuration
        );
    }
}