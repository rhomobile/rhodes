package com.rhoelementsext;

import com.rho.rhoelements.apd.ApdEngine;

import android.webkit.JavascriptInterface;

/**
 * JavaScriptInterface class for APD. Exposes APD methods to JavaScript.
 * @author Ben Kennedy (NCVT73)
 */
public class APD {
	public static final String TAG = APD.class.getSimpleName();
	private static ApdEngine myApdEngine = new ApdEngine();
    private static APD mInstance = null;
    
    public static synchronized APD getInstance() {
        if (mInstance == null) {
            mInstance = new APD();
        }
        return mInstance;
    }
    
	@Override
	protected Object clone() throws CloneNotSupportedException {
		throw new CloneNotSupportedException("Clone is not allowed.");
	}
	
	private APD() {
		
	}

    @JavascriptInterface
    public void PSExternal(int iCommand, String strParameter) {
    	myApdEngine.PSExternal(iCommand, strParameter);
    }
    
    @JavascriptInterface
    public int PSExternalEx(int iCommand, String strParameter) {
		return myApdEngine.PSExternalEx(iCommand, strParameter);
    }
    
    @JavascriptInterface
    public String PSGetLastMessage() {
    	return myApdEngine.PSGetLastMessage();
    }
}
