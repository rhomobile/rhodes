package com.rho.camera;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.Map;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.RemoteException;
import android.provider.MediaStore;
import android.provider.MediaStore.MediaColumns;
import com.rhomobile.rhodes.Base64;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.Utils;
import com.rhomobile.rhodes.file.RhoFileApi;

public class CameraRhoListener extends AbstractRhoListener implements
		IRhoListener {

	private static final String TAG = CameraRhoListener.class.getSimpleName();

	private IMethodResult mMethodResult;
	private Map<String, String> mActualPropertyMap = null;
	private static CameraRhoListener sInstance = null;
	private static int picChoosen_imagewidth, picChoosen_imageheight = 0;
	private Uri curUri = null;
	private HashMap<String,Object> resultMap = null;
	private String imgPath = null;
	private Bitmap mBitmap = null;
	private String rename = null;
	
	static CameraRhoListener getInstance() {
		return sInstance;
	}

	@Override
	public void onCreateApplication(IRhoExtManager extManager) {
		sInstance = this;
		CameraFactorySingleton.setInstance(new CameraFactory(this));
		extManager.addRhoListener(this);
		extManager.registerExtension("RhoCameraApi", new CameraExtension());
		resultMap=new HashMap<String,Object>();
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
		ByteArrayOutputStream stream = null;
		try {
			if (resultCode == Activity.RESULT_OK)
			{
				getActualPropertyMap().put("default_camera_key_path", "");
				SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_hhmmss");
				rename = "IMG_"+ dateFormat.format(new Date(System.currentTimeMillis()))+".jpg";
				String curPath = null;		
				String strCaptureUri = getActualPropertyMap().get("captureUri");		
				if (strCaptureUri != null)
				{
					captureUri = Uri.parse(getActualPropertyMap().get("captureUri"));		
				}		
				if (intent != null && intent.hasExtra(MediaStore.EXTRA_OUTPUT))
				{
					if(intent.hasExtra(MediaStore.EXTRA_OUTPUT)){
						Logger.T(TAG, "Intent extras: "+ intent.getExtras().keySet());	
						curUri = (Uri) intent.getParcelableExtra(MediaStore.EXTRA_OUTPUT);
					}
					if (curUri == null)
					{
						curUri = intent.getData();
					}
					try {
						System.gc();
						if (getActualPropertyMap().get("DeviceGallery_Key") == null){
							mBitmap = BitmapFactory.decodeFile(curUri.getPath());
						}else{
							imgPath = getFilePath(curUri);
							mBitmap = BitmapFactory.decodeFile(imgPath);
						}
						
					} catch (OutOfMemoryError e1) {
						e1.printStackTrace();
					}
					File file = null;
					if (!getActualPropertyMap().containsKey("fileName") && getActualPropertyMap().get("ChoosePicture_Key") == null){ 
						file= new File(curUri.getPath());
						file.renameTo(new File(file.getParentFile(), rename));
						RhodesActivity.getContext().sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, 
				                Uri.parse(curUri.getPath())));
					}
					picChoosen_imagewidth = mBitmap.getWidth();
					picChoosen_imageheight = mBitmap.getHeight();
					if((getActualPropertyMap().get("outputFormat").equalsIgnoreCase("dataUri"))){				
						stream = new ByteArrayOutputStream();
						mBitmap.compress(Bitmap.CompressFormat.JPEG, 100, stream);
						byte[] byteArray = stream.toByteArray();
						StringBuilder dataBuilder = new StringBuilder();
						dataBuilder.append("data:image/jpeg;base64,");
						try {
							System.gc();
							dataBuilder.append(Base64.encodeToString(byteArray, false));
						} catch (Exception e) {
							// TODO: handle exception
							e.printStackTrace();
						}
						catch(OutOfMemoryError e){
							stream = new ByteArrayOutputStream();
							mBitmap.compress(Bitmap.CompressFormat.JPEG, 50, stream);
							byteArray = stream.toByteArray();
							dataBuilder.append(Base64.encodeToString(byteArray, false));
						}
						getActualPropertyMap().put("curUri", dataBuilder.toString());						
						curUri=Uri.parse(dataBuilder.toString());
					}
					Logger.T(TAG, "Photo is captured: " + curUri);					
					mBitmap.recycle();
				}else if (captureUri != null )
				{			
					
				curUri = captureUri;
					
					
					if (getActualPropertyMap().get("dataURI") == null) {
						imgPath = getFilePath(curUri);
							File f= new File(imgPath);
							imgPath = copyImg(imgPath);
							getActualPropertyMap().put("default_camera_key_path", "default_camera_key_path_value");
							BitmapFactory.Options options = new BitmapFactory.Options();
							options.inPreferredConfig = Bitmap.Config.ARGB_8888;
							        try {
							        	mBitmap = BitmapFactory.decodeStream(new FileInputStream(f), null, options);
							        	if (!getActualPropertyMap().containsKey("fileName") && getActualPropertyMap().get("ChoosePicture_Key") == null){ 
							        	f.renameTo(new File(f.getParentFile(), rename));
										RhodesActivity.getContext().sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, 
								                Uri.parse(f.getAbsolutePath())));
							        	}
							        } catch (FileNotFoundException e) {
							            e.printStackTrace();
							        }
							picChoosen_imagewidth = mBitmap.getWidth();
							picChoosen_imageheight = mBitmap.getHeight();
							mBitmap.recycle();
					}
					
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
						
						if (!curPath.equals(targetPath)) 
						{
						//	Utils.copy(curPath, targetPath);
						//	curFile.delete();
							Logger.T(TAG, "File copied to " + targetPath);							
							curUri = Uri.fromFile(new File(targetPath));
						}
						imgPath = copyImg(curPath);
						curUri = Uri.parse(imgPath);
				}
				try{
					DefaultCameraAsyncTask async = new DefaultCameraAsyncTask(mMethodResult, resultMap, intent, resultCode);
					async.execute();					
				}
				catch(Exception ex)
				{
										
				}	
			} 
			else if (resultCode == Activity.RESULT_CANCELED) 
			{	
				getActualPropertyMap().put("default_camera_key_path", "");
				DefaultCameraAsyncTask async = new DefaultCameraAsyncTask(mMethodResult, resultMap, intent,resultCode);
				async.execute();
				CameraActivity.click_rotation = false;
			} else {
				mMethodResult.setError("Unknown error");
			}
		} catch (Throwable err) {
			Logger.E(TAG, err);	
			if (stream != null) {
				try {
					stream.reset();
					stream.close();
				} catch (Throwable e1) {
					// Do nothing
				}
			}
			mMethodResult.setError(err.getMessage());
		}
		
	}

	void setMethodResult(IMethodResult result) {
		mMethodResult = result;
	}

	void releaseMethodResult() {
		mMethodResult = null;
		resultMap.clear();
		mActualPropertyMap.clear();
		getActualPropertyMap().put("DeviceGallery_Key", "");
		getActualPropertyMap().put("ChoosePicture_Key", "");
	}

	void setActualPropertyMap(Map<String, String> propertyMap) {
		mActualPropertyMap = propertyMap;
	}

	Map<String, String> getActualPropertyMap() {
		return mActualPropertyMap;
	}
	
	/**
	 * AsyncTask class to handle keydispatchingtimedout or ANR caused 
	 * when OK or Cancel button of default camera in clicked 
	 * @param  IMethodResult  Object to set the hash map properties
	 * @param  HashMap to set properties of captured image to map
	 * @param  Intent 
	 * @param  ResultCode to decide click is OK or Cancel
	 */
	public class DefaultCameraAsyncTask extends AsyncTask<Void, Void, Void>{

		IMethodResult inMethodRes;
		HashMap<String, Object> inResultMap = new HashMap<String,Object>();
		Intent intent = new Intent();
		int resCode;
		
		public DefaultCameraAsyncTask(IMethodResult inMethodRes, HashMap<String, Object> inResultMap, Intent intent, int resCode){
            this.inMethodRes = inMethodRes;
            this.inResultMap = inResultMap;
            this.intent = intent;
            this.resCode = resCode;
        }
		
		@Override
		protected Void doInBackground(Void... params) {

			if(resCode == -1){
				if (intent != null && intent.hasExtra("error")) {
						inResultMap.put("message", ""+intent.getStringExtra("error"));
						if(intent.getStringExtra("error").contains("\\"))
							inResultMap.put("message", "File path is invalid.");
						inResultMap.put("status", "error");
					}
				else{
				inResultMap.put("status","ok");
				if(CameraSingletonObject.deprecated_choose_pic || CameraObject.deprecated_take_pic){
					inResultMap.put("image_uri",  "db/db-files/"+ curUri.toString().substring(curUri.toString().lastIndexOf("/")+1, curUri.toString().length()));
					inResultMap.put("image_format",   "jpg");			
				}
				else{
					if(getActualPropertyMap().get("default_camera_key_path") != null){
						inResultMap.put("imageUri",  imgPath);
						inResultMap.put("imageFormat",   "jpg");
					}else{
						inResultMap.put("imageUri",  curUri.toString());
						inResultMap.put("imageFormat",   "jpg");
					}			
				}
				if(picChoosen_imagewidth > 0){
					if(CameraSingletonObject.deprecated_choose_pic || CameraObject.deprecated_take_pic){
						inResultMap.put("image_width",  "" + picChoosen_imagewidth);
						inResultMap.put("image_height",  "" + picChoosen_imageheight);
					}
					else{
						inResultMap.put("imageWidth",  "" + picChoosen_imagewidth);
						inResultMap.put("imageHeight",  "" + picChoosen_imageheight);
					}
				}
				else{
					if(CameraSingletonObject.deprecated_choose_pic || CameraObject.deprecated_take_pic){
						inResultMap.put("image_width",  "" + picChoosen_imagewidth);
						inResultMap.put("image_height",  "" + picChoosen_imageheight);
					}
					else{
						inResultMap.put("imageWidth",  "" + intent.getExtras().get("IMAGE_WIDTH"));
						inResultMap.put("imageHeight",  "" + intent.getExtras().get("IMAGE_HEIGHT"));
					}
				}
				}
				
			}else if(resCode == 0){
				
				inResultMap.put("message", "User canceled operation.");
				if (intent != null && intent.hasExtra("error")) {
					inResultMap.put("message", ""+intent.getStringExtra("error"));
					if(intent.getStringExtra("error").contains("\\"))
						inResultMap.put("message", "File path is invalid.");
					inResultMap.put("status", "error");
				} else {
					inResultMap.put("status", "cancel");
				}
			}
			return null;
			
		}	
		
		@Override
		protected void onPostExecute(Void result) {
			super.onPostExecute(result);
			inMethodRes.set(inResultMap);
			releaseMethodResult();
		}
		
	}
	
	/*
	 * method to convert uri to file path
	 * 
	 * @param Uri of file
	 * @returns String path of file
	 * 
	 */
	private String getFilePath(Uri uri){
		String mImgPath = null;
		Cursor imageCursor = RhodesActivity.getContext().getContentResolver().query(
				uri, null, null, null, null);
		if(imageCursor.moveToFirst()){
			mImgPath = imageCursor.getString(imageCursor
					.getColumnIndex(MediaColumns.DATA));
			imageCursor.close();
		
		}
		return mImgPath;
	}
	
	/**
	 * Copy image.
	 * Function to copy image from sd card to application root path
	 *  
	 * @param photo
	 *            the photo
	 * @return the string
	 */
	public String copyImg(String imgPath){
		
		File oldFile = new File(imgPath);
		
		File mediafile  =  new File(RhoFileApi.getDbFilesPath(), rename);
		FileInputStream finput= null;
		FileOutputStream fout = null;
		try {
			finput = new FileInputStream(oldFile);
			fout = new FileOutputStream(mediafile);
			byte[] b = new byte[1024];
			int read = 0;
			while ((read = finput.read(b)) != -1) {
				fout.write(b, 0, read);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if(finput != null){
				try {
					finput.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			
			if(fout != null){
				try {
					fout.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		
		return mediafile.getAbsolutePath();
	}
}
