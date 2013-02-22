package com.rho.javascriptvm;

import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;

public abstract class JavascriptVMSingletonBase  {


    public static class executeScriptTask implements Runnable {
        private IJavascriptVMSingleton mApiSingleton; 
        private String script; 
        private IMethodResult mResult;

        public executeScriptTask(IJavascriptVMSingleton obj, 
                String script,         
                IMethodResult result) {
            this.mApiSingleton = obj; 
            this.script = script;        
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.executeScript(
                script,  mResult);
        }
    }

    
}
