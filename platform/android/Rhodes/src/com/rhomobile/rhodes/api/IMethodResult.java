package com.rhomobile.rhodes.api;

import java.util.List;
import java.util.Map;

public interface IMethodResult {
    void set(boolean res);
    void set(int res);
    void set(double res);
    void set(String res);
    void set(List<String> res);
    void set(Map<String, String> res);
    
    void setError(String mesage);
    void setArgError(String message);
}
