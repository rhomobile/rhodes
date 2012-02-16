package com.rhomobile.rhodes.extmanager;

public interface IRhoExtension {
	
	void onBeforeNavigate(IRhoExtData data);
	
	void onSetProperty(String name, String value, IRhoExtData data);
}




