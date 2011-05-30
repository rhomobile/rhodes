#include <rhodes.h>
#include <android/log.h>

// rhodes/platform/shared/rubyext/WebView.h
#include "rubyext/WebView.h"
//void rho_webview_navigate(const char* url, int index);

// rhodes/platform/shared/common/RhodesApp.h
#include "RhodesApp.h"
//void rho_net_request(const char *url);
//char* rho_http_normalizeurl(const char* szUrl);

#include "ruby/ext/rho/rhoruby.h"
#include <string.h>
#include <stdlib.h>





#define logging_enable false

void logi(const char* text) {
	if (logging_enable) __android_log_write(ANDROID_LOG_INFO, "NFC jni", text);
} 

void loge(const char* text) {
	__android_log_write(ANDROID_LOG_INFO, "NFC jni ERROR : ", text);
} 


static bool checkSDK() {
     bool sdk_ok = false;
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/NfcActivity");
    if (!cls) {
        loge("NfcActivity class not found !");
        return false;
    }
    jmethodID mid = env->GetStaticMethodID(cls, "getSDKVersion", "()I");
    if (!mid) { 
        loge("NfcActivity.getSDKVersion() NOT FOUND !");
        return false;
    }
    int version = env->CallStaticIntMethod(cls, mid);
    env->DeleteLocalRef(cls);
    return (version >= 9);
}

class CNfcJavaClass {
public:
    CNfcJavaClass() {
        JNIEnv *env = jnienv();
        mNfcClass = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    }

    ~CNfcJavaClass() {
        JNIEnv *env = jnienv();
        env->DeleteLocalRef(mNfcClass);
    }
    
    jclass get() {
        return mNfcClass;
    }
    
private:
    jclass mNfcClass;
};


class CRubyString {
public:
    
    CRubyString(const char* str) {
        mStr = str;
        mJavaObject = NULL;
        mEnv = NULL;
    }
    
    ~CRubyString() {
        if (mJavaObject != NULL) {
            mEnv->DeleteLocalRef(mJavaObject);
        }
    }
    
    jstring makeJavaString(JNIEnv* env) {
        mEnv = env;
        mJavaObject = env->NewStringUTF(mStr);
        return mJavaObject;
    }
    
private:    
    jstring mJavaObject;
    JNIEnv* mEnv;
    const char* mStr;
};

class CRubyByteArray {
public:
    CRubyByteArray(VALUE array) {
        if (array != 0) {
            mSize = rho_ruby_unpack_byte_array(array, 0, 0);
            mBuf = new unsigned char[mSize];
            rho_ruby_unpack_byte_array(array, mBuf, mSize);
        }
        else {
            mBuf = NULL;
            mSize = 0;
        }
        mJavaObject = NULL;
    }
    
    jbyteArray makeJavaByteArray(JNIEnv* env) {
        if (mBuf != NULL) {
            mEnv = env;
            mJavaObject = (jbyteArray)env->NewByteArray(mSize);
            jbyte* buf_p = env->GetByteArrayElements(mJavaObject, 0);
            int i;
            for (i = 0; i < mSize; i++) {
                buf_p[i] = mBuf[i];
            }
            env->ReleaseByteArrayElements(mJavaObject, buf_p, 0);
        }
        return mJavaObject;
    }
    
    ~CRubyByteArray() {
        if (mJavaObject != NULL) {
            mEnv->DeleteLocalRef(mJavaObject);
        }
        if (mBuf != NULL) {
            delete mBuf;
        }
    }
    
private:
    unsigned char* mBuf;
    int mSize;
    jbyteArray mJavaObject;
    JNIEnv* mEnv;
};


class CNFCString {
public:
	CNFCString(JNIEnv* env, jobject jo_string) {
        mBuf = NULL;
        if (jo_string != NULL) {
            const char* msg = env->GetStringUTFChars((jstring)jo_string,0);
            mSize = strlen(msg);
            mBuf = new char[mSize+1];
            strcpy(mBuf, msg);
            env->ReleaseStringUTFChars((jstring)jo_string, msg);
        }
	}

	virtual ~CNFCString() {
        if (mBuf != NULL) {
            delete mBuf;
        }
	}

    VALUE makeVALUE() {
        if (mBuf != NULL) {
            return rho_ruby_create_string((const char*)mBuf);
        }
        else  {
            return rho_ruby_get_NIL();
        }
        
    }
    
    
	char* mBuf;
	int mSize;
};



class CNFCByteArray {
public:
	CNFCByteArray(JNIEnv* env, jobject jo_bytearray) {
        logi("Construct ByteArray from java object START");
        mBuf = NULL;
        if (jo_bytearray != NULL) {
            mSize = env->GetArrayLength((jbyteArray)jo_bytearray);
            if (mSize > 0) {
                if (logging_enable) {
                    char msg[128];
                    sprintf(msg, "    array size = %d", mSize);
                    logi(msg);
                }
                mBuf = new unsigned char[mSize];
                unsigned char* jarray_buf = (unsigned char*)env->GetByteArrayElements((jbyteArray)jo_bytearray, 0);
                int i;
                for (i = 0; i < mSize; i++) {
                    mBuf[i] = jarray_buf[i];
                }
                env->ReleaseByteArrayElements((jbyteArray)jo_bytearray, (jbyte*)jarray_buf, 0);
            }
            else {
                logi("    invalid size !");
                mBuf = NULL;
            }
        }
        logi("Construct ByteArray from java object FINISH");
	}
    
    VALUE makeVALUE() {
        logi("make VALUE from byte array START");
        if (mBuf != NULL) {
            VALUE v = rho_ruby_create_byte_array(mBuf, mSize);
            logi("make VALUE from byte array FINISH");
            return v;
        }
        else  {
            logi("make VALUE from byte array FINISH NIL");
            return rho_ruby_get_NIL();
        }
        
    }


	virtual ~CNFCByteArray() {
        if (mBuf != NULL) {
            delete mBuf;
        }
	}

	unsigned char* mBuf;
	int mSize;
};




VALUE JavaHelper_exec_bytearray_bytearray(VALUE array, const char* method_name) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) {
        loge("Nfc class not found !");
        return rho_ruby_get_NIL();
    }
    jmethodID mid = env->GetStaticMethodID(cls.get(), method_name, "([B)[B");
    if (!mid) { 
        loge("Nfc method :");
        loge(method_name);
        loge("NOT FOUND !");
        return rho_ruby_get_NIL();
    }
    int size = rho_ruby_unpack_byte_array(array, 0, 0);
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(size);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(array, (unsigned char*)buf_p, size);
    
    jbyteArray j_arr =  (jbyteArray)env->CallStaticObjectMethod(cls.get(), mid, buf_j);
    
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    
    CNFCByteArray ar(env, j_arr);
    
    return ar.makeVALUE();
}

void JavaHelper_exec_void_bytearray(VALUE array, const char* method_name) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) {
        loge("Nfc class not found !");
        return;
    }
    jmethodID mid = env->GetStaticMethodID(cls.get(), method_name, "([B)V");
    if (!mid) { 
        loge("Nfc method :");
        loge(method_name);
        loge("NOT FOUND !");
        return;
    }
    int size = rho_ruby_unpack_byte_array(array, 0, 0);
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(size);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(array, (unsigned char*)buf_p, size);
    
    env->CallStaticVoidMethod(cls.get(), mid, buf_j);
    
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
}

VALUE JavaHelper_exec_bytearray_void(const char* method_name) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) {
        loge("Nfc class not found !");
        return rho_ruby_get_NIL();
    }
    jmethodID mid = env->GetStaticMethodID(cls.get(), method_name, "()[B");
    if (!mid) { 
        loge("Nfc method :");
        loge(method_name);
        loge("NOT FOUND !");
        return rho_ruby_get_NIL();
    }
    jbyteArray j_arr =  (jbyteArray)env->CallStaticObjectMethod(cls.get(), mid);
    
    CNFCByteArray ar(env, j_arr);
    
    return ar.makeVALUE();
}

int JavaHelper_exec_int_void(const char* method_name) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) {
        loge("Nfc class not found !");
        return 0;
    }
    jmethodID mid = env->GetStaticMethodID(cls.get(), method_name, "()I");
    if (!mid) { 
        loge("Nfc method :");
        loge(method_name);
        loge("NOT FOUND !");
        return 0;
    }
    return env->CallStaticIntMethod(cls.get(), mid);
}

void JavaHelper_exec_void_int(int value, const char* method_name) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) {
        loge("Nfc class not found !");
        return;
    }
    jmethodID mid = env->GetStaticMethodID(cls.get(), method_name, "(I)V");
    if (!mid) { 
        loge("Nfc method :");
        loge(method_name);
        loge("NOT FOUND !");
        return;
    }
    env->CallStaticVoidMethod(cls.get(), mid, value);
}


static bool ourNeedToFree = false;
static jclass jclass_NfcMessagePack = NULL;
static jclass jclass_NfcMessage = NULL;
static jclass jclass_NfcRecord = NULL;

static jmethodID jmethod_NfcMessagePack_getItemCount = NULL;
static jmethodID jmethod_NfcMessagePack_getItem = NULL;

static jmethodID jmethod_NfcMessage_getItemCount = NULL;
static jmethodID jmethod_NfcMessage_getItem = NULL;
static jmethodID jmethod_NfcMessage_getByteArray = NULL;
static jmethodID jmethod_NfcMessage_addConstructRecord = NULL;
static jmethodID jmethod_NfcMessage_initFromConstructRecordList = NULL;

static jmethodID jmethod_NfcRecord_getId = NULL;
static jmethodID jmethod_NfcRecord_getPayload = NULL;
static jmethodID jmethod_NfcRecord_getPayloadString = NULL;
static jmethodID jmethod_NfcRecord_getTnf = NULL;
static jmethodID jmethod_NfcRecord_getType = NULL;
static jmethodID jmethod_NfcRecord_getByteArray = NULL;
static jmethodID jmethod_NfcRecord_getSubrecords = NULL;



void initJavaIds(JNIEnv *env_param = NULL) {
    
    JNIEnv *env = NULL;//jnienv();
    if (env_param != NULL) {
        env = env_param;
    }
    else {
        env = jnienv();
    }
    
    if (env_param != NULL) {
        jclass_NfcMessagePack = env->FindClass("com/rhomobile/nfc/NfcMessagePack");
        jclass_NfcMessage = env->FindClass("com/rhomobile/nfc/NfcMessage");
        jclass_NfcRecord = env->FindClass("com/rhomobile/nfc/NfcRecord");
        ourNeedToFree = false;
    }
    else {
        jclass_NfcMessagePack = rho_find_class(env, "com/rhomobile/nfc/NfcMessagePack");
        jclass_NfcMessage = rho_find_class(env, "com/rhomobile/nfc/NfcMessage");
        jclass_NfcRecord = rho_find_class(env, "com/rhomobile/nfc/NfcRecord");
        ourNeedToFree = true;
    }
	// fill java ids
    
	jmethod_NfcMessagePack_getItemCount = env->GetMethodID( jclass_NfcMessagePack, "getItemCount", "()I");
	jmethod_NfcMessagePack_getItem = env->GetMethodID( jclass_NfcMessagePack, "getItem", "(I)Lcom/rhomobile/nfc/NfcMessage;");
    
	jmethod_NfcMessage_getItemCount = env->GetMethodID( jclass_NfcMessage, "getItemCount", "()I");
	jmethod_NfcMessage_getItem = env->GetMethodID( jclass_NfcMessage, "getItem", "(I)Lcom/rhomobile/nfc/NfcRecord;");
	jmethod_NfcMessage_getByteArray = env->GetMethodID( jclass_NfcMessage, "getByteArray", "()[B");
	jmethod_NfcMessage_addConstructRecord = env->GetMethodID( jclass_NfcMessage, "addConstructRecord", "(Lcom/rhomobile/nfc/NfcRecord;)V");
	jmethod_NfcMessage_initFromConstructRecordList = env->GetMethodID( jclass_NfcMessage, "initFromConstructRecordList", "()V");
    
	jmethod_NfcRecord_getId = env->GetMethodID( jclass_NfcRecord, "getId", "()[B");
	jmethod_NfcRecord_getPayload = env->GetMethodID( jclass_NfcRecord, "getPayload", "()[B");
	jmethod_NfcRecord_getPayloadString = env->GetMethodID( jclass_NfcRecord, "getPayloadString", "()Ljava/lang/String;");
	jmethod_NfcRecord_getTnf = env->GetMethodID( jclass_NfcRecord, "getTnf", "()I");
	jmethod_NfcRecord_getType = env->GetMethodID( jclass_NfcRecord, "getType", "()[B");
	jmethod_NfcRecord_getByteArray = env->GetMethodID( jclass_NfcRecord, "getByteArray", "()[B");
	jmethod_NfcRecord_getSubrecords = env->GetMethodID( jclass_NfcRecord, "getSubrecords", "()Lcom/rhomobile/nfc/NfcMessage;");
    
}


class CNFCMessage;

class CNFCRecord {
public:
	CNFCRecord(JNIEnv* env, jobject jrec); 
    
    VALUE makeHashValue();
    
	virtual ~CNFCRecord();
    
	CNFCByteArray* mId;
	CNFCByteArray* mPayload;
	CNFCString* mPayloadString;
	int mTnf;
	CNFCByteArray* mType;
	CNFCByteArray* mByteArray;
	CNFCMessage* mSubrecords;
	
};

typedef CNFCRecord* CNFCRecordPtr;

class CNFCMessage {
public:
	CNFCMessage(JNIEnv* env, jobject jmsg) {
		mRecordCount = env->CallIntMethod(jmsg, jmethod_NfcMessage_getItemCount);
		int i;
		mRecords = new CNFCRecordPtr[mRecordCount];
		for (i = 0; i < mRecordCount; i++) {
			mRecords[i] = new CNFCRecord(env, env->CallObjectMethod(jmsg, jmethod_NfcMessage_getItem, i));
		}
		mByteArray = new CNFCByteArray(env, env->CallObjectMethod(jmsg, jmethod_NfcMessage_getByteArray));
	}
    
	virtual ~CNFCMessage() {
		int i;
		for (i = 0; i < mRecordCount; i++) {
			delete mRecords[i];
		}
		delete[] mRecords;
		delete mByteArray;
	}
    
    VALUE makeHashValue();
    
	CNFCRecordPtr* mRecords;
	int mRecordCount;
	CNFCByteArray* mByteArray;
};

typedef CNFCMessage* CNFCMessagePtr;

CNFCRecord::CNFCRecord(JNIEnv* env, jobject jrec) {
	mSubrecords = NULL;
	mId = new CNFCByteArray(env, env->CallObjectMethod(jrec, jmethod_NfcRecord_getId));
	mPayload = new CNFCByteArray(env, env->CallObjectMethod(jrec, jmethod_NfcRecord_getPayload));
	mPayloadString = new CNFCString(env, env->CallObjectMethod(jrec, jmethod_NfcRecord_getPayloadString));
	mType = new CNFCByteArray(env, env->CallObjectMethod(jrec, jmethod_NfcRecord_getType));
	mByteArray = new CNFCByteArray(env, env->CallObjectMethod(jrec, jmethod_NfcRecord_getByteArray));
	mTnf = env->CallIntMethod(jrec, jmethod_NfcRecord_getTnf);
	jobject message = env->CallObjectMethod(jrec, jmethod_NfcRecord_getSubrecords);
	if (message != NULL) {
		mSubrecords = new CNFCMessage(env, message);
	}
}

CNFCRecord::~CNFCRecord() {
	delete mId;
	delete mPayload;
	delete mPayloadString;
	delete mType;
	delete mByteArray;
	if (mSubrecords != NULL) {
		delete mSubrecords;
	}
}

VALUE CNFCRecord::makeHashValue() {
    VALUE hash = rho_ruby_createHash();
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("raw_record"), mByteArray->makeVALUE());
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("id"), mId->makeVALUE());
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("payload"), mPayload->makeVALUE());
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("tnf"), rho_ruby_create_integer(mTnf));
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("type"), mType->makeVALUE());
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("payload_as_string"), mPayloadString->makeVALUE());
    if (mSubrecords != NULL) {
        rho_ruby_add_to_hash(hash, rho_ruby_create_string("subrecords"), mSubrecords->makeHashValue());
    }
    return hash;
}

VALUE CNFCMessage::makeHashValue() {
    VALUE hash = rho_ruby_createHash();
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("raw_message"), mByteArray->makeVALUE());
    
    VALUE records = rho_ruby_create_array();
    int i;
    for (i = 0; i < mRecordCount; i++) {
        rho_ruby_add_to_array(records, mRecords[i]->makeHashValue());
    }
    
    rho_ruby_add_to_hash(hash, rho_ruby_create_string("records"), records);
    
    return hash;
    
}


class CNFCMessagePack {
public:
    
	CNFCMessagePack(JNIEnv* env, jobject jmsg_pack) {
		mMessageCount = env->CallIntMethod(jmsg_pack, jmethod_NfcMessagePack_getItemCount);
		int i;
		mMessages = new CNFCMessagePtr[mMessageCount];
		for (i = 0; i < mMessageCount; i++) {
			mMessages[i] = new CNFCMessage(env, env->CallObjectMethod(jmsg_pack, jmethod_NfcMessagePack_getItem, i));
		}
	}
    
	virtual ~CNFCMessagePack() {
		int i;
		for (i = 0; i < mMessageCount; i++) {
			delete mMessages[i];
		}
		delete[] mMessages;
	}
    
	CNFCMessagePtr* mMessages;
    int mMessageCount;
};



class RhoCallbackNFCContainer : public rho::ICallbackObject
{
public:
	RhoCallbackNFCContainer(CNFCMessagePack* pack) {
		mMessagePack = pack;
	}
    
	virtual ~RhoCallbackNFCContainer() {
		delete mMessagePack;
	}
    
	VALUE makeString(CNFCString* str) {
        logi("                make ruby string");
		return rho_ruby_create_string((const char*)str->mBuf);
	}
    
	VALUE makeByteArray(CNFCByteArray* array) {
        logi("                make ruby byte array");
        if (array->mBuf != NULL) {
            if (logging_enable) {
                char msg[128];
                sprintf(msg, "                    array size = %d", array->mSize);
                logi(msg);
            }
            
            return rho_ruby_create_byte_array(array->mBuf, array->mSize);
        }
        else  {
            return rho_ruby_get_NIL();
        }
	}
    
    
	/*
     # in callback
     # @params['messages'] - array of messages (each message is hash)
     # message hash items : 
     #       'raw_message' - array of bytes (raw message)
     #       'records' - array of records (each record is hash)
     #              record hash items :
     #                     'raw_record' - array of bytes (raw record)
     #                     ' id' - array of bytes
     #                     'payload' - array of bytes
     #                     'tnf' - int
     #                     'type' - array of bytes 
     #                     'payload_as_string' - string, payload prepared to string (support specail formats for URI, TEXT) 
     #                     'subrecords' - array of subrecords, each records is hash (only for SMART_POSTER type)
     */
    
	// return Ruby object
	virtual unsigned long getObjectValue() {
		logi("make Ruby value for callback START");
        
		VALUE messages_array = rho_ruby_create_array();
		int i;
        
		logi("    make messages array :");
		for (i = 0; i < mMessagePack->mMessageCount; i++) {
			rho_ruby_add_to_array(messages_array, mMessagePack->mMessages[i]->makeHashValue());	
		}
        
		logi("make Ruby value for callback FINISH");
		return messages_array;
	}
    
private:
	CNFCMessagePack*	mMessagePack;
};









extern "C" void rho_nfc_set_callback(const char* callback_url) {
	if (!checkSDK()) {
		return;
	} 
	JNIEnv *env = jnienv();
	CNfcJavaClass cls;
	if (!cls.get()) return;
	jmethodID mid = env->GetStaticMethodID( cls.get(), "setCallback", "(Ljava/lang/String;)V");
	if (!mid) return;
	jstring objCallback = env->NewStringUTF(callback_url);
	env->CallStaticVoidMethod(cls.get(), mid, objCallback);
	env->DeleteLocalRef(objCallback);
}

extern "C" void rho_nfc_enable(int enable) {
	if (!checkSDK()) {
		return;
	} 
	JNIEnv *env = jnienv();
	CNfcJavaClass cls;
	if (!cls.get()) return;
	jmethodID mid = env->GetStaticMethodID( cls.get(), "setEnable", "(I)V");
	if (!mid) return;
	env->CallStaticVoidMethod(cls.get(), mid, enable);
}

extern "C" int rho_nfc_is_enabled(void) {
	if (!checkSDK()) {
		return 0;
	} 
    return JavaHelper_exec_int_void("isEnabled");    
}

extern "C" int rho_nfc_is_supported(void) {
	if (!checkSDK()) {
		return 0;
	} 
    return JavaHelper_exec_int_void("isSupported");    
}

extern "C" void rho_nfc_set_tech_callback(const char* callback_url) {
	if (!checkSDK()) {
		return;
	} 
	JNIEnv *env = jnienv();
	CNfcJavaClass cls;
	if (!cls.get()) return;
	jmethodID mid = env->GetStaticMethodID( cls.get(), "setTechCallback", "(Ljava/lang/String;)V");
	if (!mid) return;
	jstring objCallback = env->NewStringUTF(callback_url);
	env->CallStaticVoidMethod(cls.get(), mid, objCallback);
	env->DeleteLocalRef(objCallback);
}

extern "C" void rho_nfc_perform_open_application_event() {
	if (!checkSDK()) {
		return;
	} 
	JNIEnv *env = jnienv();
	CNfcJavaClass cls;
	if (!cls.get()) return;
	jmethodID mid = env->GetStaticMethodID( cls.get(), "performOpenApplicationTag", "()V");
	if (!mid) return;
	env->CallStaticVoidMethod(cls.get(), mid);
}



extern "C" void rho_nfc_set_listen_tech_list(VALUE tech_list) {

}

extern "C" VALUE rho_nfc_get_tech_list() {

    logi("rho_nfc_get_tech_list START");
	if (!checkSDK()) {
		return rho_ruby_get_NIL();
	} 
	JNIEnv *env = jnienv();
	CNfcJavaClass cls;
	if (!cls.get()) return rho_ruby_get_NIL();
	jmethodID mid = env->GetStaticMethodID( cls.get(), "get_tech_list_count", "()I");
	if (!mid) return rho_ruby_get_NIL();
	jmethodID mids = env->GetStaticMethodID( cls.get(), "get_tech_list", "(I)Ljava/lang/String;");
	if (!mids) return rho_ruby_get_NIL();
	int count =  env->CallStaticIntMethod(cls.get(), mid);

	VALUE array = rho_ruby_create_array();

	int i;
    
	for (i = 0; i < count; i++) {
		CNFCString  cs(env, env->CallStaticObjectMethod(cls.get(), mids, i));
		rho_ruby_add_to_array( array, rho_ruby_create_string(cs.mBuf));
        logi("rho_nfc_get_tech_list 05");

	}
	return array;
}

extern "C" void rho_nfc_tech_connect(const char* name) {
    logi("rho_nfc_tech_connect START");
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_connect", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objCallback = env->NewStringUTF(name);
    env->CallStaticVoidMethod(cls.get(), mid, objCallback);
    env->DeleteLocalRef(objCallback);
    logi("rho_nfc_tech_connect FINISH");
}

extern "C" void rho_nfc_tech_close(const char* name) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_close", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objCallback = env->NewStringUTF(name);
    env->CallStaticVoidMethod(cls.get(), mid, objCallback);
    env->DeleteLocalRef(objCallback);
}

extern "C" int rho_nfc_tech_is_connected(const char* name) {
    logi("rho_nfc_tech_is_connected START");
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return 0;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_is_connected", "(Ljava/lang/String;)I");
    if (!mid) return 0;
    jstring objCallback = env->NewStringUTF(name);
    int res = env->CallStaticIntMethod(cls.get(), mid, objCallback);
    env->DeleteLocalRef(objCallback);
    logi("rho_nfc_tech_is_connected FINISH");
    return res;    
}

extern "C" int rho_nfc_tech_MifareClassic_get_size() {
    return JavaHelper_exec_int_void("tech_MifareClassic_get_size");    
}

extern "C" void rho_nfc_tech_MifareClassic_write_block(int index, VALUE block) {

    logi("rho_nfc_tech_MifareClassic_write_block START");
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareClassic_write_block", "(I[B)V");
    if (!mid) return;
    
    int res = 0;
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(16);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(block, (unsigned char*)buf_p, 16);
    
    env->CallStaticVoidMethod(cls.get(), mid, index, buf_j);
    
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    
    logi("rho_nfc_tech_MifareClassic_write_block FINISH");
    
}

extern "C" VALUE rho_nfc_tech_MifareClassic_read_block(int index) {

    logi("rho_nfc_tech_MifareClassic_read_block START");
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareClassic_read_block", "(I[B)I");
    if (!mid) return rho_ruby_get_NIL();

    VALUE val = rho_ruby_get_NIL();
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(16);
    int res = env->CallStaticIntMethod(cls.get(), mid, index, buf_j);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    if (res != 0) {
          logi("java Nfc return real array");
          val = rho_ruby_create_byte_array((unsigned char*)buf_p, 16);
    }
    else {
          logi("java Nfc not filled array !!!");
    }
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    logi("rho_nfc_tech_MifareClassic_read_block FINISH");

    return val;
}

extern "C" int rho_nfc_tech_MifareClassic_get_block_count() {
    return JavaHelper_exec_int_void("tech_MifareClassic_get_block_count");
}

extern "C" int rho_nfc_tech_MifareClassic_get_sector_count() {
    return JavaHelper_exec_int_void("tech_MifareClassic_get_sector_count");
}

extern "C" int rho_nfc_tech_MifareClassic_get_blocks_in_sector_count(int index) {
    logi("rho_nfc_tech_MifareClassic_get_blocks_in_sector_count START");    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return 0;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareClassic_get_blocks_in_sector_count", "(I)I");
    if (!mid) return 0;
    logi("rho_nfc_tech_MifareClassic_get_blocks_in_sector_count FINISH");    
    return env->CallStaticIntMethod(cls.get(), mid, index);
}

extern "C" int rho_nfc_tech_MifareClassic_sector_to_block(int index) {
    logi("rho_nfc_tech_MifareClassic_sector_to_block START");    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return 0;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareClassic_sector_to_block", "(I)I");
    if (!mid) return 0;
    logi("rho_nfc_tech_MifareClassic_sector_to_block FINISH");    
    return env->CallStaticIntMethod(cls.get(), mid, index);
}

extern "C" int rho_nfc_tech_MifareClassic_get_type() {
    return JavaHelper_exec_int_void("tech_MifareClassic_get_type");
}

extern "C" VALUE rho_nfc_tech_MifareClassic_transceive(VALUE data) {
    return JavaHelper_exec_bytearray_bytearray(data, "tech_MifareClassic_transceive");
}

extern "C" VALUE rho_nfc_tag_get_id() {
    return JavaHelper_exec_bytearray_void("tag_get_id");
}


extern "C" int rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A(int index, VALUE key) {

    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A START");
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return 0;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareClassic_authenticate_sector_with_key_A", "(I[B)I");
    if (!mid) return 0;
    
    int res = 0;
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(6);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(key, (unsigned char*)buf_p, 6);
    
    res = env->CallStaticIntMethod(cls.get(), mid, index, buf_j);

    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    
    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A FINISH");
    
    return res;
    
}

extern "C" int rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B(int index, VALUE key) {
    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B START");
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return 0;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareClassic_authenticate_sector_with_key_B", "(I[B)I");
    if (!mid) return 0;
    
    int res = 0;
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(6);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(key, (unsigned char*)buf_p, 6);
    
    res = env->CallStaticIntMethod(cls.get(), mid, index, buf_j);
    
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    
    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B FINISH");
    
    return res;
    
}


extern "C" int rho_nfc_tech_Ndef_get_max_size() {
    return JavaHelper_exec_int_void("tech_Ndef_get_max_size");
}

// return string
extern "C" VALUE rho_nfc_tech_Ndef_get_type() {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_Ndef_get_type", "()Ljava/lang/String;");
    if (!mid) return rho_ruby_get_NIL();
    jstring j_str =  (jstring)env->CallStaticObjectMethod(cls.get(), mid);
    CNFCString s(env, j_str);
    return s.makeVALUE();
}

extern "C" int rho_nfc_tech_Ndef_is_writable() {
    return JavaHelper_exec_int_void("tech_Ndef_is_writable");
}

extern "C" int rho_nfc_tech_Ndef_can_make_read_only() {
    return JavaHelper_exec_int_void("tech_Ndef_can_make_read_only");
}

extern "C" int rho_nfc_tech_Ndef_make_read_only() {
    return JavaHelper_exec_int_void("tech_Ndef_make_read_only");
}

extern "C" void rho_nfc_tech_Ndef_write_Nde_message(VALUE message_in_byte_array) {
    JavaHelper_exec_void_bytearray(message_in_byte_array, "tech_Ndef_write_Nde_message");
}

// return byte[]
extern "C" VALUE rho_nfc_tech_Ndef_read_Nde_message() {
    return JavaHelper_exec_bytearray_void("tech_Ndef_read_Nde_message");
}

// return byte[]
extern "C" VALUE rho_nfc_tech_NfcA_get_Atqa() {
    return JavaHelper_exec_bytearray_void("tech_NfcA_get_Atqa");
}

extern "C" int rho_nfc_tech_NfcA_get_Sak() {
    return JavaHelper_exec_int_void("tech_NfcA_get_Sak");
}

// return byte[]
extern "C" VALUE rho_nfc_tech_NfcA_transceive(VALUE data) {
    return JavaHelper_exec_bytearray_bytearray(data, "tech_NfcA_transceive");
}


// return HASH
extern "C" VALUE rho_nfc_convert_byte_array_to_NdeRecord_hash(VALUE array) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    initJavaIds();
    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "convert_byte_array_to_NdeRecord_hash", "([B)Lcom/rhomobile/nfc/NfcRecord;");
    if (!mid) return rho_ruby_get_NIL();
    
    int size = rho_ruby_unpack_byte_array(array, 0, 0);
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(size);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(array, (unsigned char*)buf_p, size);
    
    jobject j_o =  env->CallStaticObjectMethod(cls.get(), mid, buf_j);
    
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    
    CNFCRecord r(env, j_o);
    
    return r.makeHashValue();
}

// return byte array
extern "C" VALUE rho_nfc_convert_NdeRecord_hash_to_byte_array(VALUE hash) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    initJavaIds();

    VALUE ruby_id = rho_ruby_hash_aref(hash, "id");
    VALUE ruby_type = rho_ruby_hash_aref(hash, "type");
    VALUE ruby_payload = rho_ruby_hash_aref(hash, "payload");
    VALUE ruby_tnf = rho_ruby_hash_aref(hash, "tnf");

    int tnf = (int)rho_ruby_get_int(ruby_tnf);
    CRubyByteArray c_id(ruby_id);
    CRubyByteArray c_type(ruby_type);
    CRubyByteArray c_payload(ruby_payload);
    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "convert_NdeRecord_hash_to_byte_array", "([B[BI[B)[B");
    if (!mid) return rho_ruby_get_NIL();
    
    jbyteArray j_result = (jbyteArray)env->CallStaticObjectMethod(cls.get(), mid, c_id.makeJavaByteArray(env), c_payload.makeJavaByteArray(env), tnf, c_type.makeJavaByteArray(env));
    
    CNFCByteArray ar(env, j_result);
    
    return ar.makeVALUE();
}

// return array of byte array
extern "C" VALUE rho_nfc_convert_NdeMessage_byte_array_to_NdeRecords_array(VALUE array) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    
    initJavaIds();

    CRubyByteArray c_msg(array);

    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "convert_NdeMessage_byte_array_to_NdeRecords_array", "([B)Lcom/rhomobile/nfc/NfcMessage;");
    if (!mid) return rho_ruby_get_NIL();
    
    jobject j_msg = env->CallStaticObjectMethod(cls.get(), mid, c_msg.makeJavaByteArray(env));
    
    CNFCMessage msg(env, j_msg);
    
    int i;
    
    VALUE result = rho_ruby_create_array();
    
    for (i = 0; i < msg.mRecordCount; i++) {
        CNFCRecord* rec = msg.mRecords[i];
        rho_ruby_add_to_array(result, rec->mByteArray->makeVALUE());   
        
    }
    
    return result;
}

// return byte array
extern "C" VALUE rho_nfc_convert_NdeRecords_array_to_NdeMessage_byte_array(VALUE array) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    initJavaIds();
    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid_make_msg = env->GetStaticMethodID(cls.get(), "make_empty_NfcMessage", "()Lcom/rhomobile/nfc/NfcMessage;");
    if (!mid_make_msg) return rho_ruby_get_NIL();
    jmethodID mid_make_rec = env->GetStaticMethodID(cls.get(), "convert_byte_array_to_NdeRecord_hash", "([B)Lcom/rhomobile/nfc/NfcRecord;");
    if (!mid_make_rec) return rho_ruby_get_NIL();
    
    jobject j_msg = env->CallStaticObjectMethod(cls.get(), mid_make_msg);
    
    int count = rho_ruby_array_get_size(array);
    int i;
    
    for (i = 0; i < count; i++) {
        CRubyByteArray c_rec(rho_ruby_array_get(array, i));
        
        jobject j_rec = env->CallStaticObjectMethod(cls.get(), mid_make_rec, c_rec.makeJavaByteArray(env));
        
        env->CallVoidMethod(j_msg, jmethod_NfcMessage_addConstructRecord, j_rec);
    }
    env->CallVoidMethod(j_msg, jmethod_NfcMessage_initFromConstructRecordList);
    
    CNFCByteArray ar(env, env->CallObjectMethod(j_msg, jmethod_NfcMessage_getByteArray));
    
    return ar.makeVALUE();
    
}

// String make_string_from_payload(byte[] payload, int tnf, byte[] type)
// return string
extern "C" VALUE rho_nfc_make_string_from_payload(VALUE payload, int tnf, VALUE type) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    
    CRubyByteArray c_payload(payload);
    CRubyByteArray c_type(type);
    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "make_string_from_payload", "([BI[B)Ljava/lang/String;");
    if (!mid) return rho_ruby_get_NIL();
    
    jstring j_s = (jstring)env->CallStaticObjectMethod(cls.get(), mid, c_payload.makeJavaByteArray(env), tnf, c_type.makeJavaByteArray(env));
    
    CNFCString s(env, j_s);
    
    return s.makeVALUE();
}

// return byte[]
extern "C" VALUE rho_nfc_make_payload_with_absolute_uri(const char* str) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    
    CRubyString c_s(str);
    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "make_payload_with_absolute_uri", "(Ljava/lang/String;)[B");
    if (!mid) return rho_ruby_get_NIL();
    
    jobject j_o = env->CallStaticObjectMethod(cls.get(), mid, c_s.makeJavaString(env));
    
    CNFCByteArray ar(env, j_o);
    
    return ar.makeVALUE();
    
}

// return byte[]
extern "C" VALUE rho_nfc_make_payload_with_well_known_text(const char* language, const char* str) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    CRubyString c_lang(language);
    CRubyString c_s(str);
    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "make_payload_with_well_known_text", "(Ljava/lang/String;Ljava/lang/String;)[B");
    if (!mid) return rho_ruby_get_NIL();
    
    jobject j_o = env->CallStaticObjectMethod(cls.get(), mid, c_lang.makeJavaString(env), c_s.makeJavaString(env));
    
    CNFCByteArray ar(env, j_o);
    
    return ar.makeVALUE();
    
}

// return byte[]
extern "C" VALUE rho_nfc_make_payload_with_well_known_uri(int prefix, const char* str) {
    if (!checkSDK()) {
         return rho_ruby_get_NIL();
    } 
    CRubyString c_s(str);
    
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "make_payload_with_well_known_uri", "(ILjava/lang/String;)[B");
    if (!mid) return rho_ruby_get_NIL();
    
    jobject j_o = env->CallStaticObjectMethod(cls.get(), mid, prefix, c_s.makeJavaString(env));
    
    CNFCByteArray ar(env, j_o);
    
    return ar.makeVALUE();
}

extern "C" void rho_nfc_p2p_enable_foreground_nde_push(VALUE nde_message_byte_array) {
    if (!checkSDK()) {
         return;
    } 
    JavaHelper_exec_void_bytearray(nde_message_byte_array, "p2p_enable_foreground_nde_push");
}

extern "C" void rho_nfc_p2p_disable_foreground_nde_push() {
    if (!checkSDK()) {
         return;
    } 
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "p2p_disable_foreground_nde_push", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls.get(), mid);
}

extern "C" void rho_nfc_tech_MifareUltralight_write_page(int index, VALUE block) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return;
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareUltralight_write_page", "(I[B)V");
    if (!mid) return;
    
    int res = 0;
    int size = rho_ruby_unpack_byte_array(block, 0, 0);
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(size);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(block, (unsigned char*)buf_p, size);
    
    env->CallStaticVoidMethod(cls.get(), mid, index, buf_j);
    
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
}

extern "C" VALUE rho_nfc_tech_MifareUltralight_read_pages(int index) {
    JNIEnv *env = jnienv();
    CNfcJavaClass cls;
    if (!cls.get()) return rho_ruby_get_NIL();
    jmethodID mid = env->GetStaticMethodID(cls.get(), "tech_MifareUltralight_read_pages", "(I)[B");
    if (!mid) return rho_ruby_get_NIL();

    jbyteArray j_arr =  (jbyteArray)env->CallStaticIntMethod(cls.get(), mid, index);

    CNFCByteArray ar(env, j_arr);
    
    return ar.makeVALUE();
}

extern "C" int rho_nfc_tech_MifareUltralight_get_type() {
    return JavaHelper_exec_int_void("tech_MifareUltralight_get_type");
}

extern "C" VALUE rho_nfc_tech_MifareUltralight_transceive(VALUE data) {
    return JavaHelper_exec_bytearray_bytearray(data, "tech_MifareUltralight_transceive");
}


extern "C" VALUE rho_nfc_tech_IsoDep_get_hi_layer_responce() {
    return JavaHelper_exec_bytearray_void("tech_IsoDep_get_hi_layer_responce");
}

extern "C" VALUE rho_nfc_tech_IsoDep_get_historical_bytes() {
    return JavaHelper_exec_bytearray_void("tech_IsoDep_get_historical_bytes");
}

extern "C" void rho_nfc_tech_IsoDep_set_timeout(int timeout) {
    JavaHelper_exec_void_int(timeout, "tech_IsoDep_set_timeout");
}

extern "C" VALUE rho_nfc_tech_IsoDep_transceive(VALUE data) {
    return JavaHelper_exec_bytearray_bytearray(data, "tech_IsoDep_transceive");
}

extern "C" void rho_nfc_tech_NdefFormatable_format(VALUE ndef_message_byte_array) {
    JavaHelper_exec_void_bytearray(ndef_message_byte_array, "tech_NdefFormatable_format");
}

extern "C" void rho_nfc_tech_NdefFormatable_format_read_only(VALUE ndef_message_byte_array) {
    JavaHelper_exec_void_bytearray(ndef_message_byte_array, "tech_NdefFormatable_format_read_only");
}

// return byte[]
extern "C" VALUE rho_nfc_tech_NfcB_get_application_data() {
    return JavaHelper_exec_bytearray_void("tech_NfcB_get_application_data");
}

// return byte[]
extern "C" VALUE rho_nfc_tech_NfcB_get_protocol_info() {
    return JavaHelper_exec_bytearray_void("tech_NfcB_get_protocol_info");
}

// byte[] -> byte[]
extern "C" VALUE rho_nfc_tech_NfcB_transceive(VALUE data) {
    return JavaHelper_exec_bytearray_bytearray(data, "tech_NfcB_transceive");
}

// return byte[]
extern "C" VALUE rho_nfc_tech_NfcF_get_manufacturer() {
    return JavaHelper_exec_bytearray_void("tech_NfcF_get_manufacturer");
}

// return byte[]
extern "C" VALUE rho_nfc_tech_NfcF_get_system_code() {
    return JavaHelper_exec_bytearray_void("tech_NfcF_get_system_code");
}

// byte[] -> byte[]
extern "C" VALUE rho_nfc_tech_NfcF_transceive(VALUE data) {
    return JavaHelper_exec_bytearray_bytearray(data, "tech_NfcF_transceive");
}

extern "C" int rho_nfc_tech_NfcV_get_dsf_id() {
    return JavaHelper_exec_int_void("tech_NfcV_get_dsf_id");
}

extern "C" int rho_nfc_tech_NfcV_get_responce_flags() {
    return JavaHelper_exec_int_void("tech_NfcV_get_responce_flags");
}

// byte[] -> byte[]
extern "C" VALUE rho_nfc_tech_NfcV_transceive(VALUE data) {
    return JavaHelper_exec_bytearray_bytearray(data, "tech_NfcV_transceive");
}







//  private native void callTechCallback(String callback_url, String event);
extern "C" void JNICALL Java_com_rhomobile_nfc_Nfc_callTechCallback
(JNIEnv *env, jclass, jstring js_callback_url, jstring js_event)
{
	char url[2048];
	char body[2048];

	const char* jurl = env->GetStringUTFChars(js_callback_url,0);
	const char* jevent = env->GetStringUTFChars(js_event,0);
	strcpy(url, jurl);
	env->ReleaseStringUTFChars(js_callback_url, jurl);

	strcpy(body, "&rho_callback=1");
	strcat(body, "&");
	strcat(body, "Tag_event=");
	strcat(body, jevent);
	env->ReleaseStringUTFChars(js_event, jevent);

	rho_net_request_with_data(rho_http_normalizeurl(url), body);
}

//  private native void callTechCallback(String callback_url, String event);
extern "C" void JNICALL Java_com_rhomobile_nfc_Nfc_logNative
(JNIEnv *env, jclass, jstring js_msg)
{
	char msg[2048];
    
	const char* jmsg = env->GetStringUTFChars(js_msg,0);
	strcpy(msg, jmsg);
	env->ReleaseStringUTFChars(js_msg, jmsg);
  
	logi(msg);
}





//  private native void callCallback(String callback_url, NfcMessagePack msgpack);
extern "C" void JNICALL Java_com_rhomobile_nfc_Nfc_callCallback
(JNIEnv *env, jclass, jstring js_callback_url, jobject jo_msgpack)
{
	logi("native callback START");

    initJavaIds(env);
    
	CNFCMessagePack* messagePack = new CNFCMessagePack(env, jo_msgpack);

	char url[2048];
	char body[2048];

	const char* jurl = env->GetStringUTFChars(js_callback_url,0);
	strcpy(url, jurl);
	env->ReleaseStringUTFChars(js_callback_url, jurl);

	strcpy(body, "&rho_callback=1");
	strcat(body, "&");
	strcat(body, (RHODESAPP().addCallbackObject( new RhoCallbackNFCContainer(messagePack), "messages")).c_str());

	rho_net_request_with_data(rho_http_normalizeurl(url), body);

}



