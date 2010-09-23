package com.rhomobile.rhodes.bluetooth;

import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.util.Log;

public interface IRhoBluetoothManager {
	
	// access from Native
	
	public int is_bluetooth_available();
	
	public void off_bluetooth();
	
	public void set_device_name(String device_name);

	public String get_device_name();
	
	public String get_last_error();
	
	public String create_session(String role, String callback_url);
	
	public void session_set_callback(String connected_device_name, String callback_url);
	
	public void session_disconnect(String connected_device_name);
	
	public int session_get_status(String connected_device_name);

	public String session_read_string(String connected_device_name);

	public void session_write_string(String connected_device_name, String str);
	
	public int session_read_data(String connected_device_name, Byte[] buf, int max_length);

	public void session_write_data(String connected_device_name, Byte[] buf, int length);
	
	// access from DeviceListActivity
	   
    public void onDeviceListActivityFinished( boolean is_ok, String adress);
    
    // access from Session
    
    public void onSessionConnectedOK();
    
    public void onSessionDisconnected();

    public void onSessionReadMessage(byte[] buf, int bytes);
    
    public void onSessionConnectedDeviceName(String name);
    
    public void onSessionToast(String message);
    
    
    // access from Manager
    
    public void init() ;
    
    public void onActivityResultPrivate(int requestCode, int resultCode, Intent data);

    public RhoBluetoothSession getSession();
    
    public void setDeviceName(String device_name);
    
}
