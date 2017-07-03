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

package com.rhomobile.rhodes.fcm;

import com.rhomobile.rhodes.Push;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;
import com.rhomobile.rhodes.util.ContextFactory;

public final class FCMFacade {
    private static final String TAG = FCMFacade.class.getSimpleName();
    
    static final String FCM_PUSH_CLIENT = "FireBaseClient";

    public static void Register(String senderId) {
        Logger.T(TAG, "Send FCM push register req");
        
        if (senderId == null || senderId.length() == 0) {
            Logger.W(TAG, "Trying to read deprecated FCM push sender app id from build.yml");
            senderId = Push.SENDER;
        }
        
        if (FCMRegistrar.isRegistered(ContextFactory.getContext())) {
            Logger.T(TAG, "Already has FCM push registeration");
            PushContract.handleRegistration(ContextFactory.getContext(),
                                            FCMRegistrar.getRegistrationId(ContextFactory.getContext()),
                                            FCMFacade.FCM_PUSH_CLIENT);
        }
        else {
            Logger.T(TAG, "Send FCM push register req");
            FCMRegistrar.register(ContextFactory.getContext(), senderId);
        }
    }

    public static void Unregister() {
        Logger.T(TAG, "FCM push unregister req");
        FCMRegistrar.unregister(ContextFactory.getContext());
    }

}
