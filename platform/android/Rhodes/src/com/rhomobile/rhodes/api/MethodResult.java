package com.rhomobile.rhodes.api;

import java.util.List;
import java.util.Map;

public class MethodResult implements IMethodResult {
    private enum ResultType { typeNone, typeString, typeList, typeMap, typeError, typeArgError }
    private String mCallBackUrl;
    private String mCallBackData;

    private ResultType mResultType = ResultType.typeNone;
    private String mStrResult;
    private List<String> mListResult;
    private Map<String, String> mMapResult;

    private native static void nativeSetString(String res, String url, String urlData);
    private native static void nativeSetStringList(List<String> res, String url, String urlData);
    private native static void nativeSetStringMap(Map<String, String> res, String url, String urlData);
    private native static void nativeSetArgError(String message, String url, String urlData);
    private native static void nativeSetError(String message, String url, String urlData);

    private void reset() {
        mResultType = ResultType.typeNone;
    }
    public int getResultType() {
        return mResultType.ordinal();
    }
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
            mResultType = ResultType.typeString;
            mStrResult = res;
        }
    }

    @Override
    public void set(List<String> res) {
        if (mCallBackUrl != null) {
            nativeSetStringList(res, mCallBackUrl, mCallBackData);
        }
        else {
            mResultType = ResultType.typeList;
            mListResult = res;
        }
    }

    @Override
    public void set(Map<String, String> res) {
        if (mCallBackUrl != null) {
            nativeSetStringMap(res, mCallBackUrl, mCallBackData);
        }
        else {
            mResultType = ResultType.typeMap;
            mMapResult = res;
        }
    }

    @Override
    public void setArgError(String message) {
        if (mCallBackUrl != null) {
            nativeSetError(message, mCallBackUrl, mCallBackData);
        }
        else {
            mStrResult = message;
            mResultType = ResultType.typeArgError;
        }
    }

    @Override
    public void setError(String message) {
        if (mCallBackUrl != null) {
            nativeSetError(message, mCallBackUrl, mCallBackData);
        }
        else {
            mStrResult = message;
            mResultType = ResultType.typeError;
        }
    }

}
