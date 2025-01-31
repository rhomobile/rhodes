package com.rhomobile.rhodes.kioskservices;

import android.util.Log;
import android.content.Context;
import android.content.SharedPreferences;


public class KioskManager {
    private static boolean isKioskMode = false;
    private static Boolean isEnabledFilteringEventsOnStart = null;

    public static void setKioskMode(Boolean state){
        Log.d("myLog", "Kiosk status: "+(state?"true":"false"));
        isKioskMode = state;
    }

    public static Boolean getKioskModeStatus(){
        Log.d("myLog", "Kiosk status: "+(isKioskMode?"true":"false"));
        return isKioskMode;
    }

    public static boolean GetKioskModeEnabledFilteringEventsOnStart(Context context){
        if (isEnabledFilteringEventsOnStart == null){
            SharedPreferences sharedPreferences = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
            isEnabledFilteringEventsOnStart = new Boolean(sharedPreferences.getBoolean("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START", false));
        }
        return isEnabledFilteringEventsOnStart.booleanValue();
    }

    public static void SetAdvencedKioskSettings(Context context, boolean status){
        SharedPreferences sharedPreferences = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();

        editor.putBoolean("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START", status);
        editor.apply();
        isEnabledFilteringEventsOnStart = new Boolean(status);

        SharedPreferences sharedPreferences2 = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        SharedPreferences.Editor editor2 = sharedPreferences2.edit();
        if (status) {
            editor2.putInt("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START_INDEX", 2);
        }
        else {
            editor2.putInt("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START_INDEX", 0);
        }
        editor2.apply();
    }

    public static void ClearAdvencedKioskSettings(Context context){
        isEnabledFilteringEventsOnStart = new Boolean(false);
        SharedPreferences sharedPreferences = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();

        editor.putBoolean("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START", false);
        editor.apply();

        SharedPreferences sharedPreferences2 = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        SharedPreferences.Editor editor2 = sharedPreferences2.edit();
        editor2.putInt("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START_INDEX", 0);
        editor2.apply();
    }

    public static boolean isKioskModeOnStartForAccessebility(Context context) {
        SharedPreferences sharedPreferences = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        int value = new Integer(sharedPreferences.getInt("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START_INDEX", 0));
        return (value > 0);
    }

    public static void decreaseKioskModeOnStartForAccessebility(Context context) {
        SharedPreferences sharedPreferences = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        int value = new Integer(sharedPreferences.getInt("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START_INDEX", 0));
        if (value > 0) {
            value = value - 1;
        }
        SharedPreferences sharedPreferences2 = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        SharedPreferences.Editor editor2 = sharedPreferences2.edit();
        editor2.putInt("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START_INDEX", value);
        editor2.apply();
        if (value <= 0) {
            ClearAdvencedKioskSettings(context);
        }
    }


}
