/**
 * 
 */
package com.rho.rhoelements.plugins;

import java.net.MalformedURLException;
import java.net.URL;

import android.content.Context;
import android.graphics.Rect;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsoluteLayout;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843
 *
 */
@SuppressWarnings("deprecation")
public class AddressBarPlugin extends Plugin
{
	private AutoCompleteTextView mAddressBar;
	protected AbsoluteLayout mTextViewPanel;
	private int mLeft = 0;
	private int mTop = 0;
	private static final int HEIGHT = 65; //default height
	private int mWidth;
	ArrayAdapter<CharSequence> mAdapter;
	
	/**
	 * Constructor
	 */
	public AddressBarPlugin()
	{
		mAddressBar = (AutoCompleteTextView) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","addressbar"));
		mTextViewPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","addressbar_panel"));
		mAdapter = new ArrayAdapter<CharSequence>(Common.mainActivity, android.R.layout.simple_spinner_item);
		mAddressBar.setAdapter(mAdapter);
		mAddressBar.setOnEditorActionListener(new OnEditorActionListener()
		{
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
			{
				if(actionId == EditorInfo.IME_ACTION_GO)
				{
					go();
					return true;
				}
				return false;
			}
		});
		
		mAddressBar.setOnKeyListener(new OnKeyListener(){
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				if(keyCode == KeyEvent.KEYCODE_ENTER){
					go();
					return true;
				}
				return false;
			}
		});
		
		
		//loadWebHistory();
		mAddressBar.setOnClickListener(new View.OnClickListener() {
		    public void onClick(View v) {
		    	 mAddressBar.showDropDown();
		    }
		});
		
		View mLayout =  Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","plugin_container"));
		mLayout.setOnTouchListener(new OnTouchListener() {
				
		@Override
		public boolean onTouch(View v, MotionEvent event) {
                    if (event.getAction() == MotionEvent.ACTION_DOWN) {
                        if (mAddressBar.isFocused()) {
                            Rect outRect = new Rect();
                            mAddressBar.getGlobalVisibleRect(outRect);
                            if (!outRect.contains((int)event.getRawX(), (int)event.getRawY())) {
                            	mAddressBar.clearFocus();
                                InputMethodManager imm = (InputMethodManager) v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE); 
                                imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
                            }
                        }
                    }
                    return false;
                }

		});
		
		DisplayMetrics displayMetrics = new DisplayMetrics();
		Common.mainActivity.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		mWidth = (int) (displayMetrics.widthPixels * 0.8); //default width
		mTextViewPanel.removeAllViews();
		mTextViewPanel.addView(mAddressBar, new AbsoluteLayout.LayoutParams(mWidth, HEIGHT, mLeft, mTop));
	}
	
	/**
	 * Attempts to navigate to the text that has been entered into the address bar
	 */
	public void go()
	{
    	try
    	{
    		String rawUrlString = getUrl();
    		String protocoledUrl = Common.addDefaultProtocol(rawUrlString);
    		if(!rawUrlString.equals(protocoledUrl))
    		{
    			mAddressBar.setText(protocoledUrl);
    		}
	    	Common.elementsCore.getWebViewInstance().loadUrl(protocoledUrl);
    	}
    	catch (MalformedURLException e)
    	{
    		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "URL not valid"));
    		Common.elementsCore.getWebViewInstance().loadUrl(getUrl());
    	}
	}
	
	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
	 */
	@Override
	public void onSetting(PluginSetting setting)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		if (mAddressBar == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Address bar is null"));
			return;
		}
		
		try
		{
			if (setting.getName().equalsIgnoreCase("Left"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Left"));
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mLeft = Integer.valueOf(setting.getValue());
					mTextViewPanel.removeAllViews();
					mTextViewPanel.addView(mAddressBar, new AbsoluteLayout.LayoutParams(mWidth, HEIGHT, mLeft, mTop));
				}
			}
			else if (setting.getName().equalsIgnoreCase("Top"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Top"));
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mTop = Integer.valueOf(setting.getValue());
					mTextViewPanel.removeAllViews();
					mTextViewPanel.addView(mAddressBar, new AbsoluteLayout.LayoutParams(mWidth, HEIGHT, mLeft, mTop));
				}
			}
			else if (setting.getName().equalsIgnoreCase("Width"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Width"));
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mWidth = Integer.valueOf(setting.getValue());
					mTextViewPanel.removeAllViews();
					mTextViewPanel.addView(mAddressBar, new AbsoluteLayout.LayoutParams(mWidth, HEIGHT, mLeft, mTop));
				}
			}
			else if (setting.getName().equalsIgnoreCase("Visibility"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Visibility"));
				if (setting.getValue().compareToIgnoreCase("visible") == 0)
				{
					mTextViewPanel.setVisibility(View.VISIBLE); 
				}
				else if (setting.getValue().compareToIgnoreCase("hidden") == 0)
				{
					mTextViewPanel.setVisibility(View.GONE);
				}
			}
		}
		catch (NumberFormatException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Passed parameter has wrong format"));
		}
				
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));

	}
	
	public ArrayAdapter<CharSequence> getAdapterInstance()
	{
		return mAdapter;
	}
	
	public String getUrl()
	{
		return mAddressBar.getText().toString();
	}

	/* (non-Javadoc)
	 * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
	 */
	@Override
	public void onShutdown()
	{
		
	}
	
	@Override
	public void onPageFinished(String url)
	{
		mAddressBar.setText(Common.elementsCore.getWebViewLastEnd());
	}

	@Override
	public void onPageStarted(String url)
	{
		mAddressBar.setText(Common.elementsCore.getWebViewLastStart());
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("AddressBar");
	}
	
}
