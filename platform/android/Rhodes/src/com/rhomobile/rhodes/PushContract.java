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

package com.rhomobile.rhodes;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;


public class PushContract {
    private static final String TAG = PushContract.class.getSimpleName();
    
    public static final String INTENT_PREFIX = PushContract.class.getPackage().getName();
    
    public static final String INTENT_SOURCE = INTENT_PREFIX + ".PushContract";
    public static final String INTENT_TYPE = INTENT_PREFIX + ".Type";
    public static final String INTENT_REGISTRATION_ID = INTENT_PREFIX + ".RegistrationId";
    public static final String INTENT_PUSH_CLIENT = INTENT_PREFIX + ".MessageType";
    public static final String INTENT_MESSAGE_EXTRAS = INTENT_PREFIX + ".Extras";
    public static final String INTENT_MESSAGE_JSON = INTENT_PREFIX + ".Json";

    public static final int INTENT_TYPE_UNKNOWN = 0;
    public static final int INTENT_TYPE_REGISTRATION_ID = 1;
    public static final int INTENT_TYPE_MESSAGE = 2;


    private static Intent getIntent(Context context) {
        Intent intent = new Intent(context, RhodesService.class);
        intent.putExtra(RhodesService.INTENT_SOURCE, INTENT_SOURCE);
        return intent;
    }
    
    public static void handleRegistration(Context context, String token, String pushClient) {
        Intent intent = getIntent(context);
        intent.putExtra(INTENT_TYPE, INTENT_TYPE_REGISTRATION_ID);
        intent.putExtra(INTENT_REGISTRATION_ID, token);

        context.startService(intent);
    }

    public static void handleMessage(Context context, Bundle extras, String pushClient) {
        Intent intent = getIntent(context);
        intent.putExtra(INTENT_TYPE, INTENT_TYPE_MESSAGE);
        intent.putExtra(INTENT_PUSH_CLIENT, pushClient);
        intent.putExtra(INTENT_MESSAGE_EXTRAS, extras);

        context.startService(intent);
    }

    public static void handleMessage(Context context, String json, String pushClient) {
        Intent intent = getIntent(context);
        intent.putExtra(INTENT_TYPE, INTENT_TYPE_MESSAGE);
        intent.putExtra(INTENT_PUSH_CLIENT, pushClient);
        intent.putExtra(INTENT_MESSAGE_JSON, json);

        context.startService(intent);
    }

    public static void handleError(Context context, String error, String pushClient) {
        try {
            Bundle extras = new Bundle();
            extras.putString("error", URLEncoder.encode(error, "utf-8"));

            Intent intent = getIntent(context);
            intent.putExtra(INTENT_TYPE, INTENT_TYPE_MESSAGE);
            intent.putExtra(INTENT_PUSH_CLIENT, pushClient);
            intent.putExtra(INTENT_MESSAGE_EXTRAS, extras);

            context.startService(intent);
        } catch (UnsupportedEncodingException e) {
            Logger.E(TAG, e);
        };
    }

}
