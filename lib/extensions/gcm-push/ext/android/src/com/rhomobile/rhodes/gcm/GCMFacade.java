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

import com.google.android.gcm.GCMRegistrar;
import com.rhomobile.rhodes.Push;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.PushContract;
import com.rhomobile.rhodes.util.ContextFactory;

public final class GCMFacade {
    private static final String TAG = GCMFacade.class.getSimpleName();
    
    static final String GCM_PUSH_CLIENT = "gcm";
    
    public static void Register() {
        Logger.T(TAG, "Send GCM push register req");
        if (GCMRegistrar.isRegistered(ContextFactory.getContext())) {
            Logger.T(TAG, "Already has GCM push registeration");
            PushContract.handleRegistration(ContextFactory.getContext(),
                                            GCMRegistrar.getRegistrationId(ContextFactory.getContext()),
                                            GCMFacade.GCM_PUSH_CLIENT);
        }
        else {
            Logger.T(TAG, "Send GCM push register req");
            GCMRegistrar.register(ContextFactory.getContext(), Push.SENDER);
        }
    }

    public static void Unregister() {
        Logger.T(TAG, "GCM push unregister req");
        GCMRegistrar.unregister(ContextFactory.getContext());
    }

}
