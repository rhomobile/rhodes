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

package com.rhomobile.rhodes.datetime;

import android.content.Intent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;

public class DateTimePicker {

	private static final String TAG = "DateTimePicker";

	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + "datetime.";

	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

	public static void choose(String callback, String title, long init, int v, byte[] opaque, long min_time, long max_time) {
		try {
			RhodesActivity r = RhodesActivity.getInstance();
			Intent intent = new Intent(RhodesActivity.getContext(), DateTimePickerScreen.class);
			intent.putExtra(INTENT_EXTRA_PREFIX + "callback", callback);
			intent.putExtra(INTENT_EXTRA_PREFIX + "title", title);
			intent.putExtra(INTENT_EXTRA_PREFIX + "init", init);
			intent.putExtra(INTENT_EXTRA_PREFIX + "fmt", v);
			intent.putExtra(INTENT_EXTRA_PREFIX + "opaque", opaque);
			intent.putExtra(INTENT_EXTRA_PREFIX + "min_time", min_time);
			intent.putExtra(INTENT_EXTRA_PREFIX + "max_time", max_time);

			r.startActivity(intent);
		}
		catch (Exception e) {
			reportFail("choose", e);
		}
	}

	public native static void callback(String callbackUrl, long result, byte[] opaque, boolean cancelled);

}
