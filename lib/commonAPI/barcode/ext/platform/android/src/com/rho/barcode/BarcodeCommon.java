package com.rho.barcode;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Intent;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;

import com.google.zxing.Result;
import com.rho.barcode.IBarcode;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

public abstract class BarcodeCommon extends Barcode implements IBarcode {

	public BarcodeCommon(String id){
		super(id);
		cameraNumber = Integer.parseInt(id.substring(5));
	}

	protected int cameraNumber;
	protected String takeBarcodeURL;
	protected boolean isScanning;
	protected MethodResult takeResult;
	protected String friendlyName;
	protected int camera_index;


    private static HashMap<String, Integer> camerasIds = new HashMap<String, Integer>();
    public static void addId(String cameraName, int id){
        camerasIds.put(cameraName, id);
    }
    public static int getCameraIdByName(String cameraName){
    	try{
    		return camerasIds.get(cameraName);
    	}catch(Exception e){
    		return -1;
    	}
    }


	@Override
	public void getProperty(String propertyName, IMethodResult result)
	{
		if(propertyName.equals("scannerType"))
		{
			result.set("Camera");
		}
	}

	@Override
	public void getAllProperties(IMethodResult result)
	{
		Map<String, Object> resultMap = new HashMap<String, Object>(2);
		setFriendlyName();
		resultMap.put("scannerType", "Camera");
		//resultMap.put("friendlyName", friendlyName);
		result.set(resultMap);
	}

		@Override
	public void getProperties(List<String> arrayofNames, IMethodResult result)
	{
		for(String property: arrayofNames)
		{
			if(property.equals("scannerType"))
			{
				Map<String, Object> resultMap = new HashMap<String, Object>(2);
				resultMap.put("scannerType", "Camera");
				result.set(resultMap);
				return;
			}
		}
		Logger.I(LOGTAG, "getProperties");
		result.set(new HashMap<String, Object>());
	}

	@Override
	public void getSupportedProperties(IMethodResult result)
	{
		Logger.I(LOGTAG, "getSupportedProperties");
		ArrayList<Object> resultList = new ArrayList<Object>();
		resultList.add("scannerType");
		result.set(resultList);
	}

	@Override
	public void enable(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "Enable");
		result.setError("enable is not supported on Zebra Crossing Scanner Engines. Use 'take' instead.");
	}
	
	@Override
	public void start(IMethodResult result)
	{
		Logger.I(LOGTAG, "Start");
		if(result != null) result.setError("start is not supported on Zebra Crossing Scanner Engines. Use 'take' instead.");
	}

	@Override
	public void stop(IMethodResult result)
	{
		Logger.I(LOGTAG, "Stop");
		if(result != null) result.setError("stop is not supported on Zebra Crossing Scanner Engines. Use 'take' instead.");
	}

	@Override
	public void disable(IMethodResult result)
	{
		Logger.I(LOGTAG, "Disable");
	}

	public void error(String errorMessage)
	{
		if(takeResult != null)
		{
			takeResult.setError(errorMessage);
			takeResult.release();
			takeResult = null;
			takeBarcodeURL = null;
		}
		else
		{
			Logger.D(LOGTAG, "Error failed to fire callback: result missing");
		}
		isScanning = false;
		BarcodeFactory.setDisabledState(scannerId);
		
	}

	public void cancel()
	{
		if(takeBarcodeURL == null)
		{
			if(takeResult != null)
			{
				HashMap<String, Object> resultsMap = new HashMap<String, Object>();
				//	This is where to add extra meta data if required
				resultsMap.put("barcode", "");
				resultsMap.put("status", "cancel");
				takeResult.set(resultsMap); //Does this return the object/ fire the callback?
				takeResult.release();
				takeResult = null;
			}
			else
			{
				Logger.D(LOGTAG, "Cancel failed to fire callback: result missing");
			}
		}
		else
		{
	        StringBuffer body = new StringBuffer();
	        body.append("&rho_callback=1");
	        body.append("&status=cancel");
	        RhodesActivity.safeGetInstance().getMainView().navigate(takeBarcodeURL + body, -1);
	        takeBarcodeURL = null;
	        //TODO needs testing
		}
		isScanning = false;
		BarcodeFactory.setDisabledState(scannerId);
	}

	abstract protected void setFriendlyName();

	@Override
	public void getScannerType(IMethodResult result)
	{
		result.set("Camera");
	}

	@Override
	public void onResume()
	{
	}

	@Override
	public void onPause()
	{
	}

	@Override
	public void onStop()
	{
	}

	@Override
	public void onDestroy()
	{	
	}

}