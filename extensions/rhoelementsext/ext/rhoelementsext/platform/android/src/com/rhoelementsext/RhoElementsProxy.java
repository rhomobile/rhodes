package com.rhoelementsext;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.app.Dialog;
import android.content.res.Configuration;
import android.nfc.NfcAdapter;
import com.rho.rhoelements.Common;
import com.rho.rhoelements.ElementsCore;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

import android.util.Log;
import android.view.KeyEvent;

public class RhoElementsProxy extends AbstractRhoListener
{
    public static final String RHOCONFIG = "rhoelementsext";
    public static int RECEIVER_REGISTER_STATUS = 0;
    
    private static final String TAG = RhoElementsProxy.class.getSimpleName();
    private BroadcastReceiver mNavigateReceiver;
    
    private static ElementsCore elementsCore;
    private static Activity mActivity;
    private static RhoElementsProxy instance;
        
    @Override
    public void onCreate(RhodesActivity activity, Intent intent)
    {
    	Logger.D(TAG, "onCreate -- START");
        elementsCore = new ElementsCore();
        //Create a bundle to send some relevant information to ElementsCore at init time
        Bundle bundle = new Bundle();
        bundle.putString("source", "rhodes");
        IRhoExtManager extManager = RhoExtManager.getInstance();
        elementsCore.onCreate(activity, extManager.getTopView(), bundle);

        Logger.D(TAG, "onCreate -- intent: " + intent);
        if ((intent != null) && (intent.getAction() != null) && (intent.getAction().compareTo(ElementsCore.RHOELEMENTS_SHORTCUT_ACTION) == 0) )
            RhoElementsExt.isFromIntent = true;
        mActivity = activity;
        initializeNavigateIntentReceiver();
        Logger.D(TAG, "onCreate -- END");
    }

    @Override
    public void onStart(RhodesActivity activity)
    {
    	Logger.D(TAG, "onStart -- START");
		elementsCore.configureWebKit();
		elementsCore.onStart(activity);
		initializeNavigateIntentReceiver();
		RhoElementsConfiguration reConfiguration = RhoElementsConfiguration.getInstance();
    	reConfiguration.setConfig(Common.config);
		RhoExtManager.getInstance().setConfig(RHOCONFIG, reConfiguration);
    }

    @Override
    public void onResume(RhodesActivity activity)
    {
    	Logger.D(TAG, "onResume -- START");
    	elementsCore.onResume(activity);
    }

    @Override
    public void onPause(RhodesActivity activity)
    {
    	Logger.D(TAG, "onPause -- START");
    	elementsCore.onPause(activity);
    }

    @Override
    public void onStop(RhodesActivity activity)
    {
    	Logger.D(TAG, "onStop -- START");
    	elementsCore.onStop(activity);
    }

    @Override
    public void onDestroy(RhodesActivity activity)
    {
    	Logger.D(TAG, "onDestroy -- START");
    	try
    	{
    		if (mNavigateReceiver != null)
    			activity.unregisterReceiver(mNavigateReceiver);
    	}
    	catch (IllegalArgumentException e)
    	{
    		Logger.D(TAG, "onStop -- broadcastReceiver not registered");
    	}
    	elementsCore.onDestroy(activity);
    	
    	elementsCore.unRegisterCustomReceiver();
    }

    
    @Override
    public void onNewIntent(RhodesActivity activity, Intent intent)
    {
        Logger.D(TAG, "onNewIntent -- START");
        RhoElementsExt.isFromIntent = true;
        String action = intent.getAction();
       if (!(NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)
                || NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)
                || NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action))) {
	        if (RhoExtManager.getInstance().onStartNewConfig()) //If this is true, let ElementsCore examine the intent to see whether the user is trying to load from a shortcut
	        {
	            elementsCore.onNewIntent(intent, true);
	            
	            File configXmlFile = null;
	            File rhoconfigTxtFile = null;
	            try
	            {
	                configXmlFile = new File(Common.parseAndroidURI(intent.getDataString()));
	
	                if (configXmlFile != null && configXmlFile.exists()) {
	                    rhoconfigTxtFile = new File(configXmlFile.getParent() + "/rhoconfig.txt");
	                }
	                 
	                if (rhoconfigTxtFile != null && rhoconfigTxtFile.exists()) {
	                    RhoConf.setPath(rhoconfigTxtFile.getParent());
	                    RhoExtManager.getInstance().setConfig("rhoconfig", new RhoConf.RhoConfig());
	                }
	            
	            } 
	            catch (Throwable e)
	            {
	                Logger.E(TAG, e);
	            }
	            RhoElementsConfiguration reConfiguration = RhoElementsConfiguration.getInstance();
	            reConfiguration.setConfig(Common.config);
	            RhoExtManager.getInstance().setConfig(RHOCONFIG, reConfiguration);
	        }
        }
    }
    
    @Override
    public void onActivityResult(RhodesActivity activity, int reqCode, int resCode, Intent intent) {
        Logger.D(TAG, "onActivityResult -- START");
        if (reqCode == elementsCore.RESULT_VIDEO_CODE) {
            if (resCode == android.app.Activity.RESULT_OK) {
                elementsCore.writeVideoCapture(intent.getData());
            }
        }
        else {
            Generic.getInstance().onActivityResult(reqCode, resCode);
        }
        Logger.D(TAG, "onActivityResult -- END");
    }
    
    @Override
    public Dialog onCreateDialog(RhodesActivity activity, int id/*, Bundle args*/)
    {
    	Logger.D(TAG, "onCreateDialog -- START");
    	Logger.D(TAG, "Dialog id: " + id);
    	return elementsCore.onCreateDialog(id);
    }

    @Override
    public void onCreateApplication(IRhoExtManager extManager)
    {
        Logger.T(TAG, "onCreateApplication -- START");
        instance = this;
        extManager.addRhoListener(this);
        extManager.startKeyEventUpdates(this,true); //Make important, makes KeyCapture get key events first. 
        extManager.registerExtension("rhoelements", new RhoElementsExt());
    }
    
    @Override
    public void onConfigurationChanged(RhodesActivity activity, Configuration newConfig)
    {
	Logger.T(TAG, "onConfigurationChanged -- START");
	elementsCore.onConfigurationChanged(newConfig);
    }
     @Override
   public boolean onKey(int keyCode, KeyEvent event) {
   	
    	return elementsCore.onKeyDispatchFromActivity(keyCode,event);
    }
    
    public static ElementsCore getElementsCoreInstance()
    {
    	return elementsCore;
    }
    
    public static Activity getActivity()
    {
    	return mActivity;
    }
    
    public static RhoElementsProxy getInstance()
    {
    	return instance;
    }
    
    /**
     * This initializes the intent receiver for intents coming from ElementsCore
     */
    private void initializeNavigateIntentReceiver()
    {
    	Logger.D(TAG, "initializeNavigateIntentReceiver -- START");
    	if (mNavigateReceiver != null)
    		return;
    	Logger.D(TAG, "Navigate receiver not yet initialized...creating it...");
    	mNavigateReceiver = new BroadcastReceiver()
    	{
    	    @Override
    	    public void onReceive(Context context, Intent intent)
    	    {   	
    		Logger.D("RhoElementsExt", "onReceive");
    		Logger.D("RhoElementsExt", "intent: " + intent);
    		Logger.D("RhoElementsExt", "current process_id: " + android.os.Process.myPid());
    		Logger.D("RhoElementsExt", "intent process_id: " + intent.getIntExtra("process_id", android.os.Process.myPid()));
    		if ( (intent == null) || (intent.getIntExtra("process_id", android.os.Process.myPid()) != android.os.Process.myPid()) )
    		    return;
    		if (intent.getAction().equals(ElementsCore.RHOMOBILE_NAVIGATE_ACTION))
		{
    		    if (intent.getStringExtra("url") != null)
		    {
			String url = intent.getStringExtra("url");
			Logger.D("RhoElementsProxy", "url: " + url);
			if (url.startsWith("/app"))
			{
			    String callbackUrl = url;
			    String callbackData = null;
			    Logger.D(TAG, "NavigateReceiver -- onReceive() -- Execute ruby callback");
			    if (url.indexOf('{') >= 0) //contains json data
			    {
				callbackUrl = url.substring(0, url.indexOf('{') );
				callbackData = url.substring(callbackUrl.length());
				
				int base64PlaceHolderPos = callbackData.lastIndexOf("<base64_placeholder>");
				int base64PlaceHolderLength = "<base64_placeholder>".length();
				if ( (base64PlaceHolderPos > 0) && (elementsCore.base64Data != null) )
				{
				    String suffix = url.substring(base64PlaceHolderPos + base64PlaceHolderLength, url.length());
				    callbackData = callbackData.substring(0, base64PlaceHolderPos) + elementsCore.base64Data + suffix;
				}
				
				RhoExtManager.getInstance().executeRubyCallbackWithJsonBody(callbackUrl, callbackData, "", false);
				elementsCore.base64Data = null;
			    }
			    else
			    {
				RhoExtManager.getInstance().executeRubyCallback(callbackUrl, callbackData, "", false);
			    }
			}
			else
			{
			    Logger.D(TAG, "NavigateReceiver -- onReceive() -- navigate to url: " + url);
			    
			    int base64PlaceHolderPos = url.lastIndexOf("<base64_placeholder>");
			    int base64PlaceHolderLength = "<base64_placeholder>".length();
			    if ( (base64PlaceHolderPos > 0) && (elementsCore.base64Data != null) )
			    {
				String suffix = url.substring(base64PlaceHolderPos + base64PlaceHolderLength, url.length());
				url = url.substring(0, base64PlaceHolderPos) + elementsCore.base64Data + suffix;
			    }
			    
			    RhoExtManager.getInstance().navigate(url);
			    elementsCore.base64Data = null;
			}
		    }
		}
		else if (intent.getAction().equals(ElementsCore.RHOMOBILE_QUIT_ACTION))
		{
		    Logger.D(TAG, "NavigateReceiver -- onReceive() -- quit");
		    RhoExtManager.getInstance().quitApp();
		}
    	    }
	};
	IntentFilter intentFilter = new IntentFilter(ElementsCore.RHOMOBILE_NAVIGATE_ACTION);
	intentFilter.addAction(ElementsCore.RHOMOBILE_QUIT_ACTION);
	mActivity.registerReceiver(mNavigateReceiver, intentFilter);
	Logger.D(TAG, "Navigate receiver now registered");
    }
}
