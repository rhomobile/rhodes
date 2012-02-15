package com.rhomobile.rhodes.extmanager;

public interface IRhoExtManager {
	
	void registerExtension(String strName, IRhoExtension ext);
	
    IRhoExtension getExtByName(String strName);	
}


