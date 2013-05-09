package com.motorolasolutions.rho.sensors;

import com.rho.sensors.ISensorsFactory;
import com.rhomobile.rhodes.api.RhoApiFactory;

public class SensorsFactory
        extends RhoApiFactory< Sensors, SensorsSingleton>
        implements ISensorsFactory {

    @Override
    protected SensorsSingleton createSingleton() {
        return new SensorsSingleton(this);
    }

    @Override
    protected Sensors createApiObject(String id) {
        return new Sensors(id);
    }
}
