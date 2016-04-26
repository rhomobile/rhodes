
package com.rho.rhoelements.plugins;

import org.json.JSONArray;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;


import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;

import com.rho.rhoelements.services.MSRServiceRebranded;
import com.rho.rhoelements.services.MSRServiceRebranded.MSRRequestRebranded;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.Logger;
/**
 * @author FPC843
 * This class implements the Card Reader Plugin (MSR)
 * This class does not access the MSR API's. This is done by com.rho.rhoelements.services.MSRService
 */
public class CardReaderPlugin extends Plugin
{   private static final String TAG="xyz";
    private String mSwipeUrl = null; // contains the url to navigate to on card swipe    
    private final static String[] NAMES = {"data", "mode", "encryption", "rawData", "track1", "track2", "track3", "track1Status", "track2Status", "track3Status",
    	"track1Encrypted", "track2Encrypted", "track3Encrypted", "track1EncryptedStatus", "track2EncryptedStatus", "track3EncryptedStatus", "ksn"};    
    private boolean isReceiverRegistered = false;
    private BroadcastReceiver mResponseReceiver = null;
    private boolean autoTab = false;
    private boolean autoEnter = false;
    private boolean isOpened = false;
        
    /**
     * Constructor
     */
    public CardReaderPlugin()
    {
    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
    }
	
	/** Method called when the App gets foreground focus event */
	@Override
	public void onForeground(String url)
	{		Intent msgIntent;
		//  An Application focus change is received 
		//  Application has become active. 
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	    if (isOpened)
	    {
	    if(isRebrandedDevice())
	    {
				 msgIntent = new Intent(Common.mainActivity, MSRServiceRebranded.class);
				if (!isReceiverRegistered)
				{
				    mResponseReceiver = new MSRReceiver();
		
				    Common.mainActivity.registerReceiver(mResponseReceiver, new IntentFilter(MSRServiceRebranded.MSR_SWYPE_ACTION));
				    isReceiverRegistered = true;
				    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "CardReader receiver registered"));
				}
				msgIntent.putExtra(MSRServiceRebranded.METHOD_EXTRA, MSRRequestRebranded.Cmd_Open);   
				Common.mainActivity.startService(msgIntent);   
	    }
	    
	    }
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	}
	
	/** Method called when the App gets Background focus event */
	@Override
	public void onBackground(String url)
	{		Intent msgIntent;
		//  An Application focus change is received 
		//  Application has gone into the background.					
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));		
	   
		//Close MSR
		if (isOpened)
		{   
			 if(isRebrandedDevice())
			{
				     msgIntent = new Intent(Common.mainActivity, MSRServiceRebranded.class);
				    msgIntent.putExtra(MSRServiceRebranded.METHOD_EXTRA, MSRRequestRebranded.Cmd_Close);
				    Common.mainActivity.startService(msgIntent);
				    try
				    {
					Common.mainActivity.unregisterReceiver(mResponseReceiver);
					isReceiverRegistered = false;
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "CardReader receiver unregistered"));
				    }
				    catch (IllegalArgumentException e)
				    {
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
				    }
				}
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "END"));
	}
	
    /** 
     * @see com.rho.rhoelements.plugins.Plugin#onSetting(com.rho.rhoelements.PluginSetting)
     */
    @Override
    public void onSetting(PluginSetting setting)
    {
    	Intent msgIntent =null;
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	
	if (setting == null)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "setting is null"));
		return;
	}
	try
	{   
		if(isRebrandedDevice())
	     msgIntent = new Intent(Common.mainActivity, MSRServiceRebranded.class);
		
		
	    if (setting.getName().equalsIgnoreCase("open"))
	    {
		isOpened = true; 
        	if (Common.elementsCore.bLaunchingAppHasFocus)
        	{
	       	    if (!isReceiverRegistered)
	       	    {
	       		  mResponseReceiver = new MSRReceiver();
	       		  if(isRebrandedDevice())  
	       		  {
	       		Common.mainActivity.registerReceiver(mResponseReceiver, new IntentFilter(MSRServiceRebranded.MSR_SWYPE_ACTION));
	       		  }  
	       		
	       		isReceiverRegistered = true;
	       		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "CardReader receiver registered"));
	      	    }
	       	    if(isRebrandedDevice())
	       	    msgIntent.putExtra(MSRServiceRebranded.METHOD_EXTRA, MSRRequestRebranded.Cmd_Open); 
	       	   
        	}
	    }
	    else if (setting.getName().equalsIgnoreCase("close"))
	    {
		isOpened = false;  
        	if (Common.elementsCore.bLaunchingAppHasFocus)
        	{
	       	    if (isReceiverRegistered)
	       	    {
	       		Common.mainActivity.unregisterReceiver(mResponseReceiver);
	       		mResponseReceiver = null;
	       		isReceiverRegistered = false;
	      	    }   
	       	    if(isRebrandedDevice())
	        	    msgIntent.putExtra(MSRServiceRebranded.METHOD_EXTRA, MSRRequestRebranded.Cmd_Close);  
	       	   
        	}        		
	    }
	    else if (setting.getName().equalsIgnoreCase("readevent"))
	    {
		onSetting(new PluginSetting(null, "open", null));
		mSwipeUrl = setting.getValue();
	    }
	    else if (setting.getName().equalsIgnoreCase("autoenter"))
	    {
		if (setting.getValue().equalsIgnoreCase("enabled"))
		    autoEnter = true;
		else if (setting.getValue().equalsIgnoreCase("disabled"))
		    autoEnter = false;
	    }
	    else if (setting.getName().equalsIgnoreCase("autotab"))
	    {
		if (setting.getValue().equalsIgnoreCase("enabled"))
		    autoTab = true;
		else if (setting.getValue().equalsIgnoreCase("disabled"))
		    autoTab = false;
	    }
	    else if ( (setting.getName().equalsIgnoreCase("pinTimeout")) ||
        	  (setting.getName().equalsIgnoreCase("pinEntry")) ||
        	  (setting.getName().equalsIgnoreCase("panData")) ||
        	  (setting.getName().equalsIgnoreCase("moduleName")) )
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, setting.getName() +  " is not supported"));
	    }        	
        	
	    if (Common.elementsCore.bLaunchingAppHasFocus)
	    {
        	Common.mainActivity.startService(msgIntent);
	    }
	}
	catch (NoClassDefFoundError e) //Handler for BSP's with no MSR support
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	}
    }

    public static boolean isRebrandedDevice()
	{
	    boolean res = false;
	    try
	    {
		   
	    	Class.forName("com.symbol.msr.MSR", false,RhodesActivity.safeGetInstance().getClassLoader());
	    	res = true;
	    } 
	    catch (ClassNotFoundException e)
	    {   
	    	res=false;
	    	
		}
	    return res;
	}
    
    
    /**
     * @see com.rho.rhoelements.plugins.Plugin#onShutdown()
     */
    @Override
    public void onShutdown()
    {
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
	reset();
    }

    /**
     * @see com.rho.rhoelements.plugins.Plugin#onPageStarted(java.lang.String)
     */
    @Override
    public void onPageStarted(String url)
    {
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
	reset();
    }
    
    /**
     * Resets to the default values of the plugin
     */
    private void reset()
    {
	Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "START"));
	try
	{
	    isReceiverRegistered = false;
	    autoEnter = false;
	    autoTab = false;
	    mSwipeUrl = null;
	    //Close MSR
	    if(isRebrandedDevice())
	    {
	    Intent msgIntent = new Intent(Common.mainActivity, MSRServiceRebranded.class);
	    msgIntent.putExtra(MSRServiceRebranded.METHOD_EXTRA, MSRRequestRebranded.Cmd_Close);
	    Common.mainActivity.startService(msgIntent);
	    }
	    
	   
	    Common.mainActivity.unregisterReceiver(mResponseReceiver);
	}
	catch (IllegalArgumentException e)
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Receiver was not registered"));
	}
	catch (NoClassDefFoundError e) //Handler for BSP's with no MSR support
	{
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
	}
    }
    
    /**
     * 
     * @author FPC843
     * Service class which handles the swipe events coming from the MSR service.
     * The data coming from the MSR device are either converted into key strokes or sent as JSON data.
     */
    private class MSRReceiver extends BroadcastReceiver
    {
		 /*public String bytesToHex(byte[] bytes) 
		 {
		    final char[] hexArray = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
		    char[] hexChars = new char[bytes.length * 2];
		    int v;
		    for ( int j = 0; j < bytes.length; j++ ) {
		        v = bytes[j] & 0xFF;
		        hexChars[j * 2] = hexArray[v >>> 4];
		        hexChars[j * 2 + 1] = hexArray[v & 0x0F];			        
		    }
		    return new String(hexChars);
		 }*/
		 
    	// Convert Byte array to HEX format
		public String convert(byte [] arr) 
		{				
			int length = arr.length;
			String str = "";
			String hexStr ="";
			for (int i=0; i < length; ++i ) {
				
				hexStr = Integer.toHexString(arr[i] & 0xff);
				
				if(hexStr.length()<2)
					hexStr="0"+hexStr;
				
				str += hexStr;// + " ";
			}
			
			return str.toUpperCase();
		}
		 
		@Override
		 public void onReceive(Context context, Intent intent)
		 {  
		     Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, this.toString()));
		      if (intent.getAction().equalsIgnoreCase(MSRServiceRebranded.MSR_SWYPE_ACTION))
		     {  
		    	 // UnEncrypted data
				 String cardData = "";			 
				 String sUnEncryptedTrack1 = "";
				 String sUnEncryptedTrack2 = "";
				 String sUnEncryptedTrack3 = "";
				 String sUnEncryptedTrack1Status = "false";
				 String sUnEncryptedTrack2Status = "false";
				 String sUnEncryptedTrack3Status = "false";
				 
				 // Encrypted data	
				 String rawData = "";
				 String ksn = "";	
				 String sEncryptedTrack1 = "";
				 String sEncryptedTrack2 = "";
				 String sEncryptedTrack3 = "";
				 String sEncryptedTrack1Status = "false";
				 String sEncryptedTrack2Status = "false";
				 String sEncryptedTrack3Status = "false";
				 
				 //Previous method
				 cardData = intent.getStringExtra(MSRServiceRebranded.MSR_DATA_STRING_TAG);
				 
				 byte [] baData = intent.getByteArrayExtra(MSRServiceRebranded.MSR_DATA_TAG); 
				 if (baData != null && baData.length > 0) { 
					 rawData = convert(baData); 
				 } 
				 
				 byte [] baUT1 = intent.getByteArrayExtra(MSRServiceRebranded.MSR_TRACK1_TAG); 
				 if (baUT1 != null && baUT1.length > 0) { 
				        sUnEncryptedTrack1 = convert(baUT1); 
				 } 
				
				 byte [] baUT2 = intent.getByteArrayExtra(MSRServiceRebranded.MSR_TRACK2_TAG); 
				 if (baUT2 != null && baUT2.length > 0) { 
				       sUnEncryptedTrack2 = convert(baUT2); 
				 } 
				
				 byte [] baUT3 = intent.getByteArrayExtra(MSRServiceRebranded.MSR_TRACK3_TAG); 
				 if (baUT3 != null && baUT3.length > 0) { 
				       sUnEncryptedTrack3 = convert(baUT3);                          
				 }
				 
				 int track1US = intent.getIntExtra(MSRServiceRebranded.MSR_TRACK1_STATUS_TAG, -1);
				 if (track1US == 1) { 
					 sUnEncryptedTrack1Status = "true"; 
				 } 
				
				 int track2US = intent.getIntExtra(MSRServiceRebranded.MSR_TRACK2_STATUS_TAG, -1);
				 if (track2US == 1) { 
					 sUnEncryptedTrack2Status = "true"; 
				 } 
				 
				 int track3US = intent.getIntExtra(MSRServiceRebranded.MSR_TRACK3_STATUS_TAG, -1);
				 if (track3US == 1) { 
					 sUnEncryptedTrack3Status = "true"; 
				 } 
				 
				 byte [] baT1 = intent.getByteArrayExtra(MSRServiceRebranded.MSR_TRACK1_ENCRYPTED_TAG); 
				 if (baT1 != null && baT1.length > 0) { 
				        sEncryptedTrack1 = convert(baT1); 
				 } 
				
				 byte [] baT2 = intent.getByteArrayExtra(MSRServiceRebranded.MSR_TRACK2_ENCRYPTED_TAG); 
				 if (baT2 != null && baT2.length > 0) { 
				       sEncryptedTrack2 = convert(baT2); 
				 } 
				
				 byte [] baT3 = intent.getByteArrayExtra(MSRServiceRebranded.MSR_TRACK3_ENCRYPTED_TAG); 
				 if (baT3 != null && baT3.length > 0) { 
				       sEncryptedTrack3 = convert(baT3);                          
				 } 
				 
				 int track1S = intent.getIntExtra(MSRServiceRebranded.MSR_TRACK1_ENCRYPTED_STATUS_TAG, -1);
				 if (track1S == 1) { 
				        sEncryptedTrack1Status = "true"; 
				 } 
				
				 int track2S = intent.getIntExtra(MSRServiceRebranded.MSR_TRACK2_ENCRYPTED_STATUS_TAG, -1);
				 if (track2S == 1) { 
					 sEncryptedTrack2Status = "true"; 
				 } 
				 
				 int track3S = intent.getIntExtra(MSRServiceRebranded.MSR_TRACK3_ENCRYPTED_STATUS_TAG, -1);
				 if (track3S == 1) { 
					 sEncryptedTrack3Status = "true"; 
				 } 
				 
				 byte [] baKsn = intent.getByteArrayExtra(MSRServiceRebranded.MSR_KSN_NUMBER_TAG); 
				 if (baKsn != null && baKsn.length > 0) { 
					 ksn = convert(baKsn); 
				 } 
			        
				 if (autoEnter)
				     cardData += '\n';
				 if ( (autoTab) && (!autoEnter) )
				     cardData += '\t';
				 if (cardData != null)
				 {
			            if ( (mSwipeUrl == null) || (mSwipeUrl.length() == 0) )
			        	//Generate key events
			        	Common.injectKeyEvents(cardData);
			            else
			            {
			        	try
			        	{
			        		JSONArray values = new JSONArray();
			        		values.put(cardData);
			       			values.put("CR");
			       			values.put(MSRServiceRebranded.encryption);
			       			values.put(rawData);
			       			values.put(sUnEncryptedTrack1);
			       			values.put(sUnEncryptedTrack2);
			       			values.put(sUnEncryptedTrack3);
			       			values.put(sUnEncryptedTrack1Status);
			       			values.put(sUnEncryptedTrack2Status);
			       			values.put(sUnEncryptedTrack3Status);
			       			values.put(sEncryptedTrack1);
			       			values.put(sEncryptedTrack2);
			       			values.put(sEncryptedTrack3);
			       			values.put(sEncryptedTrack1Status);
			       			values.put(sEncryptedTrack2Status);
			       			values.put(sEncryptedTrack3Status);
			       			values.put(ksn);	
			       			navigate(mSwipeUrl, NAMES, values);
			        	} 
			        	catch (NavigateException e1)
			        	{
			    			e1.printStackTrace();
			        	}
			            }
			        }
		     
		     }
		 	}
	 	}
    }
    
  


