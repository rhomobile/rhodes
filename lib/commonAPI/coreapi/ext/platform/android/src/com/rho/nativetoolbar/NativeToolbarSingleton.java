package com.rho.nativetoolbar;

import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.NativeBar;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.util.Utils;

class NativeToolbarSingleton extends NativeToolbarSingletonBase implements INativeToolbarSingleton {
    public NativeToolbarSingleton(NativeToolbarFactory factory) {
        //super(factory);
    }

    public void create(List<Map<String, Object>> toolbarElements, Map<String, Integer> toolBarProperties, IMethodResult result) {
    	NativeBar.create(NativeBar.TOOLBAR_TYPE, toolbarElements, toolBarProperties, null);
    }
    
    public void remove(IMethodResult result) {
    	NativeBar.remove();
    }
    
    public void isCreated(IMethodResult result) {
    	result.set(NativeBar.isStarted());
    }

}