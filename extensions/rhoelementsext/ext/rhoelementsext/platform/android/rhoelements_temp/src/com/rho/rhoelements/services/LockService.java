package com.rho.rhoelements.services;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;

import android.app.Activity;
import android.app.KeyguardManager;
import android.app.KeyguardManager.KeyguardLock;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.IBinder;
import android.os.PowerManager;


public class LockService extends Service {

	//TODO: Wake Lock & Wifi Lock are not reliable and we should consider using Window Flags instead
	
	// Wake lock and wifi lock
	private WifiManager wifiManager;  
	private PowerManager powerManager;
	//private KeyguardManager keyguardManager;
	
	private PowerManager.WakeLock wakeLock;
	private WifiManager.WifiLock wifiLock;	
	//private KeyguardLock keyLock;
		
	static private LockService sInstance;
	static public LockService getInstance()
	{
		return sInstance;
	}
	// This is the object that receives interactions from clients.
	private final IBinder mBinder = new LockBinder();
	
	/**
	 * Class for clients to access.  Because we know this service always
	 * runs in the same process as its clients, we don't need to deal with
	 * IPC.
	 */
	public class LockBinder extends Binder
	{
		public LockService getService()
		{
			return LockService.this;
		}
	}
	

	@Override
	public IBinder onBind(Intent intent)
	{
		return mBinder;
	}

	@Override
	public boolean onUnbind(Intent intent)
	{		
		return false;
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
	    // We don't want this service to continue running if it is explicitly
	    // stopped, so return not sticky.
	    return START_NOT_STICKY;
	}
	
	@Override
	public void onCreate() {
		try
		{
			// Keep the RhoElements window active and not fall into standby mode
			
			// Option1: Set the window flag 
			// This keeps the screen active and also the WiFi ON
			// Common.mainActivity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);  
			
			// Option2: Acquire the Wakelock & wifilock
			// This keeps the screen active and also the WiFi ON
			wifiManager  = (WifiManager)  Common.mainActivity.getSystemService (Context.WIFI_SERVICE);
	        powerManager = (PowerManager) Common.mainActivity.getSystemService (Context.POWER_SERVICE);
	        //keyguardManager = (KeyguardManager) Common.mainActivity.getSystemService(Activity.KEYGUARD_SERVICE);
	        
	        // Is the flag SCREEN_DIM_WAKE_LOCK enough? or we need the FULL_WAKE_LOCK?
	        wakeLock = powerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK | 
	        		PowerManager.ACQUIRE_CAUSES_WAKEUP, "RhoElementsWakeLock");
	        wifiLock = wifiManager.createWifiLock(WifiManager.WIFI_MODE_FULL, "RhoElementsWiFiLock");        
			//keyLock = keyguardManager.newKeyguardLock(KEYGUARD_SERVICE);
	        
	        sInstance = this;
		}
		catch(Exception e)
		{
			//Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unable to create lock service")); //This may cause an exception, possibly because the logger is not created
		    e.printStackTrace();
		}		
	}

	@Override
	public void onDestroy() {		
		super.onDestroy();
	}
	
	/**
	 * Acquires/Releases the WakeLock
	 * 
	 * @param wakeLockEnabled
	 *            whether the wakeLock is enabled.	
	 */
	public void setWakeLockEnabled(boolean wakeLockEnabled) throws Exception
	{
		try
		{
			// Acquire the wakeLock 
			if (wakeLockEnabled)
			{
				wakeLock.acquire();	
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Acquired wake lock"));
			}
			// Release the wakeLock
			else
			{			
				if (wakeLock.isHeld()) 
				{					
					wakeLock.release();		
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Released wake lock"));
				}			
			}
		}
		catch (Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unable to set wake lock"));				
		}
	}
	
	/**
	 * Acquires/Releases the Wifi Lock
	 * 
	 * @param wifiLockEnabled
	 *            whether the wifiLock is enabled.
	 * @throws Exception 	
	 */
	public void setWifiLockEnabled(boolean wifiLockEnabled) throws Exception
	{		
		// Acquire the wifiLock 
		try
		{
			if (wifiLockEnabled)
			{				
				wifiLock.acquire();	
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Acquired wifi lock"));
			}
			// Release the wifiLock
			else
			{			
				if (wifiLock.isHeld()) 
				{					
					wifiLock.release();	
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Released wifi lock"));
				}	
			}
		}
		catch (Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unable to set wifi lock"));						
		}
	}
	
	/*public void setKeyguardLockEnabled(boolean bOn) throws Exception
	{
		// Switch ON
		try
		{
			if (bOn)
			{				
				keyLock.reenableKeyguard();	
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Re enabled the lock screen"));
			}
			// Switch Off
			else
			{					
				keyLock.disableKeyguard();	
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Disabled the lock screen"));
				//NOTE: This will be re enabled only in Shutdown()				
			}	
		}
		catch (Exception e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Unable to set keyguard lock"));							
		}
	}*/		
}
