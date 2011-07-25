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

import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.database.Cursor;
import android.media.Ringtone;
import android.net.Uri;

public class RingtoneManager {

	private static final String TAG = "RingtoneManager";
	
	private static android.media.RingtoneManager manager = null;
	private static Context context = null;
	private static Ringtone current = null;
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static void init() {
		if (context == null)
			context = RhodesService.getContext();
		if (manager == null)
			manager = new android.media.RingtoneManager(context);
	}
	
	public static void play(String url) {
		try {
			init();
			
			stop();
			
			Logger.D("RingtoneManager", "play");
			current = android.media.RingtoneManager.getRingtone(context, Uri.parse(url));
			current.play();
		}
		catch (Exception e) {
			reportFail("play", e);
		}
	}
	
	public static void stop() {
		try {
			init();
			
			Logger.D("RingtoneManager", "stop");
			
			if (current != null) {
				current.stop();
				current = null;
			}
		}
		catch (Exception e) {
			reportFail("stop", e);
		}
	}
	
	public static Map<String, String> getAllRingtones() {
		try {
			init();
			
			Map<String, String> retval = new HashMap<String, String>();
			
			Logger.D("RingtoneManager", "Retrieve all ringtones");
			Cursor cursor = manager.getCursor();
			if (cursor.moveToFirst()) {
				for(int i = 0; i < cursor.getCount(); i++) {
					String name = manager.getRingtone(i).getTitle(context);
					String value = manager.getRingtoneUri(i).toString();
					Logger.D("RingtoneManager", "Retrieved ringtone '" + name + "'");
					retval.put(name, value);
				}
			}
			Logger.D("RingtoneManager", "All ringtones retrieved");
			
			return retval;
		}
		catch (Exception e) {
			reportFail("getAllRingtones", e);
		}
		
		return new HashMap<String, String>();
	}
	
}
