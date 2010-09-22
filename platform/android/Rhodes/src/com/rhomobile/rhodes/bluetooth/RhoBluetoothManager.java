package com.rhomobile.rhodes.bluetooth;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.phonebook.ContactAccessor;
import com.rhomobile.rhodes.util.PerformOnUiThread;


public class RhoBluetoothManager {

	public static String BTC_OK = "OK";
	public static String BTC_CANCEL = "CANCEL";
	public static String BTC_ERROR = "ERROR";

	public static String BT_ROLE_SERVER = "ROLE_SERVER";
	public static String BT_ROLE_CLIENT = "ROLE_CLIENT";

	private boolean mBluetoothIsEnabled = false;
	
    private static final String TAG = "RhoBluetoothManager";
    private static final boolean D = true;
	
	
    // Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;

    // Key names received from the BluetoothChatService Handler
    public static final String DEVICE_NAME = "DEVICE_NAME";
    public static final String TOAST = "toast";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 958061;
    private static final int REQUEST_ENABLE_BT = 958062;

    // Name of the connected device
    private String mDeviceName = null;
    
    private String mConnectedDeviceName = null;
    // Array adapter for the conversation thread
    //private ArrayAdapter<String> mInputStringsArrayAdapter;
    //private ArrayAdapter<String> mConversationArrayAdapter;
    StringBuffer mInput;
    // String buffer for outgoing messages
    private StringBuffer mOutStringBuffer;
	
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;

    // Member object for the chat services
    private RhoBluetoothSession mSession = null;
    
    private String mCreateSessionCallback;
    
    private String mRole;
	
    
	
	// public only for external debug !!!
	public static RhoBluetoothManager ourInstance = null;
	
	private Activity mActivity;
	//private BluetoothChat mChat;
	
	public RhoBluetoothManager () {
		// constructor
		mActivity = RhodesService.getInstance().getMainActivity();
		mDeviceName = "BADNAME";
		
		
		//RhodesService.getInstance().post(new Runnable() {
		//		public void run() {
					//Looper.prepare();
					//mHandler = new RhoHandler();
		//		}
		//});
		mInput = new StringBuffer("");
		
		
		//mChat = chat;
        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        
        if (mBluetoothAdapter != null) { 
            if (mBluetoothAdapter.isEnabled()) {
                if (mSession == null) {
                	setupSession();
                }
                mDeviceName = mBluetoothAdapter.getName();
            }
		}
	}
	
    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message) {
        if(D) Log.i(TAG, "sendMessage");
        // Check that we're actually connected before trying anything
        if (mSession.getState() != RhoBluetoothSession.STATE_CONNECTED) {
            //Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            if(D) Log.i(TAG, "NOT CONNECTED");
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mSession.write(send);

            // Reset out string buffer to zero and clear the edit text field
            mOutStringBuffer.setLength(0);
            //mOutEditText.setText(mOutStringBuffer);
        }
    }
	
	
   private void setupSession() {
        Log.d(TAG, "setupSession()");

        // Initialize the BluetoothChatService to perform bluetooth connections
        mSession = new RhoBluetoothSession(mActivity, mHandler);
        
        //mInputStringsArrayAdapter = new ArrayAdapter<String>(mActivity, R.layout.message);

        // Initialize the buffer for outgoing messages
        mOutStringBuffer = new StringBuffer("");
    }


   public void onSessionConnectedOK() {
       mInput.setLength(0);
       PerformOnUiThread.exec( new Runnable() {
			public void run() {
				fireCreateSessionCallback(BTC_OK, mConnectedDeviceName);
			}
	   },false);
   }
   
   public void onSessionDisconnected() {
	   PerformOnUiThread.exec( new Runnable() {
			public void run() {
				if (sharedInstance().getSession().getCallbackURL() == null) {
					fireCreateSessionCallback(BTC_ERROR, "");
				}
				else {
					fireSessionCallback(mConnectedDeviceName, mSession.BT_SESSION_DISCONNECT);
				}
			}
	   },false);
   }

   public void onSessionReadMessage(byte[] buf, int bytes) {
	   final String message = new String(buf, 0, bytes);
	   PerformOnUiThread.exec( new Runnable() {
			public void run() {
		       mInput.append(message);
		       fireSessionCallback(mConnectedDeviceName, mSession.BT_SESSION_INPUT_DATA_RECEIVED);
			}
	   },false);
   }
   
   public void onSessionConnectedDeviceName(String name) {
	   mConnectedDeviceName = name;
   }
   
   public void onSessionToast(String message) {
	   if (D) Log.d(TAG, "TOAST: " + message);
   }
   
   public class RhoHandler extends Handler{
       public void handleMessage(Message msg) {
           if(D) Log.i(TAG, "mHandler::handleMessage");
           switch (msg.what) {
           case MESSAGE_STATE_CHANGE:
               if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
               switch (msg.arg1) {
               case RhoBluetoothSession.STATE_CONNECTED:
                   if(D) Log.i(TAG, "STATE_CONNECTED!");
                   //Log.d(TAG, "setupSession()");
                   //mTitle.setText(R.string.title_connected_to);
                   //mTitle.append(mConnectedDeviceName);
                   //mConversationArrayAdapter.clear();
                   //mInputStringsArrayAdapter.clear();
                   mInput.setLength(0);
                   fireCreateSessionCallback(BTC_OK, mConnectedDeviceName);
                   break;
               case RhoBluetoothSession.STATE_CONNECTING:
                   //mTitle.setText(R.string.title_connecting);
                   break;
               case RhoBluetoothSession.STATE_LISTEN:
               case RhoBluetoothSession.STATE_NONE:
                   //mTitle.setText(R.string.title_not_connected);
               	fireSessionCallback(mConnectedDeviceName, mSession.BT_SESSION_DISCONNECT);
                   break;
               }
               break;
           case MESSAGE_WRITE:
               if(D) Log.i(TAG, "MESSAGE_WRITE: " + msg.arg1);
               byte[] writeBuf = (byte[]) msg.obj;
               // construct a string from the buffer
               String writeMessage = new String(writeBuf);
               //mConversationArrayAdapter.add("Me:  " + writeMessage);
               break;
           case MESSAGE_READ:
               if(D) Log.i(TAG, "MESSAGE_READ: " + msg.arg1);
               byte[] readBuf = (byte[]) msg.obj;
               // construct a string from the valid bytes in the buffer
               String readMessage = new String(readBuf, 0, msg.arg1);
               //mConversationArrayAdapter.add(mConnectedDeviceName+":  " + readMessage);
               //mInputStringsArrayAdapter.add(readMessage);
               mInput.append(readMessage);
               fireSessionCallback(mConnectedDeviceName, mSession.BT_SESSION_INPUT_DATA_RECEIVED);
               break;
           case MESSAGE_DEVICE_NAME:
               if(D) Log.i(TAG, "DEVICE_NAME: " + msg.arg1);
               // save the connected device's name
               mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
               //Toast.makeText(getApplicationContext(), "Connected to "
               //               + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
               break;
           case MESSAGE_TOAST:
               //Toast.makeText(mActivity.getApplicationContext(), msg.getData().getString(TOAST),
               //               Toast.LENGTH_SHORT).show();
               if (D) Log.d(TAG, "TOAST: " + msg.getData().getString(TOAST));
               break;
           }
       }
	   
   }
   
   
    // The Handler that gets information back from the BluetoothChatService
    private Handler mHandler = null;
	   
    public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (ourInstance != null) {
			ourInstance.onActivityResultPrivate(requestCode, resultCode, data);
		}
	}
	   
    public void onDeviceListActivityFinished( boolean is_ok, String adress) {
    	if (is_ok) {
    		BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(adress);
    		// Attempt to connect to the device
        	mSession.connect(device);
    	}
    }
    
    public void onActivityResultPrivate(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);
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
                Log.d(TAG, "BT not enabled");
                //Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                //finish();
         	   PerformOnUiThread.exec( new Runnable() {
         		   public void run() {
     					fireCreateSessionCallback(BTC_ERROR, "");
         		   }
         	   },false);
            }
        }
    }
    
	   
	   
	public static RhoBluetoothManager sharedInstance() {
		if (ourInstance == null) {
			String className;
			int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
			if (sdkVersion < Build.VERSION_CODES.ECLAIR)
				className = "RhoBluetoothManagerOld";
			else
				className = "RhoBluetoothManager";
			
			try {
				String pkgname = RhoBluetoothManager.class.getPackage().getName();
				String fullName = pkgname + "." + className;
				Class<? extends RhoBluetoothManager> klass =
					Class.forName(fullName).asSubclass(RhoBluetoothManager.class);
				ourInstance = klass.newInstance();
			}
			catch (Exception e) {
				throw new IllegalStateException(e);
			}
		}
		return ourInstance;
	}

	public RhoBluetoothSession getSession() {
		return mSession;
	}
	
	
	private void startClient() {
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
		    	if(D) Log.i(TAG, "startClient");
		        Intent serverIntent = new Intent(mActivity, RhoBluetoothDeviceListActivity.class);
		        //mActivity.startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
		        RhodesService.getInstance().startActivity(serverIntent);
			}
		},false);
	}
	
	private void startServer() {
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
		    	if(D) Log.i(TAG, "startServer");
		        if (mBluetoothAdapter.getScanMode() !=
		            BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
		            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
		            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
		            RhodesService.getInstance().startActivity(discoverableIntent);
		        }
			}
		},false);
	}
	
	public static int is_bluetooth_available() {
		return 1;
	}
	
	public static void off_bluetooth() {
		//TODO
		if (sharedInstance().mSession != null) {
			sharedInstance().mSession.stop();
		}
	}
	
	public static void set_device_name(String device_name) {
    	if(D) Log.i(TAG, "set_device_name() : "+device_name);
		sharedInstance().mDeviceName = device_name;
        if (sharedInstance().mBluetoothAdapter != null) {
        	sharedInstance().mBluetoothAdapter.setName(device_name);
        }
	}

	public static String get_device_name() {
    	if(D) Log.i(TAG, "get_device_name() : "+sharedInstance().mDeviceName);
		return sharedInstance().mDeviceName;
	}
	
	public static String get_last_error() {
		return BTC_OK;
	}
	
	public static String create_session(String role, String callback_url) {
		//return "";
    	if(D) Log.i(TAG, "create_session("+role+", "+callback_url+");");

		sharedInstance().mCreateSessionCallback = callback_url;
		sharedInstance().mRole = role;
		if (role.equalsIgnoreCase(BT_ROLE_SERVER)) {
			sharedInstance().startServer();
		}
		else {
			sharedInstance().startClient();
		}
		return BTC_OK;
	}
	
	public void fireCreateSessionCallback(String status, String connected_device_name) {
    	if(D) Log.i(TAG, "fireCreateSessionCallback");
		StringBuffer body = new StringBuffer();
		body.append("&status=");
		body.append(status);
		body.append("&connected_device_name=");
		body.append(connected_device_name);
		if (mCreateSessionCallback != null) {
			onCallback(mCreateSessionCallback, body.toString());
		}
		//mChat.onCreateSessionCallback(status, connected_device_name);
	}

	
	public static void session_set_callback(String connected_device_name, String callback_url) {
    	if(D) Log.i(TAG, "session_set_callback : "+callback_url);
		sharedInstance().getSession().setCallbackURL(callback_url);
	}
	
	public void fireSessionCallback(String connected_device_name, String event_type) {
    	if(D) Log.i(TAG, "fireSessionCallback");
		StringBuffer body = new StringBuffer();
		body.append("&connected_device_name=");
		body.append(connected_device_name);
		body.append("&event_type=");
		body.append(event_type);
		if (mSession.getCallbackURL() != null) {
			onCallback(mSession.getCallbackURL(), body.toString());
		}
		//mChat.onSessionCallback(connected_device_name, event_type);
	}
	
	// to native
	public static native void onCallback(String callback_url, String body);

	public static void session_disconnect(String connected_device_name) {
    	if(D) Log.i(TAG, "session_disconnect");
		
	}
	
	public static int session_get_status(String connected_device_name) {
    	if(D) Log.i(TAG, "session_get_status");
		//if (sharedInstance().mInputStringsArrayAdapter.isEmpty()) {
		return sharedInstance().mInput.length();
	}

	public static String session_read_string(String connected_device_name) {
    	if(D) Log.i(TAG, "session_read_string");
		//if (!sharedInstance().mInputStringsArrayAdapter.isEmpty()) {
		//	String m = sharedInstance().mInputStringsArrayAdapter.getItem(0);
		//	sharedInstance().mInputStringsArrayAdapter.remove(m);
		//	return m;
		//}
    	String t = sharedInstance().mInput.toString();
    	sharedInstance().mInput.setLength(0);
		return t;
	}

	public static void session_write_string(String connected_device_name, String str) {
    	if(D) Log.i(TAG, "session_write_string");
		sharedInstance().sendMessage(str);
	}
	
	public static int session_read_data(String connected_device_name, Byte[] buf, int max_length) {
		return 0;
	}

	public static void session_write_data(String connected_device_name, Byte[] buf, int length) {
		
	}



}
