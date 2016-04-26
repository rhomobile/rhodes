/**
 * 
 */
package com.rho.rhoelements.plugins;

import java.io.File;
import java.io.FilenameFilter;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.security.InvalidParameterException;
import java.util.Calendar;
import java.util.HashMap;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.widget.AbsoluteLayout;
import android.widget.Button;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.services.FileTransferService;
import com.rho.rhoelements.services.FileTransferService.FileTransferProtocols;
import com.rhomobile.rhodes.Logger;

/**
 * @author FPC843
 * Abstract class extended by all the buttons
 */
@SuppressWarnings("deprecation")
public abstract class ButtonPlugin extends Plugin
{
	protected Button mButton = null;
	protected AbsoluteLayout mButtonPanel;
	public static final String IMG_DESTINATION_FOLDER = "/Android/data/" + Common.packageName + "/img";
	protected static String TAG = "ButtonPlugin";
	protected int mLeft = 0;
	protected int mTop = 0;
	protected int mHeight = 0;
	protected int mWidth = 0;
	private HashMap<String, FileTransferProtocols> mProtocolMap = null;
	private BroadcastReceiver mImageTransferReceiver;
	private String mButtonUpFileName = null;
	private String mButtonDownFileName = null;
	protected Drawable mDrawableUp = null;
	protected Drawable mDrawableDown = null;
	protected String mIntentId;
	protected String mResultIdUp;
	protected String mResultIdDown;
	protected DisplayMetrics mDisplayMetrics = null;
	private String mButtonClassName = null;
	private short mFileTransferCounter = 0;
	
	/**
	 * 
	 */
	ButtonPlugin()
	{
		mProtocolMap = new HashMap<String, FileTransferProtocols>();
		mProtocolMap.put("file", FileTransferProtocols.PROTOCOL_FILE);
		mProtocolMap.put("http", FileTransferProtocols.PROTOCOL_HTTP);
		mProtocolMap.put("ftp", FileTransferProtocols.PROTOCOL_FTP);
		
		mDisplayMetrics = new DisplayMetrics();
		Common.mainActivity.getWindowManager().getDefaultDisplay().getMetrics(mDisplayMetrics);
		
		//Set default size proportionally to the greatest dimension (based on current screen orientation)
		int greatestDim = mDisplayMetrics.heightPixels >= mDisplayMetrics.widthPixels ? mDisplayMetrics.heightPixels : mDisplayMetrics.widthPixels;
		mHeight = mWidth = greatestDim / 20 + 2;
	}
	
	protected void completeInit(String intentId, String resultIdUp, String resultIdDown)
	{
		mImageTransferReceiver = new BroadcastReceiver() {
			
			@Override
			public void onReceive(Context context, Intent intent) {
				Logger.D(TAG, "Image received");
				Bundle bundle = intent.getExtras();
				if (bundle == null)
					return;
				final String returnId = bundle.getString(FileTransferService.ReturnID);
				final String returnValue = bundle.getString(FileTransferService.ReturnValue);
				Logger.D(TAG, "FileTransferService returned: " + returnValue);
				mFileTransferCounter--;
				if (returnValue.contains("rror") == false)
				{
					try
					{
						if (returnId.compareToIgnoreCase(mResultIdUp) == 0)
						{
							Drawable update = Drawable.createFromPath(mButtonUpFileName);
							if (update != null)
								mDrawableUp = update;
						}
						else if (returnId.compareToIgnoreCase(mResultIdDown) == 0)
						{
							Drawable update = Drawable.createFromPath(mButtonDownFileName);
							if (update != null)
								mDrawableDown = update;	
						}
						
						StateListDrawable newStates = new StateListDrawable();
						newStates.addState(new int[] {android.R.attr.state_pressed}, mDrawableDown);
						newStates.addState(new int[] {android.R.attr.state_focused}, mDrawableUp);
						newStates.addState(new int[] { }, mDrawableUp);
						mButton.setBackgroundDrawable(newStates);
					}
					catch (OutOfMemoryError e) //Thrown when the loaded image is too big
					{
						Logger.E(TAG, e.getMessage());
					}
				}
				if (mFileTransferCounter == 0)
					cleanUpTempFiles();
			}
		};
		IntentFilter intentFilter = new IntentFilter(mIntentId);
		Common.mainActivity.registerReceiver(mImageTransferReceiver, intentFilter);
		
		mButtonPanel.removeAllViews();
		mButtonPanel.addView(mButton, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));
		
		mButtonClassName = this.toString().substring(this.toString().lastIndexOf(".") + 1, this.toString().lastIndexOf("@"));
	}
	
	@Override
	public void onSetting(PluginSetting setting)
	{
		Logger.D(TAG, "Start");
		
		if (mButton == null)
		{
			Logger.E(TAG, "Button is null");
			return;
		}
		if (setting == null)
		{
			Logger.E(TAG, "setting is null");
			return;
		}
		
		try
		{
			if (setting.getName().equalsIgnoreCase("Left"))
			{
				Logger.D(TAG, "Left");
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mLeft = Integer.valueOf(setting.getValue());
					mButtonPanel.removeAllViews();
					mButtonPanel.addView(mButton, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));
				}
			}
			else if (setting.getName().equalsIgnoreCase("Top"))
			{
				Logger.D(TAG, "Top");
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mTop = Integer.valueOf(setting.getValue());
					mButtonPanel.removeAllViews();
					mButtonPanel.addView(mButton, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));
				}
			}
			else if (setting.getName().equalsIgnoreCase("Height"))
			{
				Logger.D(TAG, "Height");
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mHeight = Integer.valueOf(setting.getValue());
					mButtonPanel.removeAllViews();
					mButtonPanel.addView(mButton, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));
				}
			}
			else if (setting.getName().equalsIgnoreCase("Width"))
			{
				Logger.D(TAG, "Width");
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mWidth = Integer.valueOf(setting.getValue());
					mButtonPanel.removeAllViews();
					mButtonPanel.addView(mButton, new AbsoluteLayout.LayoutParams(mWidth, mHeight, mLeft, mTop));
				}
			}
			else if (setting.getName().equalsIgnoreCase("ImageUp"))
			{
				Logger.D(TAG, "ImageUp");
				String newFileName = getButtonPicture(setting.getValue(), true);
				if (newFileName != null)
					mButtonUpFileName = newFileName;
			}
			else if (setting.getName().equalsIgnoreCase("ImageDown"))
			{
				Logger.D(TAG, "ImageDown");
				String newFileName = getButtonPicture(setting.getValue(), false);
				if (newFileName != null)
					mButtonDownFileName = newFileName;
			}
			else if (setting.getName().equalsIgnoreCase("Visibility"))
			{
				Logger.D(TAG, "Visibility");
				setVisibility(setting.getValue());
			}
		}
		catch (NumberFormatException e)
		{
			Logger.E(TAG, "Passed parameter has wrong format");
		}
		catch (MalformedURLException e)
		{
			Logger.E(TAG, "Passed URL is not formatted correctly");
		} 
		catch (InvalidParameterException e)
		{
			Logger.E(TAG, "Passed parameter has wrong format");
		} 
		catch (URISyntaxException e)
		{
			Logger.E(TAG, "Passed URL is not formatted correctly");
		}
		
		Logger.D(TAG, "End");
	}
	
	private String getButtonPicture(String source, boolean isUp) throws MalformedURLException, InvalidParameterException, URISyntaxException
	{
		String res = null;
		if(source != null && source.toLowerCase().startsWith("url:")) source = source.substring(4); //Remove the new url: prefix
		
		URL imageUrl = new URL(source);
		
		String fileName = imageUrl.getFile();
		if ( (fileName == null) || (fileName.length() == 0) )
			return null;
		long timestamp = Calendar.getInstance().getTimeInMillis();
		res = IMG_DESTINATION_FOLDER + "/" + mButtonClassName + timestamp + fileName.substring(fileName.lastIndexOf('/') + 1);
		
		int port = imageUrl.getPort();
		
		Intent transferIntent = new Intent(Common.mainActivity, FileTransferService.class);
		//  Add data to the intent
		transferIntent.putExtra(FileTransferService.Copy, true);
		transferIntent.putExtra(FileTransferService.TransferProtocol, mProtocolMap.get(imageUrl.getProtocol()));
		transferIntent.putExtra(FileTransferService.Port, port);
		transferIntent.putExtra(FileTransferService.FileDestination, true);
		transferIntent.putExtra(FileTransferService.CreateFolders, true);
		transferIntent.putExtra(FileTransferService.Source, source);
		transferIntent.putExtra(FileTransferService.Destination, res);
		transferIntent.putExtra(FileTransferService.Overwrite, true);
		String userInfo = imageUrl.getUserInfo();
		if (( userInfo != null ) && (userInfo.length() > 0) )
		{
			transferIntent.putExtra(FileTransferService.Username, userInfo.substring(0, userInfo.indexOf(':')));
			transferIntent.putExtra(FileTransferService.Password, userInfo.substring(userInfo.indexOf(':') + 1, userInfo.length()));
		}
		//  Specify our Broadcast receiver to be notified when the File Transfer has finished
		transferIntent.putExtra(FileTransferService.IntentFilter, mIntentId);
		//  Return ID allows us to distinguish between responses from the FileTransferService
		String returnId;
		if (isUp)
			returnId = mResultIdUp;
		else
			returnId = mResultIdDown;
		transferIntent.putExtra(FileTransferService.ReturnID, returnId);
		//  Start the File Transfer Service
		Common.mainActivity.startService(transferIntent);
		mFileTransferCounter++;
		return Common.parseAndroidURI(res).getPath();
	}
	

	@Override
	public void onShutdown()
	{
		Logger.I(TAG, "onShutdown+");
		try
		{
			Common.mainActivity.unregisterReceiver(mImageTransferReceiver);
		}
		catch (IllegalArgumentException e)
		{
			Logger.E(TAG, "Image Transfer Receiver wasn't registered");
		}
	}
	
	/**
	 * This method allows to differentiate the behaviour of setting the Visibility property of buttons.
	 * All the buttons behave in the same way but, for instance, the Stop button doesn't, as it's only
	 * visible when a web page is loading and the Visibility property is Visible. Therefore StopButtonPlugin
	 * overrides the following default implementation
	 */
	protected void setVisibility(String visibilityValue)
	{
		if (visibilityValue.compareToIgnoreCase("visible") == 0)
		{
			mButtonPanel.setVisibility(View.VISIBLE);
		}
		else if (visibilityValue.compareToIgnoreCase("hidden") == 0)
		{
			mButtonPanel.setVisibility(View.GONE);
		}
	}
	
	private void cleanUpTempFiles()
	{
		String[] fileList = null;
		File dir = new File(IMG_DESTINATION_FOLDER);
		FilenameFilter filter = new FilenameFilter()
		{
		    public boolean accept(File dir, String name)
		    {
		        return name.contains(mButtonClassName);
		    }
		};
		if (dir != null)
		{
			fileList = dir.list(filter);
			if (fileList != null)
				for (int i=0; i<fileList.length; i++)
				{
					boolean deleteRes = new File(IMG_DESTINATION_FOLDER + "/" + fileList[i]).delete();	
					if (deleteRes)
						Logger.D(TAG, fileList[i] + " deleted");
				}
		}
	}
}
