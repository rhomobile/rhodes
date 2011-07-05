package com.rhomobile.rhodes.bluetooth;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.Handler;
import android.util.Log;

public class RhoBluetoothSession {

	// events for listener
	public static String BT_SESSION_INPUT_DATA_RECEIVED = "SESSION_INPUT_DATA_RECEIVED";
	public static String BT_SESSION_DISCONNECT = "SESSION_DISCONNECT";
	public static String BT_SESSION_ERROR = "ERROR";
	
	
	
    // Debugging
    private static final String TAG = "RhoBluetoothSession";
 
    // Name for the SDP record when creating server socket
    private static final String NAME = "btspp";

    // Unique UUID for this application
    private static final UUID MY_UUID = UUIDHelper.SERIAL_PORT_PROTOCOL_UUID;

    // Member fields
    private final BluetoothAdapter mAdapter;
    private AcceptThread mAcceptThread;
    private ConnectThread mConnectThread;
    private ConnectedThread mConnectedThread;
    private int mState;

    // Constants that indicate the current connection state
    public static final int STATE_NONE = 0;       // we're doing nothing
    public static final int STATE_LISTEN = 1;     // now listening for incoming connections
    public static final int STATE_CONNECTING = 2; // now initiating an outgoing connection
    public static final int STATE_CONNECTED = 3;  // now connected to a remote device
    
    private String mCallbackUrl;

    /**
     * Constructor. Prepares a new RhoBluetoothSession session.
     * @param context  The UI Activity Context
     * @param handler  A Handler to send messages back to the UI Activity
     */
    public RhoBluetoothSession(Context context, Handler handler) {
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        mState = STATE_NONE;
        mCallbackUrl = null;
    }

    public void setCallbackURL(String url) {
    	mCallbackUrl = url;
    }
    
    public String getCallbackURL() {
    	return mCallbackUrl;
    }
    
    /**
     * Set the current state of the chat connection
     * @param state  An integer defining the current connection state
     */
    private synchronized void setState(int state) {
    	RhoBluetoothManager.logi(TAG, "setState() " + mState + " -> " + state);
        mState = state;

        // Give the new state to the Handler so the UI Activity can update
        //mHandler.obtainMessage(RhoBluetoothManager.MESSAGE_STATE_CHANGE, state, -1).sendToTarget();
        switch (mState) {
        case RhoBluetoothSession.STATE_CONNECTED:
        	RhoBluetoothManager.logi(TAG, "     STATE_CONNECTED");
            //fireCreateSessionCallback(BTC_OK, mConnectedDeviceName);
        	mCallbackUrl = null;
        	RhoBluetoothManager.sharedInstance().onSessionConnectedOK();
            break;
        case RhoBluetoothSession.STATE_LISTEN:
        	RhoBluetoothManager.logi(TAG, "     STATE_LISTEN");
        	break;
        case RhoBluetoothSession.STATE_NONE:
        	//fireSessionCallback(mConnectedDeviceName, mSession.BT_SESSION_DISCONNECT);
        	mCallbackUrl = null;
        	RhoBluetoothManager.logi(TAG, "     STATE_NONE");
        	RhoBluetoothManager.sharedInstance().onSessionDisconnected();
            break;
        }
    }

    /**
     * Return the current connection state. */
    public synchronized int getState() {
        return mState;
    }

    /**
     * Start the chat service. Specifically start AcceptThread to begin a
     * session in listening (server) mode. Called by the Activity onResume() */
    public synchronized void start() {
    	RhoBluetoothManager.logi(TAG, "start()");

        // Cancel any thread attempting to make a connection
        if (mConnectThread != null) {mConnectThread.cancel(); mConnectThread = null;}

        // Cancel any thread currently running a connection
        if (mConnectedThread != null) {mConnectedThread.cancel(); mConnectedThread = null;}

        // Start the thread to listen on a BluetoothServerSocket
        if (mAcceptThread == null) {
            mAcceptThread = new AcceptThread();
            mAcceptThread.start();
        }
        setState(STATE_LISTEN);
    }

    /**
     * Start the ConnectThread to initiate a connection to a remote device.
     * @param device  The BluetoothDevice to connect
     */
    public synchronized void connect(BluetoothDevice device) {
    	RhoBluetoothManager.logi(TAG, "connect() to: " + device.getName());

        // Cancel any thread attempting to make a connection
        if (mState == STATE_CONNECTING) {
            if (mConnectThread != null) {mConnectThread.cancel(); mConnectThread = null;}
        }

        // Cancel any thread currently running a connection
        if (mConnectedThread != null) {mConnectedThread.cancel(); mConnectedThread = null;}

        // Start the thread to connect with the given device
        mConnectThread = new ConnectThread(device);
        mConnectThread.start();
        setState(STATE_CONNECTING);
    }

    /**
     * Start the ConnectedThread to begin managing a Bluetooth connection
     * @param socket  The BluetoothSocket on which the connection was made
     * @param device  The BluetoothDevice that has been connected
     */
    public synchronized void connected(BluetoothSocket socket, BluetoothDevice device) {
    	RhoBluetoothManager.logi(TAG, "connected()");

        // Cancel the thread that completed the connection
        if (mConnectThread != null) {mConnectThread.cancel(); mConnectThread = null;}

        // Cancel any thread currently running a connection
        if (mConnectedThread != null) {mConnectedThread.cancel(); mConnectedThread = null;}

        // Cancel the accept thread because we only want to connect to one device
        if (mAcceptThread != null) {mAcceptThread.cancel(); mAcceptThread = null;}

        // Start the thread to manage the connection and perform transmissions
        mConnectedThread = new ConnectedThread(socket);
        mConnectedThread.start();

        // Send the name of the connected device back to the UI Activity
        //Message msg = mHandler.obtainMessage(RhoBluetoothManager.MESSAGE_DEVICE_NAME);
        //Bundle bundle = new Bundle();
        //bundle.putString(RhoBluetoothManager.sharedInstance().DEVICE_NAME, device.getName());
        //msg.setData(bundle);
        //mHandler.sendMessage(msg);
    	RhoBluetoothManager.sharedInstance().onSessionConnectedDeviceName(device.getName());


        setState(STATE_CONNECTED);
    }

    /**
     * Stop all threads
     */
    public synchronized void stop() {
    	RhoBluetoothManager.logi(TAG, "stop()");
        if (mConnectThread != null) {mConnectThread.cancel(); mConnectThread = null;}
        if (mConnectedThread != null) {mConnectedThread.cancel(); mConnectedThread = null;}
        if (mAcceptThread != null) {mAcceptThread.cancel(); mAcceptThread = null;}
        setState(STATE_NONE);
    }

    /**
     * Write to the ConnectedThread in an unsynchronized manner
     * @param out The bytes to write
     * @see ConnectedThread#write(byte[])
     */
    public void write(byte[] out) {
    	RhoBluetoothManager.logi(TAG, "write()");
        // Create temporary object
        ConnectedThread r;
        // Synchronize a copy of the ConnectedThread
        synchronized (this) {
            if (mState != STATE_CONNECTED) return;
            r = mConnectedThread;
        }
        // Perform the write unsynchronized
        r.write(out);
    }

    /**
     * Indicate that the connection attempt failed and notify the UI Activity.
     */
    private void connectionFailed() {
    	RhoBluetoothManager.logi(TAG, "connectionFailed()");
        setState(STATE_NONE);

        // Send a failure message back to the Activity
        //Message msg = mHandler.obtainMessage(RhoBluetoothManager.MESSAGE_TOAST);
        //Bundle bundle = new Bundle();
        //bundle.putString(RhoBluetoothManager.TOAST, "Unable to connect device");
        //msg.setData(bundle);
        //mHandler.sendMessage(msg);
        RhoBluetoothManager.sharedInstance().onSessionToast("Unable to connect device");
    }

    /**
     * Indicate that the connection was lost and notify the UI Activity.
     */
    public void connectionLost() {
    	RhoBluetoothManager.logi(TAG, "connectionLost()");
        setState(STATE_NONE);

        // Send a failure message back to the Activity
        //Message msg = mHandler.obtainMessage(RhoBluetoothManager.MESSAGE_TOAST);
        //Bundle bundle = new Bundle();
        //bundle.putString(RhoBluetoothManager.TOAST, "Device connection was lost");
        //msg.setData(bundle);
        //mHandler.sendMessage(msg);
        RhoBluetoothManager.sharedInstance().onSessionToast("Device connection was lost");
    }

    /**
     * This thread runs while listening for incoming connections. It behaves
     * like a server-side client. It runs until a connection is accepted
     * (or until cancelled).
     */
    private class AcceptThread extends Thread {
        // The local server socket
        private final BluetoothServerSocket mmServerSocket;

        public AcceptThread() {
            BluetoothServerSocket tmp = null;

            // Create a new listening server socket
            try {
                tmp = mAdapter.listenUsingRfcommWithServiceRecord(NAME, MY_UUID);
            } catch (IOException e) {
                Log.e(TAG, "listen() failed", e);
            }
            mmServerSocket = tmp;
        }

        public void run() {
        	RhoBluetoothManager.logi(TAG, "BEGIN mAcceptThread" + this);
            setName("AcceptThread");
            BluetoothSocket socket = null;

            // Listen to the server socket if we're not connected
            while (mState != STATE_CONNECTED) {
                try {
                    // This is a blocking call and will only return on a
                    // successful connection or an exception
                    socket = mmServerSocket.accept();
                } catch (IOException e) {
                    Log.e(TAG, "accept() failed", e);
                    break;
                }

                // If a connection was accepted
                if (socket != null) {
                    synchronized (RhoBluetoothSession.this) {
                        switch (mState) {
                        case STATE_LISTEN:
                        case STATE_CONNECTING:
                            // Situation normal. Start the connected thread.
                            connected(socket, socket.getRemoteDevice());
                            break;
                        case STATE_NONE:
                        case STATE_CONNECTED:
                            // Either not ready or already connected. Terminate new socket.
                            try {
                                socket.close();
                            } catch (IOException e) {
                                Log.e(TAG, "Could not close unwanted socket", e);
                            }
                            break;
                        }
                    }
                }
            }
            RhoBluetoothManager.logi(TAG, "END mAcceptThread");
        }

        public void cancel() {
        	RhoBluetoothManager.logi(TAG, "CANCEL mAcceptThread");
            try {
                mmServerSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "close() of server failed", e);
            }
        }
    }


    /**
     * This thread runs while attempting to make an outgoing connection
     * with a device. It runs straight through; the connection either
     * succeeds or fails.
     */
    private class ConnectThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {
            mmDevice = device;
            BluetoothSocket tmp = null;

            // Get a BluetoothSocket for a connection with the
            // given BluetoothDevice
            try {
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
                Log.e(TAG, "create() failed", e);
            }
            mmSocket = tmp;
        }

        public void run() {
        	RhoBluetoothManager.logi(TAG, "BEGIN mConnectThread");
            setName("ConnectThread");

            // Always cancel discovery because it will slow down a connection
            mAdapter.cancelDiscovery();

            // Make a connection to the BluetoothSocket
            try {
                // This is a blocking call and will only return on a
                // successful connection or an exception
                mmSocket.connect();
            } catch (IOException e) {
                connectionFailed();
                // Close the socket
                try {
                    mmSocket.close();
                } catch (IOException e2) {
                	RhoBluetoothManager.loge(TAG, "unable to close() socket during connection failure");
                }
                // Start the service over to restart listening mode
                RhoBluetoothSession.this.start();
                RhoBluetoothManager.logi(TAG, "END mConnectThread");
                return;
            }

            // Reset the ConnectThread because we're done
            synchronized (RhoBluetoothSession.this) {
                mConnectThread = null;
            }

            // Start the connected thread
            connected(mmSocket, mmDevice);
            RhoBluetoothManager.logi(TAG, "END mConnectThread");
        }

        public void cancel() {
        	RhoBluetoothManager.logi(TAG, "CAMCEL mConnectThread");
            try {
                mmSocket.close();
            } catch (IOException e) {
            	RhoBluetoothManager.loge(TAG, "close() of connect socket failed");
            }
        }
    }

    /**
     * This thread runs during a connection with a remote device.
     * It handles all incoming and outgoing transmissions.
     */
    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
        	RhoBluetoothManager.logi(TAG, "create mConnectedThread");
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the BluetoothSocket input and output streams
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
            	RhoBluetoothManager.loge(TAG, "temp sockets not created");
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
        	RhoBluetoothManager.logi(TAG, "BEGIN mConnectedThread");
            byte[] buffer = new byte[1024];
            int bytes;

            // Keep listening to the InputStream while connected
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.read(buffer);

                    // Send the obtained bytes to the UI Activity
                    //mHandler.obtainMessage(RhoBluetoothManager.MESSAGE_READ, bytes, -1, buffer)
                    //        .sendToTarget();
                    byte[] buf_for_send = buffer.clone();
                    RhoBluetoothManager.sharedInstance().onSessionReadMessage(buf_for_send, bytes);
                } catch (IOException e) {
                	RhoBluetoothManager.loge(TAG, "disconnected");
                    connectionLost();
                    break;
                }
            }
        	RhoBluetoothManager.logi(TAG, "END mConnectedThread");
        }

        /**
         * Write to the connected OutStream.
         * @param buffer  The bytes to write
         */
        public void write(byte[] buffer) {
            try {
                mmOutStream.write(buffer);

                // Share the sent message back to the UI Activity
                //mHandler.obtainMessage(RhoBluetoothManager.MESSAGE_WRITE, -1, -1, buffer)
                //        .sendToTarget();
            } catch (IOException e) {
            	RhoBluetoothManager.loge(TAG, "Exception during write");
            }
        }

        public void cancel() {
        	RhoBluetoothManager.logi(TAG, "CANCEL mConnectedThread");
            try {
                mmSocket.close();
            } catch (IOException e) {
            	RhoBluetoothManager.loge(TAG, "close() of connect socket failed");
            }
        }
    }	
	
	
	
	
}
