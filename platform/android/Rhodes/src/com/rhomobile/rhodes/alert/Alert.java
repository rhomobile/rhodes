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
		private AlertDialog.Builder builder;
		
		public ShowDialog(AlertDialog.Builder b) {
			builder = b;
		}

		public void run() {
			builder.show();
		}
	};

	public static void showPopup(String message) {
		try {
			AlertDialog.Builder builder = new AlertDialog.Builder(RhodesInstance.getInstance());
			builder.setTitle("Alert");
			builder.setMessage(message);
			builder.setCancelable(false);
			builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int which) {
					// Do nothing here
				}					
			});
			RhodesInstance.getInstance().uiHandler.post(new ShowDialog(builder));
		}
		catch (Exception e) {
			Logger.E(TAG, e.getMessage());
		}
	}
	
	public static void vibrate(int duration) {
		try {
			Rhodes instance = RhodesInstance.getInstance();
			Vibrator vibrator = (Vibrator)instance.getSystemService(Context.VIBRATOR_SERVICE);
			vibrator.vibrate(duration);
		}
		catch (Exception e) {
			Logger.E(TAG, e.getMessage());
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
			Logger.E(TAG, e.getMessage());
		}
	}
	
}
