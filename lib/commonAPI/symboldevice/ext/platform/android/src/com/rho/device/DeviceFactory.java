package com.rho.device;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class DeviceFactory implements IDeviceFactory {

private DeviceSingleton singletoninstance=null;

	@Override
	public
	IDeviceSingleton getApiSingleton() {

if(singletoninstance==null)
	singletoninstance=new DeviceSingleton(this);
	
		return singletoninstance;
	}

	@Override
	public
	IDevice getApiObject(String id) {
		// TODO Auto-generated method stub
		return null;
	}
}
