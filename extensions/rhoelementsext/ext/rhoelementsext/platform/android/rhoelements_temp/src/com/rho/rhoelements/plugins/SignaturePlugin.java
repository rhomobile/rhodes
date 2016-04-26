package com.rho.rhoelements.plugins;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;

import org.json.JSONArray;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Bundle;
import android.widget.AbsoluteLayout;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.PluginSetting;
import com.rho.rhoelements.Version;
import com.rho.rhoelements.graphics.SignatureArea;
import com.rho.rhoelements.services.FileTransferService;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author Ben Kennedy (NCVT73)
 * A class to control a visible, static signature capture box. From this class you can change
 * many of the settings of the box, including saving to bitmap and vectors. 
 */
@SuppressWarnings("deprecation") //For AbsoluteLayout
public class SignaturePlugin extends Plugin
{
	private static final String		METHOD_CAPTURE		= "Capture";
	private static final String		METHOD_CLEAR		= "Clear";
	private static final String		PARAM_LEFT			= "Left";
	private static final String		PARAM_TOP			= "Top";
	private static final String		PARAM_BGCOLOR		= "Bgcolor";
	private static final String		PARAM_HEIGHT		= "Height";
	private static final String		PARAM_WIDTH			= "Width";
	private static final String		PARAM_PENCOLOR		= "Pencolor";
	private static final String		PARAM_SIGSAVEEVENT	= "Signaturesaveevent";
	private static final String		PARAM_PENWIDTH		= "Penwidth";
	private static final String		PARAM_NAME			= "Name";
	private static final String		PARAM_VECTOREVENT	= "Vectorevent";
	private static final String		PARAM_BORDER		= "Border";
	private static final String		PARAM_PASSWORD		= "Password";
	private static final String		PARAM_USERNAME		= "Username";
	private static final String		PARAM_DESTINATION	= "Destination";
	private static final String		PARAM_VISIBILITY	= "Visibility";
	private static final String		VALUE_HIDDEN		= "Hidden";
	private static final String		VALUE_VISIBLE		= "Visible";
	private static final int		DEFAULT_WIDTH		= 200;
	private static final int		DEFAULT_HEIGHT		= 150;
	private static final int		DEFAULT_TOP			= 60;
	private static final int		DEFAULT_LEFT		= 15;
	private static final int		DEFAULT_PENWIDTH	= 1;
	private static final String		DEFAULT_USERNAME	= "";
	private static final String		DEFAULT_PASSWORD	= "";
	private static final String		DEFAULT_NAME		= "Signature.bmp";
	private static final String		DEFAULT_DESTINATION	= null;
	private static final boolean	DEFAULT_VISIBILITY	= false;
	private static final boolean	DEFAULT_BORDER		= true;
	private static final int		DEFAULT_PENCOLOR	= 0xFF000000;
	private static final int		DEFAULT_BGCOLOR		= 0xFFFFFFFF;
	private static final FileType	DEFAULT_FILETYPE	= FileType.BMP;
	private static final String		VECTOR_ARRAY_NAME	= "vectorArray";
	private static final String		TRANSFER_RESULT_NAME= "transferResult";
	private static final String		IMAGE_DATA= "imageData";
	private SignatureArea 	sigArea;
	private AbsoluteLayout	sigPanel;
	private boolean			isVisible;
	private String	username;
	private String	password;
	private String	destination;
	private String	name;
	private String	sigSaveEvent;
	private String	vectorEvent;
	private int width;
	private int	height;
	private int	top;
	private int	left;
	private int	penWidth;
	private int	penColor;
	private int	bgColor;
	private FileTransferBroadcastReceiver receiver;
	private String mCaptureEvent;
	
	/**
	 * The intent filter unique to the File Transfer plugin which we 
	 * will listen to to indicate that our transfer has completed.
	 */
	public static final String FT_PLUGIN_TRANSFER_COMPLETE = "com.rho.rhoelements.plugins.SignaturePlugin.TRANSFER_COMPLETE";

	public SignaturePlugin()
	{
		sigArea = (SignatureArea) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","signature_area"));
		sigPanel = (AbsoluteLayout) Common.mainActivity.findViewById(RhoExtManager.getResourceId("id","signature_panel"));
		resetAll();
		
		receiver = new FileTransferBroadcastReceiver();
		IntentFilter intentFilter = new IntentFilter(FT_PLUGIN_TRANSFER_COMPLETE);
		Common.mainActivity.registerReceiver(receiver, intentFilter);
	}
	
	@Override
	public void onPageStarted(String url)
	{
		reset();
		sigArea.clear();
		setVisible(false);
	}

	@Override
	public void onSetting(PluginSetting setting)
	{
		String name = setting.getName();
		String value = setting.getValue();
		
		if (value.length() > 0)	Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, String.format("'%s', '%s'", name, value)));
		else					Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, name));

		if 		(name.equalsIgnoreCase(PARAM_DESTINATION))	setDestination(value);
		else if (name.equalsIgnoreCase(PARAM_USERNAME))		setUsername(value);
		else if (name.equalsIgnoreCase(PARAM_PASSWORD))		setPassword(value);
		else if (name.equalsIgnoreCase(PARAM_NAME))			setName(value);
		else if (name.equalsIgnoreCase(METHOD_CLEAR))		sigArea.clear();
		else if (name.equalsIgnoreCase(METHOD_CAPTURE))		capture();
		else if (name.equalsIgnoreCase(PARAM_VECTOREVENT))	setVectorEvent(value);
		else if (name.equalsIgnoreCase(PARAM_SIGSAVEEVENT))	setSigSaveEvent(value);
		else if(name.equalsIgnoreCase("enable")|| name.equalsIgnoreCase("enabled"))
			setVisible(true);
		else if(name.equalsIgnoreCase("disable")|| name.equalsIgnoreCase("disabled"))
			setVisible(false);
		else if (name.equalsIgnoreCase(PARAM_VISIBILITY))
		{
			if (value.equalsIgnoreCase(VALUE_VISIBLE))		setVisible(true);
			else if (value.equalsIgnoreCase(VALUE_HIDDEN))	setVisible(false);
			else	Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_VISIBILITY + " value: " + value));
		}
		else if (name.equalsIgnoreCase(PARAM_BORDER))
		{
			if (value.equalsIgnoreCase(VALUE_VISIBLE))		sigArea.setBorder(true);
			else if (value.equalsIgnoreCase(VALUE_HIDDEN))	sigArea.setBorder(false);
			else	Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_BORDER + " value: " + value));
		}
		else if (name.equalsIgnoreCase(PARAM_PENCOLOR))
		{
			try
			{
				int color = Color.parseColor(value);
				sigArea.clear();
				sigArea.setPenColor(color);
				penColor = color;
			}
			catch(IllegalArgumentException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_PENCOLOR + " value: " + value));
			}
			catch(StringIndexOutOfBoundsException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_PENCOLOR + " value: " + value));
			}
		}
		else if (name.equalsIgnoreCase(PARAM_BGCOLOR))
		{
			try
			{
				int color = Color.parseColor(value);
				sigArea.clear();
				sigArea.setBackground(color);
				bgColor = color;
			}
			catch(IllegalArgumentException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_BGCOLOR + " value: " + value));
			}
			catch(StringIndexOutOfBoundsException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_PENCOLOR + " value: " + value));
			}
		}
		else if (name.equalsIgnoreCase(PARAM_PENWIDTH))
		{
			try
			{
				int tempPenWidth = Integer.parseInt(value);
				if(tempPenWidth != penWidth)
				{
					penWidth = tempPenWidth; //split in two to prevent unusual invalid values being entered into height
					sigArea.setPenWidth(penWidth);
				}
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_HEIGHT + " value: " + value));
			}
		}
		else if (name.equalsIgnoreCase(PARAM_WIDTH))
		{
			try
			{
				int tempWidth = (int) Double.parseDouble(value);
				if(tempWidth != width)
				{
					width = tempWidth; //split in two to prevent unusual invalid values being entered into width
					if(isVisible)
					{
						sigArea.clear();
						position();
					}
				}
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_WIDTH + " value: " + value));
			}
		}
		else if (name.equalsIgnoreCase(PARAM_HEIGHT))
		{
			try
			{
				int tempHeight = (int) Double.parseDouble(value);
				if(tempHeight != height)
				{
					height = tempHeight; //split in two to prevent unusual invalid values being entered into height
					if(isVisible)
					{
						sigArea.clear();
						position();
					}
				}
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_HEIGHT + " value: " + value));
			}
		}
		else if (name.equalsIgnoreCase(PARAM_TOP))
		{
			try
			{
				int tempTop = (int) Double.parseDouble(value); 
				if(tempTop != top)
				{
					top = tempTop; //split in two to prevent unusual invalid values being entered into top
					if(isVisible)
					{
						position();
					}
				}
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_TOP + " value: " + value));
			}
		}
		else if (name.equalsIgnoreCase(PARAM_LEFT))
		{
			try
			{
				int tempLeft = (int) Double.parseDouble(value);
				if(tempLeft != left)
				{
					left = tempLeft; //split in two to prevent unusual invalid values being entered into top
					if(isVisible)
					{
						position();
					}
				}
			}
			catch(NumberFormatException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Invalid " + PARAM_LEFT + " value: " + value));
			}
		}
		else if (name.equalsIgnoreCase("signaturecaptureevent"))
		{
		    mCaptureEvent = value;
		}
		else	Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognized setting '" + name + "'"));
	}
	
	@Override
	public void onShutdown()
	{
		try
		{
			Common.mainActivity.unregisterReceiver(receiver);
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, null));
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "File transfer receiver wasn't registered"));
		}
	}
	
	/**
	 * Resets all values in the Signature Plugin.
	 */
	private void resetAll()
	{
		width = DEFAULT_WIDTH;
		height = DEFAULT_HEIGHT;
		top = DEFAULT_TOP;
		left = DEFAULT_LEFT;
		username = DEFAULT_USERNAME;
		password = DEFAULT_PASSWORD;
		name = DEFAULT_NAME;
		destination = DEFAULT_DESTINATION;
		penWidth = DEFAULT_PENWIDTH;
		sigArea.setPenWidth(penWidth);
		sigArea.setBorder(DEFAULT_BORDER);
		bgColor = DEFAULT_BGCOLOR;
		sigArea.setBackground(DEFAULT_BGCOLOR);
		penColor = DEFAULT_PENCOLOR;
		sigArea.setPenColor(DEFAULT_PENCOLOR);
		setVisible(DEFAULT_VISIBILITY);
		reset();
	}
	
	/**
	 * Minimally resets the Signature Capture Plugin. Used on navigation. Use resetAll() to
	 * reset all values.
	 */
	private void reset()
	{
		setSigSaveEvent(null);
		setVectorEvent(null);
		mCaptureEvent = null;
	}

	/**
	 * Sets the visibility of the Signature Capture Area.
	 * @param visibility the visibility to which to set the area.
	 */
	private void setVisible(boolean visibility)
	{
		if(visibility)
		{
			isVisible = true;
			sigPanel.setVisibility(AbsoluteLayout.VISIBLE);
			position();
		}
		else
		{
			isVisible = false;
			sigPanel.setVisibility(AbsoluteLayout.GONE);
		}
	}

	/**
	 * Repositions the Signature area. This is called when the position or the dimensions change
	 * and should only be called when you know that these have been changed. Otherwise you'll waste
	 * time and power. 
	 */
	private void position()
	{
		sigPanel.removeAllViews();
		sigPanel.addView(sigArea, new AbsoluteLayout.LayoutParams(width, height, left, top));
		sigArea.setDimensions(width, height);
	}

	/**
	 * Sets the username to be used for transferring the signature bitmap via FTP
	 * @param username the username to use in FTP transfers
	 */
	private void setUsername(String username)
	{
		this.username = username;
	}

	/**
	 * Sets the password to be used for transferring the signature bitmap via FTP
	 * @param password the password to use in FTP transfers
	 */
	private void setPassword(String password)
	{
		this.password = password;
	}
	
	/**
	 * Sets the FTP/HTTP/File signature bitmap destination URL
	 * @param destination the URL to send the signature bitmap 
	 */
	private void setDestination(String destination)
	{
		if(destination.startsWith("url:"))
			destination=destination.substring(4);
		this.destination = destination;
	}
	
	private void setName(String name)
	{
		if(name.length() > 0)
		{
//			if (name.endsWith(".bmp") == false)
//				name = name + ".bmp";
			this.name = name;
		}
		else
		{
			this.name = DEFAULT_NAME;
		}
	}

	/**
	 * Captures the Signature as a bitmap and:
	 * 1) sends to the specified destination if it has been set.
	 * 2) Converts the bitmap to a Base64 stream if SignatureCaptureEvent is set
	 */
	private void capture()
	{
		//Get file type by checking name extension
		String nameExtension = name.replaceFirst(".*[.](.*?)$", "$1");
		FileType fileType = null;

		if(nameExtension.equalsIgnoreCase("bmp") || nameExtension.equalsIgnoreCase("dib"))
		{
			fileType = FileType.BMP;
		}
		else if(nameExtension.equalsIgnoreCase("png"))
		{
			fileType = FileType.PNG;
		}
//		else if(nameExtenstion.equalsIgnoreCase("jpg") || 
//				nameExtenstion.equalsIgnoreCase("jpeg") ||
//				nameExtenstion.equalsIgnoreCase("jpe") ||
//				nameExtenstion.equalsIgnoreCase("jif") ||
//				nameExtenstion.equalsIgnoreCase("jfif") ||
//				nameExtenstion.equalsIgnoreCase("jfi"))
//		{
//			fileType = FileType.JPEG;
//		}
		else if(!name.contains(".")) //no dot
		{
			fileType = DEFAULT_FILETYPE;
			name = name + "." + fileType.name().toLowerCase();
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Unrecognised Signature Capture destination file type. Capture aborted."));
			return;
		}
		
		FileOutputStream fOut = null;
		boolean success = false;
		File bitmapFile = null;
		try
		{
			Bitmap bitmap = sigArea.getBitmap();
			String path = Common.getDataPath();
			if(name == null || name.length() == 0)
			{
				name = DEFAULT_NAME + fileType.name().toLowerCase();
			}
			bitmapFile = new File(path, name);
			if(bitmapFile.exists())
			{
				bitmapFile.delete();
			}
			bitmapFile.createNewFile();
			fOut = new FileOutputStream(bitmapFile);
			
			if(fileType == FileType.BMP)
			{
				FormatBitmap.writeBitmapImage(bitmap, fOut, penColor, bgColor);
			}
			else //if(fileType == FileType.PNG)
			{
				bitmap.compress(Bitmap.CompressFormat.PNG, 90, fOut);
			}
//			else //if(fileType == FileType.JPEG)
//			{
//				bitmap.compress(Bitmap.CompressFormat.JPEG, 90, fOut);
//			}
			success = true;
		}
		catch (FileNotFoundException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not create signature file."));
		}
		catch (IOException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not write to signature file."));
		}
        finally
	    {
	    	try
	    	{
	    		if(fOut != null)
	    		{
	    			fOut.flush();
	    			fOut.close();
	    		}
	    	}
	    	catch (IOException e)
	    	{
	    	    success = false;
	    	    //Nothing we can do here :S
	    	}
	    }
        
		if(success && destination != null)
		{
			if(destination.length() > 0)
			{
				sendBitmap(bitmapFile); //Send the file to FTP/HTTP/File
			}
		}
		if ( (success) && (mCaptureEvent != null) )
		{
		    sendSigToDataURI(encodeToBase64(bitmapFile));
		}
	}

	/**
	 * Sends the captured Signature Bitmap file to the specified FTP/HTTP/File destination.
	 * @param bitmapFile the file of the captured Signature Bitmap.
	 */
	private void sendBitmap(File bitmapFile)
	{
		String source = bitmapFile.getAbsolutePath();
		//source includes /mnt/sdcard/ which would be prefixed again by the file transfer service, therefore we need to delete it
		int sdcardPathPos = source.indexOf("/mnt/sdcard/");
		if (sdcardPathPos == 0)
			source = source.substring(12); //12 = length("/mnt/sdcard/")
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "source = " + source));
		Intent transferIntent = new Intent(Common.mainActivity, FileTransferService.class);

		URL url;
		try
		{	
			//Use java's built in URL parsing to extract information
			if (isRelativeURL(destination))
				destination = dereferenceURL(destination, Common.elementsCore.getCurrentUrl());
			url = new URL(destination);
			String username = null;
			String password = null;
			//The username and password set explicitely via settings prevale on the ones implicitely set in the url (Windows behavior)
			if ( (this.username == null) || (this.username.length() == 0) )
			{
				try
				{
					String[] userInfo = url.getUserInfo().split(":", 2);
		
					if(userInfo.length > 0)
					{
						username = userInfo[0];
					}
					if(userInfo.length > 1)
					{
						password = userInfo[1];
					}
				}
				catch(NullPointerException e) {}//There are no userInfo, carry on
			}
			if (username == null)
			{
				username = this.username;
			}
			if(password == null)
			{
				password = this.password;
			}
			
			String protocol = url.getProtocol();
			int port = url.getPort();
			
			//extension checking
			int lastDot = destination.lastIndexOf('.');
			if(		lastDot == -1 || //There is no dot in the name 
					destination.substring(lastDot).indexOf('/') != -1 || //The last dot comes before a directory separator therefore not an extension
					destination.substring(lastDot).indexOf('\\') != -1) //The last dot comes before a directory separator therefore not an extension
			{
				destination += "." + DEFAULT_FILETYPE.name().toLowerCase();
			}
			
			//Setup the transfer intent that will be sent to the FileTransferService
			transferIntent.putExtra(FileTransferService.Username, username);
			transferIntent.putExtra(FileTransferService.Password, password);
			transferIntent.putExtra(FileTransferService.Port, port);
			transferIntent.putExtra(FileTransferService.TransferProtocol, FileTransferService.parseProtocol(protocol));
			transferIntent.putExtra(FileTransferService.Source, source);
			transferIntent.putExtra(FileTransferService.Destination, destination);
			transferIntent.putExtra(FileTransferService.Overwrite, true);
			transferIntent.putExtra(FileTransferService.Copy, false);
			transferIntent.putExtra(FileTransferService.IntentFilter, FT_PLUGIN_TRANSFER_COMPLETE);
			transferIntent.putExtra(FileTransferService.ReturnID, "FTReturn");

			//  Start the File Transfer Service
			Common.mainActivity.startService(transferIntent);
		}
		catch (MalformedURLException e1)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Malformed destination URL: " + destination));
		}
	}
	

	/**
	 * Sets the URL to send signature save events to. 
	 * @param address the URL address to send the save events to.
	 */
	private void setSigSaveEvent(String address)
	{
		sigSaveEvent = address;
	}

	/**
	 * Sets the URL to send the Vector events to.
	 * @param address the URL address to send the save events to.
	 */
	private void setVectorEvent(String address)
	{
		vectorEvent = address;
		if(address == null || address.equals(""))
		{
			sigArea.setVectorListenee(null);//Cancels the listener
		}
		else
		{
			sigArea.setVectorListenee(this);
		}
	}
	
	/**
	 * Sends the signature save event to the previously specified URL.
	 * @param result the result of the signature save event.
	 */
	private void sendSigSave(String result)
	{
		try
		{
			navigate(sigSaveEvent, TRANSFER_RESULT_NAME, result);
		}
		catch (NavigateException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
		}
	}
	
	private void sendSigToDataURI(String encodedSignature)
	{
	    try
	    {
		navigate(mCaptureEvent, IMAGE_DATA, "data:image/png;base64,"+encodedSignature);
	    }
	    catch (NavigateException e)
	    {
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
	    }
	}
	
	/**
	 * Sends the vector event to the previously specified URL.
	 * @param vector an array of coordinates stored as floats to send to the registered destination.
	 */
	public void sendVector(ArrayList<Float> vector)
	{
		JSONArray vectorArray = new JSONArray();
		vectorArray.put("0xFFFF"); //header
		vectorArray.put("0xFFFF"); //header
		int length = vectorEvent.length() + 20; //20 being enough space for 2 0xFFFFs and then some!
		int maxLength = Common.MAX_URL - 17 - vectorEvent.length();
		for(int i = 0; i < vector.size(); i++)
		{
			String valueStringX = "0x" + String.format("%X", Math.round(vector.get(i)));
			String valueStringY = "0x" + String.format("%X", Math.round(vector.get(++i)));
			
			if(!valueStringX.equals("0x0") || !valueStringY.equals("0x0"))
			{
				length += valueStringX.length() + valueStringY.length() + 8; //+8 for the two sets of " ," s
				
				if(length > maxLength) //Send the URL, and start anew
				{
					try
					{
						navigate(vectorEvent, VECTOR_ARRAY_NAME, vectorArray);
						vectorArray = new JSONArray();
						length = vectorEvent.length() + valueStringX.length() + valueStringY.length() + 28;
					}
					catch (NavigateException e)
					{
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
					}
				}
				vectorArray.put(valueStringX);
				vectorArray.put(valueStringY);
			}
		}
		vectorArray.put("0xFFFF"); //footer
		vectorArray.put("0xFFFF"); //footer
		try
		{
			navigate(vectorEvent, VECTOR_ARRAY_NAME, vectorArray);
		}
		catch (NavigateException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Navigate Exception.. length is " + e.GetLength()));
		}
	}
	
	/**
	 * Extract the protocol from the given URL
	 * @param source the URL to extract the protocol from
	 * @return The protocol being specified, either File, HTTP or FTP
	 */
	private String getProtocolFromURL(String source)
	{
		if (source.length() < new String("file://").length() &&
				!source.startsWith("/"))
			return "error";
		else if (source.toLowerCase().startsWith("file://"))
			return "file";
		else if (source.toLowerCase().startsWith("http://"))
			return "http";
		else if (source.toLowerCase().startsWith("ftp://"))
			return "ftp";
		else
			return "";
	}
	
	/**
	 * Returns whether or not the passed URL was a relative relative URL (e.g. '../../')
	 * @param url The URL being queried
	 * @return If the URL was relative
	 */
	private boolean isRelativeURL(String url)
	{
		if ( (url.startsWith(".")) || (url.contains("//.")) )  
			return true;
		return false;
	}

	/**
	 * Given a relative URL this function will convert it into a fully qualified URL using the
	 * directory of the current page in the WebView.
	 * @param relativeURL The relative URL to parse (e.g. ../file.htm)
	 * @param currentURL The current page being displayed in the browser, e.g.
	 *            http://web.com/dir/page.htm
	 * @return A fully qualified version of the passed relative URL, e.g.
	 *         http://web.com/file.htm
	 */
	private String dereferenceURL(String relativeURL, String currentURL)
	{
		// Work out how many lavels we need to navigate up from the current URL
		// (specified as ../)
		int iLevelsUp = 0;
		String relativeURLCpy = relativeURL;
		while (relativeURLCpy.indexOf("../") > -1)
		{
			iLevelsUp++;
			relativeURLCpy = relativeURLCpy.substring(relativeURLCpy.indexOf("../") + 3,
					relativeURLCpy.length());
		}

		// We now know how many levels up we want to go from the current URL.
		// Starting at the end of the current URL search for '/' and
		// work out if we can go up that many levels

		// We want to ignore the protocol specified in the current URL
		String currentURLProtocol = getProtocolFromURL(currentURL);
		if (currentURLProtocol.length() == 0)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"Unable to determine current URL protocol, aborting file transfer"));
			return null;
		}
		int iLengthOfProtocol = currentURLProtocol.length() + 3;
		// Navigate the current URL backwards counting the number of slashes
		int iPosInCurrentURL;
		for (iPosInCurrentURL = currentURL.length() - 1; iPosInCurrentURL > iLengthOfProtocol; iPosInCurrentURL--)
		{
			if (currentURL.charAt(iPosInCurrentURL) == '/')
				iLevelsUp--;
			if (iLevelsUp == -1)
			{
				// iPosInCurrentURL is the character in the current URL after which we want to
				// paste our relative URL
				break;
			}
		}

		// If we exit the above loop and 'levelsUp' is not -1 then there were
		// insufficient slashes in the current URL to go up the number of
		// levels specified in the relative URL.
		if (iLevelsUp != -1)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"Invalid Relative URL given (" + relativeURL + ")"));
			return null;
		}

		// The actual URL we require is the juxtaposition of the current URL
		// up to 'iPosInCurrentURL' with the first non . | / | \ character
		// in the relative URL
		int iFirstNonRelativeCharacter = 0;
		for (int i = iFirstNonRelativeCharacter; i < relativeURL.length(); i++)
		{
			if (relativeURL.charAt(i) == '.' || relativeURL.charAt(i) == '/')
				iFirstNonRelativeCharacter++;
			else
				break;
		}
		if (iFirstNonRelativeCharacter == relativeURL.length())
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"Invalid Relative URL given (" + relativeURL + ")"));
			return null;
		}

		// Concatenate the absolute directory with the page / directory from the relative URL
		String output = currentURL.substring(0, iPosInCurrentURL + 1);
		output = output.concat(relativeURL.substring(iFirstNonRelativeCharacter));
		if (output.length() > Common.MAX_URL)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING,
					"Unable to dereference relative URL, resultant URL would be too long"));
			return null;
		}

		return output;
	}
	
	/**
	 * @return The version of this plugin being built
	 */
	public static Version getVersion()
	{
		return new Version("SignaturePlugin");
	}
	
	/**
	 * Class to receive the intent sent from the FileTransferService when the File Transfer
	 * has completed.  We configure the intent in such a way so that we are the only recipient.
	 */
	private class FileTransferBroadcastReceiver extends BroadcastReceiver
	{
		@Override
		public void onReceive(Context arg0, Intent receivedIntent) 
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
			//Check whether the response comes from the same process (application)
			if ( (receivedIntent == null) || (receivedIntent.getIntExtra(FileTransferService.PID, android.os.Process.myPid()) != android.os.Process.myPid()) )
	    		    return;
			Bundle b = receivedIntent.getExtras();

			//  Extract the values associated with the return Intent
			String returnValue = b.getString(FileTransferService.ReturnValue);

			// Pass the returned server message to the caller
			sendSigSave(returnValue);
		}
	}
	
	private enum FileType
	{
		PNG, BMP//,JPEG
	}
}
