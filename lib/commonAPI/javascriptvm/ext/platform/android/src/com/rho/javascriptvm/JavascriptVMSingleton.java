package com.rho.javascriptvm;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

class JavascriptVMSingleton extends JavascriptVMSingletonBase implements IJavascriptVMSingleton {
    private List<String> mIds;


    public JavascriptVMSingleton(JavascriptVMFactory factory) {
        List<String> mIds = new LinkedList<String>();
        mIds.add("ScriptEngine");
    }

    @Override
    public void executeScript(String script, IMethodResult result) {
        result.set("Result");
    }

    List<String> getIDs() {
        return mIds;
    }

    protected String getInitialDefaultID() {
        return getIDs().get(0);
    }

    public void enumerate(IMethodResult res) {
        res.set(getIDs());
    }
}