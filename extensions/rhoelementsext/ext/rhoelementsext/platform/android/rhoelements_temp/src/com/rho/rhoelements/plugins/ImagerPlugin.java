package com.rho.rhoelements.plugins;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.security.InvalidParameterException;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Size;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.Display;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;
import android.util.Patterns;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.services.FileTransferService;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author vbc863
 *
 */
@SuppressWarnings("deprecation")
public class ImagerPlugin extends Plugin
{
    public static final String IMAGER_PLUGIN_TRANSFER_COMPLETE = 
		"com.rho.rhoelements.plugins.ImagerPlugin.TRANSFER_COMPLETE";
	
    //If the imager doesnt work, like in the EV2 device, add the "Build.VERSION.INCREMENTAL String to this array.
    public static final String[] BROKEN_DEVICES_VERSION_INC = { "eng.sam.20110701.142109" };

    public static final String IMAGER_TRANSFER_RETURN_ID = "ImagerReturnID";
    private static final String		IMAGE_DATA= "imageData";

	private static final String TAG = "ImagerPlugin";
    private Camera mCamera = null;
    private int mCameraId;
    private int mDefaultCameraId;
    private AbsoluteLayout mPreviewWindow;
    private Camera.ShutterCallback mShutterCallback;
    private Camera.PictureCallback mJpegCallback;
    private Camera.PictureCallback mPostViewCallback;
    private int mRotation = 0;
    private int currentVolume = 0;
    private String mLastFlashMode = Camera.Parameters.FLASH_MODE_OFF;
    private ImageTransferBroadcastReceiver mReceiver;
    private Handler mHandler;
    private boolean mShutterClickMute = false;
    private boolean mCaptureInProgress = false;
    private MediaPlayer mPlayer;
    private boolean mSleepMode = false;
    private boolean mIsEnabled = false;

    protected String mImagerEvent = null;
    protected String mImagerEnumEvent = null;
    protected int mLeft = 0;
    protected int mTop = 0;
    protected int mWidth = ViewGroup.LayoutParams.FILL_PARENT;
    protected int mHeight = ViewGroup.LayoutParams.FILL_PARENT;
    protected int m_iDesiredWidth = -1;
    protected int m_iDesiredHeight = -1;
    protected String mFilename = Common.getDataPath() + "/Img"; //TODO revert
//    protected String mFilename = "Img.jpg"; //TODO revert
    protected int mQuality = 20;
    protected String mFlashMode = Camera.Parameters.FLASH_MODE_OFF;
    protected int mAim = 0;
    protected String mUsername = null;
    protected String mPassword = null;
    protected FileTransferService.FileTransferProtocols mProtocol = null;
    protected String mDestination = null;
    private String mCaptureEvent = null;
    protected String mSound = null;

	/**
	 * 
	 */
	public ImagerPlugin()
	{
		super();
		Logger.I(TAG, "+");
		final CameraInfo info = new android.hardware.Camera.CameraInfo();
		// find the default camera (first backwards facing camera)
		for (mDefaultCameraId = 0; mDefaultCameraId < Camera.getNumberOfCameras(); mDefaultCameraId++)
		{
			Camera.getCameraInfo(mDefaultCameraId, info);
			if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK)
				break;
		}
		mCameraId = mDefaultCameraId;
		
		//Take the sound wav value from the config.xml file
		String configSoundSetting = Common.config.getSetting("imagercapturewav");
		if(configSoundSetting != null && !configSoundSetting.equals(""))
		{
			mSound = configSoundSetting;
		}
		
		// set the default preview screen size to full screen
		Display d = ((WindowManager)Common.mainActivity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		mWidth = d.getWidth();
		mHeight = d.getHeight();

		IntentFilter intentFilter = new IntentFilter(IMAGER_PLUGIN_TRANSFER_COMPLETE);
		mReceiver = new ImageTransferBroadcastReceiver();
		Common.mainActivity.registerReceiver(mReceiver, intentFilter);
		mHandler = new Handler();

		mShutterCallback = new Camera.ShutterCallback()
		{
			
			@Override
			public void onShutter()
			{
				if (mPlayer != null)
				{
					try
					{
						mPlayer.prepare();
					}
					catch (IllegalStateException e)
					{
						Logger.W(TAG, e.getMessage());
					}
					catch (IOException e)
					{
						Logger.W(TAG, e.getMessage());
					}
				}
			}
		};
		mJpegCallback = new Camera.PictureCallback() 
		{
			@Override
			public void onPictureTaken(byte[] imageData, Camera c) 
			{
				saveBitmap(imageData);
			}
		};
		
		mPostViewCallback = new Camera.PictureCallback()
		{
			@Override
			public void onPictureTaken(byte[] data, Camera camera)
			{
				Logger.I(TAG, "We have data in PostViewCallback");
			}
		};

		try
		{
			// Create our Preview briefly off screen on init to move the black screen flash.
			Preview preview = new Preview(Common.mainActivity.getApplicationContext());
			preview.setZOrderMediaOverlay(true); //Stops WebView from trying to show ontop (as its presumeably a SurfaceView)
			preview.setClickable(true);
			mPreviewWindow = new AbsoluteLayout(Common.mainActivity.getApplicationContext());
			mPreviewWindow.setLayoutParams(new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
							
			mPreviewWindow.addView(preview, new AbsoluteLayout.LayoutParams(1, 1, -10, -10));
			
			Common.mainActivity.addContentView(mPreviewWindow, mPreviewWindow.getLayoutParams());
			mPreviewWindow.removeAllViews();
			mPreviewWindow = null;
		}
		catch (Exception e)
		{
			Logger.E(TAG, "Should only hit this Exception in testing: " + e.getMessage());
		}
	}
	
	protected void saveBitmap(byte[] imageData)
	{
		Logger.I(TAG, "saveBitmap+");
		Bitmap bitmap = BitmapFactory.decodeByteArray(imageData, 0, imageData.length);

		FileOutputStream file;
		boolean writeComplete = false;

		try
		{
			//extension checking
			int lastDot = mFilename.lastIndexOf('.');
			if(		lastDot == -1 || //There is no dot in the name 
				mFilename.substring(lastDot).indexOf('/') != -1 || //The last dot comes before a directory separator therefore not an extension
						mFilename.substring(lastDot).indexOf('\\') != -1) //The last dot comes before a directory separator therefore not an extension
			{
			
				mFilename += ".bmp";
			
			}
			file = new FileOutputStream(mFilename);
			BufferedOutputStream stream = new BufferedOutputStream(file);
			//bitmap.compress(Bitmap.CompressFormat.JPEG, mQuality, stream);
					int rotationAngle = 0;
			if ((mRotation > 45) && (mRotation < 135)) {
				rotationAngle = 180;
				

			} else if ((mRotation > 134) && (mRotation < 225)) {
				rotationAngle = 270;
				

			} else if ((mRotation > 224) && (mRotation < 315)) {
				rotationAngle = 0;
				

			} else {
				rotationAngle = 90;
				

			}
			if(Build.MODEL.contains("TC55") || Build.MODEL.contains("TC70") || Build.MODEL.contains("TC8000") || Build.MODEL.contains("TC75"))
			{
				//TC70 and TC55 needs special handling
				/*if(mRotation==90 )
					rotationAngle=90;
				else if(mRotation==270 )
					rotationAngle=270;
				else if(mRotation==0 )
					rotationAngle=0;
				else if(mRotation==180 )
					rotationAngle=180;
					*/
				rotationAngle=mRotation;
			}
			Matrix matrix = new Matrix();
			matrix.postRotate(rotationAngle);

			Bitmap rotatedBitmap = Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(),
					bitmap.getHeight(), matrix, true);
			
			rotatedBitmap.compress(Bitmap.CompressFormat.JPEG, mQuality, stream);
			stream.flush();
			stream.close();
			writeComplete = true;
		}
		catch (FileNotFoundException e)
		{
			Logger.W(TAG, e.getMessage());
		}
		catch (IOException e)
		{
			Logger.W(TAG, e.getMessage());
		}
		finally
		{
			bitmap.recycle();
			bitmap = null;
			if ((mPreviewWindow != null) && (mCamera != null))
			{
				Camera.Parameters params = mCamera.getParameters();
				if (params != null)
				{
					List<String> flashModes = params.getSupportedFlashModes();
					if ((flashModes != null) && (flashModes.contains(mFlashMode)))
					{
						params.setFlashMode(mFlashMode);
						mCamera.setParameters(params);								
					}
				}
				mCamera.startPreview();
			}

			if(writeComplete)
			{
			    if (mCaptureEvent != null)
			    {
				sendImageToDataURI(encodeToBase64(new File(mFilename)));
			    }
			    finishWrite();
			}
			
			if (mShutterClickMute)
			{
				// if we muted the System Audio Stream prior to capture, unmute it now
				AudioManager am = (AudioManager)Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);
		        am.setStreamVolume(AudioManager.STREAM_SYSTEM, currentVolume,   AudioManager.FLAG_REMOVE_SOUND_AND_VIBRATE);
				((AudioManager) Common.mainActivity.getSystemService(
						Context.AUDIO_SERVICE)).setStreamMute(AudioManager.STREAM_SYSTEM, false);
				mShutterClickMute = false;
			}
		}
	}

	private void finishWrite()
	{
		dumpParams();
		Logger.I(TAG, "Write Complete: " + mFilename);

		FileTransferService.FileTransferProtocols protocol = mProtocol;
		
		try
		{
			URL destinationUrl = new URL(mDestination);

			if (protocol == null)
			{
				protocol = FileTransferService.parseProtocol(destinationUrl.getProtocol());
			}
			
			if (protocol != null)
			{      
			     if(destinationUrl.toString().contains("http"))
			      {
				if(Patterns.WEB_URL.matcher(destinationUrl.toString()).matches())
				{
				//  Create the intent to initialise the FileTransferService, this will 
				//  contain all attributes applicable to the transfer such as source,
				//  destination etc.  The user does not have to specify all fields but
				//  if they have provided insufficient information the transfer will
				//  not succeed.  The fields which need to be specified are identical to Browser
				Intent transferIntent = new Intent(Common.mainActivity, FileTransferService.class);
				//  Add data to the intent
				transferIntent.putExtra(FileTransferService.TransferProtocol, protocol);
				transferIntent.putExtra(FileTransferService.FileDestination, false);
				transferIntent.putExtra(FileTransferService.CreateFolders, false);
				transferIntent.putExtra(FileTransferService.Port, destinationUrl.getPort());
				transferIntent.putExtra(FileTransferService.Source, "/" + mFilename);
				transferIntent.putExtra(FileTransferService.Destination, mDestination);
				transferIntent.putExtra(FileTransferService.Overwrite, true);
				transferIntent.putExtra(FileTransferService.Username, mUsername);
				transferIntent.putExtra(FileTransferService.Password, mPassword);
				transferIntent.putExtra(FileTransferService.TransferEvent, mImagerEvent);
				//  Specify our Broadcast receiver to be notified when the File Transfer has finished
				transferIntent.putExtra(FileTransferService.IntentFilter, IMAGER_PLUGIN_TRANSFER_COMPLETE);
				//  Return ID allows us to distinguish between responses from the FileTransferService
				transferIntent.putExtra(FileTransferService.ReturnID, IMAGER_TRANSFER_RETURN_ID);
				//  Start the File Transfer Service
				Common.mainActivity.startService(transferIntent);
				}
			      }
				else
				   {
				    Intent transferIntent = new Intent(Common.mainActivity, FileTransferService.class);
				//  Add data to the intent
				    transferIntent.putExtra(FileTransferService.TransferProtocol, protocol);
				    transferIntent.putExtra(FileTransferService.FileDestination, false);
				    transferIntent.putExtra(FileTransferService.CreateFolders, false);
			      	    transferIntent.putExtra(FileTransferService.Port, destinationUrl.getPort());
				    transferIntent.putExtra(FileTransferService.Source, "/" + mFilename);
				    transferIntent.putExtra(FileTransferService.Destination, mDestination);
				    transferIntent.putExtra(FileTransferService.Overwrite, true);
				    transferIntent.putExtra(FileTransferService.Username, mUsername);
				    transferIntent.putExtra(FileTransferService.Password, mPassword);
				    transferIntent.putExtra(FileTransferService.TransferEvent, mImagerEvent);
				//  Specify our Broadcast receiver to be notified when the File Transfer has finished
				    transferIntent.putExtra(FileTransferService.IntentFilter, IMAGER_PLUGIN_TRANSFER_COMPLETE);
				//  Return ID allows us to distinguish between responses from the FileTransferService
				    transferIntent.putExtra(FileTransferService.ReturnID, IMAGER_TRANSFER_RETURN_ID);
				//  Start the File Transfer Service
				    Common.mainActivity.startService(transferIntent);	
				    }
	
			}
			else
			{
			Logger.E(TAG, " null protocol has passed");	
			}
		}
		catch (MalformedURLException e)
		{
			Logger.E(TAG, "Passed URL is not formatted correctly");
		}
		mCaptureInProgress = false;
	}
	
	private void sendImageToDataURI(String encodedImage)
	{
	    try
	    {		
		//navigate(mCaptureEvent, IMAGE_DATA, "data:image/png;base64,"+encodedImage);
		//Common.elementsCore.base64Data = encodedImage;
		navigate(mCaptureEvent, IMAGE_DATA, "data:image/png;base64," + encodedImage);		
	    }
	    catch (NavigateException e)
	    {
		Logger.E(TAG, "Navigate Exception.. length is " + e.GetLength());
	    }
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Imager");
	}

	@Override
	public void onPause()
	{
	    Logger.D(TAG, "onPause+");
	    if (mPreviewWindow != null)
	    {
		mPreviewWindow.removeAllViews();
		mPreviewWindow = null;
		mSleepMode = true;
	    }
	}
	
	@Override
	public void onResume()
	{
	    Logger.D(TAG, "onResume+");
	    if (mSleepMode)
	    {
		mSleepMode = false;
		// Create our Preview view and set it as the content of our activity.
		mPreviewWindow = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","camera_panel"));
		Preview preview = new Preview(Common.mainActivity.getApplicationContext());
		preview.setZOrderMediaOverlay(true); //Stops WebView from trying to show ontop (as its presumeably a SurfaceView)
		preview.setClickable(true);
		
		mPreviewWindow.addView(preview, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));			
	    }
	    
	    //  An Application focus change is received 
	    //  Application has become active.
	    // If the window is already visible then just show it
	    if(mIsEnabled)
	    {			
		onSetting(new PluginSetting("imager", "enabled", null));
	    }
	}
	
	@Override
	public void onSetting(PluginSetting setting)
	{
		boolean paramsChanged = false;
		boolean previewChanged = false;
		
		String name = setting.getName().toLowerCase();
		String value = setting.getValue();
		
		if (value.length() > 0)
			Logger.I(TAG, "'" + name + "', '" + value + "'");
		else
			Logger.I(TAG, name);
		
		if (name.equals("enabled") || name.equals("enable"))
		{		     
			if (Common.elementsCore.isScannerEnabled())
		    {
		    	Camera mCam = null;
		    	try{
		    		mCam = Camera.open(mCameraId);
		    	}
		    	catch (RuntimeException e) 
		        {
		        	Logger.E(TAG, e.getMessage() + "Unable to Open Imager. Please ensure that the selected imager engine is not is use by some other object/application. An inuse engine cannot be used unless the object already using it frees it up!");
	                return;
		        }
		    	finally
		    	{
	                if(mCam !=null){
	                	mCam.release();
	                }
	                mCam = null;
		    	}
		    }
			mIsEnabled = true;
		    if (Common.elementsCore.bLaunchingAppHasFocus)
		    {
			int cameraId = mCameraId;
			try
			{
			    if ((value.length() > 0) && (Integer.valueOf(value.substring(3)) < Camera.getNumberOfCameras()))
			    {
				cameraId = Integer.parseInt(value.substring(3));
				Logger.I(TAG, "Camera Id set to " + cameraId);
			    }
			    else
			    {
				cameraId = mDefaultCameraId;
			    }
			}
			catch (NumberFormatException e)
			{
			    cameraId = mDefaultCameraId;
			    Logger.W(TAG, "Invalid CameraId. Fallback to default. " + e.getMessage());
			}
			catch (Exception e)
			{
				Logger.E(TAG, "New Exception " + e.getMessage());
				return;
			}
	
			if ((mPreviewWindow != null) && (cameraId != mCameraId))
				{
					// camera has been changed during preview
					mPreviewWindow.removeAllViews();
					mPreviewWindow = null;
				}
				mCameraId = cameraId;
				
				if (mPreviewWindow == null && Camera.getNumberOfCameras() > 0)
				{
					// Create our Preview view and set it as the content of our activity.
					mPreviewWindow = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","camera_panel"));
					Preview preview = new Preview(Common.mainActivity.getApplicationContext());
					preview.setZOrderMediaOverlay(true); //Stops WebView from trying to show ontop (as its presumeably a SurfaceView)
					preview.setClickable(true);
					
					//new AbsoluteLayout(Common.mainActivity.getApplicationContext());
					//mPreviewWindow.setLayoutParams(new ViewGroup.LayoutParams(
					//		ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT));
									
					mPreviewWindow.addView(preview, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));
					
					//Common.mainActivity.addContentView(mPreviewWindow, mPreviewWindow.getLayoutParams());
				}
			}			
		}
		
		else if (name.equals("disabled") || name.equals("disable"))
		{
		    if (!mIsEnabled)
			return;
		    mIsEnabled = false;
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				if (mPreviewWindow != null)
				{
					mPreviewWindow.removeAllViews();
					mPreviewWindow = null;
				}
			}			
		}

		else if (name.equals("imagerevent"))
		{
			//if(value != null && value.toLowerCase().startsWith("url:")) value = value.substring(4); //Remove the new url: prefix
			mImagerEvent = value;
		}

		else if (name.equals("capture"))
		{
//			//TODO this stuff can be removed once the camera libraries are fixed.
//			boolean brokenDevice = false;
//			
//			for(String version: BROKEN_DEVICES_VERSION_INC)
//			{
//				if(version.equals(Build.VERSION.INCREMENTAL))
//				{
//					Logger.W(TAG, "EV2 Device. Using fallback capture technique: Captured images can only be the size of the Preview Window.");
//					brokenDevice = true;
//					break;
//				}
//			}
//			if(brokenDevice)
//			{
//				doDirtyCapture();
//			}
//			else
//			{
			if (Common.elementsCore.bLaunchingAppHasFocus)
			{
				doCapture();
			}
			else
			{
				Logger.W(TAG, "Cannot capture from a background application");
			}		
//			}
			//TODO end.
			//doCapture();
		}
		
		else if (name.equals("imagerenumevent"))
		{
			//if(value != null && value.toLowerCase().startsWith("url:")) value = value.substring(4); //Remove the new url: prefix
			mImagerEnumEvent = value;
		}

		else if (name.equals("enumerate"))
		{
			CameraInfo info = new CameraInfo();
			JSONArray j = new JSONArray();
			// return a list of cameras
			for (int i=0; i < Camera.getNumberOfCameras(); i++)
			{
				Camera.getCameraInfo(i, info);
				JSONObject o = new JSONObject();
				try
				{
					o.put(Plugin.jsonObjNVPOrderString, "deviceName,friendlyName"); //Add the order in which the NVPs should be converted to strings
					o.put("deviceName","cam"+i);
					if (info.facing == CameraInfo.CAMERA_FACING_FRONT)
					{
						o.put("friendlyName","Front Facing Camera");
					}
					else
					{
						o.put("friendlyName","Back Facing Camera");
					}
					j.put(o);
				}
				catch (JSONException e)
				{
					Logger.W(TAG, e.getMessage());
				}
			}
			try
			{
				navigate(mImagerEnumEvent, "imagerArray", j);
			}
			catch (NavigateException e)
			{
				Logger.E(TAG, "Navigate Exception.. length is " + e.GetLength());
			}
		}
		
		else if (name.equals("left"))
		{
			try
			{
				int left = (int) Double.parseDouble(value); 
				if (mLeft != left)
				{
					mLeft = left;
					previewChanged = true;
				}
				
			}
			catch (NumberFormatException e)
			{
				Logger.E(TAG, "Error setting Left parameter - " + value);				
			}
		}
		
		else if (name.equals("top"))
		{
			try
			{
				int top = (int) Double.parseDouble(value); 
				if (mTop != top)
				{
					mTop = top;
					previewChanged = true;
				}
			}
			catch (NumberFormatException e)
			{
				Logger.E(TAG, "Error setting Top parameter - " + value);				
			}
		}
		
		else if (name.equals("width"))
		{
			try
			{
				int width = (int) Double.parseDouble(value); 
				if ((width > 0) && (mWidth != width))
				{
					mWidth = width;
					previewChanged = true;
				}
			}
			catch (NumberFormatException e)
			{
				Logger.E(TAG, "Error setting Width parameter - " + value);				
			}
		}
		
		else if (name.equals("height"))
		{
			try
			{
				int height = (int) Double.parseDouble(value);
				
				// GD - Set minimum height to 200 pixels to avoid problem with camera API in BSP
				if (height < 200)
					height = 200;
				
				if ((height > 0) && (mHeight != height))
				{
					mHeight = height;
					previewChanged = true;
				}
			}
			catch (NumberFormatException e)
			{
				Logger.E(TAG, "Error setting Height parameter - " + value);				
			}
		}
		else if (name.equals("desiredwidth"))
		{
			try
			{
				int width = (int)Double.parseDouble(value);
				if (width > 0 && m_iDesiredWidth != width)
				{
					m_iDesiredWidth = width;
					previewChanged = true;
				}
			}
			catch (NumberFormatException e)
			{
				Logger.E(TAG, "Error setting Desired Width parameter - " + value);				
			}
			
		}
		else if (name.equals("desiredheight"))
		{
			try
			{
				int height = (int)Double.parseDouble(value);
				if (height > 0 && m_iDesiredHeight != height)
				{
					m_iDesiredHeight = height;
					previewChanged = true;
				}
			}
			catch (NumberFormatException e)
			{
				Logger.E(TAG, "Error setting Desired Height parameter - " + value);				
			}
			
		}		
		else if (name.equals("lamp"))
		{
			String lampStr = value.toLowerCase();
			
			if ((lampStr.equals("on")) && (mFlashMode != Camera.Parameters.FLASH_MODE_TORCH))
			{
				mLastFlashMode = mFlashMode;
				mFlashMode = Camera.Parameters.FLASH_MODE_TORCH;
				paramsChanged = true;
			}
			else if ((lampStr.equals("off")) && (mFlashMode == Camera.Parameters.FLASH_MODE_TORCH))
			{
				mFlashMode = mLastFlashMode;
				paramsChanged = true;
			}
		}

		else if (name.equals("flash"))
		{
			if (value.equalsIgnoreCase("on"))
			{
				mLastFlashMode = Camera.Parameters.FLASH_MODE_ON;
				if (mFlashMode != Camera.Parameters.FLASH_MODE_TORCH)
				{
					mFlashMode = Camera.Parameters.FLASH_MODE_ON;
					paramsChanged = true;
				}
			}
			else if (value.equalsIgnoreCase("off"))
			{
				mLastFlashMode = Camera.Parameters.FLASH_MODE_OFF;
				if (mFlashMode != Camera.Parameters.FLASH_MODE_TORCH)
				{
					mFlashMode = Camera.Parameters.FLASH_MODE_OFF;
					paramsChanged = true;
				}
			}
			else if (value.equalsIgnoreCase("auto"))
			{
				mLastFlashMode = Camera.Parameters.FLASH_MODE_AUTO;
				if (mFlashMode != Camera.Parameters.FLASH_MODE_TORCH)
				{
					mFlashMode = Camera.Parameters.FLASH_MODE_AUTO;
					paramsChanged = true;
				}
			}
			else if (value.equalsIgnoreCase("red-eye"))
			{
				mLastFlashMode = Camera.Parameters.FLASH_MODE_RED_EYE;
				if (mFlashMode != Camera.Parameters.FLASH_MODE_TORCH)
				{
					mFlashMode = Camera.Parameters.FLASH_MODE_RED_EYE;
					paramsChanged = true;
				}
			}
		}
		else if (name.equals("aim"))
		{
			if (value.equalsIgnoreCase("on"))
			{
				mAim = 1;
				Logger.E(TAG, "Aim support not implemented");
			}
			else if (value.equalsIgnoreCase("off"))
			{
				mAim = 0;
			}
		}
		else if (name.equals("name"))
		{
			if (value.length() > 0)
			{
				mFilename = Common.getDataPath() + "/" + value; //TODO look at me.
			}
			else
			{
				mFilename = Common.getDataPath() + "/Img";
			}
//			if (value.length() > 0)
//			{
//				mFilename = value; //TODO look at me.
//			}
//			else
//			{
//				mFilename = "Img.jpg";
//			}
		}
		else if (name.equals("username"))
		{
			String uname = value;
			if (uname.equals(""))
			{
				mUsername = null;
			}
			else
			{
				mUsername = uname;
			}
		}
		else if (name.equals("password"))
		{
			String pword = value;
			if (pword.equals(""))
			{
				mPassword = null;
			}
			else
			{
				mPassword = pword;
			}
		}
		else if (name.equals("protocol"))
		{
			mProtocol = FileTransferService.parseProtocol(value);
		}
		else if (name.equals("destination"))
		{
			try
			{
				if(value.startsWith("url:"))
					value=value.substring(4);
				URL destination = new URL(value);
				String path = destination.getPath();
				if(!path.substring(Math.max(path.lastIndexOf('/'), path.lastIndexOf('\\')) + 1).contains("."))
				{
					mDestination = value + ".jpg";
				}
				else
				{
					mDestination = value;
				}
			}
			catch (MalformedURLException e)
			{
				Logger.W(TAG, "Bad destination URL.");	
			}
			catch(IndexOutOfBoundsException e)
			{
				Logger.W(TAG, "Bad destination URL.");
			}
		}
		else if (name.equals("sound"))
		{
			mSound = value;
		}
		else if (name.equalsIgnoreCase("imagercaptureevent"))
		{
			//if(value != null && value.toLowerCase().startsWith("url:")) value = value.substring(4); //Remove the new url: prefix
		    mCaptureEvent = value;
		}
		
		//Update the viewfinder window only if it is in the foreground
		if (Common.elementsCore.bLaunchingAppHasFocus)
		{
			if ((previewChanged) && (mPreviewWindow != null))
			{
				mPreviewWindow.removeAllViews();
				Preview preview = new Preview(Common.mainActivity.getApplicationContext());
				preview.setZOrderMediaOverlay(true); //Stops WebView from trying to show ontop (as its presumeably a SurfaceView)
				preview.setClickable(true);
				mPreviewWindow.addView(preview, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));
			}
			
			if ((paramsChanged) && (mCamera != null))
			{
				Camera.Parameters params = mCamera.getParameters();
				if (params != null)
				{
					List<String> flashModes = params.getSupportedFlashModes();
					if ((flashModes != null) && (flashModes.contains(mFlashMode)))
					{
						params.setFlashMode(mFlashMode);
						mCamera.setParameters(params);
					}
				}
			}
		}
	}

	
	private void doCapture()
	{
		if ((mPreviewWindow != null) && (mCamera != null) && (!mCaptureInProgress))
		{
			mCaptureInProgress = true;
			playShutterSound();
//			mCamera.takePicture(mShutterCallback, null, mPictureCallback);
			mCamera.takePicture(mShutterCallback, null, mPostViewCallback, mJpegCallback);
		}
	}
	
	private void playShutterSound()
	{
		if ((mSound != null) && (!mSound.equals("")))
		{
			File captureSound = null;
			try
			{
				captureSound = Common.getLocalFile(mSound);
				if ((captureSound.exists()) && (captureSound.canRead()))
				{
					AudioManager am = (AudioManager) Common.mainActivity.getSystemService(Context.AUDIO_SERVICE);
					if (am.getStreamVolume(AudioManager.STREAM_SYSTEM) > 0)
					{
						// mute the System audio stream to disable the default camera click
						currentVolume = am.getStreamVolume(AudioManager.STREAM_SYSTEM);
						am.setStreamMute(AudioManager.STREAM_SYSTEM, true);
						am.setStreamVolume(AudioManager.STREAM_SYSTEM, 0, AudioManager.FLAG_REMOVE_SOUND_AND_VIBRATE);
						mShutterClickMute = true;
					}
					
					if(mPlayer != null)
					{
						mPlayer.stop();
						mPlayer = null;
					}
					mPlayer = new MediaPlayer();
					mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
					if (am.getMode() != AudioManager.MODE_NORMAL)
					{
						am.setMode(AudioManager.MODE_NORMAL);
					}
					if (!am.isSpeakerphoneOn())
					{
						am.setSpeakerphoneOn(true);
					}
					mPlayer.setOnPreparedListener(new OnPreparedListener()
					{
						@Override
						public void onPrepared(MediaPlayer mp)
						{
							mp.start();
						}
					});
					try
					{
						mPlayer.setDataSource(captureSound.getAbsolutePath());
					}
					catch (IllegalArgumentException e)
					{
						Logger.W(TAG, e.getMessage());
						mSound = null;
					}
					catch (IllegalStateException e)
					{
						Logger.W(TAG, e.getMessage());
					}
					catch (IOException e)
					{
						Logger.W(TAG, e.getMessage());
					}
				}
				else
				{
					mSound = null;
					Logger.W(TAG, "Sound file doesn't exist: " + captureSound.getAbsolutePath());
				}
			}
			catch (InvalidParameterException e1)
			{
				Logger.W(TAG, e1.getMessage());
				mSound = null;
			}
			catch (URISyntaxException e1)
			{
				Logger.W(TAG, "Bad sound file URL");
				mSound = null;
			}
		}
		else if (mPlayer != null)
		{
			mPlayer.stop();
			mPlayer = null;
		}
	}

	@Override
	public void onPageStarted(String str)
	{
	    if (mPreviewWindow != null)
	    {
		mPreviewWindow.removeAllViews();
		mPreviewWindow = null;
	    }
	    mIsEnabled = false;
	}

	@Override
	public void onShutdown()
	{
	    if (mReceiver != null)
	    {
		Common.mainActivity.unregisterReceiver(mReceiver);
	    }
	}

	/**
	 * @author vbc863
	 *
	 */
	class Preview extends SurfaceView implements SurfaceHolder.Callback {
	    SurfaceHolder mHolder;

	    Preview(Context context) 
	    {
	        super(context);
			Logger.I(TAG, "Preview+");

	        // Install a SurfaceHolder.Callback so we get notified when the
	        // underlying surface is created and destroyed.
	        mHolder = getHolder();
	        mHolder.addCallback(this);
	        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
	    }

	    @Override
	    public void surfaceCreated(SurfaceHolder holder) 
	    {
	        // The Surface has been created, acquire the camera and tell it where
	        // to draw.
	    	Logger.I(TAG, "surfaceCreated+");
	        try 
	        {
	            mCamera = Camera.open(mCameraId);
	            mCamera.setPreviewDisplay(holder);
	        } 
	        catch (IOException e) 
	        {
	            Logger.E(TAG, e.getMessage());
	            mCamera.release();
	            mCamera = null;
	        }
	        catch (RuntimeException e) 
	        {
	            Logger.E(TAG, e.getMessage());
	            mCamera.release();
	            mCamera = null;
	        }
	    }

	    @Override
		public void surfaceDestroyed(SurfaceHolder holder) 
	    {
	        // Surface will be destroyed when we return, so stop the preview.
	        // Because the CameraDevice object is not a shared resource, it's very
	        // important to release it when the activity is paused.
			Logger.I(TAG, "surfaceDestroyed+");
	        mCamera.stopPreview();
	        mCamera.release();
	        mCamera = null;
	    }

	    @Override
		public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) 
	    {
	        // Now that the size is known, set up the camera parameters and begin
	        // the preview.
			Logger.I(TAG, "Width: " + w + " Height: " + h);
	        Camera.Parameters parameters = mCamera.getParameters();
	        
	        List<Size> sizes = parameters.getSupportedPreviewSizes();
	        Size optimalSize = getOptimalPreviewSize(sizes, w, h);
	        parameters.setPreviewSize(optimalSize.width, optimalSize.height);
	        if (mFlashMode != null)
	        {
	        	List<String> fmodes = parameters.getSupportedFlashModes();
	        	if ((fmodes != null) && (fmodes.contains(mFlashMode)))
	        	{
	        		parameters.setFlashMode(mFlashMode);
	        	}
	        }
	        mCamera.setParameters(parameters);

	        
	        //  DCC
	        //  Set the camera resolution, the user has specified a desired
	        //  width and height of the captured image (-1 if the user
	        //  has not specified a value) so choose the most appropriate 
	        //  resolution.
	        Camera.Parameters pictureParameters = mCamera.getParameters();
	        List<Size> pictureSizes = pictureParameters.getSupportedPictureSizes();
	        boolean bUserResolutionFound = false;
	        int iGreatestWidth = 0;
	        int iGreatestHeight = 0;
	        if (pictureSizes != null && 
	        		(!(m_iDesiredWidth == -1 && m_iDesiredWidth == -1)))
	        {
		        for (Size pictureSize : pictureSizes) 
		        {
		        	if (pictureSize != null && 
		        		(pictureSize.width >= m_iDesiredWidth) &&
		        		(pictureSize.height >= m_iDesiredHeight))
		        		{
		        			bUserResolutionFound = true;
		        			pictureParameters.setPictureSize(pictureSize.width, pictureSize.height);
		        			//  Do not break out of the loop here to account for both ascending and descending
		        			//  supported picture sizes arrays.
		        		}
		        	if (iGreatestWidth < pictureSize.width)
		        	{
		        		iGreatestWidth = pictureSize.width;
		        		iGreatestHeight = pictureSize.height;
		        	}
		        }
		        if (!bUserResolutionFound && iGreatestWidth > 0)
		        {
		        	//  The user specified resolution was too great for the camera, default to the highest
		        	//  camera resolution
		        	Logger.W(TAG, "User specified image resolution is too high, defaulting to highest possible camera resolution");
		        	pictureParameters.setPictureSize(iGreatestWidth, iGreatestHeight);
		        }
		        Logger.I(TAG, "Setting Camera Resolution to " + pictureParameters.getPictureSize().width + "(width) x " + pictureParameters.getPictureSize().height + " (height)");
		        mCamera.setParameters(pictureParameters);

	        }


	        mCamera.setDisplayOrientation(mRotation);
	        mCamera.startPreview();	      
	    }

	    @Override
		public void onLayout(boolean changed, int left, int top, int right, int bottom)
	    {
	    	super.onLayout(changed, left, top, right, bottom);
	    	int rotation = 0;

	    	Display display = ((WindowManager) Common.mainActivity.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
	    	int naturalOrientation = getDeviceDefaultOrientation();
	    	
	    	final CameraInfo info = new android.hardware.Camera.CameraInfo();
	    	Camera.getCameraInfo(mCameraId, info);
	    	 	
	    	boolean isFrontCamera = info.facing == CameraInfo.CAMERA_FACING_FRONT;
	    	//TODO replace with a "works with all" code. This has yet to be done because I am wondering if this is a bug in
	    	//The tablet code.
	    	if(Build.MODEL.toLowerCase().startsWith("et1") && Build.VERSION.SDK_INT >= 14)
	    	{
	        	switch (display.getOrientation())
		    	{
			    	case Surface.ROTATION_0:
			    		rotation = (isFrontCamera ? 90 : 270);
			    		break;
		
			    	case Surface.ROTATION_90:
			    		rotation = (isFrontCamera ? 0 : 180);
			    		break;
		
			    	case Surface.ROTATION_180:
			    		rotation = (isFrontCamera ? 270 : 90);
			    		break;
		
			    	case Surface.ROTATION_270:
			    		rotation = (isFrontCamera ? 180 : 0);
			    		break;
		    	}
	    	}
	    	else if(Build.MODEL.toLowerCase().startsWith("mc40") && Build.VERSION.SDK_INT >= 14 && Build.VERSION.SDK_INT < 19)
	    	{
	        	switch (display.getOrientation())
		    	{
			    	case Surface.ROTATION_0:
			    		rotation = 0;
			    		break;
		
			    	case Surface.ROTATION_90:
			    		rotation = 270;
			    		break;
		
			    	case Surface.ROTATION_180:
			    		rotation = 180;
			    		break;
		
			    	case Surface.ROTATION_270:
			    		rotation = 90;
			    		break;
		    	}
	    	}
	    	else if(naturalOrientation == Configuration.ORIENTATION_LANDSCAPE) //Landscape device, like XOOM
	    	{
	        	switch (display.getOrientation())
		    	{
			    	case Surface.ROTATION_0:
			    		rotation = 0;
			    		break;
		
			    	case Surface.ROTATION_90:
			    		rotation = 270;
			    		break;
		
			    	case Surface.ROTATION_180:
			    		rotation = 180;
			    		break;
		
			    	case Surface.ROTATION_270:
			    		rotation = 90;
			    		break;
		    	}
	    	}
	    	else //Configuration.ORIENTATION_PORTRAIT
	    	{
	    		switch (display.getOrientation())
	    		{
	    			case Surface.ROTATION_0:
	    				rotation = 90;
	    				break;
	    				
	    			case Surface.ROTATION_90:
	    				rotation = 0;
	    				break;
	    				
	    			case Surface.ROTATION_180:
	    				rotation = 270;
	    				break;
	    				
	    			case Surface.ROTATION_270:
	    				rotation = 180;
	    				break;
	    		}
	    	}


	    	if (rotation != mRotation)
	    	{
	    		mRotation = rotation;
	    		if (mCamera != null)
	    		{
	    			mCamera.stopPreview();
	    			mCamera.setDisplayOrientation(mRotation);
	    			mCamera.startPreview();
	    			if (mFlashMode != null)
	    			{
	    				Camera.Parameters parameters = mCamera.getParameters();
	    				List<String> fmodes = parameters.getSupportedFlashModes();
	    				if ((fmodes != null) && (fmodes.contains(mFlashMode)))
	    				{
	    					parameters.setFlashMode(mFlashMode);
	    					mCamera.setParameters(parameters);
	    				}
	    			}
	    		}
	    	}
			Logger.I(TAG, "left: " + left + " top: " + top + " right: " + right + " bottom: " + bottom + " rotation: " + mRotation);
		}
	    
	    /**
	     * @param sizes
	     * @param w
	     * @param h
	     * @return
	     */
	    private Size getOptimalPreviewSize(List<Size> sizes, int w, int h) 
	    {
			Logger.I(TAG, "Width: " + w + " Height: " + h);
	        final double ASPECT_TOLERANCE = 0.05;
	        double targetRatio = (double) w / h;
	        if (sizes == null) return null;

	        Size optimalSize = null;
	        double minDiff = Double.MAX_VALUE;

	        int targetHeight = h;

	        // Try to find a size match for aspect ratio and size
	        for (Size size : sizes) 
	        {
				Logger.I(TAG, "supported sizes - Width: " + size.width + " Height: " + size.height);
	            double ratio = (double) size.width / size.height;
	            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE)
	            	continue;
	            if (Math.abs(size.height - targetHeight) < minDiff) 
	            {
	                optimalSize = size;
	                minDiff = Math.abs(size.height - targetHeight);
	            }
	        }

	        // Cannot find a match for aspect ratio, just find the closest size
	        if (optimalSize == null) 
	        {
	            minDiff = Double.MAX_VALUE;
	            for (Size size : sizes) 
	            {
	                if (Math.abs(size.height - targetHeight) < minDiff) 
	                {
	                    optimalSize = size;
	                    minDiff = Math.abs(size.height - targetHeight);
	                }
	            }
	        }
			Logger.I(TAG, "optimal size - Width: " + optimalSize.width + " Height: " + optimalSize.height);
	        return optimalSize;
	    }
	}
	
	/**
	 * Class to receive the intent sent from the FileTransferService when the File Transfer
	 * has completed.  We configure the intent in such a way so that we are the only recipient.
	 * @author John Heywood, VBC863.  Initial Creation, March 2011
	 */
	public class ImageTransferBroadcastReceiver extends BroadcastReceiver
	{
		/* (non-Javadoc)
		 * @see android.content.BroadcastReceiver#onReceive(android.content.Context, android.content.Intent)
		 */
		@Override
		public void onReceive(Context arg0, Intent receivedIntent)
		{
		    if ( (receivedIntent == null) || (receivedIntent.getIntExtra(FileTransferService.PID, android.os.Process.myPid()) != android.os.Process.myPid()) )
	    		    return;
		    
		    Bundle b = receivedIntent.getExtras();
		    //  Extract the values associated with the return Intent
		    //  File Transfer does not use return ID	
		    String returnID = b.getString(FileTransferService.ReturnID);
		    final String returnValue = b.getString(FileTransferService.ReturnValue);
		    final String transferEvent = b.getString(FileTransferService.TransferEvent);
		    	
		    if (returnID.equals(IMAGER_TRANSFER_RETURN_ID))
		    {
			mHandler.post(new Runnable()
			{
			    @Override
			    public void run()
			    {
				Logger.I(TAG, transferEvent + " : " + returnValue);
				//  Navigate to the specified URL
				if (transferEvent != null && returnValue != null)
				{
					try
					{
					    navigate(transferEvent, "transferResult", returnValue);
					}
					catch (NavigateException e)
					{
						Logger.E(TAG, "Navigate Exception.. length is " + e.GetLength());
					}
				}
				else if (transferEvent != null)
				    navigate(transferEvent);	
			    }
			});
		    }
		}
	}
	
	private void dumpParams()
	{
		Logger.I(TAG, "Name: " + this.mFilename);
		Logger.I(TAG, "Camera ID: " + this.mCameraId);
		Logger.I(TAG, "Top: " + this.mTop);
		Logger.I(TAG, "Left: " + this.mLeft);
		Logger.I(TAG, "Width: " + this.mWidth);
		Logger.I(TAG, "Height: " + this.mHeight);
		Logger.I(TAG, "Flash: " + this.mFlashMode);
		Logger.I(TAG, "Destination: " + this.mDestination);
		Logger.I(TAG, "Protocol: " + this.mProtocol);
		Logger.I(TAG, "Username: " + this.mUsername);
		Logger.I(TAG, "Password: " + this.mPassword);
		Logger.I(TAG, "ImagerEvent: " + this.mImagerEvent);
		Logger.I(TAG, "ImagerEnumEvent: " + this.mImagerEnumEvent);
		Logger.I(TAG, "Sound: " + this.mSound);
	}

	/**
	 * Returns whether the device is naturally portrait or landscape.
	 * @return Configuration.ORIENTATION_LANDSCAPE if landscape, Configuration.ORIENTATION_PORTRAIT if portrait
	 * @author http://stackoverflow.com/users/1035292/user1035292
	 */
	public static int getDeviceDefaultOrientation()
	{
	    WindowManager windowManager =  (WindowManager) RhodesActivity.safeGetInstance().getSystemService(Activity.WINDOW_SERVICE);
	    Configuration config = RhodesActivity.safeGetInstance().getResources().getConfiguration();
	    int rotation = windowManager.getDefaultDisplay().getRotation();

	    if ( ((rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) && config.orientation == Configuration.ORIENTATION_LANDSCAPE)
	        || ((rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270) && config.orientation == Configuration.ORIENTATION_PORTRAIT))
	    {
	    	return Configuration.ORIENTATION_LANDSCAPE;
	    }
	    else
	    {
	    	return Configuration.ORIENTATION_PORTRAIT;
	    }
	}
}
