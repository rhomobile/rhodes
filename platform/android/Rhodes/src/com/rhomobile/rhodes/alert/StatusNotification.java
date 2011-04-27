package com.rhomobile.rhodes.alert;

import com.rhomobile.rhodes.AndroidR;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

public class StatusNotification
{
	public static Notification makeNotification(String text)
	{
		Notification notification = new Notification(
				AndroidR.drawable.icon,
				text,
				System.currentTimeMillis());
		return notification;
	}
	public static Notification simpleNotification(String tag, int notificationId, Context ctx, Intent intent, String title, String text)
	{
		Notification notification = makeNotification(text);
		PendingIntent contentIntent = PendingIntent.getService(ctx, 0, intent, 0);
		
		notification.setLatestEventInfo(ctx, title, text, contentIntent);
		notification.defaults = Notification.DEFAULT_ALL;
		notification.flags |= Notification.FLAG_AUTO_CANCEL;
		
		customNotification(tag, notificationId, ctx,notification);
		
		return notification;
	}
	public static void customNotification(String tag, int notificationId, Context ctx, Notification customNotification)
	{
		NotificationManager mNm = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
		mNm.notify(tag, notificationId, customNotification);
	}
	public static void cancelNotification(String tag, int notificationId, Context ctx)
	{
		NotificationManager mNm = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
		mNm.cancel(tag, notificationId);
	}
	public static void cancelAllNotifications(Context ctx)
	{
		NotificationManager mNm = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
		mNm.cancelAll();
	}
}
