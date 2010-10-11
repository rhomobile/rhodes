package rhomobile.bluetooth;

import java.io.IOException;

import net.rim.device.api.bluetooth.BluetoothSerialPort;
import net.rim.device.api.bluetooth.BluetoothSerialPortInfo;
import net.rim.device.api.bluetooth.BluetoothSerialPortListener;
import net.rim.device.api.ui.component.Status;
import net.rim.device.api.util.DataBuffer;

public class BluetoothPort implements BluetoothSerialPortListener {

	public interface BluetoothPortListener {
		public void onBluetoothPortDisconnect();
		public void onBluetoothPortError();
		public void onBluetoothPortDataReceived();
		public void onBluetoothPortConnected(boolean success);
	}
	
    private BluetoothSerialPort mPort;
    private static boolean mIsDataSent = true;
    private DataBuffer mWritedData;
    private DataBuffer mReadedData;
	private BluetoothPortListener mListener = null;
    private byte[] _receiveBuffer = new byte[1024];
	private String mConnectedDeviceName = "";
	
	private ConnectionListenThread mListenThread = null;
	
	private class ConnectionListenThread extends Thread {
		private BluetoothPort Port;
		private byte[] data;
		public ConnectionListenThread(BluetoothPort port) {
			Port = port;
			data = new byte[1];
		}
		
		public void run() {
			while (true) {
				try {
					sleep(50);
				}
				catch (Exception e) {
					
				}
				try {
					if (Port.mPort != null) {
						int readed = Port.mPort.read(data, 0, 1);
						if (readed > 0) {
							mReadedData.writeByteArray(data, 0, 1);
						}
						// ok
						Port.deviceConnected(true);
						Port.mListenThread = null;
						BluetoothManager.getInstance().closeBluetoothScreenSilent();
						interrupt();
					}
				}
				catch (Exception e) {
					// port not ready
				}
				
			}
		}
	}
	
	
	
	public void disconnect() {
		BluetoothManager.rhoLogInfo("BluetoothPort.disconnect()");
		if (mPort != null) {
			mPort.disconnect();
			mPort.close();
			//mListener.onBluetoothPortDisconnect();
		}
		else {
			mListener.onBluetoothPortError();
		}
		if (mListenThread != null) {
			mListenThread.interrupt();
			mListenThread = null;
		}
	}
	
	public int getStatus() {
		return mReadedData.getArrayLength();
	}
	
	public byte[] readData() {
		byte[] data = null;
		synchronized(mReadedData) {
			data = mReadedData.toArray();
			mReadedData.reset();
		}
		return data;
	}
	
	public void writeData(byte[] data) {
        synchronized(mWritedData)
        {
            mWritedData.write(data, 0, data.length);
        }
        // Call sendData to send the data.
        sendData();
	}
	
	public String getConnectedDeviceName() {
		return mConnectedDeviceName;
	}

	
	public void startListenThread() {
		//mListenThread = new ConnectionListenThread(this);
		//mListenThread.start();
		deviceConnected(true);
	}
	
    public BluetoothPort(BluetoothSerialPortInfo info, BluetoothPortListener listener)
    {
        // Fill a 1k array with the a character.
        //Arrays.fill(_receiveBuffer, (byte)'a');

        // Initialize the buffers.
        //_data = new StringBuffer();
        
    	mReadedData = new DataBuffer();
    	mWritedData = new DataBuffer();
        
        mListener = listener;

        try
        {
            if(info == null)
            {
        		BluetoothManager.rhoLogInfo("BluetoothPort.BluetoothPort() server init");
                // Open a port to listen for incoming connections.
                mPort = new BluetoothSerialPort(BluetoothSerialPort.DEFAULT_UUID, "btspp", BluetoothSerialPort.BAUD_9600, BluetoothSerialPort.DATA_FORMAT_PARITY_NONE | BluetoothSerialPort.DATA_FORMAT_STOP_BITS_1 | BluetoothSerialPort.DATA_FORMAT_DATA_BITS_8, BluetoothSerialPort.FLOW_CONTROL_NONE, 1024, 1024, this);
        		mConnectedDeviceName = "unknown";//mPort.toString();
            }
            else
            {
                // Connect to the selected device.
        		BluetoothManager.rhoLogInfo("BluetoothPort.BluetoothPort() client init");
                mPort = new BluetoothSerialPort(info, BluetoothSerialPort.BAUD_9600, BluetoothSerialPort.DATA_FORMAT_PARITY_NONE | BluetoothSerialPort.DATA_FORMAT_STOP_BITS_1 | BluetoothSerialPort.DATA_FORMAT_DATA_BITS_8, BluetoothSerialPort.FLOW_CONTROL_NONE, 1024, 1024, this);
                mConnectedDeviceName = info.getDeviceName();
            }
        }
        catch(IOException ex)
        {
    		BluetoothManager.rhoLogInfo("BluetoothPort.BluetoothPort() ERROR!");
    		mListener.onBluetoothPortError();
            //Status.show("Error: " + ex.getMessage());
        }

    }

    // Invoked when a connection is established.
    public void deviceConnected(boolean success)
    {
		BluetoothManager.rhoLogInfo("BluetoothPort.deviceConnected("+String.valueOf(success)+")");
    	if (success) {
    	}
    	mListener.onBluetoothPortConnected(success);
    }

    // Invoked when a connection is closed.
    public void deviceDisconnected()
    {
		BluetoothManager.rhoLogInfo("BluetoothPort.deviceDisconnected()");
    	mListener.onBluetoothPortDisconnect();
    }

    // Invoked when the drt state changes.
    public void dtrStateChange(boolean high)
    {
		BluetoothManager.rhoLogInfo("BluetoothPort.dtrStateChange("+String.valueOf(high)+")");
        //Status.show("DTR: " + high);
    }

    // Invoked when data has been received.
    public void dataReceived(int length)
    {
		BluetoothManager.rhoLogInfo("BluetoothPort.dataReceived("+String.valueOf(length)+")");
        int len;
        try
        {
            // Read the data that arrived.
            if((len = mPort.read(_receiveBuffer, 0, length == -1 ? _receiveBuffer.length : length)) != 0)
            {
                // If loopback is enabled write the data back.
                if(len == 1 && _receiveBuffer[0] == '\r')
                {
                    _receiveBuffer[1] = '\n';
                    ++len;
                }

                // Update the screen with the new data that arrived.
                synchronized (mReadedData) {
                	mReadedData.writeByteArray(_receiveBuffer, 0, len);
                }
                mListener.onBluetoothPortDataReceived();
            }
        } catch(IOException ioex)
        {
            // Catch and re-throw the exception.
            // throw new RuntimeException(ioex.toString());
        	mListener.onBluetoothPortError();
        }
    }

    // Invoked after all data in the buffer has been sent.
    public void dataSent()
    {
		BluetoothManager.rhoLogInfo("BluetoothPort.dataSent()");
        // Set the _dataSent flag to true to allow more data to be written.
        mIsDataSent = true;

        // Call sendData in case there is data waiting to be sent.
        sendData();
    }


    // Sends the data currently in the DataBuffer.
    private void sendData()
    {
		BluetoothManager.rhoLogInfo("BluetoothPort.sendData()");
        // Ensure we have data to send.
    	synchronized (mWritedData) {
	        if (mWritedData.getArrayLength() > 0)
	        {
	            // Ensure the last write call has resulted in the sending of the data
	            // prior to calling write again.  Calling write in sequence without waiting
	            // for the data to be sent can overwrite existing requests and result in
	            // data loss.
	            if (mIsDataSent)
	            {
	                try
	                {
	                    // Set the _dataSent flag to false so we don't send any more
	                    // data until it has been verified that this data was sent.
	                    mIsDataSent = false;
	
                        // Write out the data in the DataBuffer and reset the DataBuffer.
                        mPort.write(mWritedData.getArray(), 0, mWritedData.getArrayLength());
                        mWritedData.reset();
                		BluetoothManager.rhoLogInfo("BluetoothPort.sendData() data was sended");
	                }
	                catch (IOException ioex)
	                {
	                    // Reset _dataSent to true so we can attempt another data write.
	                    mIsDataSent = true;
	                    //System.out.println("Failed to write data. Exception: " + ioex.toString());
                		BluetoothManager.rhoLogError("BluetoothPort.sendData() ERROR during send data");
	                    mListener.onBluetoothPortError();
	                }
	            }
	            else
	            {
            		BluetoothManager.rhoLogInfo("BluetoothPort.sendData() data can not send now - wait");
	                //System.out.println("Can't send data right now, data will be sent after dataSent notify call.");
	            }
	        }
    	}
    }
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
