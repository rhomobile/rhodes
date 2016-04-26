package com.rho.battery;

import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.os.Build;

import com.rho.battery.IBatterySingleton;
import com.rho.indicators.IndicatorView;
import com.rho.indicators.IndicatorView.Layout;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.util.ContextFactory;
import com.symbol.emdk.EMDKManager;

/**
 * The singleton class for the battery indicator.
 * @author Ben Kennedy (NCVT73)
 */
public class BatterySingleton extends BroadcastReceiver implements IBatterySingleton
{
	protected static String TAG = "BatterySingleton";
	private int batteryLevel = 0;
	private boolean isPowerConnected;
	private MethodResult statusResult;
	private boolean isReceiverRegistered;
	private BatteryView batteryView;
	private int top = 0; //Default position of icon
	private int left = 0; //Default position of icon
	private Layout layout = Layout.RIGHT; //Default
	private int color = Color.BLACK; //Default
	
	private final int NO_OF_TRIES=3;
	
	private EMDK3ExtensionListener emdk3Listener=null;
	private EMDKManager emdkManager = null;
	private int averageCurrentConsumption=0;
	private int tripDuration=0;
	private EMDK3BatteryDiagnostics emdk3BatteryDiagInstance=null;
	
	public BatterySingleton(BatteryFactory factory)
	{
		super();
		Logger.D(TAG, "+");
		isReceiverRegistered = false;
		Logger.D(TAG, "-");
	}
	  

	@Override
	public void getRefreshInterval(IMethodResult result)
	{
		Logger.D(TAG, "getRefreshInterval");
		result.setError("refreshInterval is not supported on Android");
	}

	@Override
	public void setRefreshInterval(int refreshInterval, IMethodResult result)
	{
		Logger.D(TAG, "setRefreshInterval");
		result.setError("refreshInterval is not supported on Android");
	}

	@Override
	public void batteryStatus(Map<String, Object> propertyMap, IMethodResult result)
	{
		Logger.D(TAG, "batteryStatus+");
		if(isCallback(result))
		{	
			((MethodResult) result).keepAlive();
			stopBatteryStatus(null);
			statusResult = ((MethodResult) result);
		
			registerForBatteryStatus();
			
			String trigger = (String) propertyMap.get(HK_TRIGGER);
			if(!trigger.equalsIgnoreCase(BATTERY_TRIGGER_SYSTEM))
			{
				//result.setError("Only " + BATTERY_TRIGGER_SYSTEM + " is supported on this platform. Setting as " + BATTERY_TRIGGER_SYSTEM);
			}
		}
		else
		{
			forceGetBatteryInfo(result);
		}
		
		Logger.D(TAG, "batteryStatus-");
	}

	/**
	 * Registers this class from listening to battery status if it is not already registered
	 * @author Ben Kennedy (NCVT73)
	 */
	private void registerForBatteryStatus()
	{
		if(!isReceiverRegistered)
		{
			Logger.D(TAG, "registerForBatteryStatus async");
			Activity activity = RhodesActivity.safeGetInstance();
			if(activity != null)
			{
				activity.registerReceiver(this, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
				activity.registerReceiver(this, new IntentFilter(Intent.ACTION_BATTERY_LOW));
				isReceiverRegistered = true;
			}
			else Logger.E(TAG, "Could not communicate with main Activity");
		}
	}
	
	/**
	 * Unregisters this class from listening to battery status, only if the indicator is invisible and
	 * the batteryStatus is stopped
	 * @author Ben Kennedy (NCVT73)
	 */
	private void unregisterForBatteryStatus()
	{
		if(isReceiverRegistered && statusResult == null && batteryView != null && !batteryView.isShown())
		{
			Activity activity = RhodesActivity.safeGetInstance();
			if(activity != null)
			{
				activity.unregisterReceiver(this);
				isReceiverRegistered = false;
			}
		}
	}

	@Override
	public void stopBatteryStatus(IMethodResult result)
	{
		Logger.D(TAG, "stopBatteryStatus+");
		if(statusResult != null)
		{
			statusResult.release();
			statusResult = null;
		}
		unregisterForBatteryStatus();
		Logger.D(TAG, "stopBatteryStatus-");
	}

	@Override
	public void smartBatteryStatus(IMethodResult result)
	{
		Logger.D(TAG, "smartBatteryStatus");
		result.setError("smartBatteryStatus is not supported on Android");
	}

	@Override
	public void showIcon(Map<String, Object> propertyMap, IMethodResult result)
	{
		Logger.D(TAG, "showIcon+");
		
		// Parse and set all parameters if they are correct.
		// If the parameters are invalid, then ignore and use the currently set parameters/
		Object newTop = propertyMap.get(HK_TOP);
		Object newLeft = propertyMap.get(HK_LEFT);
		Object newLayout = propertyMap.get(HK_LAYOUT);
		Object newColor = propertyMap.get(HK_COLOR);
		
		if(newTop != null)
		{
			try	{	top = IndicatorView.parseDimension(newTop);}
			catch(IllegalArgumentException e) {	Logger.E(TAG, e.getMessage());}
		}
		
		if(newLeft != null)
		{
			try	{	left = IndicatorView.parseDimension(newLeft);}
			catch(IllegalArgumentException e) {	Logger.E(TAG, e.getMessage());}
		}
		
		if(newLayout != null)
		{
			try	{	layout = IndicatorView.parseLayout(newLayout);}
			catch(IllegalArgumentException e) {	Logger.E(TAG, e.getMessage());}
		}
		
		if(newColor != null)
		{
			try	{	color = IndicatorView.parseColor(newColor);}
			catch(IllegalArgumentException e) {	Logger.E(TAG, e.getMessage());}
		}
		// End parsing the parameters
		
		if(batteryLevel <= 0)
		{
			Activity activity = RhodesActivity.safeGetInstance();
			if(activity != null)
			{
				Logger.D(TAG, "Getting new battery level");
				Intent batteryIntent = activity.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
				batteryLevel = batteryIntent.getIntExtra("level", 0);
				Logger.D(TAG, "New battery level: " + batteryLevel);
			}
		}
	
		if(batteryView == null)
		{
			batteryView = new BatteryView(ContextFactory.getUiContext());
			batteryView.setupView(top, left, layout, color);
			registerForBatteryStatus();
		}
		else
		{
			batteryView.updateParams(top, left, layout, color);
		}

		batteryView.show();
		updateIndicator();
		Logger.D(TAG, "showIcon-");
	}


	@Override
	public void hideIcon(IMethodResult result)
	{
		Logger.D(TAG, "hideIcon+");
		if(batteryView != null)
		{
			batteryView.hide();
			unregisterForBatteryStatus();
		}
		else
		{
			Logger.W(TAG, "Erroneous hideIcon call. Battery indicator not visible");
		}
		Logger.D(TAG, "hideIcon-");
	}

	@Override
	public void onReceive(Context context, Intent intent)
	{
		Logger.D(TAG, "onReceive" + intent.getAction() + "+");
		String trigger = "Changed";
		boolean doRefresh = false;
		boolean isPowerConnected = (intent.getIntExtra("plugged", 0) == 0 ? false : true);
		int batteryLevel = intent.getIntExtra("level", 0);
			
		if(intent.getAction().equals(Intent.ACTION_BATTERY_LOW))
		{
			trigger="Low";
			doRefresh = true;
			if(batteryLevel != 0) this.batteryLevel = batteryLevel;
			
		}
		else
		{
			if( (this.isPowerConnected != isPowerConnected) || (this.batteryLevel != batteryLevel) )
			{
				doRefresh = true;
 				trigger = (isPowerConnected ? "Charging" : "Discharging");
 				this.isPowerConnected = isPowerConnected;
 				this.batteryLevel = batteryLevel;
			}
			
		}
		
		if(doRefresh)
		{
			updateIndicator();
			fireBatteryStatus(trigger);
		}
		
		Logger.D(TAG, "onReceive-");
	}
	
	/**
	 * Updates and redraws the battery indicator if it is visible.
	 * @author Ben Kennedy (NCVT73)
	 */
	private void updateIndicator()
	{
		Logger.D(TAG, "updateIndicator+");
		if(batteryView != null)
		{
			batteryView.setPowerState(batteryLevel, isPowerConnected);
		}
		Logger.D(TAG, "updateIndicator-");
	}
	
	/**
	 * Forces a read of the Battery Sticky Intent and sends the data to the passed in MethodResult
	 * @param result the MethodResult to send the data to.
	 * @author Ben Kennedy (NCVT73)
	 */
	private void forceGetBatteryInfo(IMethodResult result)
	{
		Activity activity = RhodesActivity.safeGetInstance();
		if(activity != null)
		{
			Intent batteryIntent = activity.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
			boolean isPlugged = (batteryIntent.getIntExtra("plugged", 0) == 1 ? true : false);
			int level = batteryIntent.getIntExtra("level", 0);
			batteryLevel = level;
			fireBatteryStatus(level, isPlugged, "User Request", result);

		}
		else
		{
			Logger.E(TAG, "Could not communicate with main Activity");
			result.setError("Internal Error. Could not communicate with Activity");
		}
	}
	
	/**
	 * Sends a batteryStatus callback with battery information if it has been registered.
	 * @param trigger how this was called
	 * @author Ben Kennedy (NCVT73)
	 */
	private void fireBatteryStatus(String trigger)
	{
		fireBatteryStatus(batteryLevel, isPowerConnected, trigger, null);
	}
	
	/**
	 * Sends a batteryStatus callback with battery information if it has been registered.
	 * @param trigger how this was called
	 * @param result the method result to which to send the battery data. If null, uses the statusResult in its place
	 * @param isPlugged if the device is plugged into AC power 
	 * @param level The level of the battery
	 * @author Ben Kennedy (NCVT73)
	 */
	private void fireBatteryStatus(int level, boolean isPlugged, String trigger, IMethodResult result)
	{
		Logger.D(TAG, "fireBatteryStatus+");
		if(result == null) result = statusResult;
		if(result != null)
		{
			Map<String, Object> resultMap = new HashMap<String, Object>();
			resultMap.put(HK_AC_LINE_STATUS, isPlugged);
			resultMap.put(HK_BATTERY_LIFE_PERCENT, level);
			resultMap.put(HK_TRIGGER, trigger);
			result.set(resultMap);
		}
		Logger.D(TAG, "fireBatteryStatus-");
	}
	
	/**
	 * Checks whether the result has a callback (true) or is empty (false)
	 * @param result the IMethodResult to query
	 * @return true is a callback exists, false if no callback exists
	 * @author Ben Kennedy (NCVT73)
	 */
	private boolean isCallback(IMethodResult result)
	{
		return (result != null && result.hasCallback());
	}
	
	@Override
	protected void finalize() throws Throwable
	{
		Logger.D(TAG, "finalize+");
		stopBatteryStatus(null);
		Logger.D(TAG, "finalize-");
		super.finalize();
	}

	@Override
	public void getTripDuration(IMethodResult result)
	{
		if(Build.MODEL.equalsIgnoreCase("mc18n0"))
		{
			result.set(this.tripDuration);
		}
		else
		result.setError("Not supported on this device");
	}

	@Override
	public void setTripDuration(int tripDuration, IMethodResult result)
	{
		if(Build.MODEL.equalsIgnoreCase("mc18n0"))
		{       if(tripDuration>=0)
		        {
					this.tripDuration=tripDuration;
					if(this.emdk3BatteryDiagInstance ==null)
			 		{
			 			emdk3BatteryDiagInstance=new EMDK3BatteryDiagnostics();
			 		}
			 		this.emdk3BatteryDiagInstance.setTripDuration(tripDuration);
		        }
		}
		else
		result.setError("Not supported on this device");
	}

	@Override
	public void getAverageCurrentConsumption(IMethodResult result)
	{
		if(Build.MODEL.equalsIgnoreCase("mc18n0"))
		{    
			result.set(this.averageCurrentConsumption);
		}
		else
		result.setError("Not supported on this device");
	}

	@Override
	public void setAverageCurrentConsumption(int averageCurrentConsumption, IMethodResult result)
	{
		//System.out.println(" BatetryLog setAverageCurrentConsumption");
		if(Build.MODEL.equalsIgnoreCase("mc18n0"))
		{        if(averageCurrentConsumption>=0)
		         {
			 		this.averageCurrentConsumption=averageCurrentConsumption;
			 		if(this.emdk3BatteryDiagInstance ==null)
			 		{
			 			emdk3BatteryDiagInstance=new EMDK3BatteryDiagnostics();
			 		}
			 		this.emdk3BatteryDiagInstance.setAverageCurrentConsumption(averageCurrentConsumption);
		         }
		}
		else
		result.setError("Not supported on this device");
	}

	@Override
	public void batteryDiagnostics(IMethodResult result)
	{
		//For Battery Diagnostics only for MC18 Android device
		
		if(Build.MODEL.equalsIgnoreCase("mc18n0"))
		{
				if (this.emdk3BatteryDiagInstance==null)
				{
					this.emdk3BatteryDiagInstance=new EMDK3BatteryDiagnostics();
								
				}
				this.emdk3BatteryDiagInstance.setMethodResult(result);
				this.emdk3BatteryDiagInstance.createEmdk3Instance();
							
		}
		else
		{
			result.setError("Not supported on this device");
		}
	}

}
