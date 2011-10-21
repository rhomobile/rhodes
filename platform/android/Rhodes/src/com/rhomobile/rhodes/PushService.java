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

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;

public class PushService {

	private static final String TAG = "PushService";

	public static final String C2DM_INTENT_PREFIX = "com.google.android.c2dm.intent.";

	private static void checkCapabilities() throws IllegalAccessException {
		if (!Capabilities.PUSH_ENABLED)
			throw new IllegalAccessException("Capability PUSH disabled");
	}

	private static boolean isOsVersionEnough() {
		int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
		return sdkVersion >= Build.VERSION_CODES.FROYO;
	}

	public static void register() throws IllegalAccessException {
		if (!isOsVersionEnough()) {
			Logger.E(TAG, "OS version is not enough for C2DM");
			return;
		}

		checkCapabilities();

		Logger.D(TAG, "Register for PUSH messages (use \"" + Push.SENDER + "\" as sender)");

		Context ctx = RhodesService.getContext();
		Intent registrationIntent = new Intent(C2DM_INTENT_PREFIX + "REGISTER");
		registrationIntent.putExtra("app", PendingIntent.getBroadcast(ctx, 0, new Intent(), 0));
		registrationIntent.putExtra("sender", Push.SENDER);
		ctx.startService(registrationIntent);
	}

	public static void unregister() throws IllegalAccessException {
		if (!isOsVersionEnough()) {
			Logger.E(TAG, "OS version is not enough for C2DM");
			return;
		}

		checkCapabilities();

		Logger.D(TAG, "Unregister from PUSH messages");

		Context ctx = RhodesService.getContext();
		Intent unregIntent = new Intent(C2DM_INTENT_PREFIX + "UNREGISTER");
		unregIntent.putExtra("app", PendingIntent.getBroadcast(ctx, 0, new Intent(), 0));
		ctx.startService(unregIntent);
	}

}
