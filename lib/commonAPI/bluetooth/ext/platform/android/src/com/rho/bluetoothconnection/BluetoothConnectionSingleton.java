package com.rho.bluetoothconnection;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;

class BluetoothConnectionSingleton extends BluetoothConnectionSingletonBase implements IBluetoothConnectionSingleton {
    public BluetoothConnectionSingleton(BluetoothConnectionFactory factory) {
        super();
    }

    public void getIsAvailable(IMethodResult result) {
        result.set(RhoBluetoothManager.is_bluetooth_available() != 0);
    }

    public void getDeviceName(IMethodResult result) {
        result.set(RhoBluetoothManager.get_device_name());
    }

    public void setDeviceName(String deviceName, IMethodResult result) {
        RhoBluetoothManager.set_device_name(deviceName);
    }

    public void getLastError(IMethodResult result) {
        result.set("OK");
    }

    public void enableBluetooth(IMethodResult result) {
        //NOTHING
    }

    public void disableBluetooth(IMethodResult result) {
        RhoBluetoothManager.off_bluetooth();
    }

    public void getConnectionByID(String connectionID, IMethodResult result) {
        result.set(connectionID);
    }

    public void createConnection(String role, IMethodResult result) {
        RhoBluetoothManager.create_session(role, result);
    }

    public void createServerConnection(IMethodResult result) {
        RhoBluetoothManager.create_custom_server_session("",result);
    }

    public void createClientConnection(String serverName, IMethodResult result) {
        RhoBluetoothManager.create_custom_client_session(serverName, result);
    }

    public void stopCurrentConnectionProcess(IMethodResult result) {
        RhoBluetoothManager.stop_current_connection_process();
    }


}
