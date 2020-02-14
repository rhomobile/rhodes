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
import com.rho.barcode.BarcodeCommon;

/**
 * A class to control the ZXing based camera scanner
 * @author Ben Kennedy NCVT73
 */
public class ZXingScanner extends BarcodeCommon
{
	public ZXingScanner(String id)
	{
		super(id);
		Logger.I(LOGTAG, "ZXingScanner Constructor");
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

	@Override
	protected void setFriendlyName()
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


}