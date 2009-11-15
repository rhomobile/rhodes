package com.rhomobile.rhodes;

import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.database.Cursor;
import android.media.Ringtone;
import android.net.Uri;
import android.util.Log;

public class RingtoneManager {

	private static android.media.RingtoneManager manager = null;
	private static Context context = null;
	private static Ringtone current = null;
	
	private static void init() {
		if (context == null)
			context = RhodesInstance.getInstance().getApplicationContext();
		if (manager == null)
			manager = new android.media.RingtoneManager(context);
	}
	
	public static void play(String url) {
		init();
		
		stop();
		
		Log.d("RingtoneManager", "play");
		current = android.media.RingtoneManager.getRingtone(context, Uri.parse(url));
		current.play();
	}
	
	public static void stop() {
		init();
		
		Log.d("RingtoneManager", "stop");
		
		if (current != null) {
			current.stop();
			current = null;
		}
	}
	
	public static Map<String, String> getAllRingtones() {
		init();
		
		Map<String, String> retval = new HashMap<String, String>();
		
		Log.d("RingtoneManager", "Retrieve all ringtones");
		Cursor cursor = manager.getCursor();
		if (cursor.moveToFirst()) {
			for(int i = 0; i < cursor.getCount(); i++) {
				String name = manager.getRingtone(i).getTitle(context);
				String value = manager.getRingtoneUri(i).toString();
				Log.d("RingtoneManager", "Retrieved ringtone '" + name + "'");
				retval.put(name, value);
			}
		}
		Log.d("RingtoneManager", "All ringtones retrieved");
		
		return retval;
	}
	
}
