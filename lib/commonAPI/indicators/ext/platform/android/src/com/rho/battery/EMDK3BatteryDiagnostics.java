package com.rho.battery;

import java.util.HashMap;

import com.symbol.emdk.EMDKManager;
import com.symbol.emdk.EMDKManager.FEATURE_TYPE;
import com.symbol.emdk.personalshopper.PersonalShopper;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.symbol.emdk.personalshopper.DiagnosticConfig;
import com.symbol.emdk.personalshopper.DiagnosticData;
import com.symbol.emdk.personalshopper.DiagnosticException;
import com.symbol.emdk.personalshopper.DiagnosticParamId;


public class EMDK3BatteryDiagnostics {
	
	protected static String TAG = "EMDK3BatryDiagnostics";
	
	private EMDK3ExtensionListener emdk3Listener=null;
	private EMDKManager emdkManager = null;
	private PersonalShopper personalShopper = null;
	DiagnosticData diagnosticData =null;
	private final int NO_OF_TRIES=3;
	
	private int averageCurrentConsumption=0;
	private int tripDuration=0;
	private IMethodResult result=null;
	
	public void setMethodResult(IMethodResult res)
	{
		this.result=res;
	}
	
	public void createEmdk3Instance()
	{
		emdk3Listener= new EMDK3ExtensionListener();
        if(emdk3Listener!=null)
        {
        	int count=1;
        	while(count<=NO_OF_TRIES)
        	{
        		emdkManager=emdk3Listener.getEMDKManager();
        		if(emdkManager!=null) 
        			break;
        		else
        		{
        			try {
						Thread.currentThread().sleep(100);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
        			count++;
        		}
        	}
        }
        
        createCradleManerInstance();
        fireDiagnosticData(result);
        Logger.I(TAG, "EMDK MANGER="+emdkManager);
	}
	private void createCradleManerInstance()
    {
    	if(emdkManager!=null)
    	{
    		personalShopper=(PersonalShopper)emdkManager.getInstance(FEATURE_TYPE.PERSONALSHOPPER);
    	}
    	if(personalShopper!=null)
    	{
    		try {
    			DiagnosticParamId diagnosticparamID = new DiagnosticParamId();      
    			 diagnosticData = personalShopper.diagnostic.getDiagnosticData(diagnosticparamID.ALL,new  DiagnosticConfig(averageCurrentConsumption,tripDuration));

    			}catch (DiagnosticException e) {
    			       e.printStackTrace();
    			}
    		

    	}
    	
    }
	private void fireDiagnosticData(IMethodResult result)
	{
		if(result == null)
		{
			Logger.D(TAG, "EMDK fireDiagnosticData result ="+result);
		}
		else
		{
			if(diagnosticData!=null)
			{
				HashMap<String,Object> resultMap=new HashMap<String,Object>();
				
				resultMap.put(IBatterySingleton.HK_STATE_OF_HEALTH_PERCENT, diagnosticData.batteryStateOfHealth);
				resultMap.put(IBatterySingleton.HK_BATTERY_CAPACITY_PERCENT, diagnosticData.batteryStateOfCharge);
				resultMap.put(IBatterySingleton.HK_BATTERY_CAPACITY_MINUTES, diagnosticData.batteryTimeToEmpty);
				resultMap.put(IBatterySingleton.HK_BATTERY_EXPIRATION_IN_MONTHS, "undefined");
				resultMap.put(IBatterySingleton.HK_PREVIOUS_BATTERY_REPLACEMENT, diagnosticData.timeSinceBatteryReplaced);
				resultMap.put(IBatterySingleton.HK_TIME_SINCE_LAST_COLD_BOOT, diagnosticData.timeSinceReboot);
				resultMap.put(IBatterySingleton.HK_REQUIRED_CHARGE_TIME, diagnosticData.batteryChargingTime);
				resultMap.put(IBatterySingleton.HK_CHARGING_TIME, diagnosticData.batteryChargingTimeElapsed);
				
				result.set(resultMap);
			}
			else
				result.setError("Diagnostic Data is NOT collected");
		}
		
	}
	
	public void  setAverageCurrentConsumption(int avgCurConsumption)
	{
		this.averageCurrentConsumption=avgCurConsumption;
	}
	
	public void  setTripDuration(int tripDur)
	{
		this.tripDuration=tripDur;
	}
	

}
