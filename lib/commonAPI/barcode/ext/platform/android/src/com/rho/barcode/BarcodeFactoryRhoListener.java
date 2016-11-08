package com.rho.barcode;

import android.view.KeyEvent;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;

import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.webview.GoogleWebView;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.webview.WebViewConfig;

public class BarcodeFactoryRhoListener extends AbstractRhoListener
{
	private static final String TAG = "BarcodeFactoryRhoListener";
	private static BarcodeFactory factory;
	
	private boolean IsFirstResume=true;
	
	private BarcodeRhoExtension mBarcodeExt = new BarcodeRhoExtension();
	public BarcodeFactoryRhoListener() { }
	
	@Override
	public void onResume(RhodesActivity activity)
	{
		Logger.I(TAG, "BarcodeFactoryRhoListener onResume");
		int myUseDWScanning = 0;
		IRhoConfig myIRhoConfig= null;
		IRhoWebView myIRhoWebView=RhodesActivity.safeGetInstance().getMainView().getWebView(0);
		GoogleWebView myGoogleWebView=null;

		try {
			myGoogleWebView=(GoogleWebView)myIRhoWebView;			
		} catch( ClassCastException e ) {
			Logger.W(TAG, "onResume: WebView is not from Google, skipping configuration" );
			myGoogleWebView = null;
		}

		if ( myGoogleWebView != null ) {
			myIRhoConfig=myGoogleWebView.getConfig();

			if(myIRhoConfig.getString("usedwforscanning") != null)
			 {
				try
				{
					myUseDWScanning = Integer.parseInt(myIRhoConfig.getString("usedwforscanning"));
					Logger.D(TAG, "BarcodeFactoryRhoListener onResume-usedwsforcanning value"+myUseDWScanning);
				}
				catch(Exception e)
				{
					Logger.W(TAG, "BarcodeFactoryRhoListener onResume-exception:- "+e.getMessage());
					myUseDWScanning=0;
				}
			 }
		}
		
		if(IsFirstResume)
		{
			Logger.I(TAG, "BarcodeFactoryRhoListener onResume--first resume");
			
			if(0==myUseDWScanning)//no DW
			{
			Logger.D(TAG, "BarcodeFactoryRhoListener onResume-do not Use DW-useDW false-before");
			factory = new BarcodeFactory(false);
			BarcodeFactorySingleton.setInstance(factory);
			Logger.I(TAG, "BarcodeFactoryRhoListener onResume-BarcodeFactory-DW-false created ");
			}
			else//DW
			{
			Logger.D(TAG, "BarcodeFactoryRhoListener onResume-Use DW-useDW true-before");
			factory = new BarcodeFactory(true);
			BarcodeFactorySingleton.setInstance(factory);
			Logger.I(TAG, "BarcodeFactoryRhoListener onResume-BarcodeFactory-DW-true created ");
			}
		
		}
		
		else
		{
			Logger.D(TAG, "BarcodeFactoryRhoListener onResume--Not first resume");
		}
		
		
		IsFirstResume=false;
		if(factory != null) factory.onResume();
		Logger.D(TAG, "BarcodeFactoryRhoListener onResume-Finish");
	}

	@Override
	public void onPause(RhodesActivity activity)
	{
		Logger.D(TAG, "onPause");
		if(factory != null) factory.onPause();
	}

	@Override
	public void onStop(RhodesActivity activity)
	{
		Logger.D(TAG, "onStop");
		if(factory != null) factory.onStop();
	}

	@Override
	public void onDestroy(RhodesActivity activity)
	{
		Logger.D(TAG, "onDestroy");
		if(factory != null) factory.onDestroy();
	}

	@Override
	public void onCreateApplication(IRhoExtManager extManager)
	{
		Logger.I(TAG, "BarcodeFactoryRhoListener onCreateApplication");
		extManager.startKeyEventUpdates(this);
		extManager.addRhoListener(this);
		//factory = new BarcodeFactory();
		//BarcodeFactorySingleton.setInstance(factory);
		extManager.registerExtension("BarcodeRhoExtension", mBarcodeExt);
	}

	@Override
	public boolean onKey(int keyCode, KeyEvent event)
	{
		//Logger.D(TAG, " onKey");
		if(factory != null) return factory.onKey(keyCode, event);
		return false;
	}
}
