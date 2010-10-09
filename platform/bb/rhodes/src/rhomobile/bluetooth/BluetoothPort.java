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
	
	public void disconnect() {
		if (mPort != null) {
			mPort.disconnect();
			mPort.close();
			//mListener.onBluetoothPortDisconnect();
		}
		else {
			mListener.onBluetoothPortError();
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
                // Open a port to listen for incoming connections.
                mPort = new BluetoothSerialPort("btspp", BluetoothSerialPort.BAUD_115200, BluetoothSerialPort.DATA_FORMAT_PARITY_NONE | BluetoothSerialPort.DATA_FORMAT_STOP_BITS_1 | BluetoothSerialPort.DATA_FORMAT_DATA_BITS_8, BluetoothSerialPort.FLOW_CONTROL_NONE, 1024, 1024, this);
        		mConnectedDeviceName = "unknown";//mPort.toString();
                //_deviceName = "unknown";
            }
            else
            {
                // Connect to the selected device.
                mPort = new BluetoothSerialPort(info, BluetoothSerialPort.BAUD_115200, BluetoothSerialPort.DATA_FORMAT_PARITY_NONE | BluetoothSerialPort.DATA_FORMAT_STOP_BITS_1 | BluetoothSerialPort.DATA_FORMAT_DATA_BITS_8, BluetoothSerialPort.FLOW_CONTROL_NONE, 1024, 1024, this);
                mConnectedDeviceName = info.getDeviceName();
            }
        }
        catch(IOException ex)
        {
            Status.show("Error: " + ex.getMessage());
        }

    }

    // Invoked when a connection is established.
    public void deviceConnected(boolean success)
    {
    	if (success) {
    	}
    	mListener.onBluetoothPortConnected(success);
    }

    // Invoked when a connection is closed.
    public void deviceDisconnected()
    {
    	mListener.onBluetoothPortDisconnect();
    }

    // Invoked when the drt state changes.
    public void dtrStateChange(boolean high)
    {
        //Status.show("DTR: " + high);
    }

    // Invoked when data has been received.
    public void dataReceived(int length)
    {
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
        // Set the _dataSent flag to true to allow more data to be written.
        mIsDataSent = true;

        // Call sendData in case there is data waiting to be sent.
        sendData();
    }


    // Sends the data currently in the DataBuffer.
    private void sendData()
    {
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
	                }
	                catch (IOException ioex)
	                {
	                    // Reset _dataSent to true so we can attempt another data write.
	                    mIsDataSent = true;
	                    //System.out.println("Failed to write data. Exception: " + ioex.toString());
	                    mListener.onBluetoothPortError();
	                }
	            }
	            else
	            {
	                //System.out.println("Can't send data right now, data will be sent after dataSent notify call.");
	            }
	        }
    	}
    }
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
