package com.rho.notification;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;

import java.util.Calendar;
import android.util.Log;
import android.os.SystemClock;

import android.net.Uri;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.TaskStackBuilder;
import android.media.RingtoneManager;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import static android.content.Context.ALARM_SERVICE;

public class NotificationScheduler {
    public static final int RHO_NOTIFICATION_CODE = 100;
    public static final String TAG="NotificationScheduler";

    public static NotificationSingleton singleton = null;
    public static RhodesActivity activity = null;

    private static String title = null;
    private static String message = null;
    private static int interval = 0;
    private static Boolean repeats = false;

    public static void setReminderEx(Map<String, Object> propertyMap, IMethodResult result)
    {
		Object titleObj = propertyMap.get(NotificationSingleton.HK_TITLE);
        if (titleObj != null && (titleObj instanceof String))
            title = (String) titleObj;

        Object messageObj = propertyMap.get(NotificationSingleton.HK_MESSAGE);
        if (messageObj != null && (messageObj instanceof String))
            message = (String) messageObj;

        Object intervalObj = propertyMap.get(NotificationSingleton.HK_INTERVAL);
        if (intervalObj != null && (intervalObj instanceof Integer))
            interval = (Integer) intervalObj;

        Object repeatObj = propertyMap.get(NotificationSingleton.HK_REPEATS);
        if (repeatObj != null && (repeatObj instanceof Boolean))
            repeats = (Boolean) repeatObj;

        NotificationScheduler.setReminder(activity, AlarmReceiver.class);	
    }

    public static void setReminder(Context context, Class<?> cls)
    {
        //Calendar calendar = Calendar.getInstance();

        //Calendar setcalendar = Calendar.getInstance();
        //setcalendar.set(Calendar.HOUR_OF_DAY, hour);
        //setcalendar.set(Calendar.MINUTE, min);
        //setcalendar.set(Calendar.SECOND, 0);

        cancelReminder(context, cls);

        //if(setcalendar.before(calendar))
        //    setcalendar.add(Calendar.DATE,1);

        ComponentName receiver = new ComponentName(context, cls);
        PackageManager pm = context.getPackageManager();

        pm.setComponentEnabledSetting(receiver,
                PackageManager.COMPONENT_ENABLED_STATE_ENABLED,
                PackageManager.DONT_KILL_APP);

        Intent new_intent = new Intent(context, cls);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, RHO_NOTIFICATION_CODE, new_intent, PendingIntent.FLAG_UPDATE_CURRENT);
        AlarmManager am = (AlarmManager) context.getSystemService(ALARM_SERVICE);
        //am.setInexactRepeating(AlarmManager.RTC_WAKEUP, setcalendar.getTimeInMillis(), AlarmManager.INTERVAL_DAY, pendingIntent);
        am.setRepeating(AlarmManager.ELAPSED_REALTIME_WAKEUP, SystemClock.elapsedRealtime() + (interval * 1000),
                (interval * 1000), pendingIntent);

    }

    public static void cancelReminderEx()
    {
        if(activity != null)
            cancelReminder(activity, AlarmReceiver.class);
    }

    public static void cancelReminder(Context context, Class<?> cls)
    {
        ComponentName receiver = new ComponentName(context, cls);
        PackageManager pm = context.getPackageManager();

        pm.setComponentEnabledSetting(receiver,
                PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
                PackageManager.DONT_KILL_APP);

        Intent new_intent = new Intent(context, cls);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, RHO_NOTIFICATION_CODE, new_intent, PendingIntent.FLAG_UPDATE_CURRENT);
        AlarmManager am = (AlarmManager) context.getSystemService(ALARM_SERVICE);
        am.cancel(pendingIntent);
        pendingIntent.cancel();
    }

    public static void showNotification(Context context, Class<?> cls)
    {
        Map<String, Object> props = new HashMap<String, Object>();

        props.put(NotificationSingleton.HK_TITLE, title);
        props.put(NotificationSingleton.HK_MESSAGE, message);

        List<String> kinds = new ArrayList<String>();
        kinds.add(INotificationSingleton.TYPE_NOTIFICATION_DIALOG);

        props.put(NotificationSingleton.HK_TYPES, kinds);

        if(singleton == null)
        {
           singleton = new NotificationSingleton();
           singleton.setContext(context);
        }
        singleton.showPopup(props, null);

        Log.i(TAG, "Notification recived!!!");
    }
}