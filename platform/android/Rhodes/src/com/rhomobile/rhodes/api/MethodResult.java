package com.rhomobile.rhodes.api;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONStringer;

import com.rhomobile.rhodes.util.JSONGenerator;

public class MethodResult implements IMethodResult {
    
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
    private String mStrCallback;
    private String mStrCallbackData;
    private long mRubyProcCallback;

    private String mResultParamName = "result";
    
    private String mObjectClassPath;
    private long mRubyObjectClass;

    private ResultType mResultType = ResultType.typeNone;
    private boolean mBooleanResult;
    private int mIntegerResult;
    private double mDoubleResult;
    private String mStrResult;
    private List<Object> mListResult;
    private Map<String, Object> mMapResult;

    private native void nativeCallBack(boolean isRuby, boolean releaseCallback);
    private native static void nativeReleaseRubyProcCallback(long rubyProc);

    private void reset() {
        mResultType = ResultType.typeNone;
    }
    
    public MethodResult(boolean isRuby) {
        mIsRuby = isRuby;
    }
    
    public void keepAlive() { mSingleShot = false; }
    public void release() {
        if (mRubyProcCallback != 0) {
            nativeReleaseRubyProcCallback(mRubyProcCallback);
        }
    }
    
    public int getResultType() {
        return mResultType.ordinal();
    }

    public String toString() {
        String res = "";
        if (mResultType == ResultType.typeBoolean) {
            res = Boolean.toString(mBooleanResult);
        } else if (mResultType == ResultType.typeInteger) {
            res = Integer.toString(mIntegerResult);
        } else if (mResultType == ResultType.typeDouble) {
            res = Double.toString(mDoubleResult);
        } else if (mResultType == ResultType.typeString) {
            res = '"' + mStrResult + '"';
        } else if (mResultType == ResultType.typeList) {
            res = mListResult.toString();
        } else if (mResultType == ResultType.typeMap) {
            res = mMapResult.toString();
        }
        return res;
    }
    public boolean getBoolean() { return mBooleanResult; }
    public int getInteger() { return mIntegerResult; }
    public double getDouble() { return mDoubleResult; }
    public String getString() { return mStrResult; }
    public List<Object> getList() { return mListResult; }
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
        return json.toString();
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
        mBooleanResult = res;
        mResultType = ResultType.typeBoolean;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }
    @Override
    public void set(int res) {
        mIntegerResult = res;
        mResultType = ResultType.typeInteger;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }
    @Override
    public void set(double res) {
        mDoubleResult = res;
        mResultType = ResultType.typeDouble;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }
    @Override
    public void set(String res) {
        mStrResult = res;
        mResultType = ResultType.typeString;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }

    @Override
    public void set(List<Object> res) {
        mListResult = res;
        mResultType = ResultType.typeList;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }

    @Override
    public void set(Map<String, Object> res) {
        mMapResult = res;
        mResultType = ResultType.typeMap;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
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
            result.setError("Cannot merge result of simple type: " + mResultType);
            break;
        }
    }

    @Override
    public void set() {
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }

    @Override
    public void setArgError(String message) {
        mStrResult = message;
        mResultType = ResultType.typeArgError;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }

    @Override
    public void setError(String message) {
        mStrResult = message;
        mResultType = ResultType.typeError;
        if (mStrCallback != null || mRubyProcCallback != 0) {
            nativeCallBack(mSingleShot, mIsRuby);
        }
    }
}
