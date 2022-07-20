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

package com.rhomobile.rhodes.alert;

import com.rhomobile.rhodes.R;

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
				R.drawable.icon,
				text,
				System.currentTimeMillis());
		return notification;
	}
	public static Notification simpleNotification(String tag, int notificationId, Context ctx, Intent intent, String title, String text)
	{
		Notification notification = makeNotification(text);
		PendingIntent contentIntent = PendingIntent.getActivity(ctx, 0, intent, PendingIntent.FLAG_IMMUTABLE);
		
		notification.setLatestEventInfo(ctx, title, text, contentIntent);
		notification.defaults = Notification.DEFAULT_ALL;
		notification.flags |= Notification.FLAG_AUTO_CANCEL;
		
		customNotification(tag, notificationId, ctx, notification);
		
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
