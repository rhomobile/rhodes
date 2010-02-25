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
package com.rhomobile.rhodes.alert;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.media.MediaPlayer;
import android.os.Vibrator;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

public class Alert {
	
	private static final String TAG = "Alert";
	
	private static class ShowDialog implements Runnable {
		private String message;
		
		public ShowDialog(String m) {
			message = m;
		}

		public void run() {
			AlertDialog.Builder builder = new AlertDialog.Builder(RhodesInstance.getInstance());
			builder.setTitle("Alert");
			builder.setMessage(message);
			builder.setCancelable(false);
			builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					// Do nothing here
				}					
			});
			builder.show();
		}
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

	public static void showPopup(String message) {
		try {
			Rhodes.performOnUiThread(new ShowDialog(message));
		}
		catch (Exception e) {
			reportFail("showPopup", e);
		}
	}
	
	public static void vibrate(int duration) {
		try {
			Rhodes instance = RhodesInstance.getInstance();
			Vibrator vibrator = (Vibrator)instance.getSystemService(Context.VIBRATOR_SERVICE);
			vibrator.vibrate(duration);
		}
		catch (Exception e) {
			reportFail("showPopup", e);
		}
	}
	
	public static void playFile(String fileName, String mediaType) {
		try {
			MediaPlayer mp = new MediaPlayer();
			String source = RhodesInstance.getInstance().getRootPath() + "/apps/" + fileName;
			mp.setDataSource(source);
			mp.prepare();
			mp.start();
		} catch (Exception e) {
			reportFail("playFile", e);
		}
	}
	
}
