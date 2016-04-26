/**
 * 
 */
package com.rho.rhoelements.plugins;

import java.io.File;
import java.io.FilenameFilter;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Calendar;
import java.util.HashMap;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.services.FileTransferService;
import com.rho.rhoelements.services.FileTransferService.FileTransferProtocols;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;

/**
 * @author FPC843
 * Abstract class implementing the logic behind command areas
 */
public abstract class CommandAreaPlugin extends Plugin
{
	protected View mCommandArea;
	protected LinearLayout mCommandAreaPanel;
	public static final String IMG_DESTINATION_FOLDER = Common.getDataPath() + "/img";
	protected int mHeight = 0;
	protected int mWidth = 0;
	private HashMap<String, FileTransferProtocols> mProtocolMap = null;
	private BroadcastReceiver mImageTransferReceiver;
	private String mBackgroundFileName = null;
	protected Drawable mDrawable = null;
	protected String mIntentId;
	protected String mResultId;
	protected DisplayMetrics mDisplayMetrics = null;
	private int mColor = 0;
	private String mAreaClassName = null;
	CommandAreaPlugin()
	{
		mProtocolMap = new HashMap<String, FileTransferProtocols>();
		mProtocolMap.put("file", FileTransferProtocols.PROTOCOL_FILE);
		mProtocolMap.put("http", FileTransferProtocols.PROTOCOL_HTTP);
		mProtocolMap.put("ftp", FileTransferProtocols.PROTOCOL_FTP);
		
		mDisplayMetrics = new DisplayMetrics();
		Common.mainActivity.getWindowManager().getDefaultDisplay().getMetrics(mDisplayMetrics);
		
		//Set default size proportionally to the greatest dimension (based on current screen orientation)
		int greatestDim = mDisplayMetrics.heightPixels >= mDisplayMetrics.widthPixels ? mDisplayMetrics.heightPixels : mDisplayMetrics.widthPixels;
		mHeight = greatestDim / 20 + 5;
		mColor = Color.parseColor( "#C0C0C0");
	}
	
	protected void completeInit(String intentId, String resultId)
	{
		mCommandArea.setMinimumHeight(mHeight);
		mCommandArea.setBackgroundColor(mColor);
		
		mImageTransferReceiver = new BroadcastReceiver() {
			
			@Override
			public void onReceive(Context context, Intent intent) {
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Image received"));
				Bundle bundle = intent.getExtras();
				if (bundle == null)
					return;
				final String returnValue = bundle.getString(FileTransferService.ReturnValue);
				if (returnValue.contains("rror") == false)
				{
					try
					{
						Drawable update = Drawable.createFromPath(mBackgroundFileName);
						if (update != null)
						{
							mDrawable = update;						
							StateListDrawable newStates = new StateListDrawable();
							newStates.addState(new int[] {android.R.attr.state_focused}, mDrawable);
							newStates.addState(new int[] { }, mDrawable);
							mCommandArea.setBackgroundDrawable(newStates);
						}
					}
					catch (OutOfMemoryError e) //Thrown when the loaded image is too big
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
					}
				}
				cleanUpTempFiles();
			}
		};
		IntentFilter intentFilter = new IntentFilter(mIntentId);
		Common.mainActivity.registerReceiver(mImageTransferReceiver, intentFilter);
		
		mCommandAreaPanel.removeAllViews();
		mCommandAreaPanel.addView(mCommandArea, new LinearLayout.LayoutParams(LayoutParams.FILL_PARENT, mHeight));
		mAreaClassName = this.toString().substring(this.toString().lastIndexOf(".") + 1, this.toString().lastIndexOf("@"));
	}
	
	@Override
	public void onSetting(PluginSetting setting)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		if (mCommandArea == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "CommandArea is null"));
			return;
		}
		if (setting == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "setting is null"));
			return;
		}
		
		try
		{
			if (setting.getName().equalsIgnoreCase("Height"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Height"));
				if (Integer.valueOf(setting.getValue()) >= 0)
				{
					mHeight = Integer.valueOf(setting.getValue());
					mCommandArea.setMinimumHeight(mHeight);
					mCommandAreaPanel.removeAllViews();
					mCommandAreaPanel.addView(mCommandArea, new LinearLayout.LayoutParams(LayoutParams.FILL_PARENT, mHeight));
				}
			}
			else if (setting.getName().equalsIgnoreCase("Image"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Image"));
				String newFileName = getButtonPicture(setting.getValue());
				if (newFileName != null)
					mBackgroundFileName = newFileName;
			}
			else if (setting.getName().equalsIgnoreCase("Color"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Color"));
				if ( (setting.getValue() != null) && (setting.getValue().length() > 0) )
				{
					try
					{
						mColor = Color.parseColor(setting.getValue());
						mCommandArea.setBackgroundColor(mColor);
					}
					catch (IllegalArgumentException e) //Color is not recognized
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, e.getMessage()));
					}
				}
			}
			else if (setting.getName().equalsIgnoreCase("Visibility"))
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Visibility"));
				if (setting.getValue().compareToIgnoreCase("visible") == 0)
				{
					mCommandAreaPanel.setVisibility(View.VISIBLE);
				}
				else if (setting.getValue().compareToIgnoreCase("hidden") == 0)
				{
					mCommandAreaPanel.setVisibility(View.GONE);
				}
			}
		}
		catch (NumberFormatException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Passed parameter has wrong format"));
		}
		catch (MalformedURLException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Passed URL is not formatted correctly"));
		}
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	private String getButtonPicture(String source) throws MalformedURLException
	{
		String res = null;
		
		URL imageUrl = new URL(source);
		
		String fileName = imageUrl.getFile();
		if ( (fileName == null) || (fileName.length() == 0) )
			return null;
		long timestamp = Calendar.getInstance().getTimeInMillis();
		res = IMG_DESTINATION_FOLDER + "/" + mAreaClassName + timestamp + fileName.substring(fileName.lastIndexOf('/') + 1);
		
		Intent transferIntent = new Intent(Common.mainActivity, FileTransferService.class);
		//  Add data to the intent
		transferIntent.putExtra(FileTransferService.Copy, true);
		transferIntent.putExtra(FileTransferService.TransferProtocol, mProtocolMap.get(imageUrl.getProtocol()));
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
		returnId = mResultId;
		
		transferIntent.putExtra(FileTransferService.ReturnID, returnId);
		//  Start the File Transfer Service
		Common.mainActivity.startService(transferIntent);
		
		return res;
	}
	
	@Override
	public void onShutdown()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		Common.mainActivity.unregisterReceiver(mImageTransferReceiver);
	}
	private void cleanUpTempFiles()
	{
		String[] fileList = null;
		File dir = new File(IMG_DESTINATION_FOLDER);
		FilenameFilter filter = new FilenameFilter()
		{
		    public boolean accept(File dir, String name)
		    {
		        return name.contains(mAreaClassName);
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
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, fileList[i] + " deleted"));
				}
		}
	}
}
