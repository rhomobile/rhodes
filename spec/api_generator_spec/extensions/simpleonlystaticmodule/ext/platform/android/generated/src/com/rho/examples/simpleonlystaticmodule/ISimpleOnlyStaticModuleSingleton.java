package com.rho.examples.simpleonlystaticmodule;



import java.util.Map;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;


public interface ISimpleOnlyStaticModuleSingleton
 {

    void calcSumm(int x, int y, IMethodResult result);     void joinStrings(List<String> strings, IMethodResult result);     void getPlatform(IMethodResult result);     void showAlertFromUIThread(IMethodResult result); 

}
