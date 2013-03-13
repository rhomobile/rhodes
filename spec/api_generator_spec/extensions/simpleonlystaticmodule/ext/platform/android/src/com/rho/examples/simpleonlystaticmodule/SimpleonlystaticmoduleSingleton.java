package com.rho.examples.simpleonlystaticmodule;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

class SimpleonlystaticmoduleSingleton extends SimpleOnlyStaticModuleSingletonBase implements ISimpleOnlyStaticModuleSingleton {
    private static final String TAG = SimpleonlystaticmoduleSingleton.class.getSimpleName();

    public SimpleonlystaticmoduleSingleton(SimpleonlystaticmoduleFactory factory) {
        //super(factory);
    }

    List<String> getIDs() {
        List<String> ids = new LinkedList<String>();
        ids.add("SCN1");
        ids.add("SCN2");
        return ids;
    }

    @Override
    public void calcSumm(int x, int y, IMethodResult result) {
        result.set(x + y);
    }

    @Override
    public void joinStrings(List<String> strings, IMethodResult result) {
        StringBuffer buf = new StringBuffer();
        for (String item : strings ) {
            Logger.T(TAG, "Join: " + item);
            buf.append(item);
        }
        result.set(buf.toString());
    }

    @Override
    public void getPlatform(IMethodResult result) {
        result.set("Android");
    }

    @Override
    public void showAlertFromUIThread(IMethodResult result) {
        
    }
    
}