/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2012 Rhomobile, Inc.
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

package com.rhomobile.rhodes.gcm;

import android.content.Context;
import android.content.Intent;

import com.google.android.gcm.GCMBaseIntentService;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;

public final class GCMIntentService extends GCMBaseIntentService {

    @Override
    protected void onRegistered(Context context, String pin) {
        Logger.T(TAG, "GCM push registation: " + pin);
        PushContract.handleRegistration(context, pin, GCMFacade.GCM_PUSH_CLIENT);
    }

    @Override
    protected void onUnregistered(Context context, String pin) {
        Logger.T(TAG, "GCM push unregistation: " + pin);
    }

    @Override
    protected void onMessage(Context context, Intent intent) {
        Logger.T(TAG, "GCM push message: " + intent);
        PushContract.handleMessage(context, intent.getExtras(), GCMFacade.GCM_PUSH_CLIENT);
    }

    @Override
    protected void onError(Context context, String error) {
        Logger.T(TAG, "GCM push error: " + error);
        PushContract.handleError(context, error, GCMFacade.GCM_PUSH_CLIENT);
    }
}
