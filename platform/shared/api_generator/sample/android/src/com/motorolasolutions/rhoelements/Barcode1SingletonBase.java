package com.motorolasolutions.rhoelements;

import java.util.List;

public abstract class Barcode1SingletonBase extends ApiSingletonBase<IBarcode1> {

    private static Barcode1SingletonBase sInstance = null;
    public static void setInstance(Barcode1SingletonBase instance) { sInstance = instance; }
    public static Barcode1SingletonBase getInstance() { return sInstance; }

    public static class GetPropsTask implements Runnable {
        private IBarcode1 mBarcode;
        private IMethodResult mResult;

        public GetPropsTask(IBarcode1 barcode, IMethodResult result) {
            mBarcode = barcode;
            mResult = result;
        }

        @Override
        public void run() {
            mBarcode.getProps(mResult);
        }
    }

    public static class GetProps1Task implements Runnable {
        private IBarcode1 mBarcode;
        private String mName;
        private IMethodResult mResult;

        public GetProps1Task(IBarcode1 barcode, String name, IMethodResult result) {
            mBarcode = barcode;
            mName = name;
            mResult = result;
        }

        @Override
        public void run() {
            mBarcode.getProps(mName, mResult);
        }
    }

    public static class GetProps2Task implements Runnable {
        private IBarcode1 mBarcode;
        private List<String> mNames;
        private IMethodResult mResult;

        public GetProps2Task(IBarcode1 barcode, List<String> names, IMethodResult result) {
            mBarcode = barcode;
            mNames = names;
            mResult = result;
        }

        @Override
        public void run() {
            mBarcode.getProps(mNames, mResult);
        }
    }

}
