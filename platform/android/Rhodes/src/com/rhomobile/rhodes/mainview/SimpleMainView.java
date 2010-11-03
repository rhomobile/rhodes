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
package com.rhomobile.rhodes.mainview;

import java.util.Map;
import java.util.Vector;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;

public class SimpleMainView implements MainView {

	private static final int WRAP_CONTENT = ViewGroup.LayoutParams.WRAP_CONTENT;
	private static final int FILL_PARENT = ViewGroup.LayoutParams.FILL_PARENT;
	
	private class ActionBack implements View.OnClickListener {
		public void onClick(View v) {
			back(0);
		}
	};
	
	private class ActionForward implements View.OnClickListener {
		public void onClick(View v) {
			forward(0);
		}
	};
	
	private class ActionHome implements View.OnClickListener {
		public void onClick(View v) {
			navigate(RhodesService.getInstance().getStartUrl(), 0);
		}
	};
	
	private class ActionOptions implements View.OnClickListener {
		public void onClick(View v) {
			navigate(RhodesService.getInstance().getOptionsUrl(), 0);
		}
	};
	
	private class ActionRefresh implements View.OnClickListener {
		public void onClick(View v) {
			reload(0);
		}
	};

	private class ActionExit implements View.OnClickListener {
		public void onClick(View v) {
			RhodesService.exit();
		}
	};

	private class ActionCustom implements View.OnClickListener {
		private String url;
		
		public ActionCustom(String u) {
			url = u;
		}
		
		public void onClick(View v) {
			PerformOnUiThread.exec(new Runnable() {
				public void run() {
					RhodesService.loadUrl(ActionCustom.this.url);
				}
			}, false);
		}
	};
	
	private LinearLayout view;
	private WebView webView;
	private LinearLayout navBar = null;
	
	public View getView() {
		return view;
	}

	public WebView getWebView(int tab_index) {
		return webView;
	}

	
	public WebView detachWebView() {
		WebView v = null;
		if (webView != null) {
			view.removeView(webView);
			v = webView;
			webView = null;
		}
		return v;
	}
	
	private View createButton(Map<Object,Object> hash) {
		Context ctx = RhodesService.getInstance().getContext();
		
		Object actionObj = hash.get("action");
		if (actionObj == null || !(actionObj instanceof String))
			throw new IllegalArgumentException("'action' should be String");
		
		String action = (String)actionObj;
		if (action.length() == 0)
			throw new IllegalArgumentException("'action' should not be empty");
		
		Drawable icon = null;
		String label = null;
		View.OnClickListener onClick = null;
		
		if (action.equalsIgnoreCase("back")) {
			icon = ctx.getResources().getDrawable(AndroidR.drawable.back);
			onClick = new ActionBack();
		}
		else if (action.equalsIgnoreCase("forward")) {
			icon = ctx.getResources().getDrawable(AndroidR.drawable.next);
			onClick = new ActionForward();
		}
		else if (action.equalsIgnoreCase("home")) {
			icon = ctx.getResources().getDrawable(AndroidR.drawable.home);
			onClick = new ActionHome();
		}
		else if (action.equalsIgnoreCase("options")) {
			icon = ctx.getResources().getDrawable(AndroidR.drawable.options);
			onClick = new ActionOptions();
		}
		else if (action.equalsIgnoreCase("refresh")) {
			icon = ctx.getResources().getDrawable(AndroidR.drawable.refresh);
			onClick = new ActionRefresh();
		}
		else if (action.equalsIgnoreCase("close") || action.equalsIgnoreCase("exit")) {
			icon = ctx.getResources().getDrawable(AndroidR.drawable.exit);
			onClick = new ActionExit();
		}
		else if (action.equalsIgnoreCase("separator"))
			return null;
		
		DisplayMetrics metrics = new DisplayMetrics();
		WindowManager wm = (WindowManager)RhodesService.getInstance().getContext().getSystemService(Context.WINDOW_SERVICE);
		wm.getDefaultDisplay().getMetrics(metrics);
		
		Object iconObj = hash.get("icon");
		if (iconObj != null) {
			if (!(iconObj instanceof String))
				throw new IllegalArgumentException("'icon' should be String");
			String iconPath = "apps/" + (String)iconObj;
			iconPath = RhoFileApi.normalizePath(iconPath);
			Bitmap bitmap = BitmapFactory.decodeStream(RhoFileApi.open(iconPath));
			if (bitmap == null)
				throw new IllegalArgumentException("Can't find icon: " + iconPath);
			bitmap.setDensity(metrics.DENSITY_MEDIUM);
			icon = new BitmapDrawable(bitmap);
		}
		
		if (icon == null) {
			Object labelObj = hash.get("label");
			if (labelObj == null || !(labelObj instanceof String))
				throw new IllegalArgumentException("'label' should be String");
			label = (String)labelObj;
		}
		
		if (icon == null && label == null)
			throw new IllegalArgumentException("One of 'icon' or 'label' should be specified");
		
		if (onClick == null)
			onClick = new ActionCustom(action);
		
		View button;
		if (icon != null) {
			ImageButton btn = new ImageButton(ctx);
			btn.setImageDrawable(icon);
			button = btn;
		}
		else {
			Button btn = new Button(ctx);
			btn.setText(label);
			button = btn;
		}
		
		button.setOnClickListener(onClick);
		
		return button;
	}
	
	@SuppressWarnings("unchecked")
	private void init(MainView v, Object params) {
		RhodesService r = RhodesService.getInstance();
		Context ctx = r.getContext();
		
		view = new LinearLayout(ctx);
		view.setOrientation(LinearLayout.VERTICAL);
		view.setGravity(Gravity.BOTTOM);
		view.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, FILL_PARENT));
		view.setId(RhodesService.RHO_MAIN_VIEW);
		
		webView = null;
		if (v != null)
			webView = v.detachWebView();
		if (webView == null)
			webView = r.createWebView();
		view.addView(webView, new LinearLayout.LayoutParams(FILL_PARENT, 0, 1));
		
		LinearLayout bottom = new LinearLayout(ctx);
		bottom.setOrientation(LinearLayout.HORIZONTAL);
		bottom.setBackgroundColor(Color.GRAY);
		bottom.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, WRAP_CONTENT, 0));
		view.addView(bottom);
		
		Vector<Object> buttons = null;
		if (params != null) {
			if (params instanceof Vector<?>) {
				buttons = (Vector<Object>)params;
			}
			else if (params instanceof Map<?,?>) {
				Map<Object,Object> settings = (Map<Object,Object>)params;
				
				Object colorObj = settings.get("color");
				if (colorObj != null && (colorObj instanceof Map<?,?>)) {
					Map<Object,Object> color = (Map<Object,Object>)colorObj;
					
					Object redObj = color.get("red");
					Object greenObj = color.get("green");
					Object blueObj = color.get("blue");
					
					if (redObj != null && greenObj != null && blueObj != null &&
							(redObj instanceof String) && (greenObj instanceof String) && (blueObj instanceof String)) {
						try {
							int red = Integer.parseInt((String)redObj);
							int green = Integer.parseInt((String)greenObj);
							int blue = Integer.parseInt((String)blueObj);
							
							bottom.setBackgroundColor(Color.rgb(red, green, blue));
						}
						catch (NumberFormatException e) {
							// Do nothing here
						}
					}
				}
				
				Object buttonsObj = settings.get("buttons");
				if (buttonsObj != null && (buttonsObj instanceof Vector<?>))
					buttons = (Vector<Object>)buttonsObj;
			}
		}
		
		if (params != null) {
			LinearLayout group = null;
			// First group should have gravity LEFT
			int gravity = Gravity.LEFT;
			for (int i = 0, lim = buttons.size(); i < lim; ++i) {
				Object param = buttons.elementAt(i);
				if (!(param instanceof Map<?,?>))
					throw new IllegalArgumentException("Hash expected");
				
				Map<Object, Object> hash = (Map<Object, Object>)param;
				
				View button = createButton(hash);
				if (button == null) {
					group = null;
					gravity = Gravity.CENTER;
					continue;
				}
				
				button.setLayoutParams(new LinearLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));
				if (group == null) {
					group = new LinearLayout(ctx);
					group.setGravity(gravity);
					group.setOrientation(LinearLayout.HORIZONTAL);
					group.setLayoutParams(new LinearLayout.LayoutParams(WRAP_CONTENT, FILL_PARENT, 1));
					bottom.addView(group);
				}
				group.addView(button);
			}
			
			// Last group should have gravity RIGHT
			if (group != null) {
				group.setGravity(Gravity.RIGHT);
				bottom.requestLayout();
			}
		}
		
		webView.requestFocus();
	}
	
	public SimpleMainView() {
		init(null, null);
	}
	
	public SimpleMainView(MainView v) {
		init(v, null);
	}
	
	public SimpleMainView(MainView v, Object params) {
		init(v, params);
	}
	
	public void back(int index) {
		RhodesService.navigateBack();
	}
	
	public void goBack() {
		webView.goBack();
	}

	public void forward(int index) {
		webView.goForward();
	}

	public void navigate(String url, int index) {
		webView.loadUrl(url);
	}
	
	public void reload(int index) {
		webView.reload();
	}
	
	public String currentLocation(int index) {
		return webView.getUrl();
	}

	public void switchTab(int index) {
		// Nothing
	}
	
	public int activeTab() {
		return 0;
	}
	
	public void loadData(String data, int index) {
		webView.loadData(data, "text/html", "utf-8");
	}

	public void addNavBar(String title, Map<Object,Object> left, Map<Object,Object> right) {
		removeNavBar();
		
		Context ctx = RhodesService.getInstance().getContext();
		
		LinearLayout top = new LinearLayout(ctx);
		top.setOrientation(LinearLayout.HORIZONTAL);
		top.setBackgroundColor(Color.GRAY);
		top.setGravity(Gravity.CENTER);
		top.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, WRAP_CONTENT, 0));
		
		View leftButton = createButton(left);
		leftButton.setLayoutParams(new LinearLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT, 1));
		top.addView(leftButton);
		
		TextView label = new TextView(ctx);
		label.setText(title);
		label.setGravity(Gravity.CENTER);
		label.setTextSize((float)30.0);
		label.setLayoutParams(new LinearLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT, 2));
		top.addView(label);
		
		if (right != null) {
			View rightButton = createButton(right);
			rightButton.setLayoutParams(new LinearLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT, 1));
			top.addView(rightButton);
		}
		
		navBar = top;
		view.addView(navBar, 0);
	}
	
	public void removeNavBar() {
		if (navBar == null)
			return;
		view.removeViewAt(0);
		navBar = null;
	}
}
