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
import android.util.Log;

public class RhoBluetoothManagerOld implements IRhoBluetoothManager {

    private static final String TAG = "RhoBluetoothManagerOld";

	public RhoBluetoothManagerOld () {
		RhoBluetoothManager.logi(TAG, "RhoBluetoothManagerOld()");
	}
	
	public void init() {}
   
	public void setDeviceName(String device_name) {}
	
	public void onSessionConnectedOK() {}

	public void onSessionDisconnected() {}

	public void onSessionReadMessage(byte[] buf, int bytes) {}

	public void onSessionConnectedDeviceName(String name) {}

	public void onSessionToast(String message) {}
	
    public static void onActivityResult(int requestCode, int resultCode, Intent data) {}
	   
    public void onDeviceListActivityFinished( boolean is_ok, String adress) {}
    
    public void onActivityResultPrivate(int requestCode, int resultCode, Intent data) {}
	   
	public RhoBluetoothSession getSession() {
		return null;
	}
	
	public int is_bluetooth_available() {
		return 0;
	}
	
	public void off_bluetooth() {
	}
	
	public void set_device_name(String device_name) {
	}

	public String get_device_name() {
		return "";
	}
	
	public int get_last_error() {
		return RhoBluetoothManager.BTC_ERROR;
	}
	
	public int create_session(String role, String callback_url) {
		fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR_STRING, "");
		return RhoBluetoothManager.BTC_ERROR;
	}
	
	public void fireCreateSessionCallback(String status, String connected_device_name) {
	}

	
	public void session_set_callback(String connected_device_name, String callback_url) {
	}
	
	public void fireSessionCallback(String connected_device_name, String event_type) {
	}
	
	public void session_disconnect(String connected_device_name) {
	}
	
	public int session_get_status(String connected_device_name) {
		return -1;
	}

	public String session_read_string(String connected_device_name) {
		return "";
	}

	public void session_write_string(String connected_device_name, String str) {
	}
	
	public int session_read_data(String connected_device_name, byte[] buf, int max_length) {
		return 0;
	}

	public void session_write_data(String connected_device_name, byte[] buf, int length) {
	}

	public void create_custom_server_session(String client_name, String callback_url) {
		fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR_STRING, "");
		return;
	}

	public void create_custom_client_session(String server_name, String callback_url) {
		fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR_STRING, "");
		return;
	}
	
	public void stop_current_connection_process() {
		return;
	}
	
}
