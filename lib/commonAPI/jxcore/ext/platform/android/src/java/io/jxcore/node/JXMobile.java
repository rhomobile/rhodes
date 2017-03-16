// License information is available from LICENSE file

package io.jxcore.node;

import io.jxcore.node.jxcore.JXcoreCallback;

import java.util.ArrayList;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.util.Log;

public class JXMobile {

  public static String getStatusString(NetworkInfo[] netInfo, Boolean asJSON) {
    String info = "NotConnected";
    for (NetworkInfo ni : netInfo) {
      if (ni.getTypeName().equalsIgnoreCase("WIFI"))
        if (ni.isConnected()) {
          info = "WiFi";
          break;
        }

      if (ni.getTypeName().equalsIgnoreCase("MOBILE"))
        if (ni.isConnected()) {
          info = "WWAN";
          break;
        }
    }

    return asJSON ? "{\"" + info + "\":1}" : info;
  }

  public static void Initialize() {
    jxcore.RegisterMethod("OnError", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        String message = (String) params.get(0);
        String stack = (String) params.get(1);

        Log.e("jxcore", "Error!: " + message + "\nStack: " + stack);
      }
    });

    jxcore.RegisterMethod("GetDocumentsPath", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        String path = jxcore.activity.getBaseContext().getFilesDir()
            .getAbsolutePath();
        jxcore.CallJSMethod(callbackId, "\"" + path + "\"");
      }
    });

    jxcore.RegisterMethod("GetConnectionStatus", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        ConnectivityManager cm = (ConnectivityManager) jxcore.activity
          .getBaseContext().getSystemService(Context.CONNECTIVITY_SERVICE);

        String info = JXMobile.getStatusString(cm.getAllNetworkInfo(), true);
        jxcore.CallJSMethod(callbackId, info);
      }
    });

    jxcore.RegisterMethod("GetDeviceName", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {

        String name = "\"" + android.os.Build.MANUFACTURER + "-"
            + android.os.Build.MODEL + "\"";

        jxcore.CallJSMethod(callbackId, name);
      }
    });

    /*
     * ADD: <uses-permission android:name="android.permission.BLUETOOTH" />
     * <uses-permission android:name="android.permission.BLUETOOTH_ADMIN" />
     */
    jxcore.RegisterMethod("ToggleBluetooth", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        Boolean enabled = (Boolean) params.get(0);
        BluetoothAdapter mBluetoothAdapter = BluetoothAdapter
            .getDefaultAdapter();
        if (mBluetoothAdapter != null) {
          if (enabled)
            mBluetoothAdapter.enable();
          else
            mBluetoothAdapter.disable();
          
          jxcore.CallJSMethod(callbackId, "null");
        } else {
          jxcore.CallJSMethod(callbackId,
              "{\"msg\":\"Bluetooth adapter is not available\"}");
        }
      }
    });

    /*
     * ADD: <uses-permission android:name="android.permission.CHANGE_WIFI_STATE"
     * />
     */
    jxcore.RegisterMethod("ToggleWiFi", new JXcoreCallback() {
      @SuppressLint("NewApi")
      @Override
      public void Receiver(ArrayList<Object> params, String callbackId) {
        Boolean enabled = (Boolean) params.get(0);
        WifiManager wifiManager = (WifiManager) jxcore.activity
            .getBaseContext().getSystemService(Context.WIFI_SERVICE);

        if (wifiManager != null) {
          wifiManager.setWifiEnabled(enabled);
          
          if (enabled) {
            wifiManager.disconnect();
            wifiManager.reconnect();
          }
          
          jxcore.CallJSMethod(callbackId, "null");
        } else {
          jxcore.CallJSMethod(callbackId,
              "{\"msg\":\"Wireless adapter is not available\"}");
        }
      }
    });
  }
}