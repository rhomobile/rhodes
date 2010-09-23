package com.rhomobile.rhodes.bluetooth;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
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
import com.rhomobile.rhodes.util.PerformOnUiThread;


public class RhoBluetoothManagerOld implements IRhoBluetoothManager {

    private static final String TAG = "RhoBluetoothManagerOld";
    private static final boolean D = true;

	
	public RhoBluetoothManagerOld () {
		if(D) Log.d(TAG, "RhoBluetoothManagerOld()");
   }
	
   public void init(){		
   }
   
	public void setDeviceName(String device_name) {
	}
   
   
   private void sendMessage(String message) {
   }
	
	
   private void setupSession() {
   }


   public void onSessionConnectedOK() {
   }
   
   public void onSessionDisconnected() {
   }

   public void onSessionReadMessage(byte[] buf, int bytes) {
   }
   
   public void onSessionConnectedDeviceName(String name) {
   }
   
   public void onSessionToast(String message) {
   }
   
	   
    public static void onActivityResult(int requestCode, int resultCode, Intent data) {
	}
	   
    public void onDeviceListActivityFinished( boolean is_ok, String adress) {
    }
    
    public void onActivityResultPrivate(int requestCode, int resultCode, Intent data) {
    }
	   

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
	
	public String get_last_error() {
		return RhoBluetoothManager.BTC_ERROR;
	}
	
	public String create_session(String role, String callback_url) {
		fireCreateSessionCallback(RhoBluetoothManager.BTC_ERROR, "");
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
	
	public int session_read_data(String connected_device_name, Byte[] buf, int max_length) {
		return 0;
	}

	public void session_write_data(String connected_device_name, Byte[] buf, int length) {
	}



}
