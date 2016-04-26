/**
 * 
 */
package com.rho.rhoelements.apd.transport;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.apd.ApdConfiguration;

/**
 * @author FPC843
 * defines the interface which the APD transport classes have to implement
 */
public abstract class ApdTransport
{
	protected final static int MAX_BUFFER_SIZE = 100;
	protected static boolean sProgressEnabled = true;
	protected enum ConnectionStatus {IDLE, DEVICEFOUND, DEVICENOTFOUND, CONNECTING, CONNECTED, TRANSFERRING}
	protected ApdConfiguration mApdConfiguration;
	protected int mOffset;
	
	public enum ApdTransportError {OK, PORTERROR, IOERROR};
	
	/**
	 * This method writes the content of the buffer passed as parameter into the transport channel 
	 * @param apdConfiguration is the current configuration read from apdconfig.xml
	 */
	public ApdTransport(ApdConfiguration apdConfiguration) 
	{
		mApdConfiguration = apdConfiguration;
		mOffset = 0;
	}
	
	/**
	 * This method writes the content of the buffer passed as parameter into the transport channel 
	 * @param out is the buffer to be transferred
	 */
	public abstract ApdTransportError write(byte[] out);
	
	/**
	 * This method open the connection to the transport channel
	 */
	public abstract ApdTransportError open();
	
	/**
	 * This method closes the connection to the transport channel
	 */
	public abstract ApdTransportError close();
	
	/**
	 * This method cleans up the resources
	 */
	public abstract void destroy();

	/**
	 * @return the progressEnabled
	 */
	public boolean isProgressEnabled() {
		return sProgressEnabled;
	}

	/**
	 * @param progressEnabled the progressEnabled to set
	 */
	public void setProgressEnabled(boolean progressEnabled) {
		sProgressEnabled = progressEnabled;
	}
	
	/**
	 * Set the current APD configuration
	 */
	public void setApdConfiguration(ApdConfiguration apdConfiguration)
	{
		mApdConfiguration = apdConfiguration;
	}
	
	protected void showDialog(final int dialogId)
	{
		if (sProgressEnabled)
			Common.mainActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					Common.mainActivity.showDialog(dialogId);
					
				}
			});
	}
	
	protected void dismissDialog(final int dialogId)
	{
		if (sProgressEnabled)
			Common.mainActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					Common.mainActivity.dismissDialog(dialogId);
				}
			});
	}
}
