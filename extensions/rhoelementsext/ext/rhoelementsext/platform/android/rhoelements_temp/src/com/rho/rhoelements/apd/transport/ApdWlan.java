package com.rho.rhoelements.apd.transport;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.wifi.WifiManager;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.ElementsCore;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.apd.ApdConfiguration;

/**
 * @author FPC843
 * Allows APD to communicate to WLan devices
 */
public class ApdWlan extends ApdTransport
{
	private ConnectionStatus mConnStatus;
	private Socket mSocket = null;
	//private boolean mTimeoutFlag = false;
	//private static final int TIMEOUT = 15000; //Operation timeout
	//private Timer mOperationTimer = null;
	
	public ApdWlan(ApdConfiguration apdConfiguration) throws Exception
	{
		super(apdConfiguration);
		mConnStatus = ConnectionStatus.IDLE;
		IntentFilter filter = new IntentFilter(WifiManager.NETWORK_STATE_CHANGED_ACTION);
		Common.mainActivity.registerReceiver(mReceiver, filter);
	}
	
	/**
	 * @see ApdTransport
	 */
	@Override
	public ApdTransportError open()
	{
		ApdTransportError res = ApdTransportError.OK; 
		mConnStatus = ConnectionStatus.IDLE;
		//Check that the ip address and the ip port are well formed
		Pattern ipAddressPattern = Pattern.compile(ApdConfiguration.IP_ADDRESS_DOTS_REGEX);
		Matcher ipAddressMatcher = ipAddressPattern.matcher(mApdConfiguration.getIpAddress());
		
		//Validate the ip port
		Pattern ipPortPattern = Pattern.compile(ApdConfiguration.PORT_REGEX);
		Matcher ipPortMatcher = ipPortPattern.matcher(String.valueOf(mApdConfiguration.getIpPort()));
		
		if (ipAddressMatcher.find() == ipPortMatcher.find() == false)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, mApdConfiguration.getIpAddress() + ":" + mApdConfiguration.getIpPort() + " is not a valid ip address"));
			return ApdTransportError.IOERROR;
		}
		
    	if (mConnStatus.ordinal() < ConnectionStatus.CONNECTING.ordinal())
        {
	    	try
	        {
	    		mSocket = new Socket();
				mConnStatus = ConnectionStatus.CONNECTING;
				showDialog(ElementsCore.APD_WAIT_DIALOG);
				mSocket.connect(new InetSocketAddress(mApdConfiguration.getIpAddress(), mApdConfiguration.getIpPort()));
				mConnStatus = ConnectionStatus.CONNECTED;
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Printer connected succesfully"));
			}
	        catch (IOException e)
	        {
	        	res = ApdTransportError.IOERROR;
	        	connectionFailed();
	        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error with connecting to the printer"));
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
	public ApdTransportError write(byte[] out)
	{
    	OutputStream outStream = null;
    	// Connect to the printer first
    	ApdTransportError res = ApdTransportError.OK; 
    	if (mConnStatus == ConnectionStatus.IDLE)
    		 res = open();
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
					outStream.flush();
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
						Thread.sleep(400); //Needed to get the message delivered to the handler in the ElementsActivity
					}
					toSendSize = Math.min(bytesAvailable, MAX_BUFFER_SIZE);
					
				}
			}
        	catch (IOException e)
        	{
        		res = ApdTransportError.IOERROR; 
        		connectionLost();
        		Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while sending data to the printer"));
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
						outStream.close();
					} 
					catch (IOException e)
					{
						e.printStackTrace();
					}
        		}
        		close();
        	}
        }
        else
        {
        	res = ApdTransportError.IOERROR;
        	Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Error while sending data to the printer"));
        }
        
        return res;
	}

	
	/**
	 * @see ApdTransport
	 */
	@Override
	public ApdTransportError close()
	{
		ApdTransportError res = ApdTransportError.OK;
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
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, null));
		try
		{
			Common.mainActivity.unregisterReceiver(mReceiver);
		}
		catch (IllegalArgumentException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Receiver was not registered"));
		}
    	close();
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
    
    private BroadcastReceiver mReceiver = new BroadcastReceiver() {
    	public void onReceive(Context context, Intent intent) {
    		if (intent.getAction().equals(WifiManager.NETWORK_STATE_CHANGED_ACTION)) {
    		    NetworkInfo info = (NetworkInfo)intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
    		    Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Connection lost"));
    		    if (info.getDetailedState() == DetailedState.DISCONNECTED) {
    		    	connectionLost();
    		    }
    		}
    	}
    };

}
