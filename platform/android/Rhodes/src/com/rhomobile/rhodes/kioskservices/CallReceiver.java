package com.rhomobile.rhodes.kioskservices;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.telecom.TelecomManager;
import android.telephony.TelephonyManager;
import android.widget.Toast;

import java.net.URISyntaxException;

public class CallReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        TelephonyManager telephonyManager = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
        TelecomManager telecomManager = (TelecomManager)context.getSystemService(Context.TELECOM_SERVICE);

        if(KioskManager.getKioskModeStatus() && telephonyManager.getCallState() == TelephonyManager.CALL_STATE_RINGING){
            telecomManager.endCall();
        }
    }
}
