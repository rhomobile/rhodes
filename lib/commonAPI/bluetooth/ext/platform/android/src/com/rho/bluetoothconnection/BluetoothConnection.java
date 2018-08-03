package com.rho.bluetoothconnection;

import java.util.Map;
import java.util.List;
import java.util.Collection;
import java.util.ArrayList;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;


public class BluetoothConnection extends BluetoothConnectionBase implements IBluetoothConnection {

    public BluetoothConnection(String id) {
        super(id);
    }

    public void getConnectionID(IMethodResult result) {
        result.set(getId());
    }

    public void getStatus(IMethodResult result) {
        result.set(RhoBluetoothManager.session_get_status(getId()));
    }

    public void setCallback(IMethodResult result) {
        RhoBluetoothManager.session_set_callback(getId(), result);
    }

    public void writeData(List<Integer> data, IMethodResult result) {
        byte[] byte_array = new byte[data.size()];
        int i;
        for (i = 0; i < data.size(); i++) {
            byte_array[i] = data.get(i).byteValue();
        }
        RhoBluetoothManager.session_write_data(getId(), byte_array, data.size());
    }

    public void readData(IMethodResult result) {
        int size = RhoBluetoothManager.session_read_data(getId(), null, 0);
        byte[] data = new byte[size];
        RhoBluetoothManager.session_read_data(getId(), data, size);

        Collection<Object> result_data = new ArrayList<Object>();
        int i;
        for (i = 0; i < size; i++) {
            result_data.add(new Integer(data[i]));
        }
        result.set(result_data);
    }

    public void writeString(String string, IMethodResult result) {
        RhoBluetoothManager.session_write_string(getId(), string);
    }

    public void readString(IMethodResult result) {
        result.set(RhoBluetoothManager.session_read_string(getId()));
    }

    public void disconnect(IMethodResult result) {
        RhoBluetoothManager.session_disconnect(getId());
    }

}
