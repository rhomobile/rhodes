/**
 * 
 */
package com.rhomobile.rhoelements.ans;


import com.symbol.rhoconnect.pushservice.ANSConstants;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;


public class ANSBroadcastReceiver extends BroadcastReceiver {
	
	private static final String TAG = ANSBroadcastReceiver.class.getSimpleName();

    @Override
    public void onReceive(Context context, Intent bcastIntent) {

        Logger.T(TAG, "Broadcast intent has received");

        StringBuffer buf = new StringBuffer("#### Intent :\n");
        buf.append(bcastIntent.toString() + "\n");
        Bundle extras = bcastIntent.getExtras();
        for (String key: extras.keySet()) {
            buf.append(key).append(": ").append(extras.get(key).toString()).append("\n");
        }
        buf.append("------------------------------------------------------------- ####");
        Logger.D(TAG,buf.toString());

        if (bcastIntent.getAction().equals(ANSConstants.ANS_REG_RESULT_ACTION)) {
            handleRegistration(context, bcastIntent.getExtras());
        }
        else if (bcastIntent.getAction().equals(ANSConstants.ANS_RECEIVE_ACTION)) {
            handleMessage(context, bcastIntent.getExtras());
        }
    }

    private void handleRegistration(Context context, Bundle extras) {
        Logger.T(TAG, "Handling rhoconnect-push relgistratoion response");

        String token = extras.getString(ANSConstants.ANS_EXTRA_TOKEN);
        String error = extras.getString(ANSConstants.ANS_EXTRA_ERROR);
        String type  = extras.getString(ANSConstants.ANS_EXTRA_RESPONSE_TYPE);

        // PROCESS REGISTRATION RESPONSE
        if (ANSConstants.ANS_REGISTER_RESPONSE.equals(type)) {
            if (error != null) {
                Logger.W(TAG, "Rhoconnect-push registration error: " + error);
                PushContract.handleError(context, error, ANSFacade.ANS_PUSH_CLIENT);
            }
            else {
                Logger.I(TAG, "Rhoconnect-push registration succeded");
                Logger.D(TAG, "Rhoconnect-push token: " + token);

                PushContract.handleRegistration(context, token, ANSFacade.ANS_PUSH_CLIENT);
            }
        }

        // PROCESS UNREGISTRATION RESPONSE
        else if (ANSConstants.ANS_UNREGISTER_RESPONSE.equals(type)) {
            if (error != null) {
                PushContract.handleError(context, error, ANSFacade.ANS_PUSH_CLIENT);
            }
            else {
                Logger.I(TAG, "Rhoconnect-push unregistration success");
            }
        }

        // PROCESS CHECK REGISTRATION RESPONSE
        else if (ANSConstants.ANS_CHECK_REGISTER_RESPONSE.equals(type)) {
            if (error != null) {
                Logger.W(TAG, "Rhoconnect-push registration check failed: " + error);
                PushContract.handleError(context, error, ANSFacade.ANS_PUSH_CLIENT);
            }
            else {
                Logger.I(TAG, "Rhoconect-push registration check succeded");
                Logger.D(TAG, "Rhoconect-push token: " + token);
                PushContract.handleRegistration(context, token, ANSFacade.ANS_PUSH_CLIENT);
            }
        }
        else {
            Logger.E(TAG, "Rhoconnect-push registration wron response type: " + type);
        }
    }

    private void handleMessage(Context context, Bundle extras) {
        Logger.T(TAG, "Processing push message");
        String jsonPayload = extras.getString(ANSConstants.ANS_EXTRA_JSON_PAYLOAD);

        Logger.D(TAG, "JSON payload: " + jsonPayload);
        PushContract.handleMessage(context, jsonPayload,  ANSFacade.ANS_PUSH_CLIENT);
    }
}
