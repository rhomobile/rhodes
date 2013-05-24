package com.rho.examples.megamodule;



import java.util.Map;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.IRhoApiObject;

import com.rhomobile.rhodes.api.IRhoApiPropertyBag;

public interface IMegamodule extends IRhoApiObject, IRhoApiPropertyBag {

    void getStringProperty(IMethodResult result);
    void getIntegerProperty(IMethodResult result);
    void setIntegerProperty(int IntegerProperty, IMethodResult result);
    void typesTest(String paramStr, boolean paramBool, int paramInt, double paramFloat, List<Integer> paramArray, Map<String, String> paramHash, IMethodResult result);
    void produceArray(IMethodResult result);
    void produceHash(IMethodResult result);
    void produceComplicatedResult(IMethodResult result);
    void showAlertFromUIThread(String message, IMethodResult result);
    void setPeriodicallyCallback(int periodInMilliseconds, IMethodResult result);
    void isPeriodicallyCallbackSetted(IMethodResult result);
    void stopPeriodicallyCallback(IMethodResult result);
    void complicatedTypesTest1(List<Map<String, Object>> paramArray, IMethodResult result);
};
