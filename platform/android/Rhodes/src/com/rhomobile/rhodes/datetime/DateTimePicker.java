/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes.datetime;

import android.content.Intent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;

public class DateTimePicker {
	
	private static final String TAG = "DateTimePicker";
	
	public static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + "datetime.";
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

	public static void choose(String callback, String title, long init, int v, byte[] opaque) {
		try {
			RhodesService r = RhodesService.getInstance();
			Intent intent = new Intent(r.getContext(), DateTimePickerScreen.class);
			intent.putExtra(INTENT_EXTRA_PREFIX + "callback", callback);
			intent.putExtra(INTENT_EXTRA_PREFIX + "title", title);
			intent.putExtra(INTENT_EXTRA_PREFIX + "init", init);
			intent.putExtra(INTENT_EXTRA_PREFIX + "fmt", v);
			intent.putExtra(INTENT_EXTRA_PREFIX + "opaque", opaque);
			
			r.startActivity(intent);
		}
		catch (Exception e) {
			reportFail("choose", e);
		}
	}
	
	public native static void callback(String callbackUrl, long result, byte[] opaque, boolean cancelled);
	
}
