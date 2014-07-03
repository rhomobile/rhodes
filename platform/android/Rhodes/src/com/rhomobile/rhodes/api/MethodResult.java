package com.rhomobile.rhodes.api;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONException;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.JSONGenerator;

public class MethodResult implements IMethodResult {
    
    private static final String TAG = MethodResult.class.getSimpleName();
    
    private class JSONObjectResultGenerator extends JSONGenerator {
        private String mObjectClassPath;
        public JSONObjectResultGenerator(Object obj, String objectClassPath) {
            super(obj);
            mObjectClassPath = objectClassPath;
        }
        @Override
        protected void parse(Object obj) throws JSONException {
            if(String.class.isInstance(obj)) {
                getStringer().object();
                getStringer().key("__rhoID").value(obj);
                getStringer().key("__rhoClass").value(mObjectClassPath);
                getStringer().endObject();
            } else {
                super.parse(obj);
            }
        }
    }
    
    public enum ResultType { typeNone, typeBoolean, typeInteger, typeDouble, typeString, typeList, typeMap, typeError, typeArgError }
    
    private boolean mIsRuby;
    private boolean mSingleShot = true;
    private String mStrCallback = "";
    private String mStrCallbackData = "";
    private long mRubyProcCallback;
    private int mTabId;

    private String mResultParamName = "result";
    
    private String mObjectClassPath;
    private long mRubyObjectClass;

    private ResultType mResultType = ResultType.typeNone;
    private ResultType mForceType = ResultType.typeNone;
    private boolean mBooleanResult = false;
    private int mIntegerResult = 0;
    private double mDoubleResult = 0.0;
    private String mStrResult;
    private Collection<Object> mListResult;
    private Map<String, Object> mMapResult;

    private native void nativeCallBack(int tabId, boolean isRuby, boolean releaseCallback);
    private native static void nativeReleaseRubyProcCallback(long rubyProc);

    private void reset() {
        mResultType = ResultType.typeNone;
    }
    
    public MethodResult(boolean isRuby) {
        mIsRuby = isRuby;
        mTabId = -1;
    }
    
    @Override
    public void keepAlive() { mSingleShot = false; }
    
    @Override
    public void forceBooleanType() {
        mForceType = ResultType.typeBoolean;
    }
    
    @Override
    public void forceIntegerType() {
        mForceType = ResultType.typeInteger;
    }
    
    @Override
    public void forceDoubleType() {
        mForceType = ResultType.typeDouble;
    }
    
    @Override
    public void release() {
        if (mRubyProcCallback != 0) {
            nativeReleaseRubyProcCallback(mRubyProcCallback);
        }
    }
    
    public int getResultType() {
        Logger.T(TAG, "getResultType: " + mResultType.name() + " - " + mResultType.ordinal());
        return mResultType.ordinal();
    }

    public String toString() {
        StringBuilder res = new StringBuilder();
        
        if (mRubyProcCallback != 0) {
            res.append("RubyProc: ").append(mRubyProcCallback);
        } else if (mStrCallback != null){
            res.append("Callback: ").append(mStrCallback);
        }
        if (mStrCallbackData != null) {
            res.append(", data: ").append(mStrCallbackData);
        }
        if (mRubyObjectClass != 0) {
            res.append("; RubyClass: ").append(mRubyObjectClass);
        } else if (mObjectClassPath != null){
            res.append("; Class path: ").append(mObjectClassPath);
        }
        
        res.append("; Tab id: ").append(mTabId);
        res.append("; resultType: ").append(mResultType.name());
        res.append("; ").append(mResultParamName).append(": ");
        
        switch (mResultType) {
        case typeBoolean:
            res.append(mBooleanResult);
            break;
        case typeInteger:
            res.append(mIntegerResult);
            break;
        case typeDouble:
            res.append(mDoubleResult);
            break;
        case typeString:
            res.append('"').append(mStrResult).append('"');
            break;
        case typeList:
            res.append(mListResult.toString());
            break;
        case typeMap:
            res.append(mMapResult.toString());
            break;
        case typeError:
        case typeArgError:
            res.append('"').append(mStrResult).append('"');
            break;
        }
        return res.toString();
    }
    public boolean getBoolean() { return mBooleanResult; }
    public int getInteger() { return mIntegerResult; }
    public double getDouble() { return mDoubleResult; }
    public String getString() { return mStrResult; }
    public Collection<Object> getList() { return mListResult; }
    public Map<String, Object> getMap() { return mMapResult; }
    public String getJson() throws JSONException {
        JSONGenerator json;
        switch(mResultType) {
        case typeList:
            if(getObjectClassPath() != null)
                json = new JSONObjectResultGenerator(getList(), getObjectClassPath());
            else
                json =  new JSONGenerator(getList());
            break;
        case typeMap:
            if(getObjectClassPath() != null)
                json = new JSONObjectResultGenerator(getMap(), getObjectClassPath());
            else
                json =  new JSONGenerator(getMap());
            break;
        case typeBoolean:
            json =  new JSONGenerator(Boolean.valueOf(getBoolean()));
            break;
        case typeInteger:
            json = new JSONGenerator(Integer.valueOf(getInteger()));
            break;
        case typeDouble:
            json = new JSONGenerator(Double.valueOf(getDouble()));
            break;
        case typeString:
            if(getObjectClassPath() != null)
                json = new JSONObjectResultGenerator(getString(), getObjectClassPath());
            else
                json =  new JSONGenerator(getString());
            break;
        default:
            json = new JSONGenerator(null);
        }
        String res = json.toString();
        Logger.D(TAG, res);
        return res;
    }

    public String getResultParamName() {
        return mResultParamName;
    }
    public void setResultParamName(String resultParamName) {
        mResultParamName = resultParamName;
    }
    public String getObjectClassPath() {
        return mObjectClassPath;
    }
    public void setObjectClassPath(String objectClassPath) {
        mObjectClassPath = objectClassPath;
    }
    public long getRubyObjectClass() {
        return mRubyObjectClass;
    }
    public void setRubyObjectClass(long rubyObjectClass) {
        mRubyObjectClass = rubyObjectClass;
    }
    @Override
    public void set(boolean res) {
        Logger.T(TAG, "set("+res+")");
        mBooleanResult = res;
        mResultType = ResultType.typeBoolean;
        Logger.T(TAG, toString());
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }
    @Override
    public void set(int res) {
        Logger.T(TAG, "set("+res+")");
        mIntegerResult = res;
        mResultType = ResultType.typeInteger;
        Logger.T(TAG, toString());
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }
    @Override
    public void set(double res) {
        Logger.T(TAG, "set("+res+")");
        mDoubleResult = res;
        mResultType = ResultType.typeDouble;
        Logger.T(TAG, toString());
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }
    @Override
    public void set(String res) {
        Logger.T(TAG, "set(\""+res+"\")");
        try {
            switch(mForceType) {
            case typeNone:
            case typeString:
                break;
            case typeBoolean:
                set(Boolean.valueOf(res).booleanValue());
                return;
            case typeInteger:
                if (res != null) {
                    set(Integer.valueOf(res).intValue());
                } else {
                    set(0);
                }
                return;
            case typeDouble:
                if (res != null) {
                    set(Double.valueOf(res).doubleValue());
                } else {
                    set(0.0);
                }
                return;
            default:
                Logger.W(TAG, "Cannot force string result to type: " + mForceType.name() + ". Returning string result: " + res);
                break;
            }
        } catch (NumberFormatException ex) {
            Logger.E(TAG, ex);
            Logger.W(TAG, "Returning string result: " + res);
        }
        mStrResult = res;
        mResultType = ResultType.typeString;
        Logger.T(TAG, toString());
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }

    @Override
    public void set(Collection<Object> res) {
        Logger.T(TAG, "set("+res+")");
        mListResult = res;
        mResultType = ResultType.typeList;
        Logger.T(TAG, toString());
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }

    @Override
    public void set(Map<String, Object> res) {
        Logger.T(TAG, "set("+res+")");
        mMapResult = res;
        mResultType = ResultType.typeMap;
        Logger.T(TAG, toString());
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }

    @Override
    public void collect(Object value) {
        if (mResultType == ResultType.typeNone) {
            mListResult = new ArrayList<Object>();
            mResultType = ResultType.typeList;
        }
        if (mResultType != ResultType.typeList) {
            if (mResultType != ResultType.typeError && mResultType != ResultType.typeArgError) {
                setError("Wrong result type when collecting list result: " + mResultType.toString());
            }
            return;
        }
        mListResult.add(value);
    }

    @Override
    public void collect(String key, Object value) {
        if (mResultType == ResultType.typeNone) {
            mMapResult = new HashMap<String, Object>();
            mResultType = ResultType.typeMap;
        }
        if (mResultType != ResultType.typeMap) {
            if (mResultType != ResultType.typeError && mResultType != ResultType.typeArgError) {
                setError("Wrong result type when collecting map result: " + mResultType.toString());
            }
            return;
        }
        mMapResult.put(key, value);
    }

    @Override
    public void setSelf(IMethodResult result) {
        switch(mResultType) {
        case typeNone:
            break;
        case typeBoolean:
            result.set(getBoolean());
            break;
        case typeInteger:
            result.set(getInteger());
            break;
        case typeDouble:
            result.set(getDouble());
            break;
        case typeString:
            result.set(getString());
            break;
        case typeList:
            result.set(getList());
            break;
        case typeMap:
            result.set(getMap());
            break;
        case typeArgError:
            result.setArgError(getString());
            break;
        case typeError:
            result.setError(getString());
            break;
        }
    }

    @Override
    public void collectSelf(IMethodResult result) {
        switch(mResultType) {
        case typeNone:
            break;
        case typeBoolean:
            result.collect(Boolean.valueOf(getBoolean()));
            break;
        case typeInteger:
            result.collect(Integer.valueOf(getInteger()));
            break;
        case typeDouble:
            result.collect(Double.valueOf(getDouble()));
            break;
        case typeString:
            result.collect(getString());
            break;
        case typeList:
            result.collect(getList());
            break;
        case typeMap:
            result.collect(getMap());
            break;
        case typeArgError:
            result.setArgError(getString());
            break;
        case typeError:
            result.setError(getString());
            break;
        }
    }

    @Override
    public void collectSelf(String key, IMethodResult result) {
        switch(mResultType) {
        case typeNone:
            break;
        case typeBoolean:
            result.collect(key, Boolean.valueOf(getBoolean()));
            break;
        case typeInteger:
            result.collect(key, Integer.valueOf(getInteger()));
            break;
        case typeDouble:
            result.collect(key, Double.valueOf(getDouble()));
            break;
        case typeString:
            result.collect(key, getString());
            break;
        case typeList:
            result.collect(key, getList());
            break;
        case typeMap:
            result.collect(key, getMap());
            break;
        case typeArgError:
            result.setArgError(getString());
            break;
        case typeError:
            result.setError(getString());
            break;
        }
    }

    @Override
    public void mergeSelf(IMethodResult result) {
        switch(mResultType) {
        case typeNone:
            break;
        case typeList:
            for (Object value: getList()) {
                result.collect(value);
            }
            break;
        case typeMap:
            for (Map.Entry<String, Object> entry: getMap().entrySet()) {
                result.collect(entry.getKey(), entry.getValue());
            }
            break;
        case typeArgError:
            result.setArgError(getString());
            break;
        case typeError:
            result.setError(getString());
            break;
        case typeBoolean:
        case typeInteger:
        case typeDouble:
        case typeString:
            throw new RuntimeException("Cannot merge result of simple type: " + mResultType);
        }
    }

    @Override
    public void set() {
        Logger.T(TAG, toString());
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }
    
    @Override
    public void setArgError(String message) {
        mStrResult = message;
        mResultType = ResultType.typeArgError;

        Logger.E(TAG, toString());

        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }

    @Override
    public void set(Throwable ex) {
        mStrResult = ex.getMessage();
        if(mStrResult == null || mStrResult.length() == 0) {
            mStrResult = ex.getClass().getSimpleName();
        }
        mResultType = ResultType.typeError;

        Logger.E(TAG, ex);
        Logger.E(TAG, toString());
        
        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }
    
    @Override
    public void setError(String message) {
        mStrResult = message;
        mResultType = ResultType.typeError;

        Logger.E(TAG, toString());

        if (mStrCallback.length() > 0 || mRubyProcCallback != 0) {
            Logger.T(TAG, "Calling native callback handler");
            nativeCallBack(mTabId, mIsRuby, mSingleShot);
        }
    }

    @Override
    public boolean hasCallback() {
        return (mStrCallback.length() > 0 || mRubyProcCallback != 0);
    }
}
