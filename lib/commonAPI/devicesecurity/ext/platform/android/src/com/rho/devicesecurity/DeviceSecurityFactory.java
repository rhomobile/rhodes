package com.rho.devicesecurity;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class DeviceSecurityFactory
        extends RhoApiFactory< DeviceSecurity, DeviceSecuritySingleton>
        implements IDeviceSecurityFactory {

    @Override
    protected DeviceSecuritySingleton createSingleton() {
        return new DeviceSecuritySingleton(this);
    }

    @Override
    protected DeviceSecurity createApiObject(String id) {
        return new DeviceSecurity(id);
    }
}
