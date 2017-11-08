package com.rho.videocapture;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;

import com.rho.videocapture.IVideocapture;
import com.rho.videocapture.VideocaptureBase;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesAppOptions;
import com.rhomobile.rhodes.api.IMethodResult;

public class Videocapture extends VideocaptureBase implements IVideocapture {
  
	private enum resolution {
		LOW,		
		HIGH
	};
	
	private String mId 				= "_DEFAULT_CAMERA";
    private String TAG 				= "Rho::" + Videocapture.class.getSimpleName() + mId;  
    
	private String mFileName 		= "";
	private int mDuration 			= 5000;
	private boolean mGallery 		= false;
	private IMethodResult mSavedUrl = null;
	private Activity mActivity		= null;    	
	public int RESULT_VIDEO_CODE	= 1;		// Result code from video capture activity
	private String path 			= null;
	private resolution mResolution 	= resolution.HIGH;    
    
    public Videocapture(String id) {
    	super(id);
        mId = id; 
        mActivity = RhodesActivity.getInstance();
        Logger.T(TAG, "Videocapture() -- END of constructor. Id: " + mId);
    }

    @Override
    public String getId() {
        return mId;
    }

	@Override
	public void getDuration(IMethodResult result)
	{		
		int duration = Integer.parseInt(videocaptureGet("duartion"));
		result.set(duration);		
	}

	@Override
	public void setDuration(int duration, IMethodResult result)
	{		
		videocaptureSet("duration", Integer.toString(duration));		
	}
	
	@Override
	public void getSaveToGallery(IMethodResult result)
	{	
		result.set(mGallery);		
	}

	@Override
	public void setSaveToGallery(boolean gallery, IMethodResult result)
	{		
		videocaptureSet("saveToGallery", new Boolean(gallery).toString());
	}
	
	@Override
	public void getFileName(IMethodResult result)
	{		
		result.set(videocaptureGet("fileName"));		
	}

	@Override
	public void setFileName(String fileName, IMethodResult result)
	{		
		videocaptureSet("fileName", fileName);		
	}

	@Override
	public void getResolution(IMethodResult result)
	{
		switch (mResolution)
		{
			case LOW: result.set("LOW"); break;			
			case HIGH: result.set("HIGH"); break;
		}
	}
	
	@Override
	public void setResolution(String resolution, IMethodResult result)
	{
		videocaptureSet("resolution", resolution);
	}	
    
    public void videocaptureSet(String name, String val)
    {		
		try
		{		    
		    if (name.equalsIgnoreCase("fileName"))
		    {
		    	mFileName = val;
		    	if (mFileName.contains(".") == false){		    		
		    		mFileName = mFileName + ".mp4";
		    	}else{
		    		//The app developer hasn't read the help file, so boom!
		    	}
		    }
		    else if (name.equalsIgnoreCase("duration"))
		    {
				mDuration = Integer.parseInt(val);
				if (mDuration < 1000) //Android crash
				{
					mDuration = 5000;
					Logger.I(TAG, "Setting default duration 5000");
				}
		    }
		    else if (name.equalsIgnoreCase("saveToGallery"))
		    {
		    	mGallery = Boolean.parseBoolean(val);
		    }
		    else if (name.equalsIgnoreCase("resolution"))
		    {
		    	if (val.equalsIgnoreCase("low"))
		    		mResolution = resolution.LOW;		    	
		    	else if (val.equalsIgnoreCase("high"))
		    		mResolution = resolution.HIGH;
		    }
		    else 
		    {
		    	Logger.E(TAG,  name + " Property not supported");
		    }		    	   
		}
		catch (NoClassDefFoundError e) 
		{
			Logger.E(TAG,  "Undefined property " + name + ". Exception: " + e.getMessage()); 
		}
    }
    
    public String videocaptureGet(String name)
    {				    
	    if (name.equalsIgnoreCase("fileName"))
	    {
	    	return mFileName;			
	    }
	    else if (name.equalsIgnoreCase("duration"))
	    {
	    	return Integer.toString(mDuration);
	    }
	    else if (name.equalsIgnoreCase("saveToGallery"))
	    {
	    	return new Boolean(mGallery).toString();
	    }
	    else if (name.equalsIgnoreCase("resolution"))
	    {
	    	switch (mResolution)
			{
				case LOW: return "LOW"; 				
				case HIGH: return "HIGH"; 
				default: return "HIGH";
			}
	    }
	    else 
	    {
	    	Logger.E(TAG,  name + " Property not supported");
	    	return "Not Applicable";
	    }
    }
    
	@Override
	public void	start(IMethodResult result)
	{
		Logger.D(TAG,  "Start API");
		
		//Save the callback function
		mSavedUrl = result;
				
		if (mSavedUrl != null)
		{
			Logger.T(TAG, "start() -- Callback API: " + mSavedUrl);
		}else{
			Logger.T(TAG, "start() -- mSavedUrl == null"); 
		}

			
		int durationInSeconds = mDuration / 1000;
		
		Intent takeVideoIntent = new Intent(MediaStore.ACTION_VIDEO_CAPTURE);	
		switch (mResolution)
		{
			case LOW: takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_VIDEO_QUALITY, 0);	 break;				
			case HIGH: takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_VIDEO_QUALITY, 1);	 break;
		}			
		takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_DURATION_LIMIT, durationInSeconds);
		//takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_OUTPUT, Uri.fromFile(file)); //Android crash
		//takeVideoIntent.putExtra(android.provider.MediaStore.EXTRA_SIZE_LIMIT, maximumSize); //Size in KB. May be for future use
		mActivity.startActivityForResult(takeVideoIntent,RESULT_VIDEO_CODE);	//Check for result in RhodesActivity	
	}

	@Override
	public void stop(IMethodResult result)
	{
		//Not supported
		Logger.D(TAG,  "Stop API");		
		result.setError("Stop API not supported on Android");
	}	

	@Override
	public void cancel(IMethodResult result)
	{		
		Logger.D(TAG,  "Cancel API");
		result.setError("Cancel API not supported on Android");		
	}
	
	public void stopCapture(Uri intentData)
	{
		Logger.T(TAG, "stopCapture() calling");
		if (mSavedUrl == null){
    		Logger.T(TAG, "stopCapture() -- mSavedUrl == null"); 
    	}else{
    		Logger.T(TAG, "stopCapture() -- Callback API: " + mSavedUrl);
    	}
		String tempName = null;
		if (mFileName == "")
		{
			SimpleDateFormat timeStampFormat = new SimpleDateFormat("yyyyMMddHHmmssSS");
			tempName = "/Video_" + timeStampFormat.format(new Date()) + ".mp4";
			
			String state = Environment.getExternalStorageState();
		    if (Environment.MEDIA_MOUNTED.equals(state)) 
		    {
		    	path = new File(Environment.getExternalStorageDirectory(), tempName).toString();
		    } 
		    else if (Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)) {
		        Logger.E(TAG, "No Write permissions for external storage, please provide adcard permissions in build.yml file");
		        Logger.I(TAG, "defaulting to the blob directory");
		        path = new File(RhodesAppOptions.getBlobPath(), tempName).toString();
		    } 
		    else {
		    	Logger.E(TAG, "Unable to access external storage, please check that the external storage is mounted and accessible");
		    	Logger.I(TAG, "defaulting to the blob directory");
		    	path = new File(RhodesAppOptions.getBlobPath(), tempName).toString();
		}
		}else{
			tempName = mFileName; //Its the developer's responsibility
			String state = Environment.getExternalStorageState();
		    if (Environment.MEDIA_MOUNTED.equals(state)) 
		    {
		    	path = new File(Environment.getExternalStorageDirectory(), tempName).toString();
		    } 
		    else if (Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)) {
		        Logger.E(TAG, "No Write permissions for external storage, please provide permissions in build.yml file");
		        Logger.I(TAG, "defaulting to the blob directory");
		        path = new File(RhodesAppOptions.getBlobPath(), tempName).toString();
		    } 
		    else {
		    	Logger.E(TAG, "Unable to access external storage, please check that the external storage is mounted and accessible");
		    	Logger.I(TAG, "defaulting to the blob directory");
		    	path = new File(RhodesAppOptions.getBlobPath(), tempName).toString();
		    }
		}		
		
		Logger.D(TAG,  "Save Captured video file at " + path);
		
		Map<String, Object> props = new HashMap<String, Object>();
		
		try
		{
			// Copy the video contents from the intent to the local file specified
			
			File tmpFile = new File(path);    	
		    AssetFileDescriptor videoAsset = mActivity.getApplicationContext().getContentResolver().openAssetFileDescriptor(intentData, "r");
		    FileInputStream fis = videoAsset.createInputStream();
		    FileOutputStream fos = new FileOutputStream(tmpFile);
		    Logger.D(TAG, "Trying to resave to local file");

		    byte[] buf = new byte[1024];
		    int len;
		    while ((len = fis.read(buf)) > 0) {
		        fos.write(buf, 0, len);
		    }
		    fis.close();
		    fos.close();
		    
		    if (!mGallery)
		    {
		    	//	Delete the file from the Gallery
		    	mActivity.getApplicationContext().getContentResolver().delete(intentData, null, null);
		    }
	    	
	    	Logger.D(TAG, "Successfully resaved to local file");
	    	//Invoke the callback function	    	
	    	int fileSize = (int) (tmpFile.length() / 1024);
			String size = Integer.toString(fileSize);
			String result = "SUCCESS";
	    	
	    	props.put("transferResult", result);
	    	props.put("fileName", path);
	    	props.put("fileSize", size);
	    	
	    	if (mSavedUrl != null){
	    		Logger.D(TAG, "Calling callback");
	    		mSavedUrl.set(props);    		
	    		Logger.D(TAG, "Callback called");	
	    	}else{
	    		Logger.D(TAG, "Callback is null");
	    	}
		    
		} 
		catch (IOException io_e) {
		    io_e.printStackTrace();
		    Logger.E(TAG,  "Exception in writing the file " + io_e.getMessage());
		    
	    	props.put("transferResult", "FAILURE");
	    	props.put("fileName", "");
	    	props.put("fileSize", "");
	    	
	    	if (mSavedUrl != null)
	    	{
	    		mSavedUrl.set(props);    		
	    	}
		}
	}

	@Override
    public void getAllProperties(IMethodResult result) {
       
	   Logger.D(TAG,  "getAllProperties API");
	   
	   List<String> names = new ArrayList<String>();
	   names.add("fileName");
	   names.add("duration");
	   names.add("saveToGallery");	 
	   names.add("resolution");	 
	   Map<String, Object> props = new HashMap<String, Object>();
	   for (String name: names)
	   {
		   props.put(name, videocaptureGet(name));
	   }
	   result.set(props);
    }
	
    @Override
    public void getProperty(String name, IMethodResult result) {
        
    	Logger.D(TAG,  "getProperty API");
    	result.set(videocaptureGet(name));
    }

    @Override
    public void getProperties(List<String> names, IMethodResult result) {
       
    	Logger.D(TAG,  "getProperties API");
    	Map<String, Object> props = new HashMap<String, Object>();
    	for (String name: names)
    	{
    		props.put(name, videocaptureGet(name));
    	}
    	result.set(props);
    }

    @Override
    public void setProperty(String name, String val, IMethodResult result) {
        
    	Logger.D(TAG,  "setProperty API");
    	videocaptureSet(name, val);

    }

    @Override
    public void setProperties(Map<String, String> props, IMethodResult result) {
        
    	Logger.D(TAG,  "setProperties API");
    	for (Map.Entry<String, String> entry: props.entrySet())
    	{
    		videocaptureSet(entry.getKey(), entry.getValue());
    	}
    }

    public void resetProperties()
    {
    	Logger.T(TAG, "resetProperties()"); 
    	mFileName = "";
 	    mDuration = 5000;
 	    mSavedUrl = null; 
 	    mGallery = false;
 	    mResolution = resolution.HIGH;
    }
    
    public void onActivityResultVideoCapture(RhodesActivity activity, int reqCode, int resCode, Intent intent)
    {
    	if (mSavedUrl == null){
    		Logger.T(TAG, "onActivityResult -- mSavedUrl == null"); 
    	}
		Logger.T(TAG, "onActivityResult -- START " + resCode + " " + reqCode);    	
    	if (resCode == android.app.Activity.RESULT_OK && reqCode == RESULT_VIDEO_CODE)
		{
    		//Save the file and invoke callback API
			stopCapture(intent.getData());
			Logger.T(TAG, "onActivityResult -- stopCapture");   
			/*if (mSavedUrl != null)
        	{
        		Map<String, Object> props = new HashMap<String, Object>();
        	
	        	props.put("transferResult", "OK");
	        	//props.put("fileName", "");
	        	//props.put("fileSize", "");
        		Logger.T(TAG, "onActivityResult -- result ok -- setting result props"); 
        		mSavedUrl.set(props);
        	}*/
		}
    	else if (resCode == Activity.RESULT_CANCELED) 
    	{
    		Logger.T(TAG, "onActivityResult -- videocapture canceled"); 
    		if (mSavedUrl != null){
	    			Logger.T(TAG, "onActivityResult -- setting error");  
	    			mSavedUrl.setError("Video capture cancelled by the user");    
    			}	
    	}
    	else
    	{  
    		Logger.E(TAG, "Video capture resulted an error. Code = " + resCode);
    		Map<String, Object> props = new HashMap<String, Object>();
        	
        	props.put("transferResult", "FAILURE");
        	props.put("fileName", "");
        	props.put("fileSize", "");
        	
        	if (mSavedUrl != null){
        		Logger.T(TAG, "onActivityResult -- result error - setting result props"); 
        		mSavedUrl.set(props);
        	}
    	}
    	Logger.T(TAG, "onActivityResult -- END");
    }
}

