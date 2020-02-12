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
import com.rho.barcode.BarcodeCommon;

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


public class InternalAndroidBarcodeScanner extends BarcodeCommon{

	public InternalAndroidBarcodeScanner(String id)
	{
		super(id);
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner Constructor");
		this.cameraNumber = Integer.parseInt(id.substring(5));
	}
	
	private static String LOGTAG = "InternalAndroidBarcodeScanner";


	@Override
	public void take(Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner take");
		BarcodeFactory.setEnabledState(scannerId);
		takeBarcodeURL = null;


		((MethodResult) result).keepAlive();
		takeResult = (MethodResult) result;

        RhodesActivity ra = RhodesActivity.safeGetInstance();
        Intent intent = new Intent(ra, com.google.barcodereader.BarcodeCaptureActivity.class);
        intent.putExtra(com.google.barcodereader.BarcodeCaptureActivity.CAMERA_INDEX_EXTRA, cameraNumber);
        intent.putExtra(com.google.barcodereader.BarcodeCaptureActivity.RHO_BARCODE_ID, this.getId());
        
        ra.startActivity(intent);
	}

	@Override
	public void take_barcode(String rubyCallbackURL, Map<String, String> propertyMap, IMethodResult result)
	{
		Logger.I(LOGTAG, "InternalAndroidBarcodeScanner take_barcode");
		BarcodeFactory.setEnabledState(scannerId);
		takeBarcodeURL = rubyCallbackURL;

        RhodesActivity ra = RhodesActivity.safeGetInstance();
        Intent intent = new Intent(ra, com.google.barcodereader.BarcodeCaptureActivity.class);
        intent.putExtra(com.google.barcodereader.BarcodeCaptureActivity.CAMERA_INDEX_EXTRA, cameraNumber);
        intent.putExtra(com.google.barcodereader.BarcodeCaptureActivity.RHO_BARCODE_ID, this.getId());
        
        ra.startActivity(intent);
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

	@Override
	protected void setFriendlyName()
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




}