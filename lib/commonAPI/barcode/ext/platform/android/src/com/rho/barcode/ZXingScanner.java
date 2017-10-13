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

/**
 * A class to control the ZXing based camera scanner
 * @author Ben Kennedy NCVT73
 */
public class ZXingScanner extends Barcode implements IBarcode
{
	private int cameraNumber;
	/** The URL to call when the deprecated method take_barcode finishes */
	private String takeBarcodeURL;
	/** Flag to detect whether the ZXing is active */
	private boolean isScanning;
	private MethodResult takeResult;
	
	private String friendlyName;

	public ZXingScanner(String id)
	{
		super(id);
		Logger.I(LOGTAG, "ZXingScanner Constructor");
		this.cameraNumber = Integer.parseInt(id.substring(5));
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
		Logger.I(LOGTAG, "ZXingScanner getProperties");
		result.set(new HashMap<String, Object>());
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
	public void enable(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "ZXingScanner enable");
		result.setError("enable is not supported on Zebra Crossing Scanner Engines. Use 'take' instead.");
	}
	
	@Override
	public void start(IMethodResult result)
	{
		Logger.I(LOGTAG, "ZXingScanner start");
		if(result != null) result.setError("start is not supported on Zebra Crossing Scanner Engines. Use 'take' instead.");
		//TODO, we can change this.
	}

	@Override
	public void stop(IMethodResult result)
	{
		Logger.I(LOGTAG, "ZXingScanner stop");
		if(result != null) result.setError("stop is not supported on Zebra Crossing Scanner Engines. Use 'take' instead.");
		//TODO, we can change this.
	}

	@Override
	public void disable(IMethodResult result)
	{
		Logger.I(LOGTAG, "ZXingScanner disable");
		
		// Do nothing
	}

	@Override
	public void getSupportedProperties(IMethodResult result)
	{
		Logger.I(LOGTAG, "ZXingScanner getSupportedProperties");
		ArrayList<Object> resultList = new ArrayList<Object>();
		resultList.add("scannerType");
		result.set(resultList);
	}

	@Override
	public void take(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "ZXingScanner take");
		BarcodeFactory.setEnabledState(scannerId);
		takeBarcodeURL = null;
		//TODO See if it has been started previously
		//Save the result for later
		((MethodResult) result).keepAlive();
		takeResult = (MethodResult) result;

		//TODO Analyse property map and set settings
		//Start the ZXing Activity
        RhodesActivity ra = RhodesActivity.safeGetInstance();
        Intent intent = new Intent(ra, com.google.zxing.client.android.CaptureActivity.class);
        intent.putExtra(com.google.zxing.client.android.CaptureActivity.CAMERA_INDEX_EXTRA, cameraNumber);
        intent.putExtra(com.google.zxing.client.android.CaptureActivity.RHO_BARCODE_ID, this.getId());
        
        ra.startActivity(intent);
	}

	@Override
	public void take_barcode(String rubyCallbackURL, Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "ZXingScanner take_barcode");
		BarcodeFactory.setEnabledState(scannerId);
		takeBarcodeURL = rubyCallbackURL;
		//TODO See if it has been started previously
		//TODO Analyse property map and set settings
		//Start the ZXing Activity
        RhodesActivity ra = RhodesActivity.safeGetInstance();
        Intent intent = new Intent(ra, com.google.zxing.client.android.CaptureActivity.class);
        intent.putExtra(com.google.zxing.client.android.CaptureActivity.CAMERA_INDEX_EXTRA, cameraNumber);
        intent.putExtra(com.google.zxing.client.android.CaptureActivity.RHO_BARCODE_ID, this.getId());
        
        ra.startActivity(intent);
	}

	/**
	 * Called when the ZXing Activity exists with an error
	 * @param errorMessage The reason for exiting
	 * @author Ben Kennedy NCVT73
	 */
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

	/**
	 * Called when the ZXing Activity is cancelled
	 * @author Ben Kennedy NCVT73
	 */
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

	/**
	 * Called when the ZXing Activity successfully decodes a barcode
	 * @param barcodeData A ZXing structure of data of the decoded barcode
	 * @author Ben Kennedy NCVT73
	 */
	public void decodeEvent(Result barcodeData)
	{
		Logger.I(LOGTAG, "decodeEvent, URL: " + takeBarcodeURL);
		if(takeResult != null)
		{
			Logger.I(LOGTAG, "decodeEvent, takeResult != null");
			if(takeBarcodeURL == null)
			{
				Logger.I(LOGTAG, "decodeEvent, takeBarcodeURL == null");
				HashMap<String, Object> resultsMap = new HashMap<String, Object>();
				//	This is where to add extra meta data if required
				resultsMap.put("barcode", barcodeData.getText());
				resultsMap.put("status", "ok");
				takeResult.set(resultsMap);
				takeResult.release();
				takeResult = null;
			}
			else
			{
				callBackToUrl(barcodeData);
			}
		}
		else if(takeBarcodeURL != null){
			Logger.I(LOGTAG, "decodeEvent, takeResult == null");
			callBackToUrl(barcodeData);
		}else{
			Logger.I(LOGTAG, "decodeEvent failed to fire callback: result missing");
		}
			
		isScanning = false;
		BarcodeFactory.setDisabledState(scannerId);
		Logger.I(LOGTAG, "decodeEvent, function end");
	}

	void callBackToUrl(Result barcodeData){
		Logger.I(LOGTAG, "decodeEvent, takeBarcodeURL != null");
		        StringBuffer body = new StringBuffer();
		        body.append("&rho_callback=1");
		        body.append("&status=ok");
		        if (barcodeData.getText() != null)
		        {
		            try
		            {
		                String b = URLEncoder.encode(barcodeData.getText(), "utf-8");
		                body.append("&barcode=");
		                body.append(b);
		            }
		            catch (UnsupportedEncodingException e)
		            {
                Logger.I(LOGTAG, "Could not encode take_barcode return data in URL");
		            }
		        }
		Logger.I(LOGTAG, "decodeEvent, URL: " + takeBarcodeURL + body);

				RhodesActivity.safeGetInstance().getMainView().navigate(takeBarcodeURL + body, -1);
				takeBarcodeURL = null;
				//TODO needs testing
			}

//	@Override
//	public void getFriendlyName(IMethodResult result)
//	{
//		setFriendlyName();
//		result.set(friendlyName);
//	}
	
	/**
	 * Sets the friendlyName parameter
	 */
	private void setFriendlyName()
	{
		if(friendlyName == null)
		{
			friendlyName = "ZXing ";
			CameraInfo cameraInfo = new CameraInfo();
			Camera.getCameraInfo(cameraNumber, cameraInfo);
			if(cameraInfo.facing == CameraInfo.CAMERA_FACING_BACK)
			{
				friendlyName += "back facing ";
			}
			else if(cameraInfo.facing == CameraInfo.CAMERA_FACING_FRONT)
			{
				friendlyName += "front facing ";
			}
			friendlyName += "software barcode scanner";
		}
	}

	@Override
	public void getScannerType(IMethodResult result)
	{
		result.set("Camera");
	}

	@Override
	public void onResume()
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onPause()
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onStop()
	{
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onDestroy()
	{
		// TODO Auto-generated method stub
		
	}
}