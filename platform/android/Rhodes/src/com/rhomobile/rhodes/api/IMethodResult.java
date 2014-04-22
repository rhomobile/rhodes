package com.rhomobile.rhodes.api;

import java.util.Collection;
import java.util.Map;

public interface IMethodResult {
    void set(boolean res);
    void set(int res);
    void set(double res);
    void set(String res);
    void set(Collection<Object> res);
    void set(Map<String, Object> res);
    
    void collect(Object value);
    void collect(String key, Object value);
    void set();
    
    void setSelf(IMethodResult globalResult);
    void collectSelf(IMethodResult globalResult);
    void collectSelf(String key, IMethodResult globalResult);
    void mergeSelf(IMethodResult result);

    void keepAlive();
    
    void forceBooleanType();
    void forceIntegerType();
    void forceDoubleType();

    void release();

    void set(Throwable ex);
    void setError(String mesage);
    void setArgError(String message);

    // hasCallback is used for backward compartability with some API's
    // use-cases are:
    // - if callback is set then return values on event or timer
    // - if callback is not set then just return current values
    // in general case it should not be used. there should be
    // two different methods for getting values and setting callbacks
    boolean hasCallback();
}
