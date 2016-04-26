package com.rho.rhoelements.services;

import android.app.IntentService;
import android.content.Intent;
import android.util.Log;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
//import java.util.logging.Logger;


import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.inputmethod.InputMethodManager;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.symbol.msr.MSR;
import com.symbol.msr.MSR.NotificationCallback;
import com.symbol.msr.MSR.SwipeCallback;


/**
 * @author FPC843
 *
 */
public class MSRServiceRebranded extends IntentService 
{
	private static final String TAG = "MSRServiceRebranded";
	
    public static final String MSR_SERVICE_NAME = "com.symbol.msr.MSR.MSRServiceRebranded";
    public static final String METHOD_EXTRA = "MSR_METHOD";   
    public static final String MSR_SWYPE_ACTION = "com.symbol.msr.MSR.MSR_SWYPE_ACTION";   
    public static String encryption = "unencrypted";
    private static final int MIN_DATA_LENGTH = 9; //Minimum length of any meaningful data from MSR
	//public static String MSR_DATA_STRING_TAG = null;
   public static String MSR_DATA_STRING_TAG = null;
  public static  String MSR_DATA_TAG = null;
  public static String MSR_TRACK1_TAG = null;
  public static String MSR_TRACK2_TAG = null;
  public static String MSR_TRACK3_TAG = null;
  public static String MSR_TRACK1_STATUS_TAG = null;
  public static String MSR_TRACK2_STATUS_TAG = null;
  public static String MSR_TRACK3_STATUS_TAG = null;
  public static String MSR_TRACK1_ENCRYPTED_TAG = null;
  public static String MSR_TRACK2_ENCRYPTED_TAG = null;
  public static String MSR_TRACK3_ENCRYPTED_TAG = null;
  public static String MSR_TRACK1_ENCRYPTED_STATUS_TAG = null;
  public static String MSR_TRACK2_ENCRYPTED_STATUS_TAG = null;
  public static String MSR_TRACK3_ENCRYPTED_STATUS_TAG = null;
  public static String MSR_TRACK1_HASHED_TAG = null;
  public static String MSR_TRACK2_HASHED_TAG = null;
  public static String MSR_TRACK3_HASHED_TAG = null;
  public static String MSR_KSN_NUMBER_TAG = null;
  
  
    private static MSR mMsr=null;
    private int flag=0;
    
    private static MSR msr;
    byte response[] = new byte[MSR.DECODER_MODE_STANDARD];
    private static byte[] enableCommand = { (byte) 0x02, (byte) 0x53, (byte) 0x1a, (byte) 0x01, (byte) 0x31 , (byte) 0x03, (byte) 0x78};
    private static byte[] disableCommand = { (byte) 0x02, (byte) 0x53, (byte) 0x1A, (byte) 0x01, (byte) 0x30 , (byte) 0x03, (byte) 0x79};
   
    private static boolean isConnected;
    static
    {
    	try
    	{
    		MSR_DATA_STRING_TAG = (String) MSR.class.getDeclaredField("DATA_STRING_TAG").get(String.class);
    		MSR_DATA_STRING_TAG=com.symbol.msr.IDTechDefs.DATA_STRING_TAG;
    		Logger.D(TAG, "MSR.DATA_STRING_TAG found");
    	}
    	catch(NoSuchFieldException e)
    	{
    		Logger.D(TAG, "No MSR.DATA_STRING_TAG");
    	}
		catch (IllegalArgumentException e)
		{
			Logger.D(TAG, "IllegalArgument for MSR.DATA_STRING_TAG");
			e.printStackTrace();
		}
		catch (SecurityException e)
		{
			Logger.D(TAG, "SecurityException for MSR.DATA_STRING_TAG");
			e.printStackTrace();
		}
		catch (IllegalAccessException e)
		{
			Logger.D(TAG, "IllegalAccess for MSR.DATA_STRING_TAG");
			e.printStackTrace();
		}
	
    	catch(Exception ex)
    	{
    		
    	}
    	if(MSR_DATA_STRING_TAG == null)
    	{
	    	try
	    	{
	    		Class c = Class.forName("com.symbol.msr.MSR.IDTechDefs");
	    		
	    		MSR_DATA_STRING_TAG = (String) c.getDeclaredField("DATA_STRING_TAG").get(String.class);
	    		Logger.D(TAG, "IDTechDefs.DATA_STRING_TAG found");
		    }
	    	catch (ClassNotFoundException e)
	    	{
				Logger.D(TAG, "No IDTechDefs");
	    	}
			catch(NoSuchFieldException e)
			{
				Logger.D(TAG, "No IDTechDefs.DATA_STRING_TAG");
			}
			catch (IllegalArgumentException e)
			{
				Logger.D(TAG, "IllegalArgument for IDTechDefs.DATA_STRING_TAG");
				e.printStackTrace();
			}
			catch (SecurityException e)
			{
				Logger.D(TAG, "SecurityException for IDTechDefs.DATA_STRING_TAG");
				e.printStackTrace();
			}
			catch (IllegalAccessException e)
			{
				Logger.D(TAG, "IllegalAccess for IDTechDefs.DATA_STRING_TAG");
				e.printStackTrace();
			}
    	}
    }
    

    public MSRServiceRebranded()
    {   
    	super("CradReaderService");
    	  
    	// MSR.MsrRegisterRecv(MSRServiceRebranded.this);
		if (msr == null)
		{ 
		   msr = new com.symbol.msr.MSR();
		   mMsr = msr;
		   isConnected = MSR.isMsrConnected();
		}
		
        MSR_DATA_TAG = com.symbol.msr.MSR.DATA_TAG;
	    MSR_TRACK1_TAG = com.symbol.msr.MSR.TRACK1_TAG;
	     MSR_TRACK2_TAG = com.symbol.msr.MSR.TRACK2_TAG;
	    MSR_TRACK3_TAG = com.symbol.msr.MSR.TRACK3_TAG;
	    MSR_TRACK1_STATUS_TAG =com.symbol.msr.MSR.TRACK1_STATUS_TAG;
	    MSR_TRACK2_STATUS_TAG = com.symbol.msr.MSR.TRACK2_STATUS_TAG;
	    MSR_TRACK3_STATUS_TAG = com.symbol.msr.MSR.TRACK3_STATUS_TAG;
	    MSR_TRACK1_ENCRYPTED_TAG = com.symbol.msr.MSR.TRACK1_ENCRYPTED_TAG;
	    MSR_TRACK2_ENCRYPTED_TAG = com.symbol.msr.MSR.TRACK2_ENCRYPTED_TAG;
	    MSR_TRACK3_ENCRYPTED_TAG = com.symbol.msr.MSR.TRACK3_ENCRYPTED_TAG;
	    MSR_TRACK1_ENCRYPTED_STATUS_TAG = com.symbol.msr.MSR.TRACK1_ENCRYPTED_STATUS_TAG;
	    MSR_TRACK2_ENCRYPTED_STATUS_TAG = com.symbol.msr.MSR.TRACK2_ENCRYPTED_STATUS_TAG;
	    MSR_TRACK3_ENCRYPTED_STATUS_TAG = com.symbol.msr.MSR.TRACK3_ENCRYPTED_STATUS_TAG;
	    MSR_TRACK1_HASHED_TAG = com.symbol.msr.MSR.TRACK1_HASHED_TAG;
	    MSR_TRACK2_HASHED_TAG = com.symbol.msr.MSR.TRACK2_HASHED_TAG;
	    MSR_TRACK3_HASHED_TAG = com.symbol.msr.MSR.TRACK3_HASHED_TAG;
	    MSR_KSN_NUMBER_TAG = com.symbol.msr.MSR.KSN_NUMBER_TAG;
		
	    
        msr.setSwypeCallback(new SwipeCallback() {
			@Override
			public void onSwipe(byte[] arg0, int arg1) {
				callOnSwipe(arg0,arg1);
				 
			}
		});
		
       MSR.setNotificationCallback(new NotificationCallback(){
        	@Override
        	public void onConnectionChange(boolean connected) {
        		 String connectString = null;
        		    if( connected ) {
        		        connectString = "connected"; 
        		       setupMSRHead();
        		       if(flag==1)
        		       {
        		   	msr.open();
        		   	flag=0;
        		       }
        		    }
        		    else {
        		        connectString = "not connected";
        		    }
        		   
        }});
    }
    
    public void callOnSwipe(byte[] data, int dataLen) {
		
		Logger.D(TAG,  "onSwipe API");
		Logger.D(TAG,  "data: " + data + " datalen: " + dataLen);
	
		if ( (data == null) || (dataLen < MIN_DATA_LENGTH) )
		{
			Log.e(TAG, "MSR has returned no valid data");
		    return;
		}	
		
		Bundle b = null;
		Method method = null;
		int methodVersion = 0;
		
		try
		{
			method = MSR.class.getMethod("processSwipeOutput", byte[].class, int.class, int.class);
			methodVersion = 3;
		}
		catch(NoSuchMethodException e){}
		
		Class c = null;
		try
		{   
			c = Class.forName("com.symbol.msr.IDTechDefs");
		}
    	catch (ClassNotFoundException e)
    	{
    		Logger.D(TAG, "No IDTechDefs");
    		
    	}
		
		if(c != null && methodVersion!=3)
		{
			try
			{
				method = c.getMethod("processSwipeOutput", byte[].class, int.class, int.class);
				methodVersion = 2;
			}
			catch(NoSuchMethodException f){}
			
			try
			{
				method = c.getMethod("processSwipeOutput", byte[].class, int.class);
				methodVersion = 1;
			}
			catch(NoSuchMethodException g){}
		}
		
		try
		{
			if(methodVersion == 3)
			{
				b = (Bundle) method.invoke(null, data, dataLen, Character.getNumericValue('\r'));
				Log.w(TAG, "MSR.processSwipeOutput OK!");
			}
			else if(methodVersion == 2)
			{
				b = (Bundle) method.invoke(null, data, dataLen, Character.getNumericValue('\r'));
				Log.w(TAG, "IDTechDefs.processSwipeOutput(x,y,z) OK!");
			}
			else if(methodVersion == 1)
			{
				b = (Bundle) method.invoke(null, data, dataLen);
				Log.w(TAG, "IDTechDefs.processSwipeOutput(x,y) OK!");
			}
			else
			{
				Log.e(TAG, "Cannot processSwipeOutput: This device needs updating with the latest MSR library");
			    return;
			}
		}
		catch (IllegalArgumentException e)
		{
			Log.e(TAG, "Cannot processSwipeOutput: IllegalArgumentException");	e.printStackTrace();
		}
		catch (IllegalAccessException e)
		{
			Log.e(TAG, "Cannot processSwipeOutput: IllegalAccessException");	e.printStackTrace();
		}
		catch (InvocationTargetException e)
		{
			Log.e(TAG, "Cannot processSwipeOutput: InvocationTargetException");	e.printStackTrace();
		}
	
		if ( b == null )
		{
			Log.e(TAG, "MSR has returned no valid data");
		    return;
		}	
	
		String cardData = b.getString(MSR_DATA_STRING_TAG);
		Logger.D(TAG, "MSR decoded data: " + cardData);
	
		//Broadcast the bundle
		Intent intent = new Intent();
		intent.setAction(MSR_SWYPE_ACTION);
		intent.putExtras(b);
		sendBroadcast(intent);

	}

    /**
     * @see android.app.IntentService#onHandleIntent(android.content.Intent)
     * The intent contains the MSR primitive and required arguments
     * The intent extras are:
     * - METHOD
     * - ARGS
     */
	  public class MSRRequestRebranded
	    {	
	    	public static final int Cmd_Open = 0;
	    	public static final int Cmd_Close = 1;
	    }
	    
    @Override
    protected void onHandleIntent(Intent intent)
    {
    	int res=-1;
		try
		{
		    if (!MSR.isMsrConnected())
		    {
		    	Logger.D(TAG, "CardReader is not connected");
		    //	return;			
		    }
		    int request = intent.getIntExtra(METHOD_EXTRA, -1);
		   
		    switch (request)
		    {
			    case MSRRequestRebranded.Cmd_Open:
			    { 
			    	res = enableMSR();
			    	break;
			    }
			    case MSRRequestRebranded.Cmd_Close:
			    {
			    	res = disableMSR();
			    	break;
			    }		    
			    default:
			    	return;
		    }
		    if (res < 0)
		    	Logger.D(TAG, "Request to MSR has failed with errcode: " + res);		    	
		}
		catch (NullPointerException e)
		{
			Logger.E(TAG, e.getMessage());
		}
		catch (NoClassDefFoundError e) //Handler for BSP's with no MSR support
		{
			Logger.E(TAG, e.getMessage());	
		}
		
    }
    
    /**
     * Sends the enable command to MSR
     */
    private int enableMSR()
    {   
    	Logger.D(TAG,  "enableMSR API");
		int ret = -1;
		try
		{   
			switch(ret=msr.open()){
			case MSR.EC_SUCCESS:
				isConnected = true;
				setupMSRHead();
				break;
			case MSR.EC_FAIL:
				break;
			case MSR.EC_DEV_NOT_READY:
			        flag=1;
				break;
			case MSR.EC_DEV_DETECTION_IN_PROGRESS:
				break;
		}	

 	return ret;
		}
			catch(java.lang.NoSuchMethodError e)
			{
//				Log.e(TAG, "This device needs updating with the latest MSR library");
			    return -1;
			}
    }
   
    private int disableMSR()
    {    int ret1=-1;
    	if(isConnected == true)
				{
                    try {
                        mMsr.applySetting( MSR.enableScript, MSR.DISABLE );
                    } catch (Exception e) {
                        Log.e(TAG, "Exception in closing MSR ");
                    }
                  ret1=  mMsr.close();
                  Logger.D(TAG, "Msr Closed");
                    isConnected = false;
                    return ret1;
                }
    	else
    	  Logger.D(TAG, "Msr Closing Failed");	
    	
    	  return ret1;
    }
    
    private void setupMSRHead()
	{
		try{
            mMsr.applySetting( MSR.terminatorScript, MSR.TRACK_TERMINATOR_CR_LF_MODE );
            mMsr.applySetting( MSR.trackSeparatorScript, '-' );
            mMsr.applySetting( MSR.trackSelectionScript, MSR.TRACK_ANY );
            mMsr.applySetting( MSR.enableScript, MSR.ENABLE );
		} catch (Exception e){
			Log.e("demo", "Exception in setting MSR "+e.getMessage());
		}
	}

}

/**
 * @author FPC843
 *
 */
