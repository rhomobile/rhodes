package com.rho.camera;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.util.HashMap;
import java.util.Map;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.MediaStore;
import com.rhomobile.rhodes.Base64;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.Utils;

public class CameraRhoListener extends AbstractRhoListener implements
		IRhoListener {

	private static final String TAG = CameraRhoListener.class.getSimpleName();

	private IMethodResult mMethodResult;
	private Map<String, String> mActualPropertyMap;
	private static CameraRhoListener sInstance;
	private static int picChoosen_imagewidth, picChoosen_imageheight;
	
	static CameraRhoListener getInstance() {
		return sInstance;
	}

	@Override
	public void onCreateApplication(IRhoExtManager extManager) {
		sInstance = this;
		CameraFactorySingleton.setInstance(new CameraFactory(this));
		extManager.addRhoListener(this);
		extManager.registerExtension("RhoCameraApi", new CameraExtension());
	}

	@SuppressLint("NewApi")
	@Override
	public void onActivityResult(RhodesActivity activity, int requestCode, int resultCode, Intent intent) {
		RhoExtManager.getInstance().dropActivityResultRequestCode(requestCode);
		if (mMethodResult == null) {
			return;
		}
		Uri captureUri = null;
		String targetPath = " ";
		Uri curUri = null;
		try {
			if (resultCode == Activity.RESULT_OK)
			{
				String curPath = null;		
				String strCaptureUri = getActualPropertyMap().get("captureUri");		
				if (strCaptureUri != null)
				{
					captureUri = Uri.parse(getActualPropertyMap().get("captureUri"));		
				}		
			
				if (intent != null && intent.hasExtra(MediaStore.EXTRA_OUTPUT))
				{
					Logger.T(TAG, "Intent extras: "+ intent.getExtras().keySet());					
					curUri = (Uri) intent.getParcelableExtra(MediaStore.EXTRA_OUTPUT);
					if (curUri == null)
					{
						curUri = intent.getData();
					}
					Bitmap bmp = BitmapFactory.decodeFile(curUri.getPath());					
					picChoosen_imagewidth = bmp.getWidth();
					picChoosen_imageheight = bmp.getHeight();
					if((getActualPropertyMap().get("outputFormat").equalsIgnoreCase("dataUri"))){				
						ByteArrayOutputStream stream = new ByteArrayOutputStream();
						bmp.compress(Bitmap.CompressFormat.JPEG, 100, stream);
						byte[] byteArray = stream.toByteArray();
						StringBuilder dataBuilder = new StringBuilder();
						dataBuilder.append("data:image/jpeg;base64,");
						dataBuilder.append(Base64.encodeToString(byteArray, false));
						getActualPropertyMap().put("curUri", dataBuilder.toString());						
						curUri=Uri.parse(dataBuilder.toString());
					}
					Logger.T(TAG, "Photo is captured: " + curUri);					
					bmp.recycle();
				}
				else if (captureUri != null )
				{
					Logger.T(TAG, "Use stored captureUri: " + captureUri);					
					curUri = captureUri;					
				}
				else
				{
					curUri = intent.getData();
					Logger.T(TAG, "Check intent data: " + curUri);
				}
		
				if (curUri.getScheme().equals("file")) 
				{					
						curPath = curUri.getPath();					
						String dataDir=RhodesActivity.safeGetInstance().getApplicationInfo().dataDir;
						dataDir=dataDir+curPath.substring(curPath.lastIndexOf("/") );
						
					    if(getActualPropertyMap().get("fileName")==null)
					    	{
					    		getActualPropertyMap().put("fileName",dataDir);
					    	}
					    if(getActualPropertyMap().get("fileName").contains(".jpg"))
					    	targetPath = getActualPropertyMap().get("fileName");
					    else
					    	targetPath = getActualPropertyMap().get("fileName")+".jpg";
						File curFile = new File(curPath);
						if (!curFile.isFile()) 
						{							
							throw new RuntimeException(
									"Captured photo file does not exist: "
											+ curPath);
						}
						if (!curPath.equals(targetPath)) 
						{
						//	Utils.copy(curPath, targetPath);
						//	curFile.delete();
							Logger.T(TAG, "File copied to " + targetPath);							
							curUri = Uri.fromFile(new File(targetPath));
						}
				}
				try{
					HashMap<String,Object> resultMap=new HashMap<String,Object>();
					resultMap.put("status","ok");
					if(CameraSingletonObject.deprecated_choose_pic || CameraObject.deprecated_take_pic){
						resultMap.put("image_uri",  curUri.toString());
						resultMap.put("image_format",   "jpg");						
					}
					else{
					   resultMap.put("imageUri",  curUri.toString());
					   resultMap.put("imageFormat",   "jpg");					 
					}
					if(picChoosen_imagewidth > 0){
						if(CameraSingletonObject.deprecated_choose_pic || CameraObject.deprecated_take_pic){
							resultMap.put("image_width",  "" + picChoosen_imagewidth);
							resultMap.put("image_height",  "" + picChoosen_imageheight);							
						}
						else{
						   resultMap.put("imageWidth",  "" + picChoosen_imagewidth);
						   resultMap.put("imageHeight",  "" + picChoosen_imageheight);							
						}
					}
					else{
						if(CameraSingletonObject.deprecated_choose_pic || CameraObject.deprecated_take_pic){
							resultMap.put("image_width",  "" + picChoosen_imagewidth);
							resultMap.put("image_height",  "" + picChoosen_imageheight);							
						}
						else{
						resultMap.put("imageWidth",  "" + intent.getExtras().get("IMAGE_WIDTH"));
						resultMap.put("imageHeight",  "" + intent.getExtras().get("IMAGE_HEIGHT"));							
						}
					}
			    	mMethodResult.set(resultMap);					
				}
				catch(Exception ex)
				{
										
				}	
			} 
			else if (resultCode == Activity.RESULT_CANCELED) 
			{	HashMap<String,Object> resultMap=new HashMap<String,Object>();
				if (intent != null && intent.hasExtra("error")) {
					resultMap.put("message", ""+intent.getStringExtra("error"));
					resultMap.put("status", "error");
				//	mMethodResult.setError(intent.getStringExtra("error"));
				} else {
					resultMap.put("status", "cancel");
				}
				mMethodResult.set(resultMap);
			} else {
				mMethodResult.setError("Unknown error");
			}
		} catch (Throwable err) {
			Logger.E(TAG, err);	
			mMethodResult.setError(err.getMessage());
		}
		
		releaseMethodResult();
	}

	void setMethodResult(IMethodResult result) {
		mMethodResult = result;
	}

	void releaseMethodResult() {
		mMethodResult = null;
	}

	void setActualPropertyMap(Map<String, String> propertyMap) {
		mActualPropertyMap = propertyMap;
	}

	Map<String, String> getActualPropertyMap() {
		return mActualPropertyMap;
	}
}
