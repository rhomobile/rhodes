package com.rhomobile.rhodes.kioskservices;

import android.util.Log;


public class KioskManager {
    private static boolean isKioskMode = false;

    public static void setKioskMode(Boolean state){
        Log.d("myLog", "Kiosk status: "+(state?"true":"false"));
        isKioskMode = state;
    }

    public static Boolean getKioskModeStatus(){
        Log.d("myLog", "Kiosk status: "+(isKioskMode?"true":"false"));
        return isKioskMode;
    }
}
