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
			context = RhodesService.getInstance().getContext();
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
