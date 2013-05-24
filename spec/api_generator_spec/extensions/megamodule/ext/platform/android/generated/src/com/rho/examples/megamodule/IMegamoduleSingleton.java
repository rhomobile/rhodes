package com.rho.examples.megamodule;



import java.util.Map;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;

import com.rhomobile.rhodes.api.IRhoApiDefaultId;


public interface IMegamoduleSingleton
       extends IRhoApiDefaultId  {


    static final String CONST1_STRING = "value1";
    static final intCONST2_INTEGER = 123;
    static final String CONST_STRING_1 = "const1";
    static final String CONST_STRING_2 = "const2";

  // hash keys used in properties and parameters

    static final String HK_ITEM_ARRAY = "itemArray"; 
    static final String HK_ITEM_STR = "itemStr"; 
    static final String HK_PARAMA2 = "parama2"; 
    static final String HK_PARAME1 = "parame1"; 
    static final String HK_STR_PARAM = "strParam"; 


    void getObjectsCount(IMethodResult result); 
    void getObjectByIndex(int index, IMethodResult result); 
    void enumerate(IMethodResult result); 

}
