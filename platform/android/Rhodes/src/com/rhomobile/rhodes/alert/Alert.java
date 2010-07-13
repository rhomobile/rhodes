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

import java.io.File;
import java.util.Map;
import java.util.Vector;

import android.app.Dialog;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Vibrator;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;
import com.rhomobile.rhodes.file.RhoFileApi;

public class Alert {
	
	private static final String TAG = "Alert";
	
	private static Dialog currentAlert = null;
	
	private static native void doCallback(String url, String id, String title);
	
	private static class CustomButton {
		
		public String id;
		public String title;
		
		public CustomButton(String t) {
			id = t;
			title = t;
		}
		
		public CustomButton(String i, String t) {
			id = i;
			title = t;
		}
	};
	
	private static class ShowDialogListener implements OnClickListener {

		private String callback;
		private String id;
		private String title;
		private Dialog dialog;
		
		public ShowDialogListener(String c, String i, String t, Dialog d) {
			callback = c;
			id = i;
			title = t;
			dialog = d;
		}
		
		public void onClick(View arg0) {
			if (callback != null) {
				doCallback(callback, Uri.encode(id), Uri.encode(title));
			}
			dialog.dismiss();
			currentAlert = null;
		}
		
	};
	
	private static class ShowDialog implements Runnable {
		private Object params;
		
		public ShowDialog(Object p) {
			params = p;
		}

		@SuppressWarnings("unchecked")
		public void run() {
			String title = "Alert";
			String message = null;
			Drawable icon = null;
			String callback = null;
			Vector<CustomButton> buttons = new Vector<CustomButton>();
			
			Rhodes r = RhodesInstance.getInstance();
			
			if (params instanceof String) {
				message = (String)params;
				buttons.addElement(new CustomButton("OK"));
			}
			else if (params instanceof Map<?,?>) {
				Map<Object, Object> hash = (Map<Object, Object>)params;
				
				Object titleObj = hash.get("title");
				if (titleObj != null && (titleObj instanceof String))
					title = (String)titleObj;
				
				Object messageObj = hash.get("message");
				if (messageObj != null && (messageObj instanceof String))
					message = (String)messageObj;
				
				Object iconObj = hash.get("icon");
				if (iconObj != null && (iconObj instanceof String)) {
					String iconName = (String)iconObj;
					if (iconName.equalsIgnoreCase("alert"))
						icon = r.getResources().getDrawable(AndroidR.drawable.alert_alert);
					else if (iconName.equalsIgnoreCase("question"))
						icon = r.getResources().getDrawable(AndroidR.drawable.alert_question);
					else if (iconName.equalsIgnoreCase("info"))
						icon = r.getResources().getDrawable(AndroidR.drawable.alert_info);
					else {
						String iconPath = RhoFileApi.normalizePath("apps/" + iconName);
						Bitmap bitmap = BitmapFactory.decodeStream(RhoFileApi.open(iconPath));
						if (bitmap != null)
							icon = new BitmapDrawable(bitmap);
					}
				}
				
				Object callbackObj = hash.get("callback");
				if (callbackObj != null && (callbackObj instanceof String))
					callback = (String)callbackObj;
				
				Object buttonsObj = hash.get("buttons");
				if (buttonsObj != null && (buttonsObj instanceof Vector<?>)) {
					Vector<Object> btns = (Vector<Object>)buttonsObj;
					for (int i = 0; i < btns.size(); ++i) {
						String itemId = null;
						String itemTitle = null;
						
						Object btnObj = btns.elementAt(i);
						if (btnObj instanceof String) {
							itemId = (String)btnObj;
							itemTitle = (String)btnObj;
						}
						else if (btnObj instanceof Map<?,?>) {
							Map<Object, Object> btnHash = (Map<Object, Object>)btnObj;
							Object btnIdObj = btnHash.get("id");
							if (btnIdObj != null && (btnIdObj instanceof String))
								itemId = (String)btnIdObj;
							Object btnTitleObj = btnHash.get("title");
							if (btnTitleObj != null && (btnTitleObj instanceof String))
								itemTitle = (String)btnTitleObj;
						}
						
						if (itemId == null || itemTitle == null) {
							Logger.E(TAG, "Incomplete button item");
							continue;
						}
						
						buttons.addElement(new CustomButton(itemId, itemTitle));
					}
				}
			}
			
			if (message == null)
				return;
			
			Dialog dialog = new Dialog(r);
			dialog.setTitle(title);
			dialog.setCancelable(false);
			dialog.setCanceledOnTouchOutside(false);
			
			LinearLayout main = new LinearLayout(r);
			main.setOrientation(LinearLayout.VERTICAL);
			main.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
			main.setPadding(10, 10, 10, 10);
			
			LinearLayout top = new LinearLayout(r);
			top.setOrientation(LinearLayout.HORIZONTAL);
			top.setGravity(Gravity.CENTER);
			top.setPadding(10, 10, 10, 10);
			top.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
			main.addView(top);
			
			if (icon != null) {
				ImageView imgView = new ImageView(r);
				imgView.setImageDrawable(icon);
				imgView.setScaleType(ImageView.ScaleType.CENTER);
				imgView.setPadding(10, 10, 10, 10);
				top.addView(imgView);
			}
			
			TextView textView = new TextView(r);
			textView.setText(message);
			textView.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
			textView.setGravity(Gravity.CENTER);
			top.addView(textView);
			
			LinearLayout bottom = new LinearLayout(r);
			bottom.setOrientation(buttons.size() > 3 ? LinearLayout.VERTICAL : LinearLayout.HORIZONTAL);
			bottom.setGravity(Gravity.CENTER);
			bottom.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
			main.addView(bottom);
			
			for (int i = 0, lim = buttons.size(); i < lim; ++i) {
				CustomButton btn = buttons.elementAt(i);
				Button button = new Button(r);
				button.setText(btn.title);
				button.setOnClickListener(new ShowDialogListener(callback, btn.id, btn.title, dialog));
				button.setLayoutParams(new LinearLayout.LayoutParams(
						lim > 3 ? LayoutParams.FILL_PARENT : LayoutParams.WRAP_CONTENT,
						LayoutParams.WRAP_CONTENT, 1));
				bottom.addView(button);
			}
			
			dialog.setContentView(main);
			dialog.show();
			
			currentAlert = dialog;
		}
	};
	
	private static class HideDialog implements Runnable {
		public void run() {
			if (currentAlert == null)
				return;
			currentAlert.dismiss();
			currentAlert = null;
		}
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

	public static void showPopup(Object params) {
		try {
			Logger.T(TAG, "showPopup");
			Rhodes.performOnUiThread(new ShowDialog(params), false);
		}
		catch (Exception e) {
			reportFail("showPopup", e);
		}
	}
	
	public static void hidePopup() {
		try {
			Logger.T(TAG, "hidePopup");
			Rhodes.performOnUiThread(new HideDialog(), false);
		}
		catch (Exception e) {
			reportFail("hidePopup", e);
		}
	}
	
	public static void vibrate(int duration) {
		try {
			if (!Capabilities.VIBRATE_ENABLED)
				throw new IllegalAccessException("VIBRATE disabled");
			Logger.T(TAG, "vibrate: " + duration);
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
			Logger.T(TAG, "playFile: " + fileName + " (" + mediaType + ")");
			MediaPlayer mp = new MediaPlayer();
			mp.setOnErrorListener(new MediaPlayer.OnErrorListener() {
				public boolean onError(MediaPlayer mp, int what, int extra) {
					Logger.E(TAG, "Error when playing file : " + what + ", " + extra);
					return false;
				}
			});
			fileName = RhoFileApi.normalizePath("apps/" + fileName);
			File f = new File(RhodesInstance.getInstance().getRootPath());
			f = new File(f, fileName);
			if (!f.exists())
				RhoFileApi.copy(fileName);
			String source = f.getCanonicalPath();
			Logger.T(TAG, "Final file name: " + source);
			mp.setDataSource(source);
			mp.prepare();
			mp.start();
		} catch (Exception e) {
			reportFail("playFile", e);
		}
	}
	
}
