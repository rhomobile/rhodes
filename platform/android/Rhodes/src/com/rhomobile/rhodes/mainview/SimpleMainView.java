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
import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;

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
			navigate(RhodesInstance.getInstance().getStartUrl(), 0);
		}
	};
	
	private class ActionOptions implements View.OnClickListener {
		public void onClick(View v) {
			navigate(RhodesInstance.getInstance().getOptionsUrl(), 0);
		}
	};
	
	private class ActionRefresh implements View.OnClickListener {
		public void onClick(View v) {
			reload(0);
		}
	};
	
	private class ActionCustomRunnable implements Runnable {
		private boolean callback;
		private String url;
		public ActionCustomRunnable(boolean c, String u) {
			callback = c;
			url = u;
		}
		
		public void run() {
			if (callback)
				RhodesInstance.getInstance().doRequest(url);
			else
				navigate(url, 0);
		}
	};
	
	private class ActionCustom implements View.OnClickListener {
		private static final String scheme = "callback:";
		private String url;
		private boolean callback;
		
		public ActionCustom(String u) {
			callback = false;
			url = u;
			if (url.startsWith(scheme)) {
				url = url.substring(scheme.length());
				callback = true;
			}
			url = RhodesInstance.getInstance().normalizeUrl(url);
		}
		
		public void onClick(View v) {
			Rhodes.performOnUiThread(new ActionCustomRunnable(callback, url), false);
		}
	};
	
	private LinearLayout view;
	private WebView webView;
	
	public View getView() {
		return view;
	}
	
	@SuppressWarnings("unchecked")
	private void init(Vector<Object> params) {
		Rhodes r = RhodesInstance.getInstance();
		
		view = new LinearLayout(r);
		view.setOrientation(LinearLayout.VERTICAL);
		view.setGravity(Gravity.BOTTOM);
		view.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, FILL_PARENT));
		view.setId(Rhodes.RHO_MAIN_VIEW);
		
		webView = r.createWebView();
		view.addView(webView, new LinearLayout.LayoutParams(FILL_PARENT, 0, 1));
		
		LinearLayout bottom = new LinearLayout(r);
		bottom.setOrientation(LinearLayout.HORIZONTAL);
		bottom.setBackgroundColor(Color.GRAY);
		bottom.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, WRAP_CONTENT, 0));
		view.addView(bottom);
		
		if (params != null) {
			LinearLayout left = new LinearLayout(r);
			left.setGravity(Gravity.LEFT);
			left.setOrientation(LinearLayout.HORIZONTAL);
			left.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, FILL_PARENT, 1));
			bottom.addView(left);
			
			LinearLayout right = new LinearLayout(r);
			right.setGravity(Gravity.RIGHT);
			right.setOrientation(LinearLayout.HORIZONTAL);
			right.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, FILL_PARENT, 1));
			bottom.addView(right);
			
			LinearLayout current = left;
			
			String rootPath = r.getRootPath() + "/apps/";
			
			for (int i = 0, lim = params.size(); i < lim; ++i) {
				Object param = params.elementAt(i);
				if (!(param instanceof Map<?,?>))
					throw new IllegalArgumentException("Hash expected");
				
				Map<Object, Object> hash = (Map<Object, Object>)param;
				
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
					icon = r.getResources().getDrawable(AndroidR.drawable.back);
					onClick = new ActionBack();
				}
				else if (action.equalsIgnoreCase("forward")) {
					icon = r.getResources().getDrawable(AndroidR.drawable.next);
					onClick = new ActionForward();
				}
				else if (action.equalsIgnoreCase("home")) {
					icon = r.getResources().getDrawable(AndroidR.drawable.home);
					onClick = new ActionHome();
				}
				else if (action.equalsIgnoreCase("options")) {
					icon = r.getResources().getDrawable(AndroidR.drawable.options);
					onClick = new ActionOptions();
				}
				else if (action.equalsIgnoreCase("refresh")) {
					icon = r.getResources().getDrawable(AndroidR.drawable.refresh);
					onClick = new ActionRefresh();
				}
				else if (action.equalsIgnoreCase("separator")) {
					current = right;
					continue;
				}
				
				Object iconObj = hash.get("icon");
				if (iconObj != null) {
					if (!(iconObj instanceof String))
						throw new IllegalArgumentException("'icon' should be String");
					String iconPath = rootPath + (String)iconObj;
					Bitmap bitmap = BitmapFactory.decodeFile(iconPath);
					if (bitmap != null)
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
				
				View button = null;
				if (icon != null) {
					ImageButton btn = new ImageButton(r);
					btn.setImageDrawable(icon);
					button = btn;
				}
				else {
					Button btn = new Button(r);
					btn.setText(label);
					button = btn;
				}
				
				if (button == null)
					continue;
				
				if (onClick == null)
					onClick = new ActionCustom(action);
				
				button.setOnClickListener(onClick);
				button.setLayoutParams(new LinearLayout.LayoutParams(WRAP_CONTENT, WRAP_CONTENT));
				current.addView(button);
			}
		}
	}
	
	public SimpleMainView() {
		init(null);
	}
	
	public SimpleMainView(Vector<Object> params) {
		init(params);
	}
	
	public void back(int index) {
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

}
