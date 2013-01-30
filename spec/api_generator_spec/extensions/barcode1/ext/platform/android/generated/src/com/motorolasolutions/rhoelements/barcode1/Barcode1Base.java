package com.motorolasolutions.rhoelements.barcode1;

import java.util.List;

import com.motorolasolutions.rhoelements.IMethodResult;

public class Barcode1Base {

    public static class GetPropsTask implements Runnable {
        private IBarcode1 mApiObject;
        private IMethodResult mResult;

        public GetPropsTask(IBarcode1 obj, IMethodResult result) {
            mApiObject = obj;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProps(mResult);
        }
    }

    public static class GetProps1Task implements Runnable {
        private IBarcode1 mApiObject;
        private String mName;
        private IMethodResult mResult;

        public GetProps1Task(IBarcode1 obj, String name, IMethodResult result) {
            mApiObject = obj;
            mName = name;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProps(mName, mResult);
        }
    }

    public static class GetProps2Task implements Runnable {
        private IBarcode1 mApiObject;
        private List<String> mNames;
        private IMethodResult mResult;

        public GetProps2Task(IBarcode1 obj, List<String> names, IMethodResult result) {
            mApiObject = obj;
            mNames = names;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProps(mNames, mResult);
        }
    }

    public static class TakeBarcodeTask implements Runnable {
        private IBarcode1 mBarcode;
        private IMethodResult mResult;

        public TakeBarcodeTask(IBarcode1 obj, IMethodResult result) {
            mBarcode = obj;
            mResult = result;
        }

        @Override
        public void run() {
            mBarcode.takeBarcode(mResult);
        }
    }
}
