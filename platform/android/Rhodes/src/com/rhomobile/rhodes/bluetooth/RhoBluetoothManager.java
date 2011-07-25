/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.bluetooth;

import android.content.Intent;
import android.os.Build;
import android.util.Log;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class RhoBluetoothManager {

	public static String BTC_OK = "OK";
	public static String BTC_CANCEL = "CANCEL";
	public static String BTC_ERROR = "ERROR";

	public static String BT_ROLE_SERVER = "ROLE_SERVER";
	public static String BT_ROLE_CLIENT = "ROLE_CLIENT";
	
    private static final String TAG = "RhoBluetoothManager";
	

	public static void loge(String tag, String msg) {
		Logger.E(tag, msg);
	}

	public static void logi(String tag, String msg) {
		Logger.I(tag, msg);
	}
    
    
	// public only for external debug !!!
	public static IRhoBluetoothManager ourInstance = null;
	
	public RhoBluetoothManager () {
		RhoBluetoothManager.logi(TAG, "RhoBluetoothManager()");
	}
	
    public static void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (ourInstance != null) {
			ourInstance.onActivityResultPrivate(requestCode, resultCode, data);
		}
	}
	   
	   
	   
	public static IRhoBluetoothManager sharedInstance() {
		if (ourInstance == null) {
			PerformOnUiThread.exec( new Runnable() {
				public void run() {
					RhoBluetoothManager.logi(TAG, "make IRhoBluetoothManager instance");
					String className = "RhoBluetoothManagerNew";
					int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
					if (sdkVersion < Build.VERSION_CODES.ECLAIR) {
						RhoBluetoothManager.logi(TAG, "sharedInstance - old version of System - NO Bluetooth !");
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
						RhoBluetoothManager.loge(TAG, "exception during make IRhoBluetoothManager instance: "+e.getMessage());
						throw new IllegalStateException(e);
					}
				}
		   },true);
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
	
	public static int session_read_data(String connected_device_name, byte[] buf, int max_length) {
		return sharedInstance().session_read_data(connected_device_name, buf, max_length);
	}

	public static void session_write_data(String connected_device_name, byte[] buf, int length) {
		sharedInstance().session_write_data(connected_device_name, buf, length);
	}

	
	public static void create_custom_server_session(String client_name, String callback_url) {
		sharedInstance().create_custom_server_session(client_name, callback_url);
	}

	public static void create_custom_client_session(String server_name, String callback_url) {
		sharedInstance().create_custom_client_session(server_name, callback_url);
	}
	
	public static void stop_current_connection_process() {
		sharedInstance().stop_current_connection_process();
	}

}
