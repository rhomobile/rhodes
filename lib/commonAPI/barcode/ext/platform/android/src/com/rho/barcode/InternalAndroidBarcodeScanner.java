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

import com.rho.barcode.IBarcode;
import com.rho.barcode.BarcodeBase;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.SparseArray;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;


import com.google.android.gms.vision.Frame;
import android.content.Context;
import com.google.android.gms.vision.MultiProcessor;
import com.google.android.gms.vision.Tracker;


public class InternalAndroidBarcodeScanner extends Barcode implements IBarcode{

	public InternalAndroidBarcodeScanner(String id)
	{
		super(id);
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner Constructor");
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
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner getProperties");
		result.set(new HashMap<String, Object>());
	}

	@Override
	public void getAllProperties(IMethodResult result)
	{
		Map<String, Object> resultMap = new HashMap<String, Object>(2);
		setFriendlyName();
		resultMap.put("scannerType", "Camera");
		result.set(resultMap);
	}

	@Override
	public void enable(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner enable");
		result.setError("enable is not supported on Internal Android Barcode Scanner. Use 'take' instead.");
	}
	
	@Override
	public void start(IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner start");
		if(result != null) result.setError("start is not supported on Internal Android Barcode Scanner. Use 'take' instead.");
	}

	@Override
	public void stop(IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner stop");
		if(result != null) result.setError("stop is not supported on ZInternal Android Barcode Scanner. Use 'take' instead.");
	}

	@Override
	public void disable(IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner disable");
	}

	@Override
	public void take(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner take");
		BarcodeFactory.setEnabledState(scannerId);
		takeBarcodeURL = null;


		((MethodResult) result).keepAlive();
		takeResult = (MethodResult) result;

        RhodesActivity ra = RhodesActivity.safeGetInstance();
        Intent intent = new Intent(ra, com.google.barcodereader.CaptureActivity.class);
        intent.putExtra(com.google.barcodereader.CaptureActivity.CAMERA_INDEX_EXTRA, cameraNumber);
        intent.putExtra(com.google.barcodereader.CaptureActivity.RHO_BARCODE_ID, this.getId());
        
        ra.startActivity(intent);
	}

	private int cameraNumber;
	private String takeBarcodeURL;
	private boolean isScanning;
	private MethodResult takeResult;
	private String friendlyName;
	private int camera_index;

	private static String LOGTAG = "InternalAndroidBarcodeScanner";





	@Override
	public void getSupportedProperties(IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner getSupportedProperties");
		ArrayList<Object> resultList = new ArrayList<Object>();
		resultList.add("scannerType");
		result.set(resultList);
	}


	@Override
	public void take_barcode(String rubyCallbackURL, Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner take_barcode");
		BarcodeFactory.setEnabledState(scannerId);
		takeBarcodeURL = rubyCallbackURL;

        RhodesActivity ra = RhodesActivity.safeGetInstance();
        Intent intent = new Intent(ra, com.google.barcodereader.CaptureActivity.class);
        intent.putExtra(com.google.barcodereader.CaptureActivity.CAMERA_INDEX_EXTRA, cameraNumber);
        intent.putExtra(com.google.barcodereader.CaptureActivity.RHO_BARCODE_ID, this.getId());
        
        ra.startActivity(intent);
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
		}
		isScanning = false;
		BarcodeFactory.setDisabledState(scannerId);
	}

	public void decodeEvent(String barcode)
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
				resultsMap.put("barcode", barcode);
				resultsMap.put("status", "ok");
				takeResult.set(resultsMap);
				takeResult.release();
				takeResult = null;
			}
			else
			{
				callBackToUrl(barcode);
			}
		}
		else{
			Logger.I(LOGTAG, "decodeEvent failed to fire callback: result missing");
		}
			
		isScanning = false;
		BarcodeFactory.setDisabledState(scannerId);
		Logger.I(LOGTAG, "decodeEvent, function end");
	}

	void callBackToUrl(String barcodeData){
		Logger.I(LOGTAG, "decodeEvent, takeBarcodeURL != null");
        StringBuffer body = new StringBuffer();
        body.append("&rho_callback=1");
        body.append("&status=ok");
        if (barcodeData != null)
        {
            try
            {
                String b = URLEncoder.encode(barcodeData, "utf-8");
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


	private void setFriendlyName()
	{
		if(friendlyName == null)
		{
			friendlyName = "Google ";
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