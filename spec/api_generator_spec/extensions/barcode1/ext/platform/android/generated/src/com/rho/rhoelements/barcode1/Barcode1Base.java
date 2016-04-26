package com.rho.rhoelements.barcode1;

import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;


public class Barcode1Base {

    public static class GetAllPropertiesTask implements Runnable {
        private IBarcode1 mApiObject;
        private IMethodResult mResult;

        public GetAllPropertiesTask(IBarcode1 obj, IMethodResult result) {
            mApiObject = obj;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getAllProperties(mResult);
        }
    }

    public static class GetPropertyTask implements Runnable {
        private IBarcode1 mApiObject;
        private String mName;
        private IMethodResult mResult;

        public GetPropertyTask(IBarcode1 obj, String name, IMethodResult result) {
            mApiObject = obj;
            mName = name;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProperty(mName, mResult);
        }
    }

    public static class GetPropertiesTask implements Runnable {
        private IBarcode1 mApiObject;
        private List<String> mNames;
        private IMethodResult mResult;

        public GetPropertiesTask(IBarcode1 obj, List<String> names, IMethodResult result) {
            mApiObject = obj;
            mNames = names;
            mResult = result;
        }

        @Override
        public void run() {
            mApiObject.getProperties(mNames, mResult);
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
