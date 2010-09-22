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


public class RhoBluetoothManagerOld extends RhoBluetoothManager {

    private static final String TAG = "RhoBluetoothManagerOld";
    private static final boolean D = true;

	
	public RhoBluetoothManagerOld () {
		if(D) Log.d(TAG, "RhoBluetoothManagerOld()");
   }
	
   public void init(){		
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
	
	
	
	public static int is_bluetooth_available() {
		return 0;
	}
	
	public static void off_bluetooth() {
	}
	
	public static void set_device_name(String device_name) {
	}

	public static String get_device_name() {
		return "";
	}
	
	public static String get_last_error() {
		return BTC_ERROR;
	}
	
	public static String create_session(String role, String callback_url) {
		return BTC_ERROR;
	}
	
	public void fireCreateSessionCallback(String status, String connected_device_name) {
	}

	
	public static void session_set_callback(String connected_device_name, String callback_url) {
	}
	
	public void fireSessionCallback(String connected_device_name, String event_type) {
	}
	
	public static void session_disconnect(String connected_device_name) {
	}
	
	public static int session_get_status(String connected_device_name) {
		return -1;
	}

	public static String session_read_string(String connected_device_name) {
		return "";
	}

	public static void session_write_string(String connected_device_name, String str) {
	}
	
	public static int session_read_data(String connected_device_name, Byte[] buf, int max_length) {
		return 0;
	}

	public static void session_write_data(String connected_device_name, Byte[] buf, int length) {
	}



}
