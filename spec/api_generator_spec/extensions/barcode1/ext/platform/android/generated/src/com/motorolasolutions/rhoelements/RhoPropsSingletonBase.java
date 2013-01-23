package com.motorolasolutions.rhoelements;

import java.util.List;

public abstract class RhoPropsSingletonBase<IApi extends IRhoApiProps, IFactory extends IRhoApiFactory<IApi> >
        extends RhoApiSingletonBase<IApi, IFactory> {

    public RhoPropsSingletonBase(IFactory factory) {
        super(factory);
    }

    public class GetPropsTask implements Runnable {
        private IApi mApiObject;
        private IMethodResult mResult;

        public GetPropsTask(IApi obj, IMethodResult result) {
            mApiObject = obj;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProps(mResult);
        }
    }

    public class GetProps1Task implements Runnable {
        private IApi mApiObject;
        private String mName;
        private IMethodResult mResult;

        public GetProps1Task(IApi obj, String name, IMethodResult result) {
            mApiObject = obj;
            mName = name;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProps(mName, mResult);
        }
    }

    public class GetProps2Task implements Runnable {
        private IApi mApiObject;
        private List<String> mNames;
        private IMethodResult mResult;

        public GetProps2Task(IApi obj, List<String> names, IMethodResult result) {
            mApiObject = obj;
            mNames = names;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProps(mNames, mResult);
        }
    }

}
