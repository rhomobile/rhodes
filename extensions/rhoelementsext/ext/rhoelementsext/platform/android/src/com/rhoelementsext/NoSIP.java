package com.rhoelementsext;

import android.content.Context;
import android.util.Log;
import android.view.inputmethod.InputMethodManager;
import android.webkit.JavascriptInterface;

import com.rho.rhoelements.Common;

/**
 * JavaScriptInterface class for NoSIP. Exposes NoSIP method to JavaScript.
 * @author Ben Kennedy (NCVT73)
 */
public class NoSIP
{	
	public static final String TAG = NoSIP.class.getSimpleName();
    
	private static NoSIP instance = null;
    
    public static synchronized NoSIP getInstance()
    {
        if (instance == null)
        {
            instance = new NoSIP();
        }
        return instance;
    }
    
	private NoSIP()
	{
		
	}
	
	@JavascriptInterface
	public void ShowSIP(final boolean showSip)
	{
		Log.e(TAG, "ShowSIP+");
		Common.elementsCore.getWebViewInstance().getView().postDelayed(new Runnable()
		{
			@Override
			public void run()
			{
				InputMethodManager imm = (InputMethodManager)Common.mainActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
				if (showSip)
				{
					imm.showSoftInput(Common.elementsCore.getWebViewInstance().getView(), 0);
				}
				else
				{
					imm.hideSoftInputFromWindow(Common.elementsCore.getWebViewInstance().getView().getWindowToken(), 0);
				}
			}
		}, 0);
		Log.e(TAG, "ShowSIP-");
	}
}
