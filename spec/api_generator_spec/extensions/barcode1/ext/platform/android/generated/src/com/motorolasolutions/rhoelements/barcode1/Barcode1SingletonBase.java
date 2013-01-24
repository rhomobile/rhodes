package com.motorolasolutions.rhoelements.barcode1;

import com.motorolasolutions.rhoelements.IMethodResult;
import com.motorolasolutions.rhoelements.IRhoApiSingleton;
import com.motorolasolutions.rhoelements.RhoPropsSingletonBase;

public abstract class Barcode1SingletonBase extends RhoPropsSingletonBase<IBarcode1, Barcode1Factory> implements IRhoApiSingleton{

    public Barcode1SingletonBase(Barcode1Factory factory) {
        super(factory);
    }

    public class TakeBarcodeTask implements Runnable {
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
