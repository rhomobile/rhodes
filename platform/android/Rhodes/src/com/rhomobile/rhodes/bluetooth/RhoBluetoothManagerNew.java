package com.rhomobile.rhodes.bluetooth;


import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.util.Iterator;
import java.util.Set;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.pm.FeatureInfo;
import android.content.pm.PackageManager;
import android.util.Log;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class RhoBluetoothManagerNew implements IRhoBluetoothManager {



	private boolean mBluetoothIsEnabled = false;
	
    private static final String TAG = "RhoBluetoothManagerNew";
 	
	
    // Key names received from the BluetoothChatService Handler
    public static final String DEVICE_NAME = "DEVICE_NAME";
    public static final String TOAST = "toast";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 958061;
    private static final int REQUEST_ENABLE_BT = 958062;

    // Name of the connected device
    private String mDeviceName = null;
    
    private String mConnectedDeviceName = null;
    //StringBuffer mInput;
    byte[] mInputBuffer;
    int    mInputBufferSize;
 	
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;

    // Member object for the chat services
    private RhoBluetoothSession mSession = null;
    
    private String mCreateSessionCallback;
    
    //private String mRole;
	
	private Activity mActivity;
	
	public RhoBluetoothManagerNew () {
		RhoBluetoothManager.logi(TAG, "RhoBluetoothManagerNew()");
	}
	
	public void init() {
		RhoBluetoothManager.logi(TAG, "init()");
		// constructor
		mActivity = RhodesActivity.getInstance();
		mDeviceName = "NONAME";
		
		mInputBuffer = new byte[1024];
		mInputBufferSize = 0;
		
		
		mBluetoothIsEnabled = false;
		// check feature
		PackageManager pm = RhodesService.getInstance().getPackageManager();
		FeatureInfo[] fs = pm.getSystemAvailableFeatures();
		int i;
		for (i = 0; i < fs.length; i++) {
			if (fs[i] != null)
				if (fs[i].name != null)
					if (fs[i].name.equals(PackageManager.FEATURE_BLUETOOTH)) {
						RhoBluetoothManager.logi(TAG, "sharedInstance - found Bluetooth feature in device !");
						mBluetoothIsEnabled = true;
					}
		}
		// check permission
		//if (pm.checkPermission(android.Manifest.permission.BLUETOOTH, RhodesService.getInstance().getMainActivity().getPackageName()) != PackageManager.PERMISSION_GRANTED) {
		try {
			BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
			if (bluetoothAdapter != null) {
				if (!bluetoothAdapter.isEnabled()) {
					RhoBluetoothManager.logi(TAG, "sharedInstance - Bluetooth is not enabled !");
					mBluetoothIsEnabled = false;
				}
			}
			RhoBluetoothManager.logi(TAG, "sharedInstance - Bluetooth permission is active !");
		}
		catch (SecurityException e) {
			RhoBluetoothManager.logi(TAG, "sharedInstance - Bluetooth permission is INACTIVE !");
			mBluetoothIsEnabled = false;
		}
		
		
		//mInput = new StringBuffer("");
		
		
        // Get local Bluetooth adapter
		if (mBluetoothIsEnabled) {
			RhoBluetoothManager.logi(TAG, "bluetooth is enabled");
			mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        
        	if (mBluetoothAdapter != null) { 
            	if (mBluetoothAdapter.isEnabled()) {
            		if (mSession == null) {
                		setupSession();
                	}
                	mDeviceName = mBluetoothAdapter.getName();
        			RhoBluetoothManager.logi(TAG, "Bluetooth device name = "+mDeviceName);
            	}
			}
		}
		else {
			RhoBluetoothManager.logi(TAG, "bluetooth is disabled");
		}
	}
	
	public void setDeviceName(String device_name) {
		RhoBluetoothManager.logi(TAG, "setDeviceName("+device_name+")");
		mDeviceName = device_name;
        if (mBluetoothAdapter != null) {
        	mBluetoothAdapter.setName(device_name);
        }
	}
	
    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message) {
		RhoBluetoothManager.logi(TAG, "sendMessage("+message+")");
        // Check that we're actually connected before trying anything
        if (mSession.getState() != RhoBluetoothSession.STATE_CONNECTED) {
            //Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
        	RhoBluetoothManager.logi(TAG, "NOT CONNECTED");
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes(Charset.forName("UTF-8"));// getBytes();
            mSession.write(send);

         }
    }
	
    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendData(byte data[]) {
    	RhoBluetoothManager.logi(TAG, "sendData()");
        // Check that we're actually connected before trying anything
        if (mSession.getState() != RhoBluetoothSession.STATE_CONNECTED) {
            //Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
        	RhoBluetoothManager.logi(TAG, "NOT CONNECTED");
            return;
        }

        // Check that there's actually something to send
        if (data.length > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            mSession.write(data);
         }
    }

    
    
   private void setupSession() {
	   RhoBluetoothManager.logi(TAG, "setupSession()");
        mSession = new RhoBluetoothSession(mActivity, null);
     }


   public void onSessionConnectedOK() {
	   RhoBluetoothManager.logi(TAG, "onSessionConnectedOK()");
	   //mInput.setLength(0);
	   mInputBufferSize = 0;
       PerformOnUiThread.exec( new Runnable() {
			public void run() {
				fireCreateSessionCallback(RhoBluetoothManager.BTC_OK, mConnectedDeviceName);
			}
	   });
   }
   
   public void onSessionDisconnected() {
	   RhoBluetoothManager.logi(TAG, "onSessionDisconnected()");
	   PerformOnUiThread.exec( new Runnable() {
			public void run() {
				if (sharedInstance().getSession().getCallbackURL() == null) {
					fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR, "");
				}
				else {
					fireSessionCallback(mConnectedDeviceName, RhoBluetoothSession.BT_SESSION_DISCONNECT);
				}
			}
	   });
   }

   public void onSessionReadMessage(byte[] buf, int bytes) {
	   RhoBluetoothManager.logi(TAG, "onSessionReadMessage()");
	   //final String message = new String(buf, 0, bytes);
	   final byte[] tmpbuf = buf.clone();
	   final int tmpbuf_length = bytes;
	   PerformOnUiThread.exec( new Runnable() {
		    private byte[] buf = tmpbuf;
		    private int buf_length = tmpbuf_length;
			public void run() {
		       //mInput.append(message);
			   int i;
			   synchronized (mInputBuffer) {
				   if (mInputBuffer.length < (mInputBufferSize + buf_length)) {
					   // need alloc new larger buffer
					   byte[] new_buf = new byte[mInputBufferSize + buf_length + 1024];
					   // copy old to new
					   for (i = 0; i < mInputBufferSize; i++) {
						   new_buf[i] = mInputBuffer[i];
					   }
					   mInputBuffer = new_buf;
				   }
				   for (i = mInputBufferSize; i < (mInputBufferSize + buf_length); i++) {
					   mInputBuffer[i] = buf[i-mInputBufferSize];
				   }
				   mInputBufferSize = mInputBufferSize + buf_length;
			   }
		       fireSessionCallback(mConnectedDeviceName, RhoBluetoothSession.BT_SESSION_INPUT_DATA_RECEIVED);
			}
	   });
   }
   
   public void onSessionConnectedDeviceName(String name) {
	   RhoBluetoothManager.logi(TAG, "onSessionConnectedDeviceName("+name+")");
	   mConnectedDeviceName = name;
   }
   
   public void onSessionToast(String message) {
	   RhoBluetoothManager.logi(TAG, "TOAST: " + message);
   }
   
 
    public void onDeviceListActivityFinished( boolean is_ok, String adress) {
    	RhoBluetoothManager.logi(TAG, "onDeviceListActivityFinished()");
    	if (is_ok) {
    		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(adress);
    		// Attempt to connect to the device
        	mSession.connect(device);
    	}
    }
    
    public void onActivityResultPrivate(int requestCode, int resultCode, Intent data) {
    	RhoBluetoothManager.logi(TAG, "onActivityResultPrivate " + resultCode);
        switch (requestCode) {
        case REQUEST_CONNECT_DEVICE:
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                // Get the device MAC address
                String address = data.getExtras()
                                     .getString(RhoBluetoothDeviceListActivity.EXTRA_DEVICE_ADDRESS);
                // Get the BLuetoothDevice object
                BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                // Attempt to connect to the device
                mSession.connect(device);
            }
            break;
        case REQUEST_ENABLE_BT:
            // When the request to enable Bluetooth returns
            if (resultCode == Activity.RESULT_OK) {
                // Bluetooth is now enabled, so set up a chat session
                setupSession();
            } else {
                // User did not enable Bluetooth or an error occured
            	RhoBluetoothManager.logi(TAG, "BT not enabled");
                //Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                //finish();
         	   PerformOnUiThread.exec( new Runnable() {
         		   public void run() {
     					fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR, "");
         		   }
         	   });
            }
        }
    }
    
	   
	   
	public RhoBluetoothManagerNew sharedInstance() {
		return (RhoBluetoothManagerNew)RhoBluetoothManager.sharedInstance();
	}

	public RhoBluetoothSession getSession() {
		return mSession;
	}
	
	
	private void startClient() {
		RhoBluetoothManager.logi(TAG, "startClient()");
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
	    		RhoBluetoothManager.logi(TAG, "startClient (UI thread command)");
		    	if (mBluetoothAdapter != null) { 
		    		RhoBluetoothManager.logi(TAG, "   mBluetoothAdapter != null");
			        Intent serverIntent = new Intent(mActivity, RhoBluetoothDeviceListActivity.class);
			        //mActivity.startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
			        RhodesService.getInstance().startActivity(serverIntent);
		    	}
		    	else {
		    		RhoBluetoothManager.logi(TAG, "   mBluetoothAdapter == null");
		    		sharedInstance().fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR, "");
		    	}
			}
		});
    }
	
	private void startServer() {
		RhoBluetoothManager.logi(TAG, "startServer()");
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
				RhoBluetoothManager.logi(TAG, "startServer (UI thread command)");
		    	if (mBluetoothAdapter != null) { 
		    		RhoBluetoothManager.logi(TAG, "   mBluetoothAdapter != null");
			        if (mBluetoothAdapter.getScanMode() != BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
			            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
			            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
			            RhodesService.getInstance().startActivity(discoverableIntent);
			            if (mSession != null) {
			            	mSession.start();
			            }
			        }
			        else {
			    		RhoBluetoothManager.logi(TAG, "   mBluetoothAdapter.getScanMode() == BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE");
			        }
		    	}
		    	else {
		    		RhoBluetoothManager.logi(TAG, "   mBluetoothAdapter == null");
		    		sharedInstance().fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR, "");
		    	}
			}
		});
	}
	
	public int is_bluetooth_available() {
		if (!mBluetoothIsEnabled) return 0;
		return 1;
	}
	
	public void off_bluetooth() {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "off_bluetooth()");
		//TODO
		if (sharedInstance().getSession() != null) {
			sharedInstance().getSession().stop();
		}
	}
	
	public void set_device_name(String device_name) {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "set_device_name("+device_name+")");
    	sharedInstance().setDeviceName(device_name);
	}

	public String get_device_name() {
		if (!mBluetoothIsEnabled) return "bluetooth is not worked";
		RhoBluetoothManager.logi(TAG, "get_device_name() : "+sharedInstance().mDeviceName);
		return sharedInstance().mDeviceName;
	}
	
	public String get_last_error() {
		if (!mBluetoothIsEnabled) return RhoBluetoothManager.BTC_ERROR;
		return RhoBluetoothManager.BTC_OK;
	}
	
	public String create_session(String role, String callback_url) {
		if (!mBluetoothIsEnabled) return RhoBluetoothManager.BTC_ERROR;
		RhoBluetoothManager.logi(TAG, "create_session("+role+", "+callback_url+");");

		sharedInstance().mCreateSessionCallback = callback_url;
		//sharedInstance().mRole = role;
		if (role.equalsIgnoreCase(RhoBluetoothManager.BT_ROLE_SERVER)) {
			sharedInstance().startServer();
		}
		else {
			sharedInstance().startClient();
		}
		return RhoBluetoothManager.BTC_OK;
	}
	
	public void fireCreateSessionCallback(String status, String connected_device_name) {
		RhoBluetoothManager.logi(TAG, "fireCreateSessionCallback(status["+status+"], connected_device_name["+connected_device_name+"])");
		StringBuffer body = new StringBuffer();
		body.append("&status=");
		body.append(status);
		body.append("&connected_device_name=");
		body.append(connected_device_name);
		if (mCreateSessionCallback != null) {
			RhoBluetoothManager.onCallback(mCreateSessionCallback, body.toString());
		}
	}

	
	public void session_set_callback(String connected_device_name, String callback_url) {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "session_set_callback : "+callback_url);
		sharedInstance().getSession().setCallbackURL(callback_url);
	}
	
	public void fireSessionCallback(String connected_device_name, String event_type) {
		RhoBluetoothManager.logi(TAG, "fireSessionCallback event_type = "+event_type);
		StringBuffer body = new StringBuffer();
		body.append("&connected_device_name=");
		body.append(connected_device_name);
		body.append("&event_type=");
		body.append(event_type);
		if (mSession.getCallbackURL() != null) {
			RhoBluetoothManager.onCallback(mSession.getCallbackURL(), body.toString());
		}
	}
	

	public void session_disconnect(String connected_device_name) {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "session_disconnect");
		//TODO
		if (sharedInstance().getSession() != null) {
			sharedInstance().getSession().stop();
		}
	}
	
	public int session_get_status(String connected_device_name) {
		if (!mBluetoothIsEnabled) return -1;
		RhoBluetoothManager.logi(TAG, "session_get_status : "+String.valueOf(sharedInstance().mInputBufferSize));
		//if (sharedInstance().mInputStringsArrayAdapter.isEmpty()) {
		return sharedInstance().mInputBufferSize;//mInput.length();
	}

	public String session_read_string(String connected_device_name) {
		if (!mBluetoothIsEnabled) return RhoBluetoothManager.BTC_ERROR;
		RhoBluetoothManager.logi(TAG, "session_read_string");
    	//String t = sharedInstance().mInput.toString();
    	//sharedInstance().mInput.setLength(0);
		String t = null;
	    synchronized (mInputBuffer) {
	    	try {
				t = new String(mInputBuffer, 0, mInputBufferSize, "UTF-8");
			} catch (UnsupportedEncodingException e) {
				RhoBluetoothManager.loge(TAG, "exception during construct string from received bytes");
				e.printStackTrace();
			}
	    	mInputBufferSize = 0;
	    }
	    if (t == null) {
			RhoBluetoothManager.logi(TAG, "  String is NULL !!!");
			fireSessionCallback(mConnectedDeviceName, RhoBluetoothSession.BT_SESSION_ERROR);
			return "";
	    }
		RhoBluetoothManager.logi(TAG, "     readed string = "+t);
		return t;
	}

	public void session_write_string(String connected_device_name, String str) {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "session_write_string("+str+")");
		sharedInstance().sendMessage(str);
	}
	
	public int session_read_data(String connected_device_name, byte[] buf, int max_length) {
		if (!mBluetoothIsEnabled) return 0;
		RhoBluetoothManager.logi(TAG, "session_read_data()");
		if ((buf == null) || (max_length == 0)) {
			return mInputBufferSize;
		}
		int real_readed = 0;
		int i;
		synchronized (mInputBuffer) {
			// read;
			real_readed = mInputBufferSize;
			if (real_readed > max_length) {
				real_readed = max_length;
			}
			// copy
			for (i = 0; i < real_readed; i++) {
				buf[i] = mInputBuffer[i];
			}
			// adjust input buf
			if (mInputBufferSize > max_length) {
				for (i = 0; i < (mInputBufferSize - max_length); i++) {
					mInputBuffer[i] = mInputBuffer[i + max_length];
				}
				mInputBufferSize = mInputBufferSize - max_length;
			}
			else {
				mInputBufferSize = 0;
			}
		}
		RhoBluetoothManager.logi(TAG, "      readed "+String.valueOf(real_readed)+" bytes");
		return real_readed;
	}

	public void session_write_data(String connected_device_name, byte[] buf, int length) {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "session_write_data()");
		//{
		//	int i;
		//	for (i = 0; i < length; i++) {
		//		Log.i(TAG, "     writed data ["+String.valueOf(i)+"] = " + String.valueOf(buf[i]));
		//	}
		//}
		sharedInstance().sendData(buf);
	}


	public void create_custom_server_session(String client_name, String callback_url) {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "create_custom_server_session(client_name["+client_name+"], callback["+callback_url+"])");
		sharedInstance().mCreateSessionCallback = callback_url;
		sharedInstance().startServer();
	}

	public void create_custom_client_session(String server_name, String callback_url) {
		if (!mBluetoothIsEnabled) return;
		final String sname = server_name;
		RhoBluetoothManager.logi(TAG, "create_custom_client_session(server_name["+server_name+"], callback["+callback_url+"])");
		sharedInstance().mCreateSessionCallback = callback_url;
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
				RhoBluetoothManager.logi(TAG, "create_custom_client_session (UI Thread command)");
		    	BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
		    	if (adapter != null) {
					Set<BluetoothDevice> devices = adapter.getBondedDevices();
					Iterator<BluetoothDevice> it = devices.iterator();
					while (it.hasNext()) {
						BluetoothDevice device = it.next();
						String name = device.getName();
						RhoBluetoothManager.logi(TAG, "        paired device : "+name);
						if (sname.equals(name)) {
							if (mSession != null) {
								RhoBluetoothManager.logi(TAG, "        try to connect.");
								mSession.connect(device);
								return;
							}
						}
					}
		    	}
		    	sharedInstance().fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR, "");
			}
		});
	}
	
	public void stop_current_connection_process() {
		if (!mBluetoothIsEnabled) return;
		RhoBluetoothManager.logi(TAG, "stop_current_connection_process()");
		//TODO
		if (sharedInstance().getSession() != null) {
			sharedInstance().getSession().stop();
		}
	}

	
	
	
	
}
