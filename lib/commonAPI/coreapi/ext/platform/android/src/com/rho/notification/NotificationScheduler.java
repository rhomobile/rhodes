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
import com.rhomobile.rhodes.Logger;
import android.os.SystemClock;

import android.net.Uri;
import androidx.core.app.NotificationCompat;
import androidx.core.app.TaskStackBuilder;
import android.media.RingtoneManager;
import android.content.SharedPreferences;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import static android.content.Context.ALARM_SERVICE;

public class NotificationScheduler {
    public static final int RHO_NOTIFICATION_CODE = 100;
    public static final String TAG = "NotificationScheduler";

    public static NotificationSingleton singleton = null;
    public static RhodesActivity activity = null;

    private static String title = null;
    private static String message = null;
    private static int interval = 0;
    private static Boolean repeats = false;
    private static final String storage_name = "rho_notification_storage";
    private static int hour = -1, minute = -1, seconds = -1;

    private static Boolean checkTime() {
        return (hour >= 0 && hour <= 23) && (minute >= 0 && minute <= 59) && (seconds >= 0 && seconds <= 59);
    }

    private static Boolean isCurrentTime() {
        return hour == -1 || minute == -1 || seconds == -1;
    }

    public static void setReminderEx(Map<String, Object> propertyMap, IMethodResult result) {
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

        Object startObj = propertyMap.get(NotificationSingleton.HK_START);
        if (startObj != null && (startObj instanceof Map<?, ?>)) {
            Map<String, Object> timeMap = (Map<String, Object>) startObj;
            Object hourObj = timeMap.get(NotificationSingleton.HK_HOUR);
            if (hourObj != null && (hourObj instanceof Integer))
                hour = (Integer) hourObj;
            else
                hour = -1;

            Object minuteObj = timeMap.get(NotificationSingleton.HK_MINUTE);
            if (minuteObj != null && (minuteObj instanceof Integer))
                minute = (Integer) minuteObj;
            else
                minute = -1;

            Object secondObj = timeMap.get(NotificationSingleton.HK_SECONDS);
            if (secondObj != null && (secondObj instanceof Integer))
                seconds = (Integer) secondObj;
            else
                seconds = -1;
        }

        SharedPreferences settings = activity.getSharedPreferences(storage_name, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();

        editor.putString(NotificationSingleton.HK_TITLE, title);
        editor.putString(NotificationSingleton.HK_MESSAGE, message);
        editor.putInt(NotificationSingleton.HK_HOUR, hour);
        editor.putInt(NotificationSingleton.HK_MINUTE, minute);
        editor.putInt(NotificationSingleton.HK_SECONDS, seconds);
        if (interval > 0)
            editor.putInt(NotificationSingleton.HK_INTERVAL, interval);
        if (repeats)
            editor.putBoolean(NotificationSingleton.HK_REPEATS, repeats);

        if (editor.commit()) {
            Logger.I(TAG, "Key is successfully saved to Shared Preferences");
        } else {
            Logger.E(TAG, "Write key to Shared Preferences is failed");
        }

        NotificationScheduler.setReminder(activity, AlarmReceiver.class);
    }

    public static void getSchedulerSettings(Context context) {
        try {

            SharedPreferences settings = context.getSharedPreferences(storage_name, Context.MODE_PRIVATE);

            String deadValue = "not exists";
            int deadInt = -1;

            title = settings.getString(NotificationSingleton.HK_TITLE, deadValue);
            if (title == deadValue) {
                title = null;
                Logger.E(TAG, "Key - HK_TITLE not found!");
            }

            message = settings.getString(NotificationSingleton.HK_MESSAGE, deadValue);
            if (message == deadValue) {
                message = null;
                Logger.E(TAG, "Key - HK_MESSAGE not found!");
            }

            interval = settings.getInt(NotificationSingleton.HK_INTERVAL, deadInt);
            if (interval == deadInt) {
                interval = 0;
                Logger.I(TAG, "Key - HK_INTERVAL not found!");
            }

            repeats = settings.getBoolean(NotificationSingleton.HK_REPEATS, false);
            hour = settings.getInt(NotificationSingleton.HK_HOUR, deadInt);
            seconds = settings.getInt(NotificationSingleton.HK_SECONDS, deadInt);
            minute = settings.getInt(NotificationSingleton.HK_MINUTE, deadInt);
        } catch (ClassCastException e) {
            message = null;
            title = null;
            Logger.E(TAG, e.getMessage());
        }

    }

    public static void setReminder(Context context, Class<?> cls) {
        Calendar now = Calendar.getInstance();
        Calendar target = Calendar.getInstance();

        if (!isCurrentTime()) {
            target.set(Calendar.HOUR_OF_DAY, hour);
            target.set(Calendar.MINUTE, minute);
            target.set(Calendar.SECOND, seconds);

            if (target.before(now))
                target.add(Calendar.DATE, 1);

        }

        cancelReminder(context, cls);

        ComponentName receiver = new ComponentName(context, cls);
        PackageManager pm = context.getPackageManager();

        pm.setComponentEnabledSetting(receiver, PackageManager.COMPONENT_ENABLED_STATE_ENABLED,
                PackageManager.DONT_KILL_APP);

        Intent new_intent = new Intent(context, cls);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, RHO_NOTIFICATION_CODE, new_intent,
                PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);
        AlarmManager am = (AlarmManager) context.getSystemService(ALARM_SERVICE);

        if (interval < 0) {
            Logger.E(TAG, "Interval is negative!!! Ignore...");
            interval = 0;
        }

        if (isCurrentTime()) {

            Logger.I(TAG, "Scheduling: " + "mode - ELAPSED_REALTIME_WAKEUP, interval - " + Integer.toString(interval));

            am.setRepeating(AlarmManager.ELAPSED_REALTIME_WAKEUP, SystemClock.elapsedRealtime() + (interval * 1000),
                    repeats ? (interval * 1000) : 0, pendingIntent);
        } else {
            Logger.I(TAG,
                    "Scheduling: " + "mode - RTC_WAKEUP, time - " + Integer.toString(target.get(Calendar.HOUR_OF_DAY))
                            + ":" + Integer.toString(target.get(Calendar.MINUTE)) + ":"
                            + Integer.toString(target.get(Calendar.SECOND)));

            if (checkTime()) {
                am.setRepeating(AlarmManager.RTC_WAKEUP, target.getTimeInMillis(), repeats ? (interval * 1000) : 0,
                        pendingIntent);
            } else {
                Logger.E(TAG, "Ivalid parameter for time!!!");
            }
        }

    }

    public static void cancelReminderEx() {
        if (activity != null)
            cancelReminder(activity, AlarmReceiver.class);
    }

    public static void cancelReminder(Context context, Class<?> cls) {
        ComponentName receiver = new ComponentName(context, cls);
        PackageManager pm = context.getPackageManager();

        pm.setComponentEnabledSetting(receiver, PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
                PackageManager.DONT_KILL_APP);

        Intent new_intent = new Intent(context, cls);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, RHO_NOTIFICATION_CODE, new_intent,
                PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);
        AlarmManager am = (AlarmManager) context.getSystemService(ALARM_SERVICE);
        am.cancel(pendingIntent);
        pendingIntent.cancel();
    }

    public static void showNotification(Context context, Class<?> cls) {
        Map<String, Object> props = new HashMap<String, Object>();

        if (title == null || message == null) {
            getSchedulerSettings(context); //try getting settings
            if(title == null || message == null)
            {
               Logger.E(TAG, "Title and message must be set!!!");
               return;
            }
        }

        props.put(NotificationSingleton.HK_TITLE, title);
        props.put(NotificationSingleton.HK_MESSAGE, message);

        List<String> kinds = new ArrayList<String>();
        kinds.add(INotificationSingleton.TYPE_NOTIFICATION_DIALOG);

        props.put(NotificationSingleton.HK_TYPES, kinds);

        if (singleton == null) {
            singleton = new NotificationSingleton();
            singleton.setContext(context);
        }
        singleton.showPopup(props, null);

        if (repeats && interval == 0) {
            setReminder(context, cls);
        }

        Logger.I(TAG, "Notification recived!!!");
    }
}