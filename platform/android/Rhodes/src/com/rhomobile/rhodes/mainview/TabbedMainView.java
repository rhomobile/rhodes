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

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.file.RhoFileApi;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RoundRectShape;
import android.graphics.drawable.shapes.Shape;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.webkit.WebView;
import android.widget.FrameLayout;
import android.widget.TabHost;
import android.widget.TabWidget;

public class TabbedMainView implements MainView {
	
	private static final String TAG = "TabbedMainView";
	
	private static final int DISABLED_BKG_COLOR = 0xFF000000 | (128 << 16) | (128 << 8) | (128);
	private static final int DISABLED_IMG_COLOR = 0xFF000000 | (164 << 16) | (164 << 8) | (164);
	
	private TabHost host;
	private Vector<TabData> tabData;
	private int tabIndex;
	
	private int mBackgroundColor = 0;
	private boolean mBackgroundColorEnable = false;
	
	private static class TabData {
		public MainView view;
		public String url;
		public boolean reload;
		public boolean loaded;
		
		public int selected_color;
		public boolean selected_color_enabled;
		public boolean disabled;
		
		public TabData() {
			loaded = false;
			selected_color_enabled = false;
			disabled = false;
		}
	};
	
	private class RhoCustomDrawable extends ColorDrawable {
		
		public static final int NORMAL = 1;
		public static final int SELECTED = 2;
		public static final int DISABLED = 3;
		
		public RhoCustomDrawable() {
			style = NORMAL;
			color = 0xFF000000;
			last_tab = false;
			first_tab = false;
		}

		public void setRhoStyle(int _style) {
			style = _style;
		}
		
		public void setRhoColor(int _color) {
			color = _color;
		}
		
		public void setRhoLastTab(boolean lt) {
			last_tab = lt;
		}
		
		public void setRhoFirstTab(boolean ft) {
			first_tab = ft;
		}

		private int modifyColorComponent(int c, int delta) {
			int r = c + delta;
			if (r < 0) r = 0;
			if (r > 255) r = 255;
			return r;
		}

		private void drawVerticalGradient(	Canvas canvas, int left, int top, int right, int bottom, 
											int color0_R, int color0_G, int color0_B,
											int color1_R, int color1_G, int color1_B) {
	        Paint paint = new Paint();
	        paint.setAntiAlias(false);
	        paint.setARGB(255, 0, 0, 0);
	        int i;
	        for (i = top ; i < bottom; i++) {
	        	int ctop = i;
	        	int cbottom = i + 1;
	        	
	        	int cR = color0_R + ((color1_R - color0_R)*(i-top))/(bottom-top-1);
	        	int cG = color0_G + ((color1_G - color0_G)*(i-top))/(bottom-top-1);
	        	int cB = color0_B + ((color1_B - color0_B)*(i-top))/(bottom-top-1);
	        	
	        	paint.setARGB(255, cR, cG, cB);
	       		canvas.drawRect(left, ctop, right, cbottom, paint);
	        }
		}
		
		
		public void draw (Canvas canvas) {
			Rect rect = getBounds();
			
			int c_R = (color & 0xFF0000) >> 16;
			int c_G = (color & 0xFF00) >> 8;
			int c_B = (color & 0xFF);
			int height = rect.bottom - rect.top + 1;
			
			int gap = 3;

			Context ctx = RhodesActivity.getContext();
			DisplayMetrics metrics = new DisplayMetrics();
			WindowManager wm = (WindowManager)ctx.getSystemService(Context.WINDOW_SERVICE);
			wm.getDefaultDisplay().getMetrics(metrics);
			
			int lcd_density = metrics.densityDpi;
			if (lcd_density > 200) gap = 4;
			int left_gap = gap;
			int right_gap = gap;
			if (lcd_density > 200) left_gap += 1;
			if (lcd_density > 200) right_gap += 1;
			if (first_tab) left_gap = 0; 
			if (last_tab) right_gap = 0; 
			
			switch (style) {
				case NORMAL: {
					int dark_k = 16;
					int dark_k2 = 128;
					int c_dark_R = modifyColorComponent( c_R, -dark_k);
					int c_dark_G = modifyColorComponent( c_G, -dark_k);
					int c_dark_B = modifyColorComponent( c_B, -dark_k);

					int c_dark2_R = modifyColorComponent( c_R, -dark_k2);
					int c_dark2_G = modifyColorComponent( c_G, -dark_k2);
					int c_dark2_B = modifyColorComponent( c_B, -dark_k2);
					
					int y0 = rect.top;
					int y1 = rect.top + ((height-gap)/10); 
					int y2 = rect.top + ((height-gap)/2); 
					int y3 = rect.bottom - ((height-gap)/10) - gap; 
					int y4 = rect.bottom - gap; 
				
					drawVerticalGradient(	canvas, rect.left+left_gap, y0, rect.right-right_gap, y1,
											c_dark2_R, c_dark2_G, c_dark2_B,
											c_dark_R, c_dark_G, c_dark_B);
					
					drawVerticalGradient(	canvas, rect.left+left_gap, y1, rect.right-right_gap, y2,
							c_dark_R, c_dark_G, c_dark_B,
							c_R, c_G, c_B);

					drawVerticalGradient(	canvas, rect.left+left_gap, y2, rect.right-right_gap, y3,
							c_R, c_G, c_B,
							c_dark_R, c_dark_G, c_dark_B);

					drawVerticalGradient(	canvas, rect.left+left_gap, y3, rect.right-right_gap, y4,
							c_dark_R, c_dark_G, c_dark_B,
							c_dark2_R, c_dark2_G, c_dark2_B);
					
			        Paint paint = new Paint();
			        paint.setAntiAlias(false);
			        paint.setARGB(32, 0, 0, 0);
		       		canvas.drawRect(rect.left+left_gap, rect.top, rect.left+left_gap+1, rect.bottom-gap, paint);
		       		canvas.drawRect(rect.right-right_gap-1, rect.top, rect.right-right_gap, rect.bottom-gap, paint);
					
				}
				break;
				case SELECTED: {
					int light_k = 16;
					int dark_k = 16;
					int light_k2 = 64;
					int dark_k2 = 64;
					
					int c_0_R = modifyColorComponent( c_R, light_k2);
					int c_0_G = modifyColorComponent( c_G, light_k2);
					int c_0_B = modifyColorComponent( c_B, light_k2);
					
					int c_1_R = modifyColorComponent( c_R, light_k);
					int c_1_G = modifyColorComponent( c_G, light_k);
					int c_1_B = modifyColorComponent( c_B, light_k);
					
					int c_3_R = modifyColorComponent( c_R, -dark_k);
					int c_3_G = modifyColorComponent( c_G, -dark_k);
					int c_3_B = modifyColorComponent( c_B, -dark_k);

					int c_4_R = modifyColorComponent( c_R, -dark_k2);
					int c_4_G = modifyColorComponent( c_G, -dark_k2);
					int c_4_B = modifyColorComponent( c_B, -dark_k2);
					
					int y0 = rect.top;
					int y1 = rect.top + ((height-gap)/4); 
					int y2 = rect.top + ((height-gap)/2); 
					int y3 = rect.bottom - ((height-gap)/4) - gap; 
					int y4 = rect.bottom-gap; 
				
					drawVerticalGradient(	canvas, rect.left+left_gap, y0, rect.right-right_gap, y1,
											c_0_R, c_0_G, c_0_B,
											c_1_R, c_1_G, c_1_B);
					
					drawVerticalGradient(	canvas, rect.left+left_gap, y1, rect.right-right_gap, y2,
							c_1_R, c_1_G, c_1_B,
							c_R, c_G, c_B);

					drawVerticalGradient(	canvas, rect.left+left_gap, y2, rect.right-right_gap, y3,
							c_R, c_G, c_B,
							c_3_R, c_3_G, c_3_B);

					drawVerticalGradient(	canvas, rect.left+left_gap, y3, rect.right-right_gap, y4,
							c_3_R, c_3_G, c_3_B,
							c_4_R, c_4_G, c_4_B);
					
			        Paint paint = new Paint();
			        paint.setAntiAlias(false);
			        paint.setARGB(32, 0, 0, 0);
		       		canvas.drawRect(rect.left+left_gap, rect.top, rect.left+left_gap+1, rect.bottom, paint);
		       		canvas.drawRect(rect.right-right_gap-1, rect.top, rect.right-right_gap, rect.bottom, paint);
			        paint.setARGB(255, c_4_R, c_4_G, c_4_B);
		       		canvas.drawRect(rect.left, rect.bottom-gap, rect.right, rect.bottom-1, paint);
				}
				break;
				case DISABLED: {
					
					c_R = (DISABLED_BKG_COLOR & 0xFF0000) >> 16;
					c_G = (DISABLED_BKG_COLOR & 0xFF00) >> 8;
					c_B = (DISABLED_BKG_COLOR & 0xFF);
					
					int dark_k = 8;
					int dark_k2 = 32;
					int c_dark_R = modifyColorComponent( c_R, -dark_k);
					int c_dark_G = modifyColorComponent( c_G, -dark_k);
					int c_dark_B = modifyColorComponent( c_B, -dark_k);

					int c_dark2_R = modifyColorComponent( c_R, -dark_k2);
					int c_dark2_G = modifyColorComponent( c_G, -dark_k2);
					int c_dark2_B = modifyColorComponent( c_B, -dark_k2);
					
					int y0 = rect.top;
					int y1 = rect.top + ((height-gap)/10); 
					int y2 = rect.top + ((height-gap)/2); 
					int y3 = rect.bottom - ((height-gap)/10) - gap; 
					int y4 = rect.bottom - gap; 
				
					drawVerticalGradient(	canvas, rect.left+left_gap, y0, rect.right-right_gap, y1,
											c_dark2_R, c_dark2_G, c_dark2_B,
											c_dark_R, c_dark_G, c_dark_B);
					
					drawVerticalGradient(	canvas, rect.left+left_gap, y1, rect.right-right_gap, y2,
							c_dark_R, c_dark_G, c_dark_B,
							c_R, c_G, c_B);

					drawVerticalGradient(	canvas, rect.left+left_gap, y2, rect.right-right_gap, y3,
							c_R, c_G, c_B,
							c_dark_R, c_dark_G, c_dark_B);

					drawVerticalGradient(	canvas, rect.left+left_gap, y3, rect.right-right_gap, y4,
							c_dark_R, c_dark_G, c_dark_B,
							c_dark2_R, c_dark2_G, c_dark2_B);
					
			        Paint paint = new Paint();
			        paint.setAntiAlias(false);
			        paint.setARGB(32, 0, 0, 0);
		       		canvas.drawRect(rect.left+left_gap, rect.top, rect.left+left_gap+1, rect.bottom-gap, paint);
		       		canvas.drawRect(rect.right-right_gap-1, rect.top, rect.right-right_gap, rect.bottom-gap, paint);
				}
				break;
			}
		}
		
		
		private int style;
		private int color;
		private boolean last_tab;
		private boolean first_tab;
	}
	
	
	private static class TabViewFactory implements TabHost.TabContentFactory {
		
		private TabData data;
		
		public TabViewFactory(TabData d) {
			data = d;
		}
		
		public View createTabContent(String tag) {
			return data.view.getView();
		}
		
	};
	
	private MainView getView(int index) {
		if (index == -1)
			index = activeTab();
		TabData data = tabData.elementAt(index);
		return data.view;
	}
	
	private void processTabHostColors(TabHost tabHost, int SelectedColor, boolean useSelectedColor) {
		
		for (int i = 0; i < tabHost.getTabWidget().getChildCount(); i++) { 
			if ((i == tabHost.getCurrentTab()) && useSelectedColor) {
				tabHost.getTabWidget().getChildAt(i).setBackgroundColor(SelectedColor);
				
				Drawable cur_d = tabHost.getTabWidget().getChildAt(i).getBackground();
				RhoCustomDrawable d = null;
				if (d instanceof RhoCustomDrawable) {
					d = (RhoCustomDrawable)cur_d;
				}
				else {
					d = new RhoCustomDrawable();
				}
				d.setRhoColor(SelectedColor);
				d.setRhoStyle(RhoCustomDrawable.SELECTED);
				d.setColorFilter(SelectedColor | 0xFF000000, android.graphics.PorterDuff.Mode.SRC);
				d.setVisible(true, true);
				d.setAlpha(255);
				d.setRhoFirstTab(i == 0);
				d.setRhoLastTab(i == (tabHost.getTabWidget().getChildCount()-1));
				tabHost.getTabWidget().getChildAt(i).setBackgroundDrawable(d);
				tabHost.getTabWidget().getChildAt(i).requestLayout();
			}
			else {
				TabData data = tabData.elementAt(i);
				if (data.disabled) {
					// return to previous selected
					tabHost.getTabWidget().getChildAt(i).setBackgroundColor(mBackgroundColor);
					
					Drawable cur_d = tabHost.getTabWidget().getChildAt(i).getBackground();
					RhoCustomDrawable d = null;
					if (d instanceof RhoCustomDrawable) {
						d = (RhoCustomDrawable)cur_d;
					}
					else {
						d = new RhoCustomDrawable();
					}
					
					d.setRhoColor(mBackgroundColor);
					d.setRhoStyle(RhoCustomDrawable.DISABLED);
					d.setColorFilter(Color.GRAY, android.graphics.PorterDuff.Mode.SRC);
					d.setVisible(true, true);
					d.setAlpha(255);
					d.setRhoFirstTab(i == 0);
					d.setRhoLastTab(i == (tabHost.getTabWidget().getChildCount()-1));
					tabHost.getTabWidget().getChildAt(i).setBackgroundDrawable(d);
					tabHost.getTabWidget().getChildAt(i).requestLayout();
				}
				else {
					if (mBackgroundColorEnable) {
						tabHost.getTabWidget().getChildAt(i).setBackgroundColor(mBackgroundColor);
	
						Drawable cur_d = tabHost.getTabWidget().getChildAt(i).getBackground();
						RhoCustomDrawable d = null;
						if (d instanceof RhoCustomDrawable) {
							d = (RhoCustomDrawable)cur_d;
						}
						else {
							d = new RhoCustomDrawable();
						}
						
						d.setRhoColor(mBackgroundColor);
						d.setRhoStyle(RhoCustomDrawable.NORMAL);
						d.setColorFilter(mBackgroundColor | 0xFF000000, android.graphics.PorterDuff.Mode.SRC);
						d.setVisible(true, true);
						d.setAlpha(255);
						d.setRhoFirstTab(i == 0);
						d.setRhoLastTab(i == (tabHost.getTabWidget().getChildCount()-1));
						tabHost.getTabWidget().getChildAt(i).setBackgroundDrawable(d);
						tabHost.getTabWidget().getChildAt(i).requestLayout();
					}
				}
			}
		} 
	}
	
	
	@SuppressWarnings("unchecked")
	public TabbedMainView(Object params) {
		Context ctx = RhodesActivity.getContext();

		mBackgroundColorEnable = false;		

		Vector<Object> tabs = null;
		if (params instanceof Vector<?>)
			tabs = (Vector<Object>)params;
		else if (params instanceof Map<?,?>) {
			Map<Object,Object> settings = (Map<Object,Object>)params;
			
			Object bkgObj = settings.get("background_color");
			if ((bkgObj != null) && (bkgObj instanceof String)) {
				int color = Integer.parseInt((String)bkgObj) | 0xFF000000;
				mBackgroundColor = color;
				mBackgroundColorEnable = true;
			}
			
			Object tabsObj = settings.get("tabs");
			if (tabsObj != null && (tabsObj instanceof Vector<?>))
				tabs = (Vector<Object>)tabsObj;
		}
		
		if (tabs == null)
			throw new IllegalArgumentException("No tabs specified");
		
		int size = tabs.size();
		
		host = new TabHost(ctx);
		
		tabData = new Vector<TabData>(size);
		tabIndex = 0;
		
		TabWidget tabWidget = new TabWidget(ctx);
		tabWidget.setId(android.R.id.tabs);
		TabHost.LayoutParams lpt = new TabHost.LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.WRAP_CONTENT, Gravity.TOP);
		host.addView(tabWidget, lpt);

		FrameLayout frame = new FrameLayout(ctx);
		frame.setId(android.R.id.tabcontent);
		FrameLayout.LayoutParams lpf = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT, Gravity.BOTTOM);
		// TODO: detect tab widget height and use it here instead of hardcoded value
		//lpf.setMargins(0, 128, 0, 0);
		host.addView(frame, lpf);
		
		host.setup();
		
		host.setOnTabChangedListener(new TabHost.OnTabChangeListener() {
			private TabHost tabHost = host;
			
			public void onTabChanged(String tabId) {
				int sel_col = 0;
				boolean sel_col_enable = false;
				try {
					int new_tabIndex = Integer.parseInt(tabId);
					TabData data = tabData.elementAt(new_tabIndex);
					if (data != null) {
						if (data.disabled) {
							// return to previous selected
							tabHost.setCurrentTab(tabIndex);
							return;
						}
					}
					boolean real_change = (tabIndex != new_tabIndex);
					tabIndex = new_tabIndex;
					if ((data.reload && real_change) || !data.loaded ) {
						RhodesService.loadUrl(data.url);
						data.loaded = true;
					}
					sel_col = data.selected_color;
					sel_col_enable = data.selected_color_enabled;
				}
				catch (NumberFormatException e) {
					Logger.E(TAG, e);
				}
				processTabHostColors(tabHost, sel_col, sel_col_enable);
			}
		});
		
		TabHost.TabSpec spec;
		DisplayMetrics metrics = new DisplayMetrics();
		WindowManager wm = (WindowManager)ctx.getSystemService(Context.WINDOW_SERVICE);
		wm.getDefaultDisplay().getMetrics(metrics);

		int selected_color = 0;
		boolean selected_color_enable = false;

		for (int i = 0; i < size; ++i) {
			Object param = tabs.elementAt(i);
			if (!(param instanceof Map<?,?>))
				throw new IllegalArgumentException("Hash expected");
			
			Map<Object, Object> hash = (Map<Object, Object>)param;
			
			Object labelObj = hash.get("label");
			if (labelObj == null || !(labelObj instanceof String))
				throw new IllegalArgumentException("'label' should be String");
			
			Object actionObj = hash.get("action");
			if (actionObj == null || !(actionObj instanceof String))
				throw new IllegalArgumentException("'action' should be String");
			
			String label = (String)labelObj;
			String action = (String)actionObj;
			String icon = null;
			boolean reload = false;
			
			boolean disabled = false;
			
			Object iconObj = hash.get("icon");
			if (iconObj != null && (iconObj instanceof String))
				icon = "apps/" + (String)iconObj;
			
			Object reloadObj = hash.get("reload");
			if (reloadObj != null && (reloadObj instanceof String))
				reload = ((String)reloadObj).equalsIgnoreCase("true");
			
			Object selected_color_Obj = hash.get("selected_color");
			if ((selected_color_Obj != null) && (selected_color_Obj instanceof String)) {
				selected_color_enable = true;
				selected_color = Integer.parseInt((String)selected_color_Obj) | 0xFF000000;
			}

			Object disabled_Obj = hash.get("disabled");
			if (disabled_Obj != null && (disabled_Obj instanceof String))
				disabled = ((String)disabled_Obj).equalsIgnoreCase("true");
			
			spec = host.newTabSpec(Integer.toString(i));
			
			// Set label and icon
			BitmapDrawable drawable = null;

			if (icon != null) {
				String iconPath = RhoFileApi.normalizePath(icon);
				Bitmap bitmap = BitmapFactory.decodeStream(RhoFileApi.open(iconPath));
				if (disabled && (bitmap != null)) {
					// replace Bitmap to gray
					bitmap = bitmap.copy(Bitmap.Config.ARGB_8888, true); // prepare mutable bitmap
					int x;
					int y;
					int bw = bitmap.getWidth();
					int bh = bitmap.getHeight();
					int nc = DISABLED_IMG_COLOR & 0xFFFFFF;
					int c;
					for (y = 0; y < bh; y++) {
						for (x = 0; x < bw; x++) {
							c = bitmap.getPixel(x, y);
							c = nc | (c & 0xFF000000);
							bitmap.setPixel(x, y, c);
						}
					}
				}
				
				if (bitmap != null)
					bitmap.setDensity(DisplayMetrics.DENSITY_MEDIUM);//Bitmap.DENSITY_NONE);
					drawable = new BitmapDrawable(bitmap);
					drawable.setTargetDensity(metrics);
			}
			if (drawable == null)
				spec.setIndicator(label);
			else
				spec.setIndicator(label, drawable);
			
			// Set view factory
			SimpleMainView view = new SimpleMainView();
			TabData data = new TabData();
			data.view = view;
			data.url = action;
			data.reload = reload;
			
			data.selected_color = selected_color;
			data.selected_color_enabled = selected_color_enable;
			data.disabled = disabled;
			
			TabViewFactory factory = new TabViewFactory(data);
			spec.setContent(factory);
			
			tabData.addElement(data);
			host.addTab(spec);
		}
		
		int sel_col = 0;
		boolean sel_col_enable = false;
		
		tabIndex = 0;
		TabData data = null; 
		boolean founded_not_disabled = false;
		
		while ((!founded_not_disabled) && (tabIndex < tabData.size())) {
			data = tabData.elementAt(tabIndex); 
			if ((data != null) && (!data.disabled)) {
				founded_not_disabled = true;
			}
			else {
				tabIndex++;
			}
		}
		if (!founded_not_disabled) {
			Logger.E(TAG, "ERROR : All tabs is disabled !!! ");
		}
		
		if (data != null) {
			sel_col = data.selected_color;
			sel_col_enable = data.selected_color_enabled;
		}
		processTabHostColors(host, sel_col, sel_col_enable);
		
		host.requestLayout();
		
		tabWidget.measure(host.getWidth(), host.getHeight());
		int hh = tabWidget.getMeasuredHeight();
		if (hh < 64) {
			hh = 64;
		}
		lpf.setMargins(0, hh, 0, 0);
		host.updateViewLayout(frame, lpf);
		
	}

	public View getView() {
		return host;
	}

	public WebView getWebView(int tab_index) {
		return getView(tab_index).getWebView(-1);
	}
	
	public WebView detachWebView() {
		return getView(activeTab()).detachWebView();
	}
	
	public void back(int index) {
		getView(index).back(0);
	}
	
	public void forward(int index) {
		getView(index).forward(0);
	}
	
	public void navigate(String url, int index) {
		getView(index).navigate(url, 0);
	}
	
	public void reload(int index) {
		getView(index).reload(0);
	}
	
	public String currentLocation(int index) {
		return getView(index).currentLocation(0);
	}

	public void switchTab(int index) {
		host.setCurrentTab(index);
		tabIndex = index;
	}
	
	public int activeTab() {
		return tabIndex;
	}

	public void goBack() {
		getView(activeTab()).goBack();
	}

	public void loadData(String data, int index) {
		getView(index).loadData(data, 0);
	}
	
	public void addNavBar(String title, Map<Object,Object> left, Map<Object,Object> right) {
		getView(activeTab()).addNavBar(title, left, right);
	}
	
	public void removeNavBar() {
		getView(activeTab()).removeNavBar();
	}
	
	@Override
	public int getTabsCount() {
		return tabData.size();
	}

}
