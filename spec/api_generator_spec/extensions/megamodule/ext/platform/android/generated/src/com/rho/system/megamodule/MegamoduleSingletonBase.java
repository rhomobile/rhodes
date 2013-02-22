package com.rho.system.megamodule;

import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.api.IMethodResult;

import com.rhomobile.rhodes.api.RhoApiDefaultId;

public abstract class MegamoduleSingletonBase 
    extends RhoApiDefaultId<IMegamodule, IMegamoduleFactory>  {

    public MegamoduleSingletonBase(MegamoduleFactory factory) {
        super(factory);
    } 

    public static class getObjectsCountTask implements Runnable {
        private IMegamoduleSingleton mApiSingleton; 
        private IMethodResult mResult;

        public getObjectsCountTask(IMegamoduleSingleton obj,         
                IMethodResult result) {
            this.mApiSingleton = obj;         
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.getObjectsCount( mResult);
        }
    }

    public static class getObjectsByIndexTask implements Runnable {
        private IMegamoduleSingleton mApiSingleton; 
        private int index; 
        private IMethodResult mResult;

        public getObjectsByIndexTask(IMegamoduleSingleton obj, 
                int index,         
                IMethodResult result) {
            this.mApiSingleton = obj; 
            this.index = index;        
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.getObjectsByIndex(
                index,  mResult);
        }
    }

    public static class enumerateTask implements Runnable {
        private IMegamoduleSingleton mApiSingleton; 
        private IMethodResult mResult;

        public enumerateTask(IMegamoduleSingleton obj,         
                IMethodResult result) {
            this.mApiSingleton = obj;         
            this.mResult = result;
        }

        @Override
        public void run() {
            mApiSingleton.enumerate( mResult);
        }
    }

    
}
