/**
 * EditText control that is invisible and invokes the Numeric keyboard 
 * since the default is always Alphanumeric
 */
package com.rho.rhoelements;

import android.content.Context;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.View;
import android.widget.EditText;

/**
 * @author pxb743
 *
 */
public class WebEditText extends EditText
{
	//Pointer to the web view to dispatch the keys
	View mWebView;
	
	/**
	 * Constructor to save the web view
	 * @param context
	 * @param view
	 */
	public WebEditText(Context context, View view)
	{
		super(context);		
		mWebView = view;		
	}
	
	public WebEditText(Context context)
	{
		super(context);		
	}

	public WebEditText(Context context, AttributeSet attrs)
	{
		super(context, attrs);		
	}
	
	public WebEditText(Context context, AttributeSet attrs, int defStyle)
	{
		super(context, attrs, defStyle);	
	}

	/**
	 * Override the dispatch key event to send the key events to the web view
	 * from the invisible Edit Text control
	 */
	@Override
	public boolean dispatchKeyEvent(KeyEvent event)
	{	
		//Ignore system events
		//if (event.isSystem()) {
            //return super.dispatchKeyEvent(event);
        //}
		
		//Dispatch the keys to the web view
		mWebView.dispatchKeyEvent(event);
		return super.dispatchKeyEvent(event);       
	}
}
