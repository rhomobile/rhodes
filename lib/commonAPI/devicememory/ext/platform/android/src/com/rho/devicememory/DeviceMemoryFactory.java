package com.rho.devicememory;

import com.rhomobile.rhodes.api.RhoApiFactory;

public class DeviceMemoryFactory implements IDeviceMemoryFactory {

	private DeviceMemorySingleton singleton=null;
   
	@Override
	public IDeviceMemorySingleton getApiSingleton() {
		
		if(singleton==null)
			singleton=new DeviceMemorySingleton();
		return singleton;
	}

	@Override
	public IDeviceMemory getApiObject(String id) {
		
		return null;
	}
}
