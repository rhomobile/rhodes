package com.motorolasolutions.rhoelements;

import java.util.List;
import java.util.Map;

public class MethodResult implements IMethodResult {
    private String mCallBackUrl;
    private String mCallBackData;

    private String mStrResult;
    private List<String> mListResult;
    private Map<String, String> mMapResult;

    private native static void nativeSetString(String res, String url, String urlData);
    private native static void nativeSetStringList(List<String> res, String url, String urlData);
    private native static void nativeSetStringMap(Map<String, String> res, String url, String urlData);
    
    public String getString() { return mStrResult; }
    public List<String> getStringList() { return mListResult; }
    public Map<String, String> getStringMap() { return mMapResult; }

    public void setCallBack(String url, String data) {
        mCallBackUrl = url;
        mCallBackData = data;
    }
    
    @Override
    public void set(String res) {
        if (mCallBackUrl != null) {
            nativeSetString(res, mCallBackUrl, mCallBackData);
        }
        else {
            mStrResult = res;
        }
    }

    @Override
    public void set(List<String> res) {
        if (mCallBackUrl != null) {
            nativeSetStringList(res, mCallBackUrl, mCallBackData);
        }
        else {
            mListResult = res;
        }
    }

    @Override
    public void set(Map<String, String> res) {
        if (mCallBackUrl != null) {
            nativeSetStringMap(res, mCallBackUrl, mCallBackData);
        }
        else {
            mMapResult = res;
        }
    }

}
