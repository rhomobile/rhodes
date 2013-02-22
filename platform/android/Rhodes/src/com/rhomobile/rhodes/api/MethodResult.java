package com.rhomobile.rhodes.api;

import java.util.List;
import java.util.Map;

public class MethodResult implements IMethodResult {
    public enum ResultType { typeNone, typeBoolean, typeInteger, typeDouble, typeString, typeList, typeMap, typeError, typeArgError }
    private String mCallBackUrl;
    private String mCallBackData;

    private ResultType mResultType = ResultType.typeNone;
    private boolean mBooleanResult;
    private int mIntegerResult;
    private double mDoubleResult;
    private String mStrResult;
    private List<String> mListResult;
    private Map<String, String> mMapResult;

    private native static void nativeSetBoolean(boolean res, String url, String urlData);
    private native static void nativeSetInteger(int res, String url, String urlData);
    private native static void nativeSetDouble(double res, String url, String urlData);
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
    public String asString() {
        String res = "";
        if (mResultType == ResultType.typeBoolean) {
            res = Boolean.toString(mBooleanResult);
        } else if (mResultType == ResultType.typeInteger) {
            res = Integer.toString(mIntegerResult);
        } else if (mResultType == ResultType.typeDouble) {
            res = Double.toString(mDoubleResult);
        } else if (mResultType == ResultType.typeString) {
            res = mStrResult;
        } else if (mResultType == ResultType.typeList) {
            res = mListResult.toString();
        } else if (mResultType == ResultType.typeMap) {
            res = mMapResult.toString();
        }
        return res;
    }
    public String getString() { return mStrResult; }
    public List<String> getStringList() { return mListResult; }
    public Map<String, String> getStringMap() { return mMapResult; }

    public void setCallBack(String url, String data) {
        mCallBackUrl = url;
        mCallBackData = data;
    }
    
    @Override
    public void set(boolean res) {
        if (mCallBackUrl != null) {
            nativeSetBoolean(res, mCallBackUrl, mCallBackData);
        }
        else {
            mBooleanResult = res;
            mResultType = ResultType.typeBoolean;
        }
    }
    @Override
    public void set(int res) {
        if (mCallBackUrl != null) {
            nativeSetInteger(res, mCallBackUrl, mCallBackData);
        }
        else {
            mIntegerResult = res;
            mResultType = ResultType.typeInteger;
        }
    }
    @Override
    public void set(double res) {
        if (mCallBackUrl != null) {
            nativeSetDouble(res, mCallBackUrl, mCallBackData);
        }
        else {
            mDoubleResult = res;
            mResultType = ResultType.typeDouble;
        }
    }
    @Override
    public void set(String res) {
        if (mCallBackUrl != null) {
            nativeSetString(res, mCallBackUrl, mCallBackData);
        }
        else {
            mStrResult = res;
            mResultType = ResultType.typeString;
        }
    }

    @Override
    public void set(List<String> res) {
        if (mCallBackUrl != null) {
            nativeSetStringList(res, mCallBackUrl, mCallBackData);
        }
        else {
            mListResult = res;
            mResultType = ResultType.typeList;
        }
    }

    @Override
    public void set(Map<String, String> res) {
        if (mCallBackUrl != null) {
            nativeSetStringMap(res, mCallBackUrl, mCallBackData);
        }
        else {
            mMapResult = res;
            mResultType = ResultType.typeMap;
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
