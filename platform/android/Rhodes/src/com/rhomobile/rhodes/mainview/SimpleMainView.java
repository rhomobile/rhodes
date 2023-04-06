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

import java.util.ArrayList;
import java.util.Iterator;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.io.InputStream;
import java.util.Map;
import java.util.Vector;
import java.util.List;

import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.nativeview.IRhoCustomView;
import com.rhomobile.rhodes.nativeview.RhoNativeViewManager;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

import android.app.ActionBar.LayoutParams;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.AbsoluteLayout;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.rhomobile.rhodes.webview.RhoWebViewClient;
import com.rhomobile.rhodes.SecurityTokenGenerator;

public class SimpleMainView implements MainView {

	private final static String TAG = "SimpleMainView";	
	
	private static final int WRAP_CONTENT = ViewGroup.LayoutParams.WRAP_CONTENT;
	private static final int FILL_PARENT = ViewGroup.LayoutParams.MATCH_PARENT;
	
	private class ActionBack implements View.OnClickListener {
		public void onClick(View v) {
			goBack();//back(0);
		}
	};
	
	public static class TransparencyViewForCaptureTouchEvents extends View {

		public TransparencyViewForCaptureTouchEvents(Context context) {
			super(context);
		}
		
	}
	
	private void addWebViewToMainView(IRhoWebView webView, int index, LinearLayout.LayoutParams params) {
		view.addView(webView.getContainerView(), index, params);
		
		Utils.platformLog("Development Extra Simple View WV", "$$$$$ setup $$$$$");
		
		Context activity = ContextFactory.getUiContext();

		
		View al = webView.getView();
			
		if (al instanceof WebView) {
			// we have android WebView
			((WebView)al).getSettings().setUserAgentString(SecurityTokenGenerator.getInstance().getSecureAgent());

			// check for exist developer extension
			try {
				Class<?> dev_class = Class.forName("com.rho.development.Development");
				// we have Development extension !
				
				// check for already add transparency view
				ViewGroup cv = webView.getContainerView();
				int child_count = cv.getChildCount();
				int i;
				boolean hasTransparencyView = false;
				for (i = 0; i < child_count; i++) {
					View v = cv.getChildAt(i);
					if (v instanceof TransparencyViewForCaptureTouchEvents) {
						hasTransparencyView = true;
					}
				}
				
				if (!hasTransparencyView) {
					final View dst = al;
					View transView = new TransparencyViewForCaptureTouchEvents(activity);
					transView.setClickable(true);
					transView.setBackgroundColor(Color.TRANSPARENT);
					transView.setOnTouchListener(new View.OnTouchListener() {
			
						private View mDST = dst;
						private int mCurrentMax = 0;
			
						@Override
						public boolean onTouch(View v, MotionEvent event) {
							//Utils.platformLog("Development Extra Simple View TV", "$$$$$ TOUCH EVENT TV ["+MotionEvent.actionToString(event.getAction())+", "+String.valueOf(event.getPointerCount())+"] $$$$$");
							if ((event.getActionMasked() == MotionEvent.ACTION_UP) || (event.getActionMasked() == MotionEvent.ACTION_POINTER_UP)) {
								//Utils.platformLog("Development Extra Simple View", "$$$$$ UP TV ["+String.valueOf(event.getPointerCount())+"] $$$$$");
								if ((event.getPointerCount() == 3) && (mCurrentMax == 3)) {
									//Utils.platformLog("Development Extra Simple View", "$$$$$ TRIPLE TAP TV $$$$$");
									RhodesActivity.onTripleTap();
								}
								if ((event.getPointerCount() == 4) && (mCurrentMax == 4)) {
									//Utils.platformLog("Development Extra Simple View", "$$$$$ QUADRO TAP TV $$$$$");
									RhodesActivity.onQuadroTap();
								}
								if (event.getPointerCount() == 1) {
									mCurrentMax = 0;
								}
							}
							else if (event.getAction() == MotionEvent.ACTION_CANCEL) {
								mCurrentMax = 0;
							}
							else if ((event.getActionMasked() == MotionEvent.ACTION_DOWN) || (event.getActionMasked() == MotionEvent.ACTION_POINTER_DOWN)) {
								if (event.getPointerCount() > mCurrentMax) {
									mCurrentMax = event.getPointerCount();
								}
							}
							mDST.dispatchTouchEvent(event);
							return false;
							
						}
					});	
					
					cv.addView(transView, new AbsoluteLayout.LayoutParams(FILL_PARENT, FILL_PARENT, 0, 0));
					cv.bringChildToFront(transView);
				}
			
			} catch (ClassNotFoundException e) {
				//e.printStackTrace();
			}
		
		}
		

	}
	
	private void removeWebViewFromMainView() {
		view.removeView(webView.getContainerView());
	}
	
	public class MyView extends LinearLayout {
		public MyView(Context ctx) {
			super(ctx);
		}
		
		protected void onSizeChanged (int w, int h, int oldw, int oldh) {
			super.onSizeChanged(w, h, oldw, oldh);
			StringBuilder msg = new StringBuilder();
			msg.append(" Main Window :: onSizeChanged() old [ ");
			msg.append(w);
			msg.append(" x ");
			msg.append(h);
			msg.append(" ]  new [ ");
			msg.append(oldw);
			msg.append(" x ");
			msg.append(oldh);
			msg.append(" ]");
			Utils.platformLog("SimpleMainView.View", msg.toString());
		}
	}
	
	private class ActionForward implements View.OnClickListener {
		public void onClick(View v) {
			forward(0);
		}
	};
	
	private class ActionHome implements View.OnClickListener {
		public void onClick(View v) {
			navigate(RhodesAppOptions.getStartUrl(), 0);
		}
	};
	
	private class ActionOptions implements View.OnClickListener {
		public void onClick(View v) {
			navigate(RhodesAppOptions.getOptionsUrl(), 0);
		}
	};
	
	private class ActionRefresh implements View.OnClickListener {
		public void onClick(View v) {
			reload(0);
		}
	};

	private class ActionExit implements View.OnClickListener {
		public void onClick(View v) {
			restoreWebView();
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
			});
		}
	};
	
	private LinearLayout view;
	private IRhoWebView webView;
	private IRhoCustomView mRhoCustomView = null;
	private LinearLayout navBar = null;
	private LinearLayout toolBar = null;
	
	private int mCustomBackgroundColor = 0;
	private boolean mCustomBackgroundColorEnable = false;
	
	public View getView() {
		return view;
	}

	@Override
	public IRhoWebView getWebView(int tab_index) {
		return webView;
	}

    @Override
    public IRhoWebView getWebView(Object nativeWebView) {
        return webView;
    }
    
    @Override
    public int getWebViewTab(Object nativeWebView) {
        return 0;
    }

    public void setCustomView(IRhoCustomView customView) {
        restoreWebView();
        if (customView != null) {
            removeWebViewFromMainView();

            mRhoCustomView = customView;

            if (navBar != null) {
                view.removeView(navBar);
            }
            if (toolBar != null) {
                view.removeView(toolBar);
            }
            int index = 0;
            if (navBar != null) {
                view.addView(navBar, index);
                index++;
            }
            view.addView(customView.getContainerView(), index, new LinearLayout.LayoutParams(FILL_PARENT, 0, 1));
            index++;
            if (toolBar != null) {
                view.addView(toolBar, index);
            }
        } else {
            mRhoCustomView = null;
        }
    }

    public void restoreWebView() {
        if (mRhoCustomView != null) {
            view.removeView(mRhoCustomView.getContainerView());
            mRhoCustomView.destroyView();
            mRhoCustomView = null;

			if (navBar != null) {
				view.removeView(navBar);
			}
			if (toolBar != null) {
				view.removeView(toolBar);
			}
			
			int index = 0;
			if (navBar != null) {
				view.addView(navBar, index);
				index++;
			}

			addWebViewToMainView(webView, index, new LinearLayout.LayoutParams(FILL_PARENT, 0, 1));
			index++;
			if (toolBar != null) {
				view.addView(toolBar, index);
			}
		}

        if (mRhoCustomView != null) {
            mRhoCustomView.destroyView();
            mRhoCustomView = null;
        }
    }

    private String processForNativeView(String _url) {
        Logger.T(TAG, "processForNativiewView: " + _url);

    	String url = _url;
    	String callback_prefix = "call_stay_native";

    	// find protocol:navto pairs

    	int last = -1;
    	int cur = url.indexOf(":", last+1);
    	while (cur > 0) {
    		String protocol = url.substring(last+1, cur);
    		String navto = url.substring(cur+1, url.length());
    		
    		if (callback_prefix.equals(protocol)) {
    			// navigate but still in native view
    			String cleared_url = url.substring(callback_prefix.length()+1, url.length());
    			return cleared_url;
    		}
    		// check protocol for nativeView
    		IRhoCustomView nvf = RhoNativeViewManager.getNativeViewByType(protocol);
    		if (nvf != null) {
    			if (mRhoCustomView != null) {
    				if ( !protocol.equals(mRhoCustomView.getViewType()) ) {
        				setCustomView(nvf);
    				}
    			}
    			else {
    				setCustomView(nvf);
    			}
    			if (mRhoCustomView != null) {
	    			mRhoCustomView.navigate(navto);
	   				return "";
    			}
    		}
    		last = cur;
    		int c1 = url.indexOf(":", last+1);
    		int c2 = url.indexOf("/", last+1);
    		if ((c1 < c2)) {
    			if (c1 <= 0) {
    				cur = c2;
    			}
    			else {
    				cur = c1;
    			}
    		}
    		else {
    			if (c2 <= 0) {
    				cur = c1;
    			}
    			else {
    				cur = c2;
    			}
    		}
    	}
    	restoreWebView();
    	return url;
    }
	
	
	public IRhoWebView detachWebView() {
		restoreWebView();
		IRhoWebView v = null;
		if (webView != null) {
			removeWebViewFromMainView();
			v = webView;
			webView = null;
		}
		return v;
	}

    public void destroy() {
        if (webView != null) {
            webView.destroy();
        }
    }

	private View createButton(Map<Object,Object> hash) {
		Context ctx = RhodesActivity.getContext();
		
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
			icon = ctx.getResources().getDrawable(R.drawable.back);
			onClick = new ActionBack();
		}
		else if (action.equalsIgnoreCase("forward")) {
			if (RhodesService.isJQTouch_mode()) {
				return null;
			}
			icon = ctx.getResources().getDrawable(R.drawable.next);
			onClick = new ActionForward();
		}
		else if (action.equalsIgnoreCase("home")) {
			icon = ctx.getResources().getDrawable(R.drawable.home);
			onClick = new ActionHome();
		}
		else if (action.equalsIgnoreCase("options")) {
			icon = ctx.getResources().getDrawable(R.drawable.options);
			onClick = new ActionOptions();
		}
		else if (action.equalsIgnoreCase("refresh")) {
			icon = ctx.getResources().getDrawable(R.drawable.refresh);
			onClick = new ActionRefresh();
		}
		else if (action.equalsIgnoreCase("close") || action.equalsIgnoreCase("exit")) {
			icon = ctx.getResources().getDrawable(R.drawable.exit);
			onClick = new ActionExit();
		}
		else if (action.equalsIgnoreCase("separator"))
			return null;
		
		DisplayMetrics metrics = new DisplayMetrics();
		WindowManager wm = (WindowManager)ctx.getSystemService(Context.WINDOW_SERVICE);
		wm.getDefaultDisplay().getMetrics(metrics);
		
		Object iconObj = hash.get("icon");
		if (iconObj != null) {
			if (!(iconObj instanceof String))
				throw new IllegalArgumentException("'icon' should be String");
			
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
			if (is == null) {
				throw new IllegalArgumentException("Can't find icon file: " + iconPath);
			}
			Bitmap bitmap = BitmapFactory.decodeStream(is);
			if (bitmap == null)
				throw new IllegalArgumentException("Can't find icon: " + iconPath);
			bitmap.setDensity(DisplayMetrics.DENSITY_MEDIUM);
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
			if (mCustomBackgroundColorEnable) {
				Drawable d = btn.getBackground();
				if (d != null) {
					d.setColorFilter(mCustomBackgroundColor, android.graphics.PorterDuff.Mode.SRC_OVER);
				}
				else {
					btn.setBackgroundColor(mCustomBackgroundColor);
				}
			}
		}
		else {
			Button btn = new Button(ctx);
			btn.setText(label);
			if (mCustomBackgroundColorEnable) {
				btn.setBackgroundColor(mCustomBackgroundColor);
				int gray = (((mCustomBackgroundColor & 0xFF0000) >> 16) + ((mCustomBackgroundColor & 0xFF00) >> 8) + ((mCustomBackgroundColor & 0xFF)))/3; 
				if (gray > 128) {
					btn.setTextColor(0xFF000000);
				}
				else {
					btn.setTextColor(0xFFFFFFFF);
				}
			}
			button = btn;
		}
		
		button.setOnClickListener(onClick);
		
		return button;
	}
	
	@SuppressWarnings("unchecked")
	private void setupToolbar(LinearLayout tool_bar, Object params, Object options) {
		Context ctx = RhodesActivity.getContext();

        mCustomBackgroundColorEnable = false;

        
		List<Object> buttons = null;
		if (params != null) {
			if (params instanceof List<?>) {
				buttons = (List<Object>)params;
			}
			if ((options != null) && (options instanceof Map<?,?>)) 
			{
				Map<Object,Object> settings = (Map<Object,Object>)options;
				
				Object colorObj = settings.get("color");
				if (colorObj != null && (colorObj instanceof Map<?,?>)) {
					Map<Object,Object> color = (Map<Object,Object>)colorObj;
					
					Object redObj = color.get("red");
					Object greenObj = color.get("green");
					Object blueObj = color.get("blue");
					
					if (redObj != null && greenObj != null && blueObj != null &&
							(redObj instanceof Integer) && (greenObj instanceof Integer) && (blueObj instanceof Integer)) {
						try {
							int red = ((Integer)redObj).intValue() ;//Integer.parseInt((String)redObj);
							int green = ((Integer)greenObj).intValue() ;//Integer.parseInt((String)greenObj);
							int blue = ((Integer)blueObj).intValue() ;//Integer.parseInt((String)blueObj);
							
							mCustomBackgroundColor = ((red & 0xFF ) << 16) | ((green & 0xFF ) << 8) | ((blue & 0xFF )) | 0xFF000000;
							mCustomBackgroundColorEnable = true;
							
							tool_bar.setBackgroundColor(Color.rgb(red, green, blue));
						}
						catch (NumberFormatException e) {
							// Do nothing here
						}
					}
				}
				
				Object bkgObj = settings.get("backgroundColor");
				if ((bkgObj != null) && (bkgObj instanceof Integer)) {
					int color = ((Integer)bkgObj).intValue() ;//Integer.decode(((String)bkgObj)).intValue();
					int red = (color & 0xFF0000) >> 16;
					int green = (color & 0xFF00) >> 8;
					int blue = (color & 0xFF);
					tool_bar.setBackgroundColor(Color.rgb(red, green, blue));
					mCustomBackgroundColor = color | 0xFF000000;
					mCustomBackgroundColorEnable = true;
				}
				
				//Object buttonsObj = settings.get("buttons");
				//if (buttonsObj != null && (buttonsObj instanceof Vector<?>))
				//	buttons = (Vector<Object>)buttonsObj;
			}
		}
		
		if (params != null) {
			LinearLayout group = null;
			// First group should have gravity LEFT
			int gravity = Gravity.LEFT;
			Object[] buttons_array = buttons.toArray();
			
			
			for (int i = 0, lim = buttons_array.length; i < lim; ++i) {
				Object param = buttons_array[i];
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
					tool_bar.addView(group);
				}
				group.addView(button);
			}
			
			// Last group should have gravity RIGHT
			if (group != null) {
				group.setGravity(Gravity.RIGHT);
				tool_bar.requestLayout();
			}
		}
	}
	
	private void init(Object params) {
		Context activity = ContextFactory.getUiContext();
		
		view = new MyView(activity);
		view.setOrientation(LinearLayout.VERTICAL);
		view.setGravity(Gravity.BOTTOM);
		view.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, FILL_PARENT));
		
		LinearLayout bottom = new LinearLayout(activity);
		bottom.setOrientation(LinearLayout.HORIZONTAL);
		bottom.setBackgroundColor(Color.GRAY);
		bottom.setLayoutParams(new LinearLayout.LayoutParams(FILL_PARENT, WRAP_CONTENT, 0));
		view.addView(bottom);
		
		toolBar = bottom;
		
		setupToolbar(toolBar, params, null);
	}
	
	public SimpleMainView() {
		init(null);
	}
	
	public SimpleMainView(IRhoWebView v) {
		init(null);
		setWebView(v, 0);
	}
	
	public SimpleMainView(IRhoWebView v, Object params) {
		init(params);
		setWebView(v, 0);
	}

    @Override
    public void setWebView(IRhoWebView view, int tabIndex) {
		IRhoWebView rwv = detachWebView();
		if (rwv != null) {
			// destory
		}
        webView = view;
        addWebViewToMainView(webView, 0, new LinearLayout.LayoutParams(FILL_PARENT, 0, 1));
    }

	public void setWebBackgroundColor(int color) {
		view.setBackgroundColor(color);
		webView.getContainerView().setBackgroundColor(color);
		webView.getView().setBackgroundColor(color);
	}

    public void back(int index) {
        restoreWebView();
        
		String startUrl = RhoConf.getString("start_path");
		
		if(webView == null) return;

        boolean bStartPage = startUrl.equals(webView.getUrl());

        if ( !bStartPage && webView.canGoBack() ) {
            webView.goBack();
        }
        else
        {    
            RhodesActivity.safeGetInstance().moveTaskToBack(true);
        }
    }

	public void goBack() 
	{
		RhodesService.navigateBack();
	}

	public void forward(int index) {
		restoreWebView();
		webView.goForward();
	}

	public void navigate(String url, int index) {
		//Utils.platformLog("@&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@", "navigate("+url+")");
		String cleared_url = processForNativeView(url);
		Logger.I(TAG, "Cleared URL: " + url);
		if (cleared_url.length() > 0) {
			// check for handle because if we call loadUrl - WebView do not check this url for handle
			if (!RhodesService.getInstance().handleUrlLoading(cleared_url)) {
				webView.loadUrl(cleared_url);
			}
		}
	}

	
    //@Override
    public void executeJS(String js, int index) {
    	
    	//Utils.platformLog("@$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$@", "ExecuteJS("+js+")");
    	//((android.webkit.WebView)webView.getView()).executeJS();
    	if ((android.os.Build.VERSION.SDK_INT < 14) || (android.os.Build.VERSION.SDK_INT >= 19)) {    // 14 is 4.0, 19 is 4.4
    		navigate("javascript:"+js, index);
    		return;
    	}
    	
    	
		Method mStringByEvaluatingJavaScriptFromString = null;
		Method mSendMessage = null;
	    Object mWebViewCore = null;
	    Object mBrowserFrame = null;
	    boolean mHasPossibleUseOfReflectionExecuteJS = false;
	    Object webViewObject = this;
	    Class webViewClass = android.webkit.WebView.class;
	    try {
	        Field mp = webViewClass.getDeclaredField("mProvider");
	        mp.setAccessible(true);
	        webViewObject = mp.get((android.webkit.WebView)webView.getView());
	        webViewClass = webViewObject.getClass();
	        Field wc = webViewClass.getDeclaredField("mWebViewCore");
	        wc.setAccessible(true);
	        mWebViewCore = wc.get(webViewObject);
	        if (mWebViewCore != null) {
	        	
	        	mSendMessage = mWebViewCore.getClass().getDeclaredMethod("sendMessage", Message.class);
	        	mSendMessage.setAccessible(true);

	        	/*
	        	Field bf= mWebViewCore.getClass().getDeclaredField("mBrowserFrame");
	            
	        	bf.setAccessible(true);
	        	mBrowserFrame = bf.get(mWebViewCore);
	            mStringByEvaluatingJavaScriptFromString = mBrowserFrame.getClass().getDeclaredMethod("stringByEvaluatingJavaScriptFromString", String.class);
	            mStringByEvaluatingJavaScriptFromString.setAccessible(true);   
				*/
	        }
	        mHasPossibleUseOfReflectionExecuteJS = true;
	    } catch (Throwable e) {
	    	mHasPossibleUseOfReflectionExecuteJS = false;
	        //e.printStackTrace();
	    }		    		

	    boolean mHasReflectionWasExecutedOK = false;
	    
	    if (mHasPossibleUseOfReflectionExecuteJS && (mSendMessage != null)) {
			try {
				//mStringByEvaluatingJavaScriptFromString.invoke(mBrowserFrame, js);
				Message execJSCodeMsg = Message.obtain(null, 194, js);
				mSendMessage.invoke(mWebViewCore, execJSCodeMsg);
				mHasReflectionWasExecutedOK = true;
				//Utils.platformLog("@#########################@", "EvaluateJS("+js+")");
			} catch (Throwable e) {
				//e.printStackTrace();
			}
	    }
	    
	    if (!mHasReflectionWasExecutedOK) {
	        //com.rhomobile.rhodes.WebView.executeJs(js, index);
	    	navigate("javascript:"+js, index);
	    	
	    }
    }

	public void reload(int index) {
		if (mRhoCustomView != null) {
			mRhoCustomView.getView().invalidate();
		}
		else {
			webView.reload();
		}
	}

	public void stopNavigate(int index) {
	    if (mRhoCustomView == null) {
	        webView.stopLoad();
	    }
	    else {
	        mRhoCustomView.stop();
	    }
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
		restoreWebView();
		webView.loadData(data, "text/html", "utf-8");
	}

	public void addNavBar(String title, Map<Object,Object> left, Map<Object,Object> right) {
		removeNavBar();
		
		Context ctx = RhodesActivity.getContext();
		
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
	
	public void setToolbar(Object params, Object options) {
		toolBar.setBackgroundColor(Color.GRAY);
		toolBar.removeAllViews();
		setupToolbar(toolBar, params, options);
		toolBar.requestLayout();
		view.requestLayout();
	}
	
	public void removeToolbar() {
		toolBar.removeAllViews();
		toolBar.requestLayout();
		view.requestLayout();
	}

	@Override
	public int getTabsCount() {
		return 1;
	}

    @Override
    public void saveCurrentPage(String format, String path, int index) {
        if (format.equalsIgnoreCase(MainView.FORMAT_JPEG)) {
            webView.capture(IRhoWebView.CaptureFormat.CAPTURE_FORMAT_JPEG, path);
        }
        else {
            Logger.E(TAG, "Wrong format to save current page: " + format);
        }
    }

    public String get_current_url(int tab) {
        if (webView == null) {
            return "";
        }
        return webView.getUrl();
    }

    @Override
    public void removeSplashScreen() {
    }
    
    @Override
	public String getTabDefaultUrl() {
		// TODO Auto-generated method stub
		return "";
	}

}
