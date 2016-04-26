package com.rho.rhoelements.plugins;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.NoSuchElementException;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.EMML;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.services.FileTransferService;
import com.rhomobile.rhodes.Logger;

/**
 * @author Ben Kennedy (NCVT73)
 * A Plugin class for EMML Profiles.
 */
public class ProfilePlugin extends Plugin
{
	private EMML emml;
	private String name;
	private FileTransferBroadcastReceiver receiver;
	private int pageCount;
	private String cacheDir;
	private List<ImportWaiter> profileQueue;
	private boolean	tempProfileUsed;

	/**
	 * The intent filter unique to the File Transfer plugin which we 
	 * will listen to to indicate that our transfer has completed.
	 */
	public static final String FT_PLUGIN_TRANSFER_COMPLETE = "com.rho.rhoelements.plugins.ProfilePlugin.TRANSFER_COMPLETE";
	private static final String TAG = "ProfilePlugin";

	/** Construct plugin with reference to core for navigation callback, etc. */
	public ProfilePlugin()
	{		
		/*
		 * This ArrayList is synchronised because many threads use it. We have done this so pending imports can be cleared before applying
		 * and apply commands can reliably be queued on the pending import object. 
		 */
		profileQueue = Collections.synchronizedList(new ArrayList<ImportWaiter>()); 
		File cacheDirFile = Common.mainActivity.getCacheDir();
		if(!cacheDirFile.exists())
		{
			if(!cacheDirFile.mkdirs());
			{
				Logger.W(TAG, "Cannot create cache directory");
			}
		}
		cacheDir = "/" + cacheDirFile.getAbsolutePath();
		emml = EMML.getEMML(); //Maybe change me to go thru elementsactivity
		tempProfileUsed = false;
		pageCount = 0;
		name = "";
		receiver = new FileTransferBroadcastReceiver();
		IntentFilter intentFilter = new IntentFilter(FT_PLUGIN_TRANSFER_COMPLETE);
		Common.mainActivity.registerReceiver(receiver, intentFilter);
		Logger.I(TAG, "-");
	}

	@Override
	public void onShutdown()
	{
		try
		{
			emml.clearProfiles();
			Common.mainActivity.unregisterReceiver(receiver);
			Logger.I(TAG, "onShutdown-");
		}
		catch (IllegalArgumentException e)
		{
			Logger.E(TAG, "File transfer receiver wasn't registered");
		}
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version ("Profile");
	}

	@Override
	public void onPageStarted(String url)
	{
		name = "";
		pageCount++;
		if(tempProfileUsed)
		{
			try
			{
				emml.deleteProfilePage(""); //Deletes the temporary profile page
				tempProfileUsed = false;
			}
			catch(NoSuchElementException e)
			{
				//ignore
			}
		}
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		if (setting.getValue().length() > 0)
			Logger.I(TAG, String.format("'%s', '%s'", setting.getName(), setting.getValue()));
		else
			Logger.I(TAG, setting.getName());

		String name = setting.getName();
		if (name.equals("name"))
		{
			this.name = setting.getValue();
		}
		else if (name.equals("import") || name.equalsIgnoreCase("importProfile"))
		{
			URL emmp;
			try
			{
				String emmpString = setting.getValue();
				if(emmpString != null && emmpString.toLowerCase().startsWith("url:")) emmpString = emmpString.substring(4);
				emmp = new URL(emmpString);
				Logger.I(TAG, "EmmpURI: " + emmp.toString());
				String username = null;
				String password = null;
				try
				{
					String[] userInfo = emmp.getUserInfo().split(":", 2);

					if(userInfo.length > 0)
					{
						username = userInfo[0];
					}
					if(userInfo.length > 1)
					{
						password = userInfo[1];
					}
				}
				catch(NullPointerException e){}//There are no userInfo, carry on
				
				String protocol = emmp.getProtocol();
				String source = protocol + "://" + emmp.getHost() + emmp.getPath();
				int port = emmp.getPort();
				String destination = cacheDir + File.separator + (new Date()).getTime() + (Math.random() * 1000000 + "").substring(0, 5);

				Intent  transferIntent = new Intent(Common.mainActivity, FileTransferService.class);

				//  Add data to the intent
				transferIntent.putExtra(FileTransferService.Username, username);
				transferIntent.putExtra(FileTransferService.Password, password);
				transferIntent.putExtra(FileTransferService.Port, port);
				transferIntent.putExtra(FileTransferService.TransferProtocol, FileTransferService.parseProtocol(protocol));
				transferIntent.putExtra(FileTransferService.Source, source);
				transferIntent.putExtra(FileTransferService.Destination, destination);
				transferIntent.putExtra(FileTransferService.Overwrite, true);
				transferIntent.putExtra(FileTransferService.Copy, true);

				//  Specify our Broadcast receiver to be notified when the File Transfer has 
				//  finished
				transferIntent.putExtra(FileTransferService.IntentFilter, FT_PLUGIN_TRANSFER_COMPLETE);
				//  Return ID allows us to distinguish between responses from the FileTransferService
				//  but the FileTransferPlugin does not need to distinguish.
				transferIntent.putExtra(FileTransferService.ReturnID, destination);

				synchronized(profileQueue)
				{
					profileQueue.add(new ImportWaiter(pageCount, destination, this.name));
				}

				//  Start the File Transfer Service
				Common.mainActivity.startService(transferIntent);
				name = null;
			}
			catch (MalformedURLException e)
			{
				Logger.W(TAG, "Import Profile: Badly formed URL syntax: " + setting.getValue());
			}


		}
		else if (name.equals("clear"))
		{
			String value = setting.getValue();
			boolean found = false;
			synchronized(profileQueue)
			{
				for(int i = 0; i < profileQueue.size() && !found; i++)
				{
					if(profileQueue.get(i).getName().equalsIgnoreCase(value))
					{
						profileQueue.remove(i);
						found = true;
					}
				}
			}
			try
			{
				emml.deleteProfilePage(value);
			}
			catch(NoSuchElementException e)
			{
				if(!found)
				{
					Logger.W(TAG, e.getLocalizedMessage());
				}
			}
			this.name = "";
		}
		else if (name.equals("apply"))
		{
			String value = setting.getValue();
			String[] splitStrings= value.split("[.]", 2);
			String profileName = "";

			if(splitStrings.length > 1)
			{
				profileName = splitStrings[0];
			}
			
			boolean found = false;
			synchronized(profileQueue)
			{
				for(int i = 0; i < profileQueue.size() && !found; i++)
				{
					if(profileQueue.get(i).getName().equalsIgnoreCase(profileName))
					{
						ImportWaiter currentWaiter = profileQueue.remove(i);
						currentWaiter.pushApply(value);
						profileQueue.add(i, currentWaiter);
						found = true;
					}
				}
			}

			if(!found)
			{
				applyProfile(value);
			}
		}
		else
		{
			Logger.I(TAG, "Incorrect syntax on EMMLProfile tag. Name: " + setting.getName() + ", Value: " + setting.getValue());
		}
	}

	/**
	 * Applies a EMML profile.
	 * @param value the in the format [pageName.]profileName
	 */
	private void applyProfile(String value)
	{
		try
		{
			Common.pluginManager.applySettings(emml.getProfile(value));
			Logger.I(TAG, "Applied profile: " + value);
		}
		catch(NoSuchElementException e)
		{
			Logger.W(TAG, e.getLocalizedMessage());
		}
	}

	/**
	 * Class to receive the intent sent from the FileTransferService when the File Transfer
	 * has completed.  We configure the intent in such a way so that we are the only recipient.
	 */
	private class FileTransferBroadcastReceiver extends BroadcastReceiver
	{
		/* (non-Javadoc)
		 * @see android.content.BroadcastReceiver#onReceive(android.content.Context, android.content.Intent)
		 */
		@Override
		public void onReceive(Context arg0, Intent receivedIntent) 
		{
			Logger.I(TAG, "FileTransferBroadcastReceiver.onReceive-");
			
			Bundle b = receivedIntent.getExtras();

			//  Extract the values associated with the return Intent
			String destination = b.getString(FileTransferService.ReturnID);
			String returnValue = b.getString(FileTransferService.ReturnValue);

			//If the return was positive (the transaction completed successfully) 
			if(returnValue.startsWith("ok") || returnValue.startsWith("OK") || returnValue.startsWith("Ok") || returnValue.startsWith("oK"))
			{
				synchronized(profileQueue)
				{
					//Find the associated Waiter
					for(int i = 0; i < profileQueue.size(); i++)
					{
						ImportWaiter tempProfile = profileQueue.get(i); 
						if(tempProfile.getDestination().equals(destination))
						{
							profileQueue.remove(i); //We've got it, delete from the queue.
							if(!((pageCount != tempProfile.getPageCount()) && (tempProfile.getName().equals("")))) //If the profile is still in scope...
							{
								FileInputStream fileStream = null;
								try
								{
									fileStream = new FileInputStream(destination);
									emml.importProfilePage(fileStream, tempProfile.getName()); //parses it and stores in EMML
									if(tempProfile.getName().equals(""))
									{
										tempProfileUsed = true;
									}
									for(String string: tempProfile.getQueue())
									{					
										applyProfile(string);//Runs all of the built up "apply profile" calls for this page.
									}
								}
								catch (FileNotFoundException e1)
								{
									Logger.W(TAG, "Internal error. Could not read local profile page: " + tempProfile.getName());
								}
								catch (IOException e)
								{
									Logger.W(TAG, "Internal error. Could not read local profile page: " + tempProfile.getName());
								}
								finally
								{
									try
									{
										fileStream.close();
									}
									catch(Exception e){}
								}
							}
							return;//All done, finish.
						}
					}
				}
				//The profile was deleted before the importing could finish.
				Logger.W(TAG, "Profile import ignored as profile is no longer in scope.");
			}
			else // The return value was an error.
			{
				Logger.W(TAG, "Failed to import profile: " + returnValue);
				synchronized(profileQueue)
				{
					//Even tho its errored we still need to remove from the queue
					for(int i = 0; i < profileQueue.size(); i++)
					{
						ImportWaiter tempProfile = profileQueue.get(i); 
						if(tempProfile.getDestination().equals(destination))
						{
							profileQueue.remove(i);
							return;
						}
					}
				}
			}	
		}
	}

	/**
	 * A class to keep track of import profile commands that are waiting for their file to be transferred.
	 * It stores the pageCount of when it was constructed so it can tell if it is still relevant to the current page
	 * and the destination of the local file.
	 * @author Ben Kennedy (NCVT73)
	 */
	private class ImportWaiter
	{
		private int pageCount;
		private String destination;
		private String name;
		private ArrayList<String> applyQueue;

		public ImportWaiter(int pageCount, String destination, String name)
		{
			this.name = name;
			this.pageCount = pageCount;
			this.destination = destination;
			applyQueue = new ArrayList<String>();
		}
	
		/**
		 * @return the name
		 */
		public String getName()
		{
			return name;
		}

		/**
		 * @return the pageCount
		 */
		public int getPageCount()
		{
			return pageCount;
		}

		/**
		 * @return the destination
		 */
		public String getDestination()
		{
			return destination;
		}
		
		public void pushApply(String name)
		{
			applyQueue.add(name);
		}
		
		public ArrayList<String> getQueue()
		{
			return applyQueue;
		}
	}
}
