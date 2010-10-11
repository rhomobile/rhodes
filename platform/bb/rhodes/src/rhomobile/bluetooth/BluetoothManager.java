package rhomobile.bluetooth;

import rhomobile.RhodesApplication;

import net.rim.device.api.bluetooth.BluetoothSerialPort;
import net.rim.device.api.bluetooth.BluetoothSerialPortInfo;
import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.system.DeviceInfo;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.container.MainScreen;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyFixnum;
import com.xruby.runtime.builtin.RubyInteger;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;


public class BluetoothManager /*extends RubyBasic*/  implements BluetoothScreen.BluetoothScreenCallback, BluetoothPort.BluetoothPortListener {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Bluetooth");
	

	public static String BT_OK = "OK";
	public static String BT_CANCEL = "CANCEL";
	public static String BT_ERROR = "ERROR";

	public static String BT_ROLE_SERVER = "ROLE_SERVER";
	public static String BT_ROLE_CLIENT = "ROLE_CLIENT";
	
	public static String BT_SESSION_INPUT_DATA_RECEIVED = "SESSION_INPUT_DATA_RECEIVED";
	public static String BT_SESSION_DISCONNECT = "SESSION_DISCONNECT";
	
	private static BluetoothManager ourBluetoothManager = null;
	
	private String mCreateSessionCallback = null;
	private String mSessionCallback = null;
	private String mConnectedDeviceName = null;
	private String mLocalDeviceName = null;
	private BluetoothScreen mScreen = null;
	private BluetoothSerialPortInfo[] mDevices = null;
	private BluetoothPort mPort = null;


	
	public static void rhoLogInfo(String msg) {
		//LOG.INFO(msg);
		LOG.ERROR(msg);
	}
	
	public static void rhoLogError(String msg) {
		LOG.ERROR(msg);
	}
	
	public static BluetoothManager getInstance() {
		if (ourBluetoothManager == null) {
			ourBluetoothManager = new BluetoothManager();
			ourBluetoothManager.init();
		}
		return ourBluetoothManager;
	}
	
	private void init() {
		mLocalDeviceName = "Blackberry device";//DeviceInfo.getDeviceName();
		mScreen = null;
		mPort = null;
	}
	
	private void freeAll() {
		if (mScreen != null) {
			rhoPopScreen(mScreen);
			mScreen = null;
		}
	}
	
	public static void rhoPushScreen(MainScreen screen) {
		RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
		final MainScreen scr = screen;
        app.invokeLater(new Runnable() 
        {
            public void run() 
            {
            	UiApplication app = UiApplication.getUiApplication();
            	app.pushScreen(scr);
            	app.requestForeground();
            }
        });
	}
	
	public static void rhoPopScreen(MainScreen screen) {
		RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
		final MainScreen scr = screen;
        app.invokeLater(new Runnable() 
        {
            public void run() 
            {
            	UiApplication app = UiApplication.getUiApplication();
            	app.popScreen(scr);
            	app.requestForeground();
            }
        });
	}
	
	public void closeBluetoothScreenSilent() {
		RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
		final BluetoothScreen scr = mScreen;
		mScreen = null;
		if (scr != null) {
			app.invokeLater(new Runnable() 
	        {
	            public void run() 
	            {
					if (scr != null) {
						scr.closeSilent();
					}
	            }
	        });
		}
	}
	
	// BluetoothScreenCallback
	public void onBluetoothScreenCancel() {
		rhoLogInfo("onBluetoothScreenCancel()");
		if (mPort != null) {
			mPort.disconnect();
			mPort = null;
		}
		fireCreateSessionCallback(BT_CANCEL, "");
		mScreen = null;
	}
	
	// BluetoothScreenCallback
	public void onBluetoothScreenSelect(int index) {
		rhoLogInfo("onBluetoothScreenSelect("+String.valueOf(index)+")");
		UiApplication app = UiApplication.getUiApplication();
		final int i = index;
        app.invokeLater(new Runnable() 
        {
            public void run() 
            {
        		getInstance().mPort = new BluetoothPort(mDevices[i], getInstance());
            }
        });
		mScreen = null;
	}
	
	// BluetoothPortListener
	public void onBluetoothPortDisconnect() {
		fireSessionCallback(mPort.getConnectedDeviceName(), BT_SESSION_DISCONNECT);
	}
	
	// BluetoothPortListener
	public void onBluetoothPortError() {
		fireSessionCallback(mPort.getConnectedDeviceName(), BT_ERROR);
	}
	
	// BluetoothPortListener
	public void onBluetoothPortDataReceived() {
		fireSessionCallback(mPort.getConnectedDeviceName(), BT_SESSION_INPUT_DATA_RECEIVED);
	}
	
	// BluetoothPortListener
	public void onBluetoothPortConnected(boolean success) {
		if (success) {
			fireCreateSessionCallback(BT_OK, mPort.getConnectedDeviceName());
		}
		else {
			fireCreateSessionCallback(BT_ERROR, mPort.getConnectedDeviceName());
		}
		closeBluetoothScreenSilent();
	}
	
	private void fireCreateSessionCallback(String status, String connected_device_name) {
		String body = "&status=";
		body += status;
		body += "&connected_device_name=";
		body += connected_device_name;
		if (mCreateSessionCallback != null) {
			rhoLogInfo("fire Create Session Callback status="+status+", connected_device_name="+connected_device_name);
			fireRhodeCallback(mCreateSessionCallback, body);
		}
		else {
			rhoLogError("fire Create Session Callback ERROR - Create Session callback not defined");
		}
	}

	private void fireSessionCallback(String connected_device_name, String event_type) {
		String body = "&connected_device_name=";
		body += connected_device_name;
		body += "&event_type=";
		body += event_type;
		if (mSessionCallback != null) {
			rhoLogInfo("fire Session Callback connected_device_name="+connected_device_name+", event="+event_type);
			fireRhodeCallback(mSessionCallback, body);
		}
		else {
			rhoLogError("fire Session Callback ERROR - Session callback not defined");
		}
	}
	
	private void fireRhodeCallback(String callback_url, String body) {
		RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
		HttpHeaders headers = new HttpHeaders();
		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
		app.postUrl(callback_url, body, headers);
	}
	
	
	public static int is_bluetooth_available() {
		return BluetoothSerialPort.isSupported()?1:0;
	}
	
	public static void off_bluetooth() {
		rhoLogInfo("off_bluetooth()");
		getInstance().freeAll();
		ourBluetoothManager = null;
	}
	
	public static void set_device_name(String device_name) {
		getInstance().mLocalDeviceName = device_name;
	}
	
	public static String get_device_name() {
		return getInstance().mLocalDeviceName;
	}
	public static String get_last_error() {
		return BT_OK;
	}
	
	public static String create_session(String role, String callback_url) {
		rhoLogInfo("create_session("+role+", "+callback_url+")");
		getInstance().mCreateSessionCallback = callback_url;
		if (BT_ROLE_CLIENT.equals(role)) {
			// client
			if (is_bluetooth_available() != 0) {
				getInstance().mDevices = BluetoothSerialPort.getSerialPortInfo();
				int count = getInstance().mDevices.length;
				String devices[] = new String[count];
				int i;
				for (i = 0; i < count; i++) {
					devices[i] = getInstance().mDevices[i].getDeviceName(); 
				}
				getInstance().mScreen = new BluetoothScreen(BluetoothScreen.BLUETOOTH_SCREEN_TYPE_CLIENT, devices, getInstance());
			}
			else {
				getInstance().mScreen = new BluetoothScreen(BluetoothScreen.BLUETOOTH_SCREEN_TYPE_UNSUPPORTED, null, getInstance());
			}
			rhoPushScreen(getInstance().mScreen);
		}
		else {
			// server
			if (is_bluetooth_available() != 0) {
				getInstance().mPort = new BluetoothPort(null, getInstance());
				//getInstance().mScreen = new BluetoothScreen(BluetoothScreen.BLUETOOTH_SCREEN_TYPE_SERVER, null, getInstance());
				//rhoPushScreen(getInstance().mScreen);
				getInstance().mPort.startListenThread();
			}
			else {
				getInstance().mScreen = new BluetoothScreen(BluetoothScreen.BLUETOOTH_SCREEN_TYPE_UNSUPPORTED, null, getInstance());
				rhoPushScreen(getInstance().mScreen);
			}
		}
		return BT_OK;
	}

	public static void session_set_callback(String connected_device_name, String callback_url) {
		rhoLogInfo("session_set_callback("+connected_device_name+", "+callback_url+")");
		getInstance().mSessionCallback = callback_url;
	}
	
	public static void session_disconnect(String connected_device_name) {
		rhoLogInfo("session_disconnect("+connected_device_name+")");
		if (getInstance().mPort != null) {
			getInstance().mPort.disconnect();
			getInstance().mPort = null;
		}
		else {
			getInstance().fireSessionCallback("", BT_ERROR);
		}
	}
	
	public static int session_get_status(String connected_device_name) {
		rhoLogInfo("session_get_status("+connected_device_name+")");
		if (getInstance().mPort != null) {
			int status = getInstance().mPort.getStatus();
			rhoLogInfo("session_get_status() return = "+String.valueOf(status));
			return status; 
		}
		else {
			getInstance().fireSessionCallback("", BT_ERROR);
		}
		return 0;
	}
	
	public static String session_read_string(String connected_device_name) {
		rhoLogInfo("session_read_string("+connected_device_name+")");
		if (getInstance().mPort != null) {
			byte[] data = getInstance().mPort.readData(); 
			String str = new String(data,0,data.length);
			rhoLogInfo("session_read_string() return = ["+str+"]");
			return str;
		}
		else {
			getInstance().fireSessionCallback("", BT_ERROR);
		}
		return "";
	}
	
	public static void session_write_string(String connected_device_name, String str) {
		rhoLogInfo("session_write_string("+connected_device_name+", "+str+")");
		if (getInstance().mPort != null) {
			byte[] data = str.getBytes();
			getInstance().mPort.writeData(data);
		}
		else {
			getInstance().fireSessionCallback("", BT_ERROR);
		}
	}
	
	public static byte[] session_read_data(String connected_device_name) {
		rhoLogInfo("session_read_data()");
		if (getInstance().mPort != null) {
			return getInstance().mPort.readData();
		}
		else {
			getInstance().fireSessionCallback("", BT_ERROR);
		}
		byte[] m = new byte[0];
		return m;
	}
	
	public static void session_write_data(String connected_device_name, byte[] data) {
		rhoLogInfo("session_write_data()");
		if (getInstance().mPort != null) {
			getInstance().mPort.writeData(data);
		}
		else {
			getInstance().fireSessionCallback("", BT_ERROR);
		}
	}
		
	
	public static void initMethods(RubyClass klass) {
		
		klass.getSingletonClass().defineMethod("is_bluetooth_available", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				RubyValue res = RubyConstant.QFALSE;
				if (is_bluetooth_available() != 0) {
					res = RubyConstant.QTRUE;
				}
				return res;
			}
		});
		klass.getSingletonClass().defineMethod("off_bluetooth", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				off_bluetooth();
				return RubyConstant.QNIL;
			}
		});
		
		klass.getSingletonClass().defineMethod("set_device_name", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				if ( arg0 instanceof RubyString )
				{
					String device_name = arg0.toString();
					set_device_name(device_name);
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.set_device_name: wrong argument type.Should be String");
				}
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("get_device_name", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return ObjectFactory.createString(get_device_name());
			}
		});
		klass.getSingletonClass().defineMethod("get_last_error", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return ObjectFactory.createString(get_last_error());
			}
		});
		klass.getSingletonClass().defineMethod("create_session", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
				if ( (arg1 instanceof RubyString) && (arg2 instanceof RubyString) )
				{
					String role = arg1.toString();
					String callback_url = arg2.toString();
					return ObjectFactory.createString(create_session(role, callback_url));
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.create_session: wrong argument type.Should be String");
				}
			}
		});		
		
		klass.getSingletonClass().defineMethod("session_set_callback", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
				if ( (arg1 instanceof RubyString) && (arg2 instanceof RubyString) )
				{
					String connected_device_name = arg1.toString();
					String callback_url = arg2.toString();
					session_set_callback(connected_device_name, callback_url);
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.session_set_callback: wrong argument type.Should be String");
				}
				return RubyConstant.QNIL;
			}
		});		
		klass.getSingletonClass().defineMethod("session_disconnect", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				if ( arg0 instanceof RubyString )
				{
					String connected_device_name = arg0.toString();
					session_disconnect(connected_device_name);
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.session_disconnect: wrong argument type.Should be String");
				}
				return RubyConstant.QNIL;
			}
		});
		klass.getSingletonClass().defineMethod("session_get_status", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				if ( arg0 instanceof RubyString )
				{
					String connected_device_name = arg0.toString();
					int status = session_get_status(connected_device_name);
					return ObjectFactory.createInteger(status);
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.session_get_status: wrong argument type.Should be String");
				}
			}
		});
		klass.getSingletonClass().defineMethod("session_read_string", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				if ( arg0 instanceof RubyString )
				{
					String connected_device_name = arg0.toString();
					String str = session_read_string(connected_device_name);
					return ObjectFactory.createString(str);
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.session_read_string: wrong argument type.Should be String");
				}
			}
		});
		klass.getSingletonClass().defineMethod("session_write_string", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
				if ( (arg1 instanceof RubyString) && (arg2 instanceof RubyString) )
				{
					String connected_device_name = arg1.toString();
					String str = arg2.toString();
					session_write_string(connected_device_name, str);
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.session_write_string: wrong argument type.Should be String");
				}
				return RubyConstant.QNIL;
			}
		});		
		klass.getSingletonClass().defineMethod("session_read_data", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				if ( arg0 instanceof RubyString )
				{
					String connected_device_name = arg0.toString();
					byte[] data = session_read_data(connected_device_name);
					RubyArray ruby_ar = ObjectFactory.createArray(data.length, data.length, false);
					int i;
					for (i = 0; i < data.length; i++) {
						ruby_ar.and(ObjectFactory.createFixnum(data[i]));
					}
					return ruby_ar;
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in session_read_data: wrong argument type.Should be String");
				}
			}
		});
		klass.getSingletonClass().defineMethod("session_write_data", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
				if ( (arg1 instanceof RubyString) && (arg2 instanceof RubyArray) )
				{
					String connected_device_name = arg1.toString();
					RubyArray ruby_ar = (RubyArray)arg2;
					int i;
					byte[] data = new byte[ruby_ar.size()];
					for (i = 0; i < ruby_ar.size(); i++) {
						RubyValue num = ruby_ar.get(i);
						int b = 0;
						if (num instanceof RubyFixnum) {
							RubyFixnum rv = (RubyFixnum)num;
							b = rv.toInt();
						}
						else {
							if (num instanceof RubyInteger) {
								RubyInteger rv = (RubyInteger)num;
								b = rv.toInt();
							}
							else {
								if (num instanceof RubySymbol) {
									RubySymbol rv = (RubySymbol)num;
									b = rv.toInt();
								}
								else {
									  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.session_write_data: wrong argument type.Should be Array of Fixnums or Integers");
								}
							}
						}
    					data[i] = (byte)b;  
					}
					session_write_data(connected_device_name, data);	
				}
				else {
					  throw new RubyException(RubyRuntime.ArgumentErrorClass, "in RhoBluetooth.session_write_data: wrong argument type.Should be String second should be Array");
				}
				return RubyConstant.QNIL;
			}
		});		
	}
}
