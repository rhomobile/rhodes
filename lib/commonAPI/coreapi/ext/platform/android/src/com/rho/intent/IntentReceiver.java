package com.rho.intent;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class IntentReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        IntentSingleton singleton = (IntentSingleton)IntentFactorySingleton.getInstance().getApiSingleton();
        singleton.onNewIntent(IIntentSingleton.BROADCAST, intent);
    }

}
