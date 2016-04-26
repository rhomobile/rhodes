package com.rho.smartcradle;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import android.util.Log;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.symbol.emdk.EMDKManager;
import com.symbol.emdk.EMDKManager.FEATURE_TYPE;
import com.symbol.emdk.personalshopper.CradleConfig.CradleLocation;
import com.symbol.emdk.personalshopper.PersonalShopper;
import com.symbol.emdk.personalshopper.CradleException;
import com.symbol.emdk.personalshopper.CradleInfo;
import com.symbol.emdk.personalshopper.CradleLedFlashInfo;


class SmartCradleSingleton implements ISmartCradleSingleton {
	
	private final String TAG="SmartCradleSingleton";
	private final String INVALID_VALUE="Invalid Value";
	private final String NOT_SUPPORTED="Not Supported";
	private final String ERROR_READ_ONLY="Read Only Property";
	private final int NO_OF_TRIES=3;
	
	
	private String _chargeState=ISmartCradleSingleton.SMART_CRADLE_SLOW;
	
	private int _rowId=0;
	private int _columnId=0;
	private int _wallId=0;
	
	
	private String _serialNo="";
	private String _partNo="INVALID";
	private String _mfgDate="";
	private String _hardWareId="";
	private String _firmWareVersion="";
	
	private EMDK3ExtensionListener emdk3Listener=null;
	private EMDKManager emdkManager = null;
	//private CradleManager cradleManager = null;
	private PersonalShopper personalShopper = null;
	//private MC18Cradle mc18Cradle=null;
	
	
    public SmartCradleSingleton(SmartCradleFactory factory) {
        //super(factory);
    }
    public SmartCradleSingleton() {
    	
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
        Logger.I(TAG, "EMDK MANGER="+emdkManager);
       
    }

    private void createCradleManerInstance()
    {
    	if(emdkManager!=null)
    	{
    		personalShopper=(PersonalShopper)emdkManager.getInstance(FEATURE_TYPE.PERSONALSHOPPER);
    	}
    	
    	if(personalShopper!=null )
    	{
    		try {
				personalShopper.cradle.enable();
			} catch (CradleException e) {
				
				Logger.E(TAG,"CradleException in getCradleInstance()="+e.getMessage());
				e.printStackTrace();
			}
    		try {
				if(personalShopper.cradle.isEnabled())
					populateCradleProperties();
			} catch (CradleException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    }
    
    private void populateCradleProperties()
    {
    	// h/w ReadOnly properties....
    	CradleInfo cradleInfo=null;
    	
		try {
			cradleInfo = personalShopper.cradle.getCradleInfo();
		} catch (CradleException  e) {
			Logger.E(TAG,"CradleException in getCradleInfo()="+e.getMessage());
			e.printStackTrace();
		}
		
    	if(cradleInfo!=null)
    	{
    		_firmWareVersion=cradleInfo.getFirmwareVersion();
    		_hardWareId=cradleInfo.getHardwareID();
    		_mfgDate=cradleInfo.getDateOfManufacture();
    		_serialNo=cradleInfo.getSerialNumber();
    		
		//workaround for single slot cradle issue	
			for( int i=1;i < cradleInfo.getPartNumber().length();i++)
			{  
          		char ch1=_partNo.charAt(i);
				if (! ((ch1 == '-') || (ch1 >= 'a' && ch1 <= 'z') || (ch1 >= 'A' && ch1 <= 'Z') || (ch1 >= '0' && ch1 <= '9')))
				{
				_partNo=(cradleInfo.getPartNumber()).substring(0,i);
				 Log.d("SmartCradleSingleton"," Inside FAILLED" + i);
				break;
				}
                                else
				 _partNo=cradleInfo.getPartNumber();			  					
                        }    		
    		
    	}
    	
    	// Writable Properties.
    	
    	QueryForChargeState();
    	QueryForLocation();
    	
    	
    	
    }
    private void QueryForLocation()
    {
    	try {
			CradleLocation location=personalShopper.cradle.config.getLocation();
			_columnId=location.column;
			_rowId=location.row;
			_wallId=location.wall;
			
		} catch (CradleException e) {
			Logger.E(TAG,"CradleException in getLocation()="+e.getMessage());
			e.printStackTrace();
		}
    }
    
    
    
    private void QueryForChargeState()
    {
    	try {
			boolean chargeState=personalShopper.cradle.config.getFastChargingState();
			if(chargeState==true)
				_chargeState=ISmartCradleSingleton.SMART_CRADLE_FAST;
			else
				_chargeState=ISmartCradleSingleton.SMART_CRADLE_SLOW;
			
		} catch (CradleException e) {
			Logger.E(TAG,"CradleException in initializing mc18Cradle="+e.getMessage());
			e.printStackTrace();
		}
    }
    
    
    List<Object> getIDs() {
        List<Object> ids = new LinkedList<Object>();
        ids.add("SCN1");
        ids.add("SCN2");
        return ids;
    }

	@Override
	public void getChargeState(IMethodResult result) {
		
		populateCradleProperties();
		result.set(_chargeState);
		
	}

	@Override
	public void setChargeState(String chargeState, IMethodResult result) {
		
		if(chargeState.equalsIgnoreCase(ISmartCradleSingleton.SMART_CRADLE_FAST))
			{
				_chargeState=ISmartCradleSingleton.SMART_CRADLE_FAST;
				result.set(true);
			}
		else if(chargeState.equalsIgnoreCase(ISmartCradleSingleton.SMART_CRADLE_SLOW))
			{
				_chargeState=ISmartCradleSingleton.SMART_CRADLE_SLOW;
				result.set(true);
			}
		else
		{
			_chargeState=ISmartCradleSingleton.SMART_CRADLE_SLOW;
			 result.setError("INVALID_VALUE");
			 result.set(true);
		}
		setChargeStateNow();
	}

	@Override
	public void getRowId(IMethodResult result) {
		
		populateCradleProperties();
		result.set(_rowId);
		
	}

	@Override
	public void setRowId(int rowId, IMethodResult result) {
		
		if(rowId >=0)
		{
				_rowId=rowId;
				setLocationNow();
				result.set(true);
		}
		else
		{
			_rowId=0;
			result.set(INVALID_VALUE);
		}
		
	}

	@Override
	public void getColumnId(IMethodResult result) {
		
		populateCradleProperties();
		result.set(_columnId);
		
	}

	@Override
	public void setColumnId(int columnId, IMethodResult result) {
		if(columnId >=0)
		{
				_columnId=columnId;
				setLocationNow();
				result.set(true);
		}
		else
		{
			_columnId=0;
			result.set(INVALID_VALUE);
		}
	}

	@Override
	public void getWallId(IMethodResult result) {


		populateCradleProperties();
		result.set(_wallId);
		
	}

	@Override
	public void setWallId(int wallId, IMethodResult result) {
		if(wallId >=0)
		{
				_wallId=wallId;
				setLocationNow();
				result.set(true);
		}
		else
		{
			_wallId=0;
			result.set(INVALID_VALUE);
		}
		
	}
	
	private void setLocationNow()
	{
		CradleLocation loc=(personalShopper.cradle.config).new CradleLocation(_wallId,_rowId,_columnId);
		try {
			personalShopper.cradle.config.setLocation(loc);
		} catch (CradleException e) {
			
			e.printStackTrace();
		}
	}
	private void setChargeStateNow()
	{
		boolean chargeState=false;
		if(_chargeState.equalsIgnoreCase(ISmartCradleSingleton.SMART_CRADLE_FAST))
			chargeState=true;
		else
			chargeState=false;
		
		try {
			personalShopper.cradle.config.setFastChargingState(chargeState);
		} catch (CradleException e) {
			Logger.E(TAG,"CradleException in setFastChargingState()="+e.getMessage());
			e.printStackTrace();
		}
	}

	@Override
	public void getHardwareId(IMethodResult result) {
		
		populateCradleProperties();
		result.set(_hardWareId);
	}

	@Override
	public void getPartNumber(IMethodResult result) {
		
		populateCradleProperties();
		result.set(_partNo);
	}

	@Override
	public void getSerialNumber(IMethodResult result) {
		
		populateCradleProperties();
		result.set(_serialNo);
	}

	@Override
	public void getMfgDate(IMethodResult result) {

		populateCradleProperties();
		result.set(_mfgDate);
	}

	@Override
	public void getDriverVersion(IMethodResult result) {
	
		result.setError(NOT_SUPPORTED);
		
	}

	@Override
	public void getCApiVersion(IMethodResult result) {
		result.setError(NOT_SUPPORTED);
	}

	@Override
	public void getFirmwareVersion(IMethodResult result) {

		populateCradleProperties();
		result.set(_firmWareVersion);
		
	}

	
	@Override
	public void unlock(int ledOnDuration, int ledOffDuration, int timeout,
			IMethodResult result) {
				
		unlockForAndroid(ledOnDuration,ledOffDuration,timeout,true);
			
				
		
		
	}

	@Override
	public void unlockEx(Map<String, Object> propertyMap, IMethodResult result) {
		int ledOnDuration=500;
		int ledOffDuration=500;
		int timeout=15;
		boolean smoothEffect=true;
	
		if(propertyMap.containsKey("ledOnDuration"))
			ledOnDuration=Integer.parseInt((String) propertyMap.get("ledOnDuration"));
		if(propertyMap.containsKey("ledOffDuration"))
			ledOffDuration=Integer.parseInt((String) propertyMap.get("ledOffDuration"));
		if(propertyMap.containsKey("timeout"))
			timeout=Integer.parseInt((String) propertyMap.get("timeout"));
		if(propertyMap.containsKey("smoothingEffect"))
		  smoothEffect= Boolean.parseBoolean((String) propertyMap.get("smoothingEffect"));
	   
		unlockForAndroid(ledOnDuration,ledOffDuration,timeout,smoothEffect);
				
		
	}

	private void unlockForAndroid(int ledOnDuration,int ledOffDuration,int timeout,boolean smoothEffect )
	{   
		CradleLedFlashInfo flashInfo=new CradleLedFlashInfo(ledOnDuration,ledOffDuration,smoothEffect);
		try {
			personalShopper.cradle.unlock(timeout, flashInfo);
		} catch (CradleException e) {
			Logger.E(TAG,"CradleException in unlock()="+e.getMessage());
			e.printStackTrace();
		}
	}
	@Override
	public void getProperty(String propertyName, IMethodResult result) {
		
		if((propertyName.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_COLUMN_ID)) || (propertyName.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_ROW_ID))|| (propertyName.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_WALL_ID)))
			result.set(getPropertyValueInteger(propertyName));
		else
			result.set( getPropertyValueString(propertyName));
	}

	@Override
	public void getProperties(List<String> arrayofNames, IMethodResult result) {
	
		HashMap<String,Object> props=new HashMap<String,Object>();
		for(String name:arrayofNames)
		{
			if((name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_COLUMN_ID)) ||(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_ROW_ID))||(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_WALL_ID)))
				props.put(name, getPropertyValueInteger(name));
			else
				props.put(name, getPropertyValueString(name));
		}
		result.set(props);
		
	}

	@Override
	public void getAllProperties(IMethodResult result) {
		
		
		List<String> names = new ArrayList<String>();
		names.add(ISmartCradleSingleton.PROPERTY_COLUMN_ID);
		names.add(ISmartCradleSingleton.PROPERTY_WALL_ID);
		names.add(ISmartCradleSingleton.PROPERTY_ROW_ID);
		 Map<String, Object> props = new HashMap<String, Object>();
           for (String name: names)
           {
                   props.put(name, getPropertyValueInteger(name));
           }
		   names.clear();
		names.add(ISmartCradleSingleton.PROPERTY_CHARGE_STATE);
		names.add(ISmartCradleSingleton.PROPERTY_FIRMWARE_VERSION);
		names.add(ISmartCradleSingleton.PROPERTY_HARDWARE_ID);
		names.add(ISmartCradleSingleton.PROPERTY_PART_NUMBER);
		names.add(ISmartCradleSingleton.PROPERTY_SERIAL_NUMBER);
		names.add(ISmartCradleSingleton.PROPERTY_MFG_DATE);
           // These are the only supported properties on Android
          
		   
		   for(String name:names)
		{
			props.put(name, getPropertyValueString(name));
		}
           result.set(props);
		   
	}

	@Override
	public void setProperty(String propertyName, String propertyValue,
			IMethodResult result) {
		
		SmartCradleSetProp(propertyName,propertyValue);
		result.set(true);
		
	}

	@Override
	public void setProperties(Map<String, String> propertyMap,
			IMethodResult result) {
		
		 for (Map.Entry<String, String> entry: propertyMap.entrySet())
         {
                    String key = entry.getKey();
		        	Object value = entry.getValue();
					String err=ERROR_READ_ONLY;
					String strValue = "";
                      
                      if(value instanceof Integer) strValue = ((Integer) value).toString();
                      else if(value instanceof String) strValue = (String) value;
                      else continue;
				   
				   
                 if(	 key.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_HARDWARE_ID) ||
                		 key.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_MFG_DATE) ||
                		 key.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_MFG_DATE) ||
                		 key.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_SERIAL_NUMBER)||
                		 key.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_PART_NUMBER) ||
                		 key.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_FIRMWARE_VERSION)
                	)
					{
						result.setError(ERROR_READ_ONLY);
               		}		
                 else
					 SmartCradleSetProp(key, strValue);
               		
                	 		
         }
		 
         result.set(true);
		
	}
    private void SmartCradleSetProp(String name,String value)
    {
    	if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_COLUMN_ID) && Integer.parseInt(value)<=255 && Integer.parseInt(value)>=0)
    		{    
    			_columnId=Integer.parseInt(value);
    			setLocationNow();
    		}
    	else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_ROW_ID)  && Integer.parseInt(value)<=255 && Integer.parseInt(value)>=0)
    		{
    			_rowId=Integer.parseInt(value);
    			setLocationNow();
    		}
    	else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_WALL_ID)  && Integer.parseInt(value)<=255 && Integer.parseInt(value)>=0)
    		{
    			_wallId=Integer.parseInt(value);
    			setLocationNow();
    		}			
    	else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_CHARGE_STATE))
    		{    if(value.equalsIgnoreCase(ISmartCradleSingleton.SMART_CRADLE_FAST))
    			 _chargeState=value.toLowerCase(); 
			    else
				 _chargeState=ISmartCradleSingleton.SMART_CRADLE_SLOW;	
					
                   setChargeStateNow();
    		}
    	
    	
    }
   private String getPropertyValueString(String name)
   {
	   String str="";
	   if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_CHARGE_STATE))
		   str=_chargeState;
	   else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_FIRMWARE_VERSION))
		   str=_firmWareVersion;
	   else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_HARDWARE_ID))
		   str=_hardWareId;
	   else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_MFG_DATE))
		   str=_mfgDate;
	   else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_PART_NUMBER))
	         {
		    for( int i=1;i<_partNo.length();i++)
			{   char ch1=_partNo.charAt(i);
				if (! ((ch1 == '-') || (ch1 >= 'a' && ch1 <= 'z') || (ch1 >= 'A' && ch1 <= 'Z') || (ch1 >= '0' && ch1 <= '9')))
				{
				_partNo=_partNo.substring(0,i);
				break;
				}
                        } 
  		str=_partNo; 
	        }
	   else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_SERIAL_NUMBER))
		   str=_serialNo;
	   
	   return str;
   }
   private int getPropertyValueInteger(String name)
   {
	   int value=0;
	   if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_COLUMN_ID))
		   value=_columnId;
	   else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_ROW_ID))
		   value=_rowId;
	   else if(name.equalsIgnoreCase(ISmartCradleSingleton.PROPERTY_WALL_ID))
		   value=_wallId;
	   return value;
   }

@Override
public void flashLed(Map<String, Object> propertyMap, IMethodResult result) {
	int ledOnDuration=10;
	int ledOffDuration=5;
	int ledFlashCount=10;
	boolean smoothEffect=false;

	if(propertyMap.containsKey("ledOnDuration"))
			ledOnDuration=Integer.parseInt((String) propertyMap.get("ledOnDuration"));
		if(propertyMap.containsKey("ledOffDuration"))
			ledOffDuration=Integer.parseInt((String) propertyMap.get("ledOffDuration"));
		if(propertyMap.containsKey("smoothingEffect"))
		  smoothEffect= Boolean.parseBoolean((String) propertyMap.get("smoothingEffect"));
		if(propertyMap.containsKey("ledFlashCount"))
		ledFlashCount=Integer.parseInt((String) propertyMap.get("ledFlashCount"));
		
	flashLedNow(ledOnDuration,ledOffDuration,smoothEffect,ledFlashCount);

	
}
private void flashLedNow(int ledOnDuration,int ledOffDuration,boolean smoothEffect,int ledFlashCount)
{
	CradleLedFlashInfo flashInfo=new CradleLedFlashInfo(ledOnDuration,ledOffDuration,smoothEffect);
	if(personalShopper.cradle!=null)
	{
		try {
			personalShopper.cradle.flashLed(ledFlashCount, flashInfo);
		} catch (CradleException e) {
			Logger.E(TAG,"CradleException in unlock()="+e.getMessage());
			e.printStackTrace();
		}
	}
	}
  }
