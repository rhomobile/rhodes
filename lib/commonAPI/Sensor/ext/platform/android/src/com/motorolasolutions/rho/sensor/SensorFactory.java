package com.motorolasolutions.rho.sensor;

import com.rho.sensor.ISensorFactory;
import com.rhomobile.rhodes.api.RhoApiFactory;

public class SensorFactory
        extends RhoApiFactory< Sensor, SensorSingleton>
        implements ISensorFactory {

    @Override
    protected SensorSingleton createSingleton() {
        return new SensorSingleton(this);
    }

    @Override
    protected Sensor createApiObject(String id) {
        return new Sensor(id);
    }
}
