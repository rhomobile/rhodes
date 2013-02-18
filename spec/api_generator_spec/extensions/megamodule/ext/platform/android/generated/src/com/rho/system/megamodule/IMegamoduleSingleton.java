package com.rho.system.megamodule;



import java.util.Map;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

import com.rhomobile.rhodes.api.IRhoApiDefaultId;


public interface IMegamoduleSingleton
       extends IRhoApiDefaultId  {

    void getObjectsCount(IMethodResult result);     void getObjectsByIndex(int index, IMethodResult result);     void enumerate(IMethodResult result); 

}
