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
    }

    public static void ClearAdvencedKioskSettings(Context context){
        isEnabledFilteringEventsOnStart = new Boolean(false);
        SharedPreferences sharedPreferences = context.getSharedPreferences("ADVANCED_KIOSK_SETTINGS", 0);
        SharedPreferences.Editor editor = sharedPreferences.edit();

        editor.putBoolean("KIOSK_MODE_ENABLE_FILTERING_EVENTS_ON_START", false);
        editor.apply();
    }
}
