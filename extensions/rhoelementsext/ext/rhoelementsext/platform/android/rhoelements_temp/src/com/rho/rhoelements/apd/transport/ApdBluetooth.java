package com.rho.rhoelements.apd.transport;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Timer;
import java.util.TimerTask;
import java.util.UUID;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.ElementsCore;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.NavigateException;
import com.rho.rhoelements.apd.ApdConfiguration;

/**
 * @author FPC843
 * Allows APD to communicate to BT devices
 */
public class ApdBluetooth extends ApdTransport
{
	/*private static final UUID ELEMENTS_UUID =
        UUID.fromString("ac3098e4-4f4b-45f7-ba93-cb2321ab0f79");*/
	private static final UUID ELEMENTS_UUID =
        UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	private static final String BT_MAC_ADDRESS_PATTERN = "([0-9A-Fa-f]{2}([:])){5}([0-9A-Fa-f]{2})";
	private BluetoothAdapter mAdapter;
	private BluetoothDevice mDevice;
	private ConnectionStatus mConnStatus;
	private DiscovererThread mDiscovererThread;
	private BluetoothSocket mSocket = null;
	private boolean mTimeoutFlag = false;
	public static final int TIMEOUT = 15000; //Operation timeout
	private Timer mOperationTimer = null;
	private OutputStream outStream = null;
	private boolean mIsConnectionlost =false;

	
	
	/**
	 * Constructor
	 * @param btAddress is the MAC address of the BT device we want to connect to
	 */
	public ApdBluetooth(ApdConfiguration apdConfiguration)
	{
		super(apdConfiguration);
		mConnStatus = ConnectionStatus.IDLE;
	}
	
	/**
     * Initiate a connection to a remote device.
     */
	@Override
    public ApdTransportError open()
    {
        ApdTransportError res = ApdTransportError.OK; 
        mConnStatus = ConnectionStatus.IDLE;
        Pattern macAddressPattern = Pattern.compile(BT_MAC_ADDRESS_PATTERN);
		Matcher macAddressMatcher = macAddressPattern.matcher(mApdConfiguration.getBtMac());
		
		if (macAddressMatcher.find()) //if the input is a valid mac address
		{
			Common.mainActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run()
				{
					mAdapter = BluetoothAdapter.getDefaultAdapter();
				}
			});
			mOperationTimer = new Timer();
			mOperationTimer.schedule(new TimerAction(), TIMEOUT);
			while ( (mAdapter == null) && (mTimeoutFlag == false) ) {};
			mOperationTimer.cancel();
			mTimeoutFlag = false;
			if (mDiscovererThread == null)
				mDiscovererThread = new DiscovererThread();
			/*
			 * Start the device discovery to find the device we want to connect to
			 */
			mOperationTimer = new Timer();
			mOperationTimer.schedule(new TimerAction(), TIMEOUT);
			while ( (mAdapter == null) && (mTimeoutFlag == false) ) {};
			mOperationTimer.cancel();
			mTimeoutFlag = false;
			if (!mAdapter.isEnabled())
				mAdapter.enable();
			mOperationTimer = new Timer();
			mOperationTimer.schedule(new TimerAction(), TIMEOUT);
			while ( (mAdapter.isEnabled() == false) && (mTimeoutFlag == false) ) {};
			mOperationTimer.cancel();
			if (mTimeoutFlag)
	        {
	        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Could not activate Bluetooth"));
	        	mTimeoutFlag = false;
				return ApdTransportError.IOERROR;
	        }
			IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
			filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
			filter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);
			if (mDiscovererThread == null)
				mDiscovererThread = new DiscovererThread();
	        Common.mainActivity.registerReceiver(mDiscovererThread.mReceiver, filter);
	        Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Discovery started..."));
	        mDiscovererThread.start();      
	        mOperationTimer = new Timer();
	        mOperationTimer.schedule(new TimerAction(), TIMEOUT);
	        while ( (mConnStatus == ConnectionStatus.IDLE) && (mTimeoutFlag == false) ) {}
	        Common.mainActivity.unregisterReceiver(mDiscovererThread.mReceiver);
	        mDiscovererThread = null;
	        mOperationTimer.cancel();
	        if (mTimeoutFlag)
	        {
	        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "BT operation took too long"));
	        	mTimeoutFlag = false;
	        	mConnStatus = ConnectionStatus.IDLE;
				return ApdTransportError.IOERROR;
	        }
	        if (mConnStatus == ConnectionStatus.DEVICENOTFOUND)
	        {
	        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mApdConfiguration.getBtMac() + " device not found."));
				return ApdTransportError.IOERROR;
	        }
		}
		else
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mApdConfiguration.getBtMac() + " is not a valid BT MAC address."));
			return ApdTransportError.IOERROR;
		}
    	
		if (mConnStatus.ordinal() < ConnectionStatus.CONNECTING.ordinal())
        {	
	    	try
	        {
	    		mSocket = mDevice.createInsecureRfcommSocketToServiceRecord(ELEMENTS_UUID);
				mConnStatus = ConnectionStatus.CONNECTING;
				showDialog(ElementsCore.APD_WAIT_DIALOG);
				mSocket.connect();
				mConnStatus = ConnectionStatus.CONNECTED;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Printer connected succesfully"));
			}
	        catch (IOException e)
	        {
	        	res = ApdTransportError.IOERROR;
	        	connectionFailed();
	        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error with connecting to the printer"));
	        	e.printStackTrace();
			}
	        finally
	        {
	        	dismissDialog(ElementsCore.APD_WAIT_DIALOG);
	        }
        }
        return res;
    }

    /**
     * @see ApdTransport
     */
	@Override
    public synchronized ApdTransportError write(final byte[] out)
    {
	
	boolean isPortOpenedBythisInstance = false;
    	// Connect to the printer first
    	ApdTransportError res = ApdTransportError.OK; 
    		
		  if (mConnStatus == ConnectionStatus.IDLE)
			{
			 res = open();
			 isPortOpenedBythisInstance=true;
			}

			 
        if (mConnStatus == ConnectionStatus.CONNECTED)
        {
        	try
        	{
        		final int bufferLength = out.length;
        		int bytesAvailable = out.length;
        		int toSendSize = Math.min(bytesAvailable, MAX_BUFFER_SIZE);
        		mOffset = 0;
        		outStream = mSocket.getOutputStream();
        		showDialog(ElementsCore.APD_PROGRESS_DIALOG);
        		mConnStatus = ConnectionStatus.TRANSFERRING;
				while (bytesAvailable > 0)
				{
					outStream.write(out, mOffset, toSendSize);
					bytesAvailable -= toSendSize;
					mOffset += toSendSize;
					if (sProgressEnabled)
					{
						Common.mainActivity.runOnUiThread(new Runnable() {
							
							@Override
							public void run() {
								int progress = mOffset * 100 / bufferLength;
								Common.elementsCore.mProgressHandler.sendEmptyMessage(progress);
							}
							
							
						});
						wait(400); //Needed to get the message delivered to the handler in the ElementsActivity
					}
					toSendSize = Math.min(bytesAvailable, MAX_BUFFER_SIZE);
					
				}
				mConnStatus = ConnectionStatus.CONNECTED;
				
			}
        	catch (IOException e)
        	{
        		
        		res = ApdTransportError.IOERROR; 
        		dismissDialog(ElementsCore.APD_PROGRESS_DIALOG);
        		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while sending data to the printer"));
        		connectionLost();
        		if(e.toString().indexOf("Connection timed out")>0)
        		{
        		mIsConnectionlost = true;
        		}
     
        		
        		
			} catch (InterruptedException e)
			{
				res = ApdTransportError.IOERROR; 
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while sending data to the printer"));
			} 
        	finally
        	{
        		if (outStream != null)
        		{
        			try
        			{
						outStream.flush();
					} 
					catch (IOException e)
					{
						e.printStackTrace();
					}
        		}
        		
				if(true == isPortOpenedBythisInstance)
				{
					close();
				}
					
				
				
				if(mIsConnectionlost == true)
				{
					mIsConnectionlost = false;
				
						if(isPortOpenedBythisInstance==false)
						{
						open();
						}

							    
        			rewrite(out);
				}
	    	}
        }
        return res;
    }
    
	
	
    /**
     * for writing when connection is lost and regained
     */

    private synchronized ApdTransportError rewrite(final byte[] out)
    {
	
	boolean isPortOpenedBythisInstance = false;
    	// Connect to the printer first
    	ApdTransportError res = ApdTransportError.OK; 
    		
		  if (mConnStatus == ConnectionStatus.IDLE)
			{
			 res = open();
			 isPortOpenedBythisInstance=true;
			}

			 
        if (mConnStatus == ConnectionStatus.CONNECTED)
        {
        	try
        	{
        		final int bufferLength = out.length;
        		int bytesAvailable = out.length;
        		int toSendSize = Math.min(bytesAvailable, MAX_BUFFER_SIZE);
        		mOffset = 0;
        		outStream = mSocket.getOutputStream();
        		showDialog(ElementsCore.APD_PROGRESS_DIALOG);
        		mConnStatus = ConnectionStatus.TRANSFERRING;
				while (bytesAvailable > 0)
				{
					outStream.write(out, mOffset, toSendSize);
					bytesAvailable -= toSendSize;
					mOffset += toSendSize;
					if (sProgressEnabled)
					{
						Common.mainActivity.runOnUiThread(new Runnable() {
							
							@Override
							public void run() {
								int progress = mOffset * 100 / bufferLength;
								Common.elementsCore.mProgressHandler.sendEmptyMessage(progress);
							}
							
							
						});
						wait(400); //Needed to get the message delivered to the handler in the ElementsActivity
					}
					toSendSize = Math.min(bytesAvailable, MAX_BUFFER_SIZE);
					
				}
				mConnStatus = ConnectionStatus.CONNECTED;
				
			}
        	catch (IOException e)
        	{
        		
        		res = ApdTransportError.IOERROR; 
        		dismissDialog(ElementsCore.APD_PROGRESS_DIALOG);
        		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while sending data to the printer"));
        		connectionLost();

     
        		
        		
			} catch (InterruptedException e)
			{
				res = ApdTransportError.IOERROR; 
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while sending data to the printer"));
			} 
        	finally
        	{
        		if (outStream != null)
        		{
        			try
        			{
						outStream.flush();
					} 
					catch (IOException e)
					{
						e.printStackTrace();
					}
        		}
        		
				if(true == isPortOpenedBythisInstance)
				{
					close();
				}
	    	}
        }
        return res;
    }
	
    /**
     * Indicate that the connection attempt failed and notify the UI Activity.
     */
    private void connectionFailed()
    {
    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Connection to printer failed"));
    	close();
    }
    
    /**
     * Indicate that the connection was lost and notify the UI Activity.
     */
    private void connectionLost()
    {
    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Connection to printer lost"));
    	close();
    }

    /**
     * @see ApdTransport
     */
    @Override
    public ApdTransportError close()
    {
    	ApdTransportError res = ApdTransportError.OK;
    	
    	  try
        			 {
						if(outStream!=null)
						{
						outStream.close();
						}
					 } 
					catch (IOException e)
					{
						e.printStackTrace();
					}
    	
    	if (mSocket != null)
    	{
    	
    		try
    		{
				mSocket.close();
			} 
    		catch (IOException e)
    		{
				e.printStackTrace();
				res = ApdTransportError.IOERROR;
			}
    	}
    	outStream = null;
    	mSocket = null;
    	mConnStatus = ConnectionStatus.IDLE;
    	return res;
    }
	/**
	 * @see ApdTransport
	 */
    @Override
    public void destroy()
    {
    	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, null));
    	close();
    	if (mDiscovererThread != null)
    		Common.mainActivity.unregisterReceiver(mDiscovererThread.mReceiver);
    }
    
	/**
	 * @return the mDevice
	 */
	public BluetoothDevice getDevice() {
		return mDevice;
	}

	/**
	 * @param device the mDevice to set
	 */
	public void setDevice(BluetoothDevice device) {
		mDevice = device;
	}
	
	
	/**
	 * @author FPC843
	 * This class implements the thread which executes the BT device discovery
	 */
	private class DiscovererThread extends Thread
    {
    	// Create a BroadcastReceiver for ACTION_FOUND
    	public final BroadcastReceiver mReceiver = new BroadcastReceiver() {
    	    public void onReceive(Context context, Intent intent) {
     	    	if (BluetoothDevice.ACTION_FOUND.equals(intent.getAction()))
    	    	{
	    	    	mDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
	    	    	if (mDevice.getAddress().compareToIgnoreCase(mApdConfiguration.getBtMac()) == 0 )
	    	    	{
	    	    		mAdapter.cancelDiscovery();
	    	    		mConnStatus = ConnectionStatus.DEVICEFOUND;
	    	    		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Printer " + mDevice.getAddress() + " succesfully found"));
	    	    		return;
	    	    	}
    	    	}
    	    	else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(intent.getAction()))
    	    	{
    	    		if (mConnStatus != ConnectionStatus.DEVICEFOUND)
    	    		{
	    	    		mConnStatus = ConnectionStatus.DEVICENOTFOUND;
	    	    		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Printer " + mApdConfiguration.getBtMac() + " not found"));
    	    		}
    	    	}
    	    	else if (BluetoothDevice.ACTION_ACL_DISCONNECTED.equals(intent.getAction()))
    	    	{
    	    			mConnStatus = ConnectionStatus.IDLE;
    	    			close();
	    	    		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Printer got disconnected"));
    	    	}
    	    }
    	};
    	
    	public void run()
    	{
    		mAdapter.startDiscovery();
    	}	
    }
	
	private class TimerAction extends TimerTask
	{

		@Override
		public void run()
		{
			mTimeoutFlag = true;
		}
	}
}
