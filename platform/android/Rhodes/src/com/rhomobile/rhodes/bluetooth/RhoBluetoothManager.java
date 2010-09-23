package com.rhomobile.rhodes.bluetooth;

import java.security.BasicPermission;
import java.security.Permission;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.pm.FeatureInfo;
import android.content.pm.PackageManager;
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
	
    private static final String TAG = "RhoBluetoothManager";
    private static final boolean D = true;
	

	// public only for external debug !!!
	public static IRhoBluetoothManager ourInstance = null;
	
	public RhoBluetoothManager () {
		if(D) Log.d(TAG, "RhoBluetoothManager()");
	}
	
    public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (ourInstance != null) {
			ourInstance.onActivityResultPrivate(requestCode, resultCode, data);
		}
	}
	   
	   
	   
	public static IRhoBluetoothManager sharedInstance() {
		if (ourInstance == null) {
			String className = "RhoBluetoothManagerNew";
			int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
			if (sdkVersion < Build.VERSION_CODES.ECLAIR) {
				if(D) Log.d(TAG, "sharedInstance - old version of System - NO Bluetooth !");
				className = "RhoBluetoothManagerOld";
			}
			try {
				String pkgname = RhoBluetoothManager.class.getPackage().getName();
				String fullName = pkgname + "." + className;
				Class<? extends IRhoBluetoothManager> klass =
					Class.forName(fullName).asSubclass(IRhoBluetoothManager.class);
				ourInstance = klass.newInstance();
				ourInstance.init();
			}
			catch (Exception e) {
				throw new IllegalStateException(e);
			}
		}
		return ourInstance;
	}

	
	public static int is_bluetooth_available() {
		return sharedInstance().is_bluetooth_available();
	}
	
	public static void off_bluetooth() {
		sharedInstance().off_bluetooth();
	}
	
	public static void set_device_name(String device_name) {
		sharedInstance().set_device_name(device_name);
	}

	public static String get_device_name() {
		return sharedInstance().get_device_name();
	}
	
	public static String get_last_error() {
		return sharedInstance().get_last_error();
	}
	
	public static String create_session(String role, String callback_url) {
		return sharedInstance().create_session(role, callback_url);
	}
	
	public static void session_set_callback(String connected_device_name, String callback_url) {
		sharedInstance().session_set_callback(connected_device_name, callback_url);
	}
	
	// to native
	public static native void onCallback(String callback_url, String body);

	public static void session_disconnect(String connected_device_name) {
		sharedInstance().session_disconnect(connected_device_name);
	}
	
	public static int session_get_status(String connected_device_name) {
		return sharedInstance().session_get_status(connected_device_name);
	}

	public static String session_read_string(String connected_device_name) {
		return sharedInstance().session_read_string(connected_device_name);
	}

	public static void session_write_string(String connected_device_name, String str) {
		sharedInstance().session_write_string(connected_device_name, str);
	}
	
	public static int session_read_data(String connected_device_name, Byte[] buf, int max_length) {
		return sharedInstance().session_read_data(connected_device_name, buf, max_length);
	}

	public static void session_write_data(String connected_device_name, Byte[] buf, int length) {
		sharedInstance().session_write_data(connected_device_name, buf, length);
	}



}
