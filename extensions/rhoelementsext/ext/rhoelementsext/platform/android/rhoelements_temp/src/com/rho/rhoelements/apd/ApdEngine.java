package com.rho.rhoelements.apd;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.security.InvalidParameterException;
import java.util.Timer;
import java.util.TimerTask;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.widget.Toast;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.apd.ApdCommands;
import com.rho.rhoelements.apd.ApdConfiguration;
import com.rho.rhoelements.apd.ApdConfiguration.Language;
import com.rho.rhoelements.apd.ApdConfiguration.Transport;
import com.rho.rhoelements.apd.transport.ApdBluetooth;
import com.rho.rhoelements.apd.transport.ApdTransport;
import com.rho.rhoelements.apd.transport.ApdTransportFactory;
import com.rho.rhoelements.apd.transport.ApdTransport.ApdTransportError;
import com.rho.rhoelements.common.UnzipUtility;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class ApdEngine
{
	private static final String APD_VERSION = "Symbol APD V1.00.00A";
	private static final String EOL = "\r\n";
	private static final String RESPONSE_FILE = Common.getDataPath() + "/APD/prtdrv.rsp";
	private static final String DATA_FILE = Common.getDataPath() + "/APD/prtdrv.lbl";
	private static final String PRINTER_DATA_FILE = Common.getDataPath() + "/APD/prtdrv.prn";
	
	public static final String DEFAULT_FORMAT_PATH = Common.getDataPath() + "/APD/Formats/";
	public static final String DEFAULT_TEMPLATE_PATH = Common.getDataPath() + "/APD/Template/";
	public static final String DEFAULT_ID_PATH = Common.getDataPath() + "/APD/ID_Label/";
	private static final String PRN_ID_LABEL = "ID";
	private static final int APD_WLAN_DEFAULT_PORT = 515;
	private static final int APD_BT_DEFAULT_PORT = 1;
	private static final int ASCII_A = 65;
	private static final String EMBEDDEDCOMMANDPATTERN = "\\$\\$\\[CMD:([0-9A-Fa-f]{4})(,.*)*\\]\\$\\$";
	private static final int MAX_DATASTRING = 1024;
	private static BluetoothAdapter adapter;
	private boolean mTimeoutFlag = false;
	private String mLastMessage;
	private static ApdConfiguration mApdConfiguration;
	private ApdTransport mApdTransport;
	private int mPrnDataCount = 0;
	private String mPrnDataList[] = null;
	public String mPrnStringList[] = null;
	private String mPrnDataFormat = null;
	//Acceptable data types in variable data
	private enum ApdDataType {
		CHAR,
		STRING,
		HEX,
		DECIMAL,
		UNSIGNED,
		INVALID
	}
	//Variable data type formatting
	String prnFormatting[] = {"%c", "%s", "%x", "%d", "%u"};
	
	private enum ApdPathIndex {
		FORMAT_PATH,
		TEMPLATE_PATH,
		IDLABEL_PATH
	}
	public enum ApdError {
		ERR_OK,
		ERR_INV_ID,
		ERR_FILE,
		ERR_PORT,
		ERR_NODATA,
		ERR_NOFILE,
		ERR_NOQTY,
		ERR_WINDOW,
		ERR_SKT,
		ERR_CON,
		ERR_SEND,
		ERR_CTS,
		ERR_SETUP,
		ERR_BUFF,
		ERR_CREATE,
		ERR_INIT,
		ERR_CLOSE,
		ERR_DESTROY,
		ERR_DEINIT,
		ERR_LOST
	}
	
	private enum AddressType {
		MAC_ADDRESS,
		IP_ADDRESS
	}
	
	/**
	 * Constructor
	 */
	public ApdEngine()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		
		//Unzip the APD files if they haven't been unzipped yet.
		boolean isApdUnzippedOk = UnzipUtility.unzipFileIfRequired(Common.getDataPath() + "/apd.zip", 
				Common.getDataPath() + "/",
				Common.getDataPath() + "/APD");
		if (isApdUnzippedOk == false)
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "an error occured while unzipping apd files (formats and templates)"));
		
		mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		try
		{
			mApdConfiguration = ApdConfigurator.readConfiguration();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "apdconfig.xml read"));
			mApdTransport = ApdTransportFactory.createTransport(mApdConfiguration);
		} 
		catch (Exception e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while constructing ApdEngine"));
			e.printStackTrace();
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}
	
	/**
	 * destroys the resources created in the constructor
	 */
	public void destroy()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		mApdConfiguration = null;
		if (mApdTransport != null)
			mApdTransport.destroy();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	}

	/**
	 * Generic command interface (This should not be called directly).  
	 * @param command is the command to be executed
	 * @param params is a string coding the parameters to the command
	 * @return //TODO: write a description of the return 
	 */
	public synchronized int PSExternalEx(int command, String params)
	{
		return PSExternal(command, params).ordinal();
	}
	
	/**
	 * Generic command interface (This should not be called directly).  
	 * @param command is the command to be executed
	 * @param params is a string coding the parameters to the command
	 * @return //TODO: write a description of the return 
	 */
	public synchronized ApdError PSExternal(int command, String params)
	{
		Common.log("PSExternal");//testing
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		if (params != null)
		{
			params = params.replaceAll("\\\\r", "\r");
			params = params.replaceAll("\\\\n", "\n");
		}
		switch (command)
		{
			case ApdCommands.Cmd_PSSendString:
			{
				res = PSSendStringIndirect(params);
				break;
			}
			case ApdCommands.Cmd_PSSendFormat:
			{
				res = PSSendFormatIndirect(params);
				break;
			}
			case ApdCommands.Cmd_PSPrintIdLabel:
			{
				res = PSPrintIdLabelIndirect(params);
				break;
			}
			case ApdCommands.Cmd_PSGetVersion:
			{
				res = writeApdResponse(APD_VERSION);
				break;
			}
			
			case ApdCommands.Cmd_PSBthPowerOn:
			{
				res = switchBthPower(true);
				break;
			}
			case ApdCommands.Cmd_PSBthPowerOff:
			{
				res = switchBthPower(false);
				break;
			}
			case ApdCommands.Cmd_PSGetLastMessage:
			{
				res = writeApdResponse(mLastMessage);
				break;
			}
			case ApdCommands.Cmd_PSSetFormatPath:
			{
				res = changeCurrentPath(ApdPathIndex.FORMAT_PATH, params);
				break;
			}
			case ApdCommands.Cmd_PSSetTemplatePath:
			{
				res = changeCurrentPath(ApdPathIndex.TEMPLATE_PATH, params);
				break;
			}
			case ApdCommands.Cmd_PSSetIDLabelPath:
			{
				res = changeCurrentPath(ApdPathIndex.IDLABEL_PATH, params);
				break;
			}
			case ApdCommands.Cmd_PSGetPrinterList:
			{
				res = writeApdResponse(PSGetPrinterList());
				break;
			}
			case ApdCommands.Cmd_PSOpenPort:
			{
				res = openPort(true);
				break;
			}
			case ApdCommands.Cmd_PSClosePort:
			{
				res = openPort(false);
				break;
			}
			case ApdCommands.Cmd_PSPowerOn:
			case ApdCommands.Cmd_PSPowerOff:
			{
				res = ApdError.ERR_CREATE;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_functionnotsupported"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
				break;
			}
			case ApdCommands.Cmd_PSShowProgress:
			{
				if (mApdTransport != null)
					mApdTransport.setProgressEnabled(true);
				break;
			}
			case ApdCommands.Cmd_PSHideProgress:
			{
				if (mApdTransport != null)
					mApdTransport.setProgressEnabled(false);
				break;
			}
			case ApdCommands.Cmd_PSSetPrinter:
			{
				res = setCurrentPrinter(params);
				break;
			}
			case ApdCommands.Cmd_PSSendData:
			{
				res = PSSendDataIndirect(params);
				break;
			}
			default:
			{
				res = ApdError.ERR_INIT;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_unknownerror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
				break;
			}
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}

	/**
	 * This method implements the Cmd_PSSendString command, which allows data to be sent directly without needing to place
	 * it in a file first.
	 * 
	 * @param data is a string containing either data to be sent to the printer or an 'embedded' command followed by data.
	 * @return
	 */
	private ApdError PSSendStringIndirect(String data)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		if (data != null)
		{
			Pattern embeddedCommandPattern = Pattern.compile(EMBEDDEDCOMMANDPATTERN);
			Matcher embeddedCommandMatcher = embeddedCommandPattern.matcher(data);
			
			if (embeddedCommandMatcher.find()) //If data is an embedded command...
			{			
				ApdCommand embeddedCommand = apdCommandFromEmbedded(data.substring(embeddedCommandMatcher.start(), embeddedCommandMatcher.end()));
				if (embeddedCommand.getCode() != ApdCommands.Cmd_PSSendString) //The APD specs say that Cmd_PSSendString cannot be embedded
				{
					if ( (embeddedCommandMatcher.end() + 2) <= data.length() )
						mPrnStringList = data.substring(embeddedCommandMatcher.end() + 2).split(EOL);
					res = PSExternal(embeddedCommand.getCode(), embeddedCommand.getParams());
				}
			}
			else
			{
				try
				{
					
					if (mApdTransport.write(data.getBytes()).ordinal() > ApdTransportError.OK.ordinal())
					{
						res = ApdError.ERR_SEND;
						mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_sendfailed"));
						Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
					}
				}
				catch (Exception e)
				{
					res = ApdError.ERR_SEND;
					mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_sendfailed"));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
				}
			}
		}
		else
		{
			res = ApdError.ERR_NODATA;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_nodata"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	/**
	 * This method implements the Cmd_PSSendFormat command, which allows to send a format file to the printer
	 * 
	 * @param formatFileName is the format filename including the path. Generally the extension is not specified
	 * @return //TODO: write a description of the return 
	 */
	private ApdError PSSendFormatIndirect(String formatFileName)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		if (formatFileName != null)
		{
			/*
			 * The first thing to check is whether the filename contains an extension or not. Generally there is not extension
			 * as this is determined by the printer type (the content of the format file is printer dependent).
			 * There might be situations where the extension may be passed, for instance when a format file can be used with
			 * any printer.
			 */
			formatFileName = getFileName(ApdPathIndex.FORMAT_PATH, formatFileName);
			if (formatFileName == null)
			{
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_configurationerror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, mLastMessage));
				return ApdError.ERR_INIT;
			}
			/*
			 * Check whether the format file contains embedded command
			 */
			FileReader formatFileReader = null;
			char[] buffer = null;
			try
			{
				File formatFile = new File(formatFileName);
				formatFileReader = new FileReader(formatFile);
				buffer = new char[(int)formatFile.length()];
				formatFileReader.read(buffer);
				String bufferString = new String(buffer);
				Pattern embeddedCommandPattern = Pattern.compile(EMBEDDEDCOMMANDPATTERN);
				Matcher embeddedCommandMatcher = embeddedCommandPattern.matcher(bufferString);
				
				if (embeddedCommandMatcher.find()) //If data is an embedded command...
				{			
					ApdCommand embeddedCommand = apdCommandFromEmbedded(bufferString.substring(embeddedCommandMatcher.start(), embeddedCommandMatcher.end()));
					if (embeddedCommand.getCode() != ApdCommands.Cmd_PSSendString) //The APD specs say that Cmd_PSSendString cannot be embedded
					{
						mPrnStringList = bufferString.substring(embeddedCommandMatcher.end() + 2).split(EOL);
						res = PSExternal(embeddedCommand.getCode(), embeddedCommand.getParams());
					}
				}
				else
					res = prnSendData(formatFileName);
			}
			catch (FileNotFoundException e)
			{
				res = ApdError.ERR_NOFILE;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_filenotfound"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, mLastMessage));
			} 
			catch (IOException e)
			{
				res = ApdError.ERR_BUFF;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ioerror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
				e.printStackTrace();
			}
			finally
			{
				if (formatFileReader != null)
					try
					{
						formatFileReader.close();
					}
					catch (IOException e)
					{
						e.printStackTrace();
					}
			}
		}
		else
		{
			res = ApdError.ERR_INIT;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_nodata"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, mLastMessage));
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
		
	/**
	 * Commands all active printers in the vicinity of the terminal to print a label showing their PID
	 * TODO: the C implementation of this doesn't do what the descriptions says. Yet, it sounds like making sense only for BT printers, maybe because it's only meant to be working
	 * with SPAN printers (SPAN = Symbol Personal Area Network)
	 * @param labelFileName is the id filename
	 * @return
	 */
	private ApdError PSPrintIdLabelIndirect(String labelFileName)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		if ( (labelFileName == null) || (labelFileName.compareTo("") == 0) ) //Filename is omitted
				labelFileName = PRN_ID_LABEL + "." + (char)(mApdConfiguration.getLanguage().ordinal() + ASCII_A);
		else //labelFileName is not empty
			if (labelFileName.indexOf('.') < 0) //The filename does not contain the extension
				labelFileName = labelFileName + "." + (char)(mApdConfiguration.getLanguage().ordinal() + ASCII_A);
		
		res = prnSendData(mApdConfiguration.getLabelIdPath() + labelFileName);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	/**
	 * 
	 */
	private ApdError writeApdResponse(String response)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		FileWriter fstream = null;
		BufferedWriter out = null;
		try
	    {
		    // Create file 
		    fstream = new FileWriter(RESPONSE_FILE);
		    out = new BufferedWriter(fstream);
		    out.write(response);
		    out.flush();
	    }
	    catch (IOException e)
	    {
		    res = ApdError.ERR_FILE;
		    mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ioerror"));
		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
        }
	    finally
	    {
	    	try
	    	{
				if (fstream != null)
					fstream.close();
				if (out != null)
					out.close();
			} catch (IOException e)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ioerror"))));
			}
	    }
	    Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
	    return res;
	}
	
	/**
	 * 
	 */
	private ApdError openPort(boolean open)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		ApdTransportError apdTransportError;
		try
		{
			if (open)
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Attempting to open channel"));
				apdTransportError = mApdTransport.open();
			}
			else
			{
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Attempting to close channel"));
				apdTransportError = mApdTransport.close();
			}
			if (apdTransportError.ordinal() > ApdTransportError.OK.ordinal())
			{
				res = ApdError.ERR_PORT;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_porterror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
			}
		} 
		catch (Exception e) {
			res = ApdError.ERR_PORT;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_porterror"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	/**
	 * Direct version of PSGetVersion
	 * @return the current version of the APD driver
	 */
	public String PSGetVersion()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		return APD_VERSION;
	}
	
	/**
	 * Direct version of PSGetLastMessage
	 * @return the last error message returned by APD
	 */
	synchronized public String PSGetLastMessage()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, mLastMessage));
		return mLastMessage;
	}
	
	private ApdError switchBthPower(boolean powerOn)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		IntentFilter filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
		BroadcastReceiver btActivationReceiver = new BroadcastReceiver() {
			
			@Override
			public void onReceive(Context context, Intent intent)
			{
				if (intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF) == BluetoothAdapter.STATE_ON)
				{
					Toast.makeText(Common.mainActivity, Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_bton")), Toast.LENGTH_SHORT).show();
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "BT enabled"));
				}
				//It's purposely STATE_TURNING_OFF rather than STATE_OFF
				else if (intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF) == BluetoothAdapter.STATE_TURNING_OFF) 
				{
					Toast.makeText(Common.mainActivity, Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_btoff")), Toast.LENGTH_SHORT).show();
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "BT disabled"));
				}
			}
		};
        Common.mainActivity.registerReceiver(btActivationReceiver, filter);
		if (adapter == null)
		{
			Common.mainActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run()
				{
					adapter = BluetoothAdapter.getDefaultAdapter();	
				}
			});
		}
		Timer operationTimer = new Timer();
		operationTimer.schedule(new TimerAction(), ApdBluetooth.TIMEOUT);
		while ( (adapter == null) && (mTimeoutFlag == false) ) {};
		operationTimer.cancel();
		mTimeoutFlag = false;
		if (adapter != null)
		{
			Timer mOperationTimer = new Timer();
			mOperationTimer.schedule(new TimerAction(), ApdBluetooth.TIMEOUT);
			if ( powerOn && (adapter.isEnabled() == false) )
				adapter.enable();
			else if ( (powerOn == false) && (adapter.isEnabled() == true) )
				adapter.disable();
			while ( (adapter.isEnabled() != powerOn) && (mTimeoutFlag == false) ) {};
			mOperationTimer.cancel();
	        if (mTimeoutFlag)
	        {
	        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "BT operation took too long"));
	        	mTimeoutFlag = false;
				res = ApdError.ERR_INIT;
	        }
		}
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		Common.mainActivity.unregisterReceiver(btActivationReceiver);
		return res;
	}
	
	private class TimerAction extends TimerTask
	{

		@Override
		public void run()
		{
			mTimeoutFlag = true;
		}
	}
	
	/**
	 * The input validation for this method is carried out by the calling method
	 */
	private ApdCommand apdCommandFromEmbedded(String embeddedCommand)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdCommand res = null;
		String command = embeddedCommand.substring(7, 11); //this is the substring containing the command
		String params = null;
		if (embeddedCommand.indexOf(",") >= 0)
			params = embeddedCommand.substring(embeddedCommand.indexOf(",") + 1, embeddedCommand.indexOf("]"));
		// need to convert the command code from hexadecimal to decimal
		res = new ApdCommand(Integer.parseInt(command, 16), params);
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	/**
	 * This method changes the default path for either Format, Template or IdLabel files
	 * @param index FORMAT_PATH to change the current Format path, TEMPLATE_PATH or IDLABEL_PATH 
	 * @param path the new path to be set
	 * @return a relevant ApdError
	 */
	private ApdError changeCurrentPath(ApdPathIndex index, String path)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		if ( (path != null) && (path.compareTo("") != 0) )
		{
			try
			{
				File newPathFile = new File(Common.parseAndroidURI(path).getPath());
				if ( (newPathFile.isDirectory() == false) || (newPathFile.exists() == false) )
				{
					mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_pathnotvalid"));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
					return ApdError.ERR_FILE;
				}
				if (path.endsWith("/") == false)
					path = path + "/";
				if (index == ApdPathIndex.FORMAT_PATH)
					mApdConfiguration.setFormatPath(path);
				else if (index == ApdPathIndex.TEMPLATE_PATH)
					mApdConfiguration.setTemplatePath(path);
				else
					mApdConfiguration.setLabelIdPath(path);
			}
			catch (InvalidParameterException e1)
			{
				// Do nothing
			} 
			catch (URISyntaxException e1)
			{
				// Do nothing
			}
			try
			{
				ApdConfigurator.writeConfiguration(mApdConfiguration);
			}
			catch (IOException e)
			{
				res = ApdError.ERR_FILE;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ioerror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
			}
		}
		else
		{
			res = ApdError.ERR_NODATA;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_pathnotvalid"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	/**
	 * 
	 * @param apdPrinterId is the PID of the device to be set as current printer
	 * @return a relevant ApdError
	 */
	private ApdError setCurrentPrinter(String apdPrinterId)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_INV_ID;
		ApdConfiguration newPrinter = new ApdConfiguration();
		String address;
		StringBuffer apdPrinterIdBuffer = new StringBuffer(apdPrinterId);
		
		openPort(false); //Closes the current connection if any is active
		if (isPatternMatching(apdPrinterIdBuffer.toString(), ApdConfiguration.FOUR_CHAR_ID_WIRELESS_REGEX) >= 0) //If the pid is a 4-char one
		{
			//Check whether the PID contains an address (either MAC or IP)
			int barPosition = apdPrinterIdBuffer.toString().indexOf('|');
			if (barPosition == 4) //there is a bar but it's not where it should be
			{
				apdPrinterIdBuffer.insert(4, ':');
				barPosition++;
			}
			address = apdPrinterIdBuffer.substring(barPosition + 1, apdPrinterIdBuffer.length());
			//Check whether the PID is for a BT or WLAN printer
			boolean validAddress;
			if (apdPrinterIdBuffer.charAt(0) == 'B')
			{
				//A mac address is expected
				//Check whether the address is a valid MAC address
				validAddress = false;
				if (isPatternMatching(address, ApdConfiguration.MAC_ADDRESS_COLUMNS_REGEX) >= 0)
					validAddress = true;
				else if (isPatternMatching(address, ApdConfiguration.MAC_ADDRESS_NOCOLUMNS_REGEX) >= 0)
				{
					validAddress = true;
					address = fixAddress(address, AddressType.MAC_ADDRESS);
				}
		
				if (validAddress == false)
				{
					mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_pidnotvalid"));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
					return ApdError.ERR_INV_ID; //BT PID does not contain a valid MAC address
				}
				newPrinter.setBtMac(address);
				newPrinter.setRfComm(APD_BT_DEFAULT_PORT);
				newPrinter.setTransport(Transport.Bluetooth);
			}
			else if (apdPrinterIdBuffer.charAt(0) == 'W')
			{
				//An IP address is expected
				//Check whether the address is a valid IP address
				validAddress = false;
				if (isPatternMatching(address, ApdConfiguration.IP_ADDRESS_DOTS_REGEX) >= 0)
					validAddress = true;
				else
				{
					if (isPatternMatching(address, ApdConfiguration.IP_ADDRESS_NODOTS_REGEX) >= 0)
					{
						validAddress = true;
						//Need to add the columns to the mac address
						address = fixAddress(address, AddressType.IP_ADDRESS);
					}
				}
				if (validAddress == false)
				{
					mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_pidnotvalid"));
					Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
					return ApdError.ERR_INV_ID; //BT PID does not contain a valid MAC address
				}
				newPrinter.setIpAddress(address);
				newPrinter.setIpPort(APD_WLAN_DEFAULT_PORT);
				newPrinter.setTransport(Transport.Wlan);
			}
			// Check the port number
			int columnPosition = apdPrinterIdBuffer.toString().indexOf(':');
			if (columnPosition >= barPosition) //first column is not where expected
			{
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_pidnotvalid"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
				return ApdError.ERR_INV_ID;
			}
			//Check that the port number is valid
			if ( (barPosition - columnPosition > 1) && (isPatternMatching(apdPrinterIdBuffer.substring(columnPosition + 1, barPosition - 1), ApdConfiguration.PORT_REGEX) >= 0) )
				newPrinter.setIpPort(Integer.valueOf(apdPrinterIdBuffer.substring(columnPosition + 1, barPosition)));
			
			newPrinter.setModel(Integer.valueOf(apdPrinterIdBuffer.substring(2, 4)));
			newPrinter.setId(0);
			newPrinter.setLanguage(Language.values()[apdPrinterIdBuffer.charAt(1) - ASCII_A]);
			
			newPrinter.setFormatPath(mApdConfiguration.getFormatPath());
			newPrinter.setTemplatePath(mApdConfiguration.getTemplatePath());
			newPrinter.setLabelIdPath(mApdConfiguration.getLabelIdPath());
			
			mApdConfiguration = newPrinter;
			res = ApdError.ERR_OK;
			//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
			try
			{
				ApdConfigurator.writeConfiguration(mApdConfiguration);
				mApdTransport.destroy();
				mApdTransport = null;
				mApdTransport = ApdTransportFactory.createTransport(mApdConfiguration);
			}
			catch (IOException e)
			{
				res = ApdError.ERR_FILE;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ioerror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
			} 
			catch (Exception e)
			{
				res = ApdError.ERR_INIT;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_unknownerror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
				e.printStackTrace();
			}
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
			return res;
		}
		
		mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_pidnotsupported"));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		if (isPatternMatching(apdPrinterId, ApdConfiguration.FIVE_CHAR_ID_TOSHIBA_REGEX) >= 0) //If the pid is a 5-char one
			return ApdError.ERR_SETUP; //this pid is not supported
		
		if (isPatternMatching(apdPrinterId, ApdConfiguration.FOUR_CHAR_ID_WIRED_REGEX) >= 0)
			return ApdError.ERR_SETUP; //this pid is not supported
		
		if (isPatternMatching(apdPrinterId, ApdConfiguration.FIVE_CHAR_ID_SPAN_REGEX) >= 0)
			return ApdError.ERR_SETUP; //this pid is not supported
		
		if (isPatternMatching(apdPrinterId, ApdConfiguration.FIVE_CHAR_ID_WIRED_REGEX) >= 0)
			return ApdError.ERR_SETUP; //this pid is not supported
		
		if (isPatternMatching(apdPrinterId, ApdConfiguration.SEVEN_CHAR_ID_WIRED_REGEX) >= 0)
			return ApdError.ERR_SETUP; //this pid is not supported
		
		if (isPatternMatching(apdPrinterId, ApdConfiguration.SEVEN_CHAR_ID_SPAN_REGEX) >= 0)
			return ApdError.ERR_SETUP; //this pid is not supported
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	
	/**
	 * Requests the PID of the current printer
	 * @return the string coding the printer PID 
	 */
	synchronized public String PSGetPrinterList()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		String res = null;
		//If the printer connection is wlan
		if ( (mApdConfiguration.getBtMac() == null) && (mApdConfiguration.getIpAddress() != null) )
			res = String.format("W%c%02d:%d|%S", (char)(mApdConfiguration.getLanguage().ordinal() + ASCII_A), 
										   mApdConfiguration.getModel(), 
										   mApdConfiguration.getIpPort(),
										   removeSeparatorFromAddress(mApdConfiguration.getIpAddress(), AddressType.IP_ADDRESS));
		//if the printer connection is bluetooth
		else if ( (mApdConfiguration.getIpAddress() == null) && (mApdConfiguration.getBtMac() != null) )
		{
			//For mantaining the compatibility with the win version, the columns from the bt mac address have to be removed
			res = String.format("B%c%02d:%d|%S", (char)(mApdConfiguration.getLanguage().ordinal() + ASCII_A),
												 mApdConfiguration.getModel(),
												 mApdConfiguration.getRfComm(),
												 removeSeparatorFromAddress(mApdConfiguration.getBtMac(), AddressType.MAC_ADDRESS));
		}
		else //in case the printer configuration is inconsistent
			res = null;
		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	private int isPatternMatching(String input, String patternString)
	{
		Pattern pattern = Pattern.compile(patternString);
		Matcher patternMatcher = pattern.matcher(input);
		if (patternMatcher.find())
			return patternMatcher.start();
		else
			return -1;
	}
	
	private String fixAddress(String address, AddressType addressType)
	{
		int upperBound = 0;
		int increment = 0;
		char separator;
		
		switch (addressType.ordinal())
		{
			case 0: //AddressType.MAC_ADDRESS
			{
				upperBound = 10;
				increment = 2;
				separator = ':';
				break;
			}
			case 1: //AddressType.IP_ADDRESS
			{
				upperBound = 9;
				increment = 3;
				separator = '.';
				break;
			}
			default:
				return address;
		}
		
		StringBuffer addressBuffer = new StringBuffer(address);
		int j = 0;
		for (int i=increment; i<=upperBound; i=i+increment)
		{
			addressBuffer.insert(i+j, separator);
			j++;
		}
		return addressBuffer.toString();
	}
	
	private String removeSeparatorFromAddress(String address, AddressType addressType)
	{
		StringBuffer addressBuffer = new StringBuffer();
		String separator;
		if (addressType == AddressType.MAC_ADDRESS)
			separator = ":";
		else
			separator = ".";
		for (int i=0; i < address.length(); i++)
			if (address.substring(i, i+1).compareTo(separator) != 0)
				addressBuffer.append(address.substring(i, i+1));
		return addressBuffer.toString();
	}
	
	private String getFileName(ApdPathIndex pathIndex, String fileName)
	{
		String res = null;
		
		String ext = "";
		int posOfFinalSlash = fileName.lastIndexOf("/");
		try
		{
			if ( (fileName.indexOf(".") == -1 ) || (fileName.lastIndexOf(".") < posOfFinalSlash) ) //no extension is given
			{
				if (mApdConfiguration.getModel() > 0)			
					ext = "." + String.format("%c%02d", (char)(mApdConfiguration.getLanguage().ordinal() + ASCII_A), mApdConfiguration.getModel());
				else
					ext = "." + String.format("%c", (char)(mApdConfiguration.getLanguage().ordinal() + ASCII_A));
			}
			if (posOfFinalSlash == -1) //Path is not in the parameter...
			{
				if (pathIndex == ApdPathIndex.FORMAT_PATH)  
					res = mApdConfiguration.getFormatPath() + fileName; 
				else if (pathIndex == ApdPathIndex.TEMPLATE_PATH)
					res = mApdConfiguration.getTemplatePath() + fileName;
				else
					res = mApdConfiguration.getLabelIdPath() + fileName;
			}
			else
				res = fileName;
			try
			{
				res = Common.parseAndroidURI(res + ext).getPath();
			} 
			catch (InvalidParameterException e)
			{
				res = null;
			} 
			catch (URISyntaxException e)
			{
				res = null;
			}
		}
		catch (NullPointerException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while reading APD configuration"));
			res = null;
		}
		return res;
	}
	
	/**
	 * This direct command may be used to send data to the printer without having to put data into a file
	 * @param pcData is an array of strings containing the data to send to the printer
	 * @param iQty is the number of times that the data have to be sent
	 * @return
	 */
	synchronized public ApdError PSSendData(String pcData[], int iQty)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		mPrnStringList = pcData;
		return PSExternal(ApdCommands.Cmd_PSSendData, String.valueOf(iQty)); 
	}
	
	private ApdError PSSendDataIndirect(String quantity)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		ApdError intermediateRes;
		String templateFileName;
		int intQuantity = 0;
		try
		{
			intQuantity = Integer.valueOf(quantity);
		} 
		catch (NumberFormatException e)
		{
			res = ApdError.ERR_NOQTY;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_nodata"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		if (intQuantity == 0)
			return res;
		if (prnTextToList() > 0)
		{
			templateFileName = getFileName(ApdPathIndex.TEMPLATE_PATH, mPrnDataFormat);
			if (templateFileName == null)
			{
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_configurationerror"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, mLastMessage));
				return ApdError.ERR_INIT;
			}
			//Check whether the template file exists
			File templateFile = new File(templateFileName);
			if (templateFile.exists() == false)
			{
				if (isStoredFormatSupported())
					intermediateRes = prnStoredFormat(mApdConfiguration.getLanguage(), intQuantity);
				else
					intermediateRes = ApdError.ERR_NOFILE;
			}
			else
				intermediateRes = prnGenericDrv(templateFileName, intQuantity);
			if (intermediateRes == ApdError.ERR_OK)
				res = prnSendData(DATA_FILE);
			prnDestroyList();
		}		
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	private void prnDestroyList()
	{
		mPrnDataList = null;
		mPrnDataCount = 0;
	}
	
	private int prnTextToList()
	{
		int wLoop;
		mPrnDataCount = 0;
		
		prnDestroyList();
		if (mPrnStringList != null)
		{
			mPrnDataCount = mPrnStringList.length;
			if (mPrnDataCount > 0)
			{
				mPrnDataList = new String[mPrnDataCount];
				for (wLoop = 0; wLoop < mPrnDataCount; wLoop++)
				{
					// Capture the first field (format name).
					if (wLoop == 0)
						mPrnDataFormat = mPrnStringList[wLoop];
					
					mPrnDataList[wLoop] = new String(mPrnStringList[wLoop]);
				}
			}
		}
		mPrnStringList = null;
		
		// if no data in mPrnStringList,try the file
		if (mPrnDataCount == 0)
		{
			FileInputStream fileInputStream = null;
			
			try
			{
				fileInputStream = new FileInputStream(PRINTER_DATA_FILE);
				BufferedReader input =  new BufferedReader(new FileReader(PRINTER_DATA_FILE));
				input.mark(fileInputStream.available());
				while (input.readLine() != null)
					mPrnDataCount++;
				input.reset();
				mPrnDataList = new String[mPrnDataCount];
				int i = 0;
				String line;
				while ((line = input.readLine()) != null)
					mPrnDataList[i++] = new String(line);
			} 
			catch (FileNotFoundException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			finally
			{
				try
				{
					if (fileInputStream != null)
						fileInputStream.close();
				}
				catch (IOException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return mPrnDataCount;
	}
	
	private StringBuffer getNullTerminatedStringBuffer(String input)
	{
		if (input == null)
			return null;
		int nullCharPos = input.indexOf('\0');
		return new StringBuffer(input.substring(0, nullCharPos));
	}
	
	private ApdError prnGenericDrv(String formatFileName, int quantity)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		boolean bLooking = true;
		boolean bFound = false;
		int iLoop;
		int iLen = 0;
		int iPos = 0;
		int iField = 0;
		int iPcStr;
		int iFormat;
		int iHere;
		int iNext;
		StringBuffer outputContent = new StringBuffer();
		StringBuffer pcNumber = new StringBuffer();
		FileWriter outputFileStream = null;
		StringBuffer nullTerminatedOutput = null;
		FileReader fileReader = null;
		
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		try
		{
			File formatFile = new File(formatFileName);
			fileReader = new FileReader(new File(formatFileName));
			char[] formatContent = new char[(int)formatFile.length()];
			fileReader.read(formatContent);
			outputFileStream = new FileWriter(DATA_FILE);
			outputContent.setLength(MAX_DATASTRING);
			
			for (iLoop = 0; iLoop < formatContent.length; iLoop++)
			{
				if (formatContent[iLoop] == '$')
				{
					if ( bFound && (iLen == 1) )
					{
						//Reached the end of ']$$'
						outputContent.setCharAt(iPos, '\0');
						iLen = 0;
						iPos = 0;
						bFound = false;
						bLooking = true;
						nullTerminatedOutput = getNullTerminatedStringBuffer(outputContent.toString());
						if ((iPcStr = (nullTerminatedOutput.indexOf(":"))) > 0)
						{
							outputContent.setCharAt(iPcStr, '\0');
							iPcStr++;
							String temp = nullTerminatedOutput.toString().substring(iPcStr);
							nullTerminatedOutput.replace(0, temp.length() - 1, temp);
							nullTerminatedOutput.delete(temp.length(), nullTerminatedOutput.length() - 1);
							nullTerminatedOutput.setLength(temp.length());
							for (iFormat = 0; iFormat < ApdDataType.INVALID.ordinal(); iFormat++)
								if (nullTerminatedOutput.toString().contains(prnFormatting[iFormat]))
									break;
							if (iFormat < ApdDataType.INVALID.ordinal())
							{
								for (iHere = iNext = 0; 
									 iHere < nullTerminatedOutput.length();
									 iHere++, iNext++)
								{
									if (nullTerminatedOutput.charAt(iHere) == '\\')
									{
										iHere++;
										switch (nullTerminatedOutput.charAt(iHere))
										{
											case '0':
											{
												nullTerminatedOutput.setCharAt(iHere, '\0');
												break;
											}
											case 'a':
											{
												nullTerminatedOutput.setCharAt(iHere, '\007');
												break;
											}
											case 'b':
											{
												nullTerminatedOutput.setCharAt(iHere, '\b');
												break;
											}
											case 'f':
											{
												nullTerminatedOutput.setCharAt(iHere, '\f');
												break;
											}
											case 'r':
											{
												nullTerminatedOutput.setCharAt(iHere, '\r');
												break;
											}
											case 't':
											{
												nullTerminatedOutput.setCharAt(iHere, '\t');
												break;
											}
											case 'n':
											{
												nullTerminatedOutput.setCharAt(iHere, '\n');
												break;
											}
											case 'x':
											{
												pcNumber.setCharAt(0, nullTerminatedOutput.charAt(iHere + 1));
												pcNumber.setCharAt(1, nullTerminatedOutput.charAt(iHere + 2));
												//pcNumber.setCharAt(2, '\0');
												if (isPatternMatching(pcNumber.toString(), "\b[0-9a-fA-F]+\b") >= 0)
												{
													iHere += 2;
													nullTerminatedOutput.setCharAt(iHere, (char)(Integer.valueOf(pcNumber.toString(), 16).intValue()));
												}
												else
												{
													iHere--;
												}
												break;
											}
											case '\\':
											{
												nullTerminatedOutput.setCharAt(iHere, '\\');
												break;
											}
											default:
											{
												iHere--;
												break;
											}
										}
										if (iHere > iNext)
											nullTerminatedOutput.setCharAt(iNext, outputContent.charAt(iHere));
									}
								}
								if (iNext < nullTerminatedOutput.length())
									nullTerminatedOutput.setCharAt(iNext, '\0');
							}
						}
						else
						{
							iFormat = ApdDataType.INVALID.ordinal();
						}
						
						if (outputContent.charAt(0) == 'Q')
						{
							String formattedQuantity = null;
							if (iFormat == ApdDataType.CHAR.ordinal())
								formattedQuantity = String.format(nullTerminatedOutput.toString(), (char)quantity);
							else if (iFormat == ApdDataType.HEX.ordinal())
								formattedQuantity = String.format(nullTerminatedOutput.toString(), quantity);
							else if (iFormat == ApdDataType.STRING.ordinal())
								formattedQuantity = String.valueOf(quantity);
							else
								formattedQuantity = String.format("%d", quantity); //Java doesn't support %u
								
							if (formattedQuantity != null)
								outputFileStream.write(formattedQuantity);
						}
						else if (outputContent.charAt(0) == 'N')
						{
							iField = Integer.valueOf(outputContent.substring(1,2));
							while ( (iField > 0) && (iField < mPrnDataCount) )
							{
								if (mPrnDataList[iField].length() < MAX_DATASTRING)
								{
									outputFileStream.write(mPrnDataList[iField] + EOL);
								}
								iField++;
							}
						}
						else
						{
							iField = Integer.valueOf(nullTerminatedOutput.toString());
								//If field number refers to valid data include it!
							if ( (iField > 0) && (iField < mPrnDataCount) )
								outputFileStream.write(mPrnDataList[iField]);
						}
					}
					else
					{
						iLen++;
					}
				}
				else
				{
					if (bFound) // ']' without a "$$" so put it with the data
					{
						if (iPos < outputContent.length())
						{
							outputContent.setCharAt(iPos++, ']');
						}
						else if (bLooking) // Buffer full, but no $$[, so send it anyway
						{
							outputFileStream.write(outputContent.toString(), 0, iPos);
							iPos = 0;
						}		
					}
					if (bLooking) // If looking for the '[' then consider it found if more than two '$'
					{
						if ( (bFound = (formatContent[iLoop] == '[')) && (iLen > 1) != false)
							iLen -= 2;
					}
					else // If within the "$$[" look for the ']'
					{
						bFound = formatContent[iLoop] == ']';
					}
					while (iLen > 0) // Put any surplus '$' in the data.
					{
						if (iPos < outputContent.length())
							outputContent.setCharAt(iPos++, '$');
						else
							if (bLooking) // Buffer full, but no $$[, so send it anyway
							{
								outputFileStream.write(outputContent.toString(), 0, iPos);
								iPos = 0;
							}
						iLen--;
					}
					if (bFound)
					{
						if (bLooking)
						{
							if (iPos > 0)
								outputFileStream.write(outputContent.toString(), 0, iPos);
							iLen = 0;
							iPos = 0;
							bFound = false;
							bLooking = false;
						}
						else
							iLen = 0;
					}
					else
					{
						if (iPos < outputContent.length())
							outputContent.setCharAt(iPos++, formatContent[iLoop]);
						else
						{
							if (bLooking) // Buffer full, but no $$[, so send it anyway
							{
								outputFileStream.write(outputContent.toString(), 0, iPos);
								iPos = 0;
							}
						}
					}
				}
			}
			if (iPos > 0)
				outputFileStream.write(outputContent.toString(), 0, iPos);
		}
		catch (FileNotFoundException e)
		{
			res = ApdError.ERR_NOFILE;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_filenotfound"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		} 
		catch (IOException e)
		{
			res = ApdError.ERR_BUFF;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ioerror"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		finally
		{
			try
			{
				if (outputFileStream != null)
					outputFileStream.close();
				if (fileReader != null)
					fileReader.close();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	private boolean isStoredFormatSupported()
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		boolean res = false;
		
		if (mApdConfiguration != null)
		{
			if ( (mApdConfiguration.getLanguage() == Language.C) ||
				 (mApdConfiguration.getLanguage() == Language.E) ||
				 (mApdConfiguration.getLanguage() == Language.M) ||
				 (mApdConfiguration.getLanguage() == Language.R) ||
				 (mApdConfiguration.getLanguage() == Language.S) ||
				 (mApdConfiguration.getLanguage() == Language.T) ||
				 (mApdConfiguration.getLanguage() == Language.Z))
				return true;
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	private ApdError prnStoredFormat(Language language, int quantity)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		//FileOutputStream fileOutputStream = null;
		FileWriter fileOutputStream = null;
		StringBuffer pcTxBuff;
		try
		{
			pcTxBuff = new StringBuffer();
			//fileOutputStream = new FileOutputStream(PRINTER_DATA_FILE); 
			File dataFile = new File(DATA_FILE);
			if (dataFile.exists())
				dataFile.delete();
			fileOutputStream = new FileWriter(DATA_FILE);
			if (language == Language.R)
			{
				pcTxBuff.append(String.format("^P|%s|%d", mPrnDataList[0], quantity));
			}
			else if (language == Language.M)
			{
				pcTxBuff.append(String.format("{\r\nB,%s,N,%d |\r\n", mPrnDataList[0], quantity));
			}
			else if (language == Language.T)
			{
				String format = null;
				if (mApdConfiguration.getId() == 0)
					format = new String("\27X%c\01%c"); //TODO: This has to be re-checked
				else
					format = new String("X%c\01%c");
				pcTxBuff.append(String.format(format, (char)(Integer.valueOf(mPrnDataList[0]).intValue()), (char)quantity));
			}
			else if (language == Language.E)
			{
				pcTxBuff.append(String.format("FR\"%s\"\r\n?\r\n", mPrnDataList[0]));
			}
			else if (language == Language.C)
			{
				pcTxBuff.append(String.format("! UF F%s.FMT\r\n%d\r\n", mPrnDataList[0], quantity));
			}
			else if (language == Language.S)
			{
				pcTxBuff.append(String.format("\27A\r\n\27TB%02d\r\n", Integer.valueOf(mPrnDataList[0]).intValue()));
			}
			else if (language == Language.Z)
			{
				pcTxBuff.append(String.format("^XA^XF%s^FS\r\n", mPrnDataList[0]));
			}
			
			if (pcTxBuff.length() > 0)
				fileOutputStream.write(pcTxBuff.toString());
			fileOutputStream.flush();
			pcTxBuff.delete(0, pcTxBuff.length()); //Reset the string buffer
			
			//Fill data fields
			for (int i=1; i < mPrnDataCount; i++)
			{
				if (language == Language.R)
				{
					pcTxBuff.append(String.format("|%s", mPrnDataList[i]));
				}
				else if (language == Language.M)
				{
					pcTxBuff.append(String.format("%d,\"%s\" |\r\n", i, mPrnDataList[i]));
				}
				else if (language == Language.T)
				{
					pcTxBuff.append(String.format("%s\n", mPrnDataList[i]));
				}
				else if ( (language == Language.E) || (language == Language.C) )
				{
					pcTxBuff.append(String.format("%s\r\n", mPrnDataList[i]));
				}
				else if (language == Language.S)
				{
					pcTxBuff.append(String.format("\27D%s\r\n", mPrnDataList[i]));
				}
				else if (language == Language.Z)
				{
					pcTxBuff.append(String.format("^FN%d^FD%s^FS\r\n", i, mPrnDataList[i]));
				}
			}
			if (pcTxBuff.length() > 0)
				fileOutputStream.write(pcTxBuff.toString());
			fileOutputStream.flush();
			pcTxBuff.delete(0, pcTxBuff.length()); //Reset the string buffer
			
			//Format trailer
			if (language == Language.R)
			{
				pcTxBuff.append(String.format("|^"));
			}
			else if (language == Language.M)
			{
				pcTxBuff.append(String.format("}\r\n"));
			}
			else if (language == Language.T)
			{
				if (mApdConfiguration.getId() > 0)
				{
					fileOutputStream.write(mApdConfiguration.getId());
					fileOutputStream.flush();
				}
			}
			else if (language == Language.E)
			{
				pcTxBuff.append(String.format("P%u,1\r\n", quantity));
			}
			else if (language == Language.C)
			{
			}
			else if (language == Language.S)
			{
				pcTxBuff.append(String.format("\27Q%04d\r\n\27Z\r\n", quantity));
			}
			else if (language == Language.Z)
			{
				pcTxBuff.append(String.format("^PQ%d^XZ\r\n", quantity));
			}
			if (pcTxBuff.length() > 0)
				fileOutputStream.write(pcTxBuff.toString());
			fileOutputStream.flush();
		}
		catch (FileNotFoundException e)
		{
			res = ApdError.ERR_NOFILE;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_filenotfound"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		} 
		catch (IOException e)
		{
			res = ApdError.ERR_INIT;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_unknownerror"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		finally
		{
			try
			{
				if (fileOutputStream != null)
					fileOutputStream.close();
			}
			catch (IOException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	private ApdError prnSendData(String fileName)
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdError res = ApdError.ERR_OK;
		//mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_ok"));
		
		byte buffer[] = null;
		InputStream is = null;
		
		File file = new File(fileName);
		if (file.exists() == false)
		{
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_filenotfound"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
			return ApdError.ERR_NOFILE;
		}
		try
		{
			is = new FileInputStream(file);
			buffer = new byte[(int)file.length()];
			is.read(buffer, 0, buffer.length);
			if (mApdTransport.write(buffer).ordinal() > ApdTransportError.OK.ordinal())
			{
				res = ApdError.ERR_SEND;
				mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_sendfailed"));
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
			}
			
		} 
		catch (FileNotFoundException e)
		{
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_filenotfound"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		catch (NullPointerException e)
		{
			res = ApdError.ERR_SEND;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_sendfailed"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
			e.printStackTrace();
		}
		catch (IOException e)
		{
			res = ApdError.ERR_SEND;
			mLastMessage = Common.mainActivity.getString(RhoExtManager.getResourceId("string","apd_sendfailed"));
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mLastMessage));
		}
		finally
		{
			if (is != null)
				try
				{
					is.close();
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
		}
		return res;
	}
	
	private class ApdCommand
	{
		/**
		 * @return the code
		 */
		public int getCode() {
			return code;
		}

		/**
		 * @return the params
		 */
		public String getParams() {
			return params;
		}

		private int code;
		private String params;
		
		/**
		 * @param code
		 * @param params
		 */
		public ApdCommand(int code, String params)
		{
			this.code = code;
			this.params = params;
		}
	}
}
