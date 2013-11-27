package com.rhomobile.rhodes.api;

import java.util.List;
import java.util.Map;

public interface IMethodResult {
    void set(boolean res);
    void set(int res);
    void set(double res);
    void set(String res);
    void set(List<Object> res);
    void set(Map<String, Object> res);
    
    void collect(Object value);
    void collect(String key, Object value);
    void set();
    
    void setSelf(IMethodResult globalResult);
    void collectSelf(IMethodResult globalResult);
    void collectSelf(String key, IMethodResult globalResult);
    void mergeSelf(IMethodResult result);
    
    void forceBooleanType();
    void forceIntegerType();
    void forceDoubleType();

    void setError(String mesage);
    void setArgError(String message);

    boolean hasCallback();
}
