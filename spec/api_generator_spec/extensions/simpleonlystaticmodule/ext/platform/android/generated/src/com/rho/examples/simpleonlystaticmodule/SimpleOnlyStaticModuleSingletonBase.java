package com.rho.examples.simpleonlystaticmodule;

import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;

public abstract class SimpleOnlyStaticModuleSingletonBase  {


    public static class calcSummTask implements Runnable {
        private ISimpleOnlyStaticModuleSingleton mApiSingleton; 
        private int x; 
        private int y; 
        private IMethodResult mResult;

        public calcSummTask(ISimpleOnlyStaticModuleSingleton obj, 
                int x, 
                int y,         
                IMethodResult result) {
            this.mApiSingleton = obj; 
            this.x = x;
            this.y = y;        
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.calcSumm(
                x, 
                y,  mResult);
        }
    }

    public static class joinStringsTask implements Runnable {
        private ISimpleOnlyStaticModuleSingleton mApiSingleton; 
        private List<String> strings; 
        private IMethodResult mResult;

        public joinStringsTask(ISimpleOnlyStaticModuleSingleton obj, 
                List<String> strings,         
                IMethodResult result) {
            this.mApiSingleton = obj; 
            this.strings = strings;        
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.joinStrings(
                strings,  mResult);
        }
    }

    public static class getPlatformTask implements Runnable {
        private ISimpleOnlyStaticModuleSingleton mApiSingleton; 
        private IMethodResult mResult;

        public getPlatformTask(ISimpleOnlyStaticModuleSingleton obj,         
                IMethodResult result) {
            this.mApiSingleton = obj;         
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.getPlatform( mResult);
        }
    }

    public static class showAlertFromUIThreadTask implements Runnable {
        private ISimpleOnlyStaticModuleSingleton mApiSingleton; 
        private IMethodResult mResult;

        public showAlertFromUIThreadTask(ISimpleOnlyStaticModuleSingleton obj,         
                IMethodResult result) {
            this.mApiSingleton = obj;         
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.showAlertFromUIThread( mResult);
        }
    }

    
}
