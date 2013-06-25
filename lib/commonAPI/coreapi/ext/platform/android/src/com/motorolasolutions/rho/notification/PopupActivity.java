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

package com.motorolasolutions.rho.notification;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.SparseArray;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.R;

public class PopupActivity extends BaseActivity
{
	private static String BUTTON_ID = "button_id";
	private static String BUTTON_TITLE = "button_title";
	private static String BUTTON_INDEX = "button_index";
	
	private static final String TAG = PopupActivity.class.getSimpleName();
	private static final String INTENT_EXTRA_PREFIX = RhodesService.INTENT_EXTRA_PREFIX + ".popup";
	private static Dialog currentAlert = null;
	private static TextView s_textView = null;
	private static native void doCallback(String url, String id, String title);
	private static SparseArray<MethodResult> callbackMap = new SparseArray<MethodResult>();
	private static int callbackIdCount = 0;

	private static class CustomButton
	{
		public String id;
		public String title;
		public CustomButton(String i, String t)
		{
			id = i;
			title = t;
		}
	};

	private static class ShowDialogListener implements OnClickListener
	{
		private int callbackId;
		private String id;
		private String title;
		private Dialog dialog;
		private int index;
		
		public ShowDialogListener(int c, String i, String t, int index, Dialog d)
		{
			callbackId = c;
			id = i;
			title = t;
			dialog = d;
			this.index = index;
		}

		public synchronized void onClick(View arg0)
		{
			Map<String, Object> callbackData = new HashMap<String, Object>();
			callbackData.put(BUTTON_ID, id);
			callbackData.put(BUTTON_TITLE, title);
			callbackData.put(BUTTON_INDEX, index + "");
			fireCallback(callbackId, callbackData);
			
			dialog.dismiss();
			currentAlert = null;
		}
	};

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		InputStream iconStream = null;

		Window window = getWindow();
		WindowManager.LayoutParams lp = window.getAttributes();
		lp.type = WindowManager.LayoutParams.TYPE_APPLICATION_ATTACHED_DIALOG;
		window.setAttributes(lp);
		window.addFlags(WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM);

		Bundle extras = getIntent().getExtras();
		if (extras == null)
		{
			Logger.E(TAG, "No data passed to popup activity");
			finish();
			return;
		}

		String title = extras.getString(INTENT_EXTRA_PREFIX + ".title");
		String message = extras.getString(INTENT_EXTRA_PREFIX + ".message");
		String iconName = extras.getString(INTENT_EXTRA_PREFIX + ".icon");
		int callbackId = extras.getInt(INTENT_EXTRA_PREFIX + ".callback");

		ArrayList<String> buttonIds = new ArrayList<String>();
		Object idsObj = extras.get(INTENT_EXTRA_PREFIX + ".buttons.ids");
		if (idsObj != null)
		{
			if (idsObj instanceof String[])
			{
				String[] ids = (String[]) idsObj;
				for (int i = 0; i < ids.length; ++i)
					buttonIds.add(ids[i]);
			}
			else if (idsObj instanceof ArrayList<?>)
			{
				ArrayList<?> ids = (ArrayList<?>) idsObj;
				for (Object idObj : ids)
					buttonIds.add(idObj.toString());
			}
		}
		ArrayList<String> buttonTitles = new ArrayList<String>();
		Object titlesObj = extras.get(INTENT_EXTRA_PREFIX + ".buttons.titles");
		if (titlesObj != null)
		{
			if (titlesObj instanceof String[])
			{
				String[] titles = (String[]) titlesObj;
				for (int i = 0; i < titles.length; ++i)
					buttonTitles.add(titles[i]);
			}
			else if (titlesObj instanceof ArrayList<?>)
			{
				ArrayList<?> titles = (ArrayList<?>) titlesObj;
				for (Object titleObj : titles)
					buttonTitles.add(titleObj.toString());
			}
		}

		Vector<CustomButton> buttons = new Vector<CustomButton>();
		if (buttonIds.size() != buttonTitles.size())
		{
			Logger.E(TAG, "Corrupted data passed to popup activity");
			finish();
			return;
		}

		for (int i = 0; i < buttonIds.size(); ++i)
			buttons.addElement(new CustomButton(buttonIds.get(i), buttonTitles.get(i)));

		Drawable icon = null;
		if (iconName != null)
		{
			Resources res = getResources();
			if (iconName.equalsIgnoreCase("alert"))
				icon = res.getDrawable(R.drawable.alert_alert);
			else if (iconName.equalsIgnoreCase("question"))
				icon = res.getDrawable(R.drawable.alert_question);
			else if (iconName.equalsIgnoreCase("info"))
				icon = res.getDrawable(R.drawable.alert_info);
			else
			{
				String iconPath = RhoFileApi.normalizePath(iconName);
				iconStream = RhoFileApi.open(iconPath);
				Bitmap bitmap = BitmapFactory.decodeStream(iconStream);
				if (bitmap != null)
					icon = new BitmapDrawable(bitmap);
			}
		}

		createDialog(title, message, icon, buttons, callbackId);
		if(iconStream != null)
		{
			try
			{
				iconStream.close();
			}
			catch(IOException e)
			{
				Logger.D(TAG, "Couldnt close the icon stream");
			}
		}
	}

	private synchronized void createDialog(String title, String message, Drawable icon, Vector<CustomButton> buttons, int callbackId)
	{
		Context ctx = this;

		Logger.T(TAG, "Creating dialog{ title: " + title + ", message: " + message + ", buttons cnt: " + buttons.size());

		int nTopPadding = 10;

		Dialog dialog = new Dialog(ctx);
		if (title == null || title.length() == 0)
		{
			dialog.getWindow();
			dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
		}
		else
		{
			dialog.setTitle(title);
			nTopPadding = 0;
		}

		dialog.setCancelable(false);
		dialog.setCanceledOnTouchOutside(false);

		LinearLayout main = new LinearLayout(ctx);
		main.setOrientation(LinearLayout.VERTICAL);
		main.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		main.setPadding(10, nTopPadding, 10, 10);

		LinearLayout top = new LinearLayout(ctx);
		top.setOrientation(LinearLayout.HORIZONTAL);
		top.setGravity(Gravity.CENTER);
		top.setPadding(10, nTopPadding, 10, 10);
		top.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
		main.addView(top);

		if (icon != null)
		{
			ImageView imgView = new ImageView(ctx);
			imgView.setImageDrawable(icon);
			imgView.setScaleType(ImageView.ScaleType.CENTER);
			imgView.setPadding(10, nTopPadding, 10, 10);
			top.addView(imgView);
		}

		if (message != null)
		{
			TextView textView = new TextView(ctx);
			s_textView = textView;
			textView.setText(message);
			textView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
			textView.setGravity(Gravity.CENTER);
			top.addView(textView);
		}

		LinearLayout bottom = new LinearLayout(ctx);
		bottom.setOrientation(buttons.size() > 3 ? LinearLayout.VERTICAL : LinearLayout.HORIZONTAL);
		bottom.setGravity(Gravity.CENTER);
		bottom.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
		main.addView(bottom);

		for (int i = 0, lim = buttons.size(); i < lim; ++i)
		{
			final CustomButton btn = buttons.elementAt(i);
			Button button = new Button(ctx);
			button.setText(btn.title);
			button.setOnClickListener(new ShowDialogListener(callbackId, btn.id, btn.title, i, dialog));
			button.setLayoutParams(new LinearLayout.LayoutParams(lim > 3 ? LayoutParams.MATCH_PARENT : LayoutParams.WRAP_CONTENT,
					LayoutParams.WRAP_CONTENT, 1));
			bottom.addView(button);
		}

		dialog.setContentView(main);

		dialog.setOnCancelListener(new DialogInterface.OnCancelListener()
		{
			@Override
			public void onCancel(DialogInterface dialog)
			{
				finish();
			}
		});
		dialog.setOnDismissListener(new DialogInterface.OnDismissListener()
		{
			@Override
			public void onDismiss(DialogInterface dialog)
			{
				finish();
			}
		});

		dialog.show();

		currentAlert = dialog;
	}

	public static void showDialog(Object params, IMethodResult result)
	{
		showDialog(RhodesService.getInstance(), params, result);
	}

	@SuppressWarnings("unchecked")
	public static void showDialog(RhodesService ctx, Object params, IMethodResult result)
	{
		int callbackId;
		synchronized(callbackMap)
		{
			callbackId = callbackIdCount++;
			((MethodResult) result).keepAlive();
			callbackMap.put(callbackId, ((MethodResult) result));
		}
		
		String title = "";
		String message = null;
		String icon = null;
		ArrayList<String> buttonIds = new ArrayList<String>();
		ArrayList<String> buttonTitles = new ArrayList<String>();

		if (params instanceof String)
		{
			message = (String) params;
			buttonIds.add("OK");
			buttonTitles.add("OK");
		}
		else if (params instanceof Map<?, ?>)
		{
			Map<Object, Object> hash = (Map<Object, Object>) params;

			Object titleObj = hash.get("title");
			if (titleObj != null && (titleObj instanceof String))
				title = (String) titleObj;

			Object messageObj = hash.get("message");
			if (messageObj != null && (messageObj instanceof String))
				message = (String) messageObj;

			Object iconObj = hash.get("icon");
			if (iconObj != null && (iconObj instanceof String))
				icon = (String) iconObj;

			Object buttonsObj = hash.get("buttons");
			if (buttonsObj != null && (buttonsObj instanceof List<?>))
			{
				List<Object> btns = (List<Object>) buttonsObj;
				for (int i = 0; i < btns.size(); ++i)
				{
					String itemId = null;
					String itemTitle = null;

					Object btnObj = btns.get(i);
					Logger.D(TAG, "button object=" + btnObj.getClass().getName());
					if (btnObj instanceof String)
					{
						itemId = (String) btnObj;
						itemTitle = (String) btnObj;
					}
					else if (btnObj instanceof Map<?, ?>)
					{
						Map<Object, Object> btnHash = (Map<Object, Object>) btnObj;
						Object btnIdObj = btnHash.get("id");
						if (btnIdObj != null && (btnIdObj instanceof String))
							itemId = (String) btnIdObj;
						Object btnTitleObj = btnHash.get("title");
						if (btnTitleObj != null && (btnTitleObj instanceof String))
							itemTitle = (String) btnTitleObj;
					}

					if (itemId == null || itemTitle == null)
					{
						Logger.E(TAG, "Incomplete button item");
						continue;
					}

					buttonIds.add(itemId);
					buttonTitles.add(itemTitle);
				}
			}
			else
			{
				Logger.W(TAG, "Unknown button description object. Will not show dialog without buttons");
				result.setError("Button list has been incorrectly defined. Dialog will not launch");
				return;
			}
		}

		Intent intent = new Intent(ctx, PopupActivity.class);
		// intent.addFlags(/*Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_NO_USER_ACTION*/);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".title", title);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".message", message);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".icon", icon);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".callback", callbackId);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".buttons.ids", buttonIds);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".buttons.titles", buttonTitles);
		ctx.startActivity(intent);
	}

	public static void showStatusDialog(String title, String message, String hide)
	{
		showStatusDialog(RhodesService.getInstance(), title, message, hide);
	}

	public synchronized static void showStatusDialog(Context ctx, String title, String message, String hide)
	{
		if (currentAlert != null)
		{
			s_textView.setText(message);
			return;
		}

		Intent intent = new Intent(ctx, PopupActivity.class);
		// intent.addFlags(/*Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_NO_USER_ACTION*/);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".title", title);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".message", message);
		intent.putExtra(INTENT_EXTRA_PREFIX + ".buttons.ids", new String[] { hide });
		intent.putExtra(INTENT_EXTRA_PREFIX + ".buttons.titles", new String[] { hide });
		ctx.startActivity(intent);
	}

	public synchronized static void hidePopup()
	{
		if (currentAlert == null)
			return;
		currentAlert.dismiss();
		currentAlert = null;
	}
	
	public static synchronized void fireCallback(int callbackId, Map<String, Object> data)
	{
		MethodResult result = callbackMap.get(callbackId);
		if(result != null)
		{
			result.set(data);
			result.release();
			callbackMap.remove(callbackId);
		}
	}
}
