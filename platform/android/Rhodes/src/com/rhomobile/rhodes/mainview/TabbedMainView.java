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

package com.rhomobile.rhodes.mainview;

import java.io.InputStream;
import java.util.Hashtable;
import java.util.Map;
import java.util.Vector;
import java.util.List;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.Utils;

import android.app.Activity;
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
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebBackForwardList;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TabHost;
import android.widget.TabWidget;

public class TabbedMainView implements MainView {
	
	private static final String TAG = "TabbedMainView";
	
	private static final int DISABLED_BKG_COLOR = 0xFF000000 | (110 << 16) | (110 << 8) | (110);
	private static final int DISABLED_IMG_COLOR = 0xFF000000 | (140 << 16) | (140 << 8) | (140);
	
	private TabHost host;
	private Vector<TabData> tabData;
	private int tabIndex;
	
	private int mBackgroundColor = 0;
	private boolean mBackgroundColorEnable = false;
	private boolean[] isTabLoaded;
	private String[] tabDefaultUrl;
	//private String mChangeTabCallback = null;
	IMethodResult mChangeTabCallback = null;
	
	private boolean mIsReallyOnScreen = false;
	
	//private String mLoadUrlAfterLoadToScreen = null;
	
	//private static native void onTabBarChangeTabCallback(String callback_url, String body);
	
	
	private void callChangeTabCallback(int index) {
		
		if (mChangeTabCallback == null) {
			Utils.platformLog("TABBAR", "mChangeTabCallback is NULL !");
			return;
		}
		
		//String body = "&rho_callback=1" + "&tab_index=" + String.valueOf(index);
		//onTabBarChangeTabCallback(mChangeTabCallback, body);
		Map<String,Object> resHash = new Hashtable<String,Object>();
		
		resHash.put("tab_index", String.valueOf(index));
		mChangeTabCallback.set(resHash);
		Utils.platformLog("TABBAR", "TabIndex changed, tab_index = "+String.valueOf(index));
		
	}
	
	private static class TabData {
		public MainView view;
		public String url;
		public boolean reload;
		public boolean loaded;
		public String title;
		public int selected_color;
		public boolean selected_color_enabled;
		public boolean disabled;
		
		public TabData() {
			loaded = false;
			selected_color_enabled = false;
			disabled = false;
			title = null;
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
					
					int dark_k = 0;
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
	
	private MainView getTabMainView(int index) {
		if (index == -1)
			index = activeTab();
		TabData data = tabData.elementAt(index);
		return data.view;
	}

    private MainView getTabMainView(Object handle) throws IllegalArgumentException {
        for(TabData tab: tabData) {
            if (tab.view.getWebView(-1).getView() == handle) {
                return tab.view;
            }
        }
        throw new IllegalArgumentException("Wrong WebView handle: " + handle );
    }

    private int getTab(Object handle) throws IllegalArgumentException {
        for(int i=0; i < tabData.size(); ++i) {
            if (tabData.get(i).view.getWebView(-1).getView() == handle) {
                return i;
            }
        }
        throw new IllegalArgumentException("Wrong WebView handle: " + handle );
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
	public TabbedMainView(Object params, Object options, IMethodResult callback) {
		Context ctx = ContextFactory.getUiContext();
        RhodesActivity activity = RhodesActivity.safeGetInstance();

		if (callback != null) {
			mChangeTabCallback = callback;
		}
		
		mBackgroundColorEnable = false;		

		List<Object> tabs = null;
		boolean place_tabs_bottom = false;
		if (params instanceof List<?>) {
			tabs = (List<Object>)params;
		}
		if (options instanceof Map<?,?>) {
			Map<Object,Object> settings = (Map<Object,Object>)options;
			
			Object bkgObj = settings.get("backgroundColor");
			if (bkgObj == null) {
				bkgObj = settings.get("background_color");
			}
			if ((bkgObj != null) && (bkgObj instanceof Integer)) {
				int color = ((Integer)bkgObj).intValue();//;Integer.parseInt((String)bkgObj) | 0xFF000000;
				mBackgroundColor = color;
				mBackgroundColorEnable = true;
			}
			
			//Object callbackObj = settings.get("on_change_tab_callback");
			//if ((callbackObj != null) && (callbackObj instanceof String)) {
			//	mChangeTabCallback = new String(((String)callbackObj));
			//}

			Object placeBottomObj = settings.get("placeTabsBottom");
			if (placeBottomObj == null) {
				placeBottomObj = settings.get("place_tabs_bottom");
			}
			if ((placeBottomObj != null) && (placeBottomObj instanceof Boolean)) {
				place_tabs_bottom = ((Boolean)placeBottomObj).booleanValue();//;((String)placeBottomObj).equalsIgnoreCase("true");
			}
			
			
			//Object tabsObj = settings.get("tabs");
			//if (tabsObj != null && (tabsObj instanceof Vector<?>))
			//	tabs = (Vector<Object>)tabsObj;
		}
		
		if (tabs == null)
			throw new IllegalArgumentException("No tabs specified");
		
		Object[] tabs_array = tabs.toArray();
		
		int size = tabs_array.length;
		isTabLoaded = new boolean[size];
		host = new TabHost(ctx, null);
		tabDefaultUrl = new String[size];
		tabData = new Vector<TabData>(size);
		tabIndex = 0;
		
		TabWidget tabWidget = new TabWidget(ctx);
		tabWidget.setId(android.R.id.tabs);

		FrameLayout frame = new FrameLayout(ctx);
		FrameLayout.LayoutParams lpf = null;
		TabHost.LayoutParams lpt = null;
		if (place_tabs_bottom) {

			lpt = new TabHost.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT, Gravity.BOTTOM);
			host.addView(tabWidget, lpt);
			
			
			frame.setId(android.R.id.tabcontent);
			lpf = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT, Gravity.TOP);
			host.addView(frame, lpf);

		}
		else {
			lpt = new TabHost.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT, Gravity.TOP);
			host.addView(tabWidget, lpt);

			frame = new FrameLayout(ctx);
			frame.setId(android.R.id.tabcontent);
			lpf = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT, Gravity.BOTTOM);
			host.addView(frame, lpf);
		}
		
		host.setup();
		
		TabHost.TabSpec spec;
		DisplayMetrics metrics = new DisplayMetrics();
		WindowManager wm = (WindowManager)ctx.getSystemService(Context.WINDOW_SERVICE);
		wm.getDefaultDisplay().getMetrics(metrics);

		int selected_color = 0;
		boolean selected_color_enable = false;

		for (int i = 0; i < size; ++i) {
			Object param = tabs_array[i];
			isTabLoaded[i] = false;
			if (!(param instanceof Map<?,?>))
				throw new IllegalArgumentException("Hash expected");
			
			Map<Object, Object> hash = (Map<Object, Object>)param;
			
			Object labelObj = hash.get("label");
			if (!(labelObj instanceof String))
				throw new IllegalArgumentException("'label' should be String");
			
			Object actionObj = hash.get("action");
			
			boolean use_current_view_for_tab = false;
			Object use_current_view_for_tab_Obj = hash.get("useCurrentViewForTab");
			if (use_current_view_for_tab_Obj != null) {
				use_current_view_for_tab = (use_current_view_for_tab_Obj.toString()).equalsIgnoreCase("true");
			}

			if (use_current_view_for_tab) {
				actionObj = new String("none");
			}
			if (actionObj == null || !(actionObj instanceof String))
				throw new IllegalArgumentException("'action' should be String");
			
			
			String label = (labelObj == null) ? null : (String)labelObj;
			String action = (String)actionObj;
			boolean reload = false;
			boolean disabled = false;
			int web_bkg_color = 0xFFFFFFFF;
			
			Object reloadObj = hash.get("reload");
			if (reloadObj != null && (reloadObj instanceof Boolean))
				reload = ((Boolean)reloadObj).booleanValue();//((String)reloadObj).equalsIgnoreCase("true");
			
			Object selected_color_Obj = hash.get("selectedColor");
			if ((selected_color_Obj != null) && (selected_color_Obj instanceof Integer)) {
				selected_color_enable = true;
				selected_color = ((Integer)selected_color_Obj).intValue() | 0xFF000000;//Integer.parseInt((String)selected_color_Obj) | 0xFF000000;
			}

			Object disabled_Obj = hash.get("disabled");
			if (disabled_Obj != null && (disabled_Obj instanceof Boolean))
				disabled = ((Boolean)disabled_Obj).booleanValue();//((String)disabled_Obj).equalsIgnoreCase("true");
			
			Object web_bkg_color_Obj = hash.get("backgroundColor");
			if (web_bkg_color_Obj != null && (web_bkg_color_Obj instanceof Integer)) {
				web_bkg_color = ((Integer)web_bkg_color_Obj).intValue() | 0xFF000000;//Integer.parseInt((String)web_bkg_color_Obj) | 0xFF000000;
			}
			
			spec = host.newTabSpec(Integer.toString(i));
			
			// Set label and icon
			BitmapDrawable drawable = null;
            Bitmap bitmap = getIconBitmap(tabs_array[i]);
            if (bitmap != null) {
                drawable = new BitmapDrawable(ctx.getResources(), bitmap);
                drawable.setTargetDensity(metrics);
            }

            if (drawable == null) {
                spec.setIndicator(label);
            }
            else {
                spec.setIndicator(label, drawable);
            }

            SimpleMainView view = null;
            if (use_current_view_for_tab) {
                MainView mainView = activity.getMainView();
                action = mainView.currentLocation(-1);
                IRhoWebView webView = mainView.detachWebView();
                view = new SimpleMainView(webView);
            }
            if (view == null) {
                view = new SimpleMainView(RhoExtManager.getImplementationInstance().createWebView(activity, i));
            }

			// Set view factory
			
			if (web_bkg_color_Obj != null) {
				if (!use_current_view_for_tab) {
					view.setWebBackgroundColor(web_bkg_color);
				}
				host.setBackgroundColor(web_bkg_color);
			}
			tabDefaultUrl[i] = action;
			TabData data = new TabData();
			data.view = view;
			data.url = action;
			data.reload = reload;
			data.title = label;
			if (use_current_view_for_tab) {
				data.loaded = true;
				tabIndex = i;
				RhodesActivity.safeGetInstance().setTitle(data.title);
			}
			
			data.selected_color = selected_color;
			data.selected_color_enabled = selected_color_enable;
			data.disabled = disabled;
			
			TabViewFactory factory = new TabViewFactory(data);
			spec.setContent(factory);
			
			tabData.addElement(data);
			host.addTab(spec);
		}
		
		
		tabWidget.measure(host.getWidth(), host.getHeight());
		int hh = tabWidget.getMeasuredHeight();
		//if (hh < 64) {
		//	hh = 64;
		//}
		if (place_tabs_bottom) {
			lpf.setMargins(0, 0, 0, hh);
		}
		else {
			lpf.setMargins(0, hh, 0, 0);
		}
		host.updateViewLayout(frame, lpf);

	       //host.getTabWidget().getChildTabViewAt(index);
        for (int i = 0; i < size; ++i) {
            ViewGroup tab = (ViewGroup)host.getTabWidget().getChildTabViewAt(i);
            ImageView iconView = (ImageView)tab.findViewById(android.R.id.icon);

            Bitmap bitmap = getIconBitmap(tabs_array[i]);
            if (bitmap != null) {
                
                Logger.T(TAG, "Add icon to Holo style tab: " + i);
                
                BitmapDrawable drawable = new BitmapDrawable(ctx.getResources(), bitmap);
                drawable.setTargetDensity(metrics);
                iconView.setImageDrawable(drawable);
                iconView.setVisibility(View.VISIBLE);
            }
        }
    }

    private Bitmap getIconBitmap(Object tabParamsObj) {
        Map<Object, Object> hash = (Map<Object, Object>)tabParamsObj;

        Object iconObj = hash.get("icon");
        
        Bitmap bitmap = null;

        if (iconObj != null) {
            
        	
			InputStream is = null;
			String iconPath = (String)iconObj;
			is = RhoFileApi.open(iconPath);
			if (is == null) {
				iconPath = RhoFileApi.normalizePath(iconPath);
				is = RhoFileApi.open(iconPath);
			}
			if (is == null) {
				iconPath = "apps/" + (String)iconObj;
				iconPath = RhoFileApi.normalizePath(iconPath);
				is = RhoFileApi.open(iconPath);
			}
            Logger.T(TAG, "Bitmap file path: " + iconPath);

            bitmap = BitmapFactory.decodeStream(is);

            if (bitmap != null) {
                boolean disabled = false;
                Object disabled_Obj = hash.get("disabled");
                if (disabled_Obj != null && (disabled_Obj instanceof Boolean))
                    disabled = ((Boolean)disabled_Obj).booleanValue();

                if (disabled) {
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
                bitmap.setDensity(DisplayMetrics.DENSITY_MEDIUM);//Bitmap.DENSITY_NONE);
            }
        }
        return bitmap;
    }

	private class TabHostClickListener implements View.OnTouchListener, View.OnClickListener, TabHost.OnTabChangeListener {
		
		public TabHost tabHost;
		private float x = -1000;
		private float y = -1000;
		private int sel_col = 0;
		private boolean sel_col_enable = false;
		
		public boolean onTouch(View v, MotionEvent event) {
			x = event.getX() + v.getLeft();
			y = event.getY() + v.getTop();
			//Utils.platformLog("#$#$#$#$#$#$#$#$#$", "onTouch( "+String.valueOf((int)x)+" , "+String.valueOf((int)y)+" )");
			return false;
		}

		public void onClick(View v) {
			//Utils.platformLog("#$#$#$#$#$#$#$#$#$", "onClick( "+String.valueOf((int)x)+" , "+String.valueOf((int)y)+" )");
			// found child item
			for (int i = 0; i < tabHost.getTabWidget().getChildCount(); i++) { 
				int curIndex = i;
				int left = tabHost.getTabWidget().getChildAt(curIndex).getLeft();
				int top = tabHost.getTabWidget().getChildAt(curIndex).getTop();
				int width = tabHost.getTabWidget().getChildAt(curIndex).getWidth();
				int height = tabHost.getTabWidget().getChildAt(curIndex).getHeight();
				//Utils.platformLog("#$#$#$#$#$#$#$#$#$", "current item have rect [ "+String.valueOf(left)+" , "+String.valueOf(top)+" , "+String.valueOf(width)+" , "+String.valueOf(height)+" ]");
				if (	( ( left <= x) && (x <= (left+width))) &&
						( (top <= y) && (y <= (top+height))) ) {
					//Utils.platformLog("#$#$#$#$#$#$#$#$#$", "clicked item no "+String.valueOf(curIndex));
					onTabChangedIndex(curIndex, true);
					InputMethodManager imm = (InputMethodManager) RhodesActivity.getContext().getSystemService(Activity.INPUT_METHOD_SERVICE);
					imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
					return;
				}
			}
			//Utils.platformLog("#$#$#$#$#$#$#$#$#$", "not found clicked item");
		}

		private void onTabChangedIndex(int index, boolean byself) {
			//Utils.platformLog("#$#$#$#$#$#$#$#$#$", "onTabChangedIndex( "+String.valueOf(index)+" )");
			int new_tabIndex = index;
			sel_col = 0;
			sel_col_enable = false;
			TabData data = tabData.elementAt(new_tabIndex);
			if (data == null) {
				return;
			}
			if (data.disabled) {
				// return to previous selected
				tabHost.setCurrentTab(tabIndex);
				return;
			}
			boolean real_change = (tabIndex != new_tabIndex);
			if (real_change && byself) {
				tabHost.setCurrentTab(new_tabIndex);
				// all will processed when onTabChanged received
				return;
			}
			tabIndex = new_tabIndex;

			if (real_change) {
				callChangeTabCallback(tabIndex);
			}
			RhodesActivity.safeGetInstance().setTitle(data.title);
			if (((data.reload /*|| real_change*/) || !data.loaded) && !isTabLoaded[tabIndex] ) {
				if (mIsReallyOnScreen) {
					RhodesService.loadUrl(data.url);
					data.loaded = true;
				}
			}
			sel_col = data.selected_color;
			sel_col_enable = data.selected_color_enabled;
			
			if (mIsReallyOnScreen && real_change) {
				processTabHostColors(tabHost, sel_col, sel_col_enable);
			}
		}
		
		public void onTabChanged(String tabId) {
			try {
				int new_tabIndex = Integer.parseInt(tabId);
				onTabChangedIndex(new_tabIndex, false);
			}
			catch (NumberFormatException e) {
				Logger.E(TAG, e);
			}
		}
		
		
	}
	
	public void loadFirstPage() {

		int sel_col = 0;
		boolean sel_col_enable = false;
		
		int cur_tabIndex = 0;
		TabData data = null; 
		boolean founded_not_disabled = false;
		
		while ((!founded_not_disabled) && (cur_tabIndex < tabData.size())) {
			data = tabData.elementAt(cur_tabIndex); 
			if ((data != null) && (!data.disabled)) {
				founded_not_disabled = true;
			}
			else {
				cur_tabIndex++;
			}
		}
		if (!founded_not_disabled) {
			Logger.E(TAG, "ERROR : All tabs is disabled !!! ");
		}
		
		if ((tabIndex != cur_tabIndex) && (tabIndex != 0)) {
			data = tabData.elementAt(tabIndex); 
		}
		else {
			tabIndex = cur_tabIndex;
		}
		
		
		
		if (data != null) {
			sel_col = data.selected_color;
			sel_col_enable = data.selected_color_enabled;
		}
		//processTabHostColors(host, sel_col, sel_col_enable);
		
		host.requestLayout();

		TabHostClickListener listener = new TabHostClickListener();
		listener.tabHost = host;
		
		host.setOnTabChangedListener(listener);
		
		for (int i = 0; i < host.getTabWidget().getChildCount(); i++) { 
				host.getTabWidget().getChildAt(i).setOnTouchListener( listener);
				host.getTabWidget().getChildAt(i).setOnClickListener(listener);
		}

		if (data != null) {
			try {
				if (!data.loaded) {
					RhodesService.loadUrl(data.url);
					data.loaded = true;
				}
				sel_col = data.selected_color;
				sel_col_enable = data.selected_color_enabled;
			}
			catch (NumberFormatException e) {
				Logger.E(TAG, e);
			}
			processTabHostColors(host, sel_col, sel_col_enable);
		}

		host.setCurrentTab(tabIndex);
		
		RhodesActivity.safeGetInstance().post( new Runnable() {
			public void run() {
				//Utils.platformLog("TabbedMainView", "invoke post setup code in UI thread");

				// invoke in UI thread
				int i;
				for (i = 0; i < tabData.size(); i++) {
					TabData data = tabData.elementAt(i);
					if (i != tabIndex) {
						data.loaded = false;
					}
					if (i != tabIndex) {
						IRhoWebView wv = getWebView(i);
						wv.clear();
					}
				}
				TabData selected_data = tabData.elementAt(tabIndex);
				if (!selected_data.loaded) {
					RhodesService.loadUrl(selected_data.url);
					selected_data.loaded = true;
				}
				mIsReallyOnScreen = true;
			}
		});
		
		
	}
    
    private boolean isValidIndex( int index ) {
        boolean ok = ((index >= -1) && (index < getTabsCount()));
        
        if ( !ok ) {
            Logger.E(TAG,"TabBar invalid tab index [ " + String.valueOf(index) + " ] !!!");
        }
        
        return ok;
    }
	
	@Override
	public View getView() {
		return host;
	}

   @Override
    public void setWebView(IRhoWebView view, int index) {
        
        if ( !isValidIndex(index) ) {
            return;
        }
        
       getTabMainView(index).setWebView(view, -1);
    }

    @Override
    public IRhoWebView getWebView(int tab_index) {
        if ( !isValidIndex(tab_index) ) {
            return null;
        }

        return getTabMainView(tab_index).getWebView(-1);
    }

    @Override
    public IRhoWebView getWebView(Object handle) {
        return getTabMainView(handle).getWebView(-1);
    }

    @Override
    public int getWebViewTab(Object handle) {
        return getTab(handle);
    }

    @Override
    public IRhoWebView detachWebView() {
        return getTabMainView(activeTab()).detachWebView();
    }

    @Override
    public void destroy() {
        for(TabData tab: tabData) {
            tab.view.destroy();
        }
    }

    public void back(int index) {
        if ( !isValidIndex(index) ) {
            return;
        }
        WebBackForwardList mWebBackForwardList = getWebView(index).copyBackForwardList();
        // restrict back if user want to go back tab 1 from tab 0 with default page loaded on tab 1 
        if ((activeTab() == 0 && index > 0 && mWebBackForwardList.getCurrentIndex() <= 1)){
        	return ;
        }
        
        //1st condition : go back from tab 0 always
        //2nd condition : if active tab and navigate back index is differ
        //3rd condition : if other than default pages are loaded on all tabs except tab 0
        if (activeTab() == 0 || (index != activeTab() && index!= -1)
				|| mWebBackForwardList.getCurrentIndex() > 1) {
			getTabMainView(index).back(0);
	}
    }
	
	public void forward(int index) {
        if ( !isValidIndex(index) ) {
            return;
        }

		getTabMainView(index).forward(0);
	}
	
	public void navigate(String url, int index) {
        if ( !isValidIndex(index) ) {
            return;
        }
        
        if(index > -1) {
        	isTabLoaded[index] = true;
        }

		getTabMainView(index).navigate(url, 0);
	}
	
	public void reload(int index) {
        if ( !isValidIndex(index) ) {
            return;
        }

		getTabMainView(index).reload(0);
	}
	
	public String currentLocation(int index) {
        if ( !isValidIndex(index) ) {
            return null;
        }

		return getTabMainView(index).currentLocation(0);
	}

	public void switchTab(int index) {
		if(((index >= 0) && (tabData.size()-1)>=index))
		{
			if (!tabData.get(index).disabled) {
				host.setCurrentTab(index);
				tabIndex = index;
			}
			else {
				Logger.I(TAG, "Not switching to disabled tab... "+"total tabs="+tabData.size()+" index="+index );
			}
		}
		else
		{
			Logger.I(TAG, "Not switching to invalid tab... "+"total tabs="+tabData.size()+" index="+index );
		}
	}
	
	public int activeTab() {
		return tabIndex;
	}
	
	public String getTabDefaultUrl() {
		return getWebView(activeTab()).getUrl();
		
	}

	public void goBack() {
		WebBackForwardList mWebBackForwardList = getWebView(activeTab()).copyBackForwardList();
		if (activeTab() == 0 || mWebBackForwardList.getCurrentIndex() > 1) {
			getTabMainView(activeTab()).goBack();
		}
	}

	public void loadData(String data, int index) {
        if ( !isValidIndex(index) ) {
            return;
        }

		getTabMainView(index).loadData(data, 0);
	}
	
	public void addNavBar(String title, Map<Object,Object> left, Map<Object,Object> right) {
		getTabMainView(activeTab()).addNavBar(title, left, right);
	}
	
	public void removeNavBar() {
		getTabMainView(activeTab()).removeNavBar();
	}
	
	@Override
	public int getTabsCount() {
		return tabData.size();
	}

    @Override
    public void executeJS(String js, int index) {
        if ( !isValidIndex(index) ) {
            return;
        }

        getTabMainView(index).executeJS(js, 0);
    }

    @Override
    public void stopNavigate(int index) {
        if ( !isValidIndex(index) ) {
            return;
        }

        getTabMainView(index).stopNavigate(0);
    }


    @Override
    public void saveCurrentPage(String format, String path, int index) {
        if ( !isValidIndex(index) ) {
            return;
        }

        getTabMainView(index).saveCurrentPage(format, path, 0);
    }
    
	public String get_current_url(int tab_index) {
        if ( !isValidIndex(tab_index) ) {
            return null;
        }

		return getTabMainView(tab_index).get_current_url(tab_index);
	}

    @Override
    public void removeSplashScreen() {
    }     

}
