package com.rho.nativetabbar;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.NativeBar;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.util.Utils;

class NativeTabbarSingleton extends NativeTabbarSingletonBase implements INativeTabbarSingleton {
    public NativeTabbarSingleton(NativeTabbarFactory factory) {
        //super(factory);
    }

    public void create(List<Map<String, Object>> tabElements, Map<String, Object> tabBarProperties, IMethodResult result) {
    	if (result != null) {
    		Utils.platformLog("TABBAR", "create() result callback is not null !");
    	}
    	else {
    		Utils.platformLog("TABBAR", "create() result callback is NULL !");
    	}
    	NativeBar.create(NativeBar.TABBAR_TYPE, tabElements, tabBarProperties, result);
    }
    
    public void currentTabIndex(IMethodResult result) {
    	result.set(NativeBar.activeTab());
    }
    
    public void remove(IMethodResult result) {
    	NativeBar.remove();
    }
    
    public void setTabBadge(int tabIndex, String badge, IMethodResult result) {
    	// unsupported on Android
    }
    
    public void switchTab(int tabIndex, IMethodResult result) {
    	NativeBar.switchTab(tabIndex);
    }
    
    public void isCreated(IMethodResult result) {
    	result.set(NativeBar.isStarted());
    }
    
    public void removeTab(int tabIndex, IMethodResult result) {
    	//unsupported
    }

}