package com.rho.bluetoothconnection;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class BluetoothConnectionFactory
        extends RhoApiFactory< BluetoothConnection, BluetoothConnectionSingleton>
        implements IBluetoothConnectionFactory {

    @Override
    protected BluetoothConnectionSingleton createSingleton() {
        return new BluetoothConnectionSingleton(this);
    }

    @Override
    protected BluetoothConnection createApiObject(String id) {
        return new BluetoothConnection(id);
    }
}
