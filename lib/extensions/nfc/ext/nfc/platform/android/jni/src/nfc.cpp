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









class CNFCString {
public:
	CNFCString(JNIEnv* env, jobject jo_string) {
		const char* msg = env->GetStringUTFChars((jstring)jo_string,0);
		mSize = strlen(msg);
		mBuf = new char[mSize+1];
		strcpy(mBuf, msg);
		env->ReleaseStringUTFChars((jstring)jo_string, msg);
	}

	virtual ~CNFCString() {
		delete mBuf;
	}

	char* mBuf;
	int mSize;
};



class CNFCByteArray {
public:
	CNFCByteArray(JNIEnv* env, jobject jo_bytearray) {
        logi("Construct ByteArray from java object START");
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
        logi("Construct ByteArray from java object FINISH");
	}


	virtual ~CNFCByteArray() {
        if (mBuf != NULL) {
            delete mBuf;
        }
	}


	unsigned char* mBuf;
	int mSize;
};













extern "C" void rho_nfc_set_callback(const char* callback_url) {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
	if (!cls) return;
	jmethodID mid = env->GetStaticMethodID( cls, "setCallback", "(Ljava/lang/String;)V");
	if (!mid) return;
	jstring objCallback = env->NewStringUTF(callback_url);
	env->CallStaticVoidMethod(cls, mid, objCallback);
	env->DeleteLocalRef(objCallback);
}

extern "C" void rho_nfc_enable(int enable) {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
	if (!cls) return;
	jmethodID mid = env->GetStaticMethodID( cls, "setEnable", "(I)V");
	if (!mid) return;
	env->CallStaticVoidMethod(cls, mid, enable);
}

extern "C" int rho_nfc_is_enabled(void) {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
	if (!cls) return 0;
	jmethodID mid = env->GetStaticMethodID( cls, "isEnabled", "()I");
	if (!mid) return 0;
	return env->CallStaticIntMethod(cls, mid);
}

extern "C" int rho_nfc_is_supported(void) {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
	if (!cls) return 0;
	jmethodID mid = env->GetStaticMethodID( cls, "isSupported", "()I");
	if (!mid) return 0;
	return env->CallStaticIntMethod(cls, mid);
}

extern "C" void rho_nfc_set_tech_callback(const char* callback_url) {
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
	if (!cls) return;
	jmethodID mid = env->GetStaticMethodID( cls, "setTechCallback", "(Ljava/lang/String;)V");
	if (!mid) return;
	jstring objCallback = env->NewStringUTF(callback_url);
	env->CallStaticVoidMethod(cls, mid, objCallback);
	env->DeleteLocalRef(objCallback);
}

extern "C" void rho_nfc_set_listen_tech_list(VALUE tech_list) {

}

extern "C" VALUE rho_nfc_get_tech_list() {

    logi("rho_nfc_get_tech_list START");
	JNIEnv *env = jnienv();
	jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
	if (!cls) return rho_ruby_get_NIL();
    logi("rho_nfc_get_tech_list 01");
	jmethodID mid = env->GetStaticMethodID( cls, "get_tech_list_count", "()I");
	if (!mid) return rho_ruby_get_NIL();
    logi("rho_nfc_get_tech_list 02");
	jmethodID mids = env->GetStaticMethodID( cls, "get_tech_list", "(I)Ljava/lang/String;");
	if (!mids) return rho_ruby_get_NIL();
    logi("rho_nfc_get_tech_list 03");
	int count =  env->CallStaticIntMethod(cls, mid);

	VALUE array = rho_ruby_create_array();

	int i;
    logi("rho_nfc_get_tech_list 04");
    
	for (i = 0; i < count; i++) {
		CNFCString  cs(env, env->CallStaticObjectMethod(cls, mids, i));
		rho_ruby_add_to_array( array, rho_ruby_create_string(cs.mBuf));
        logi("rho_nfc_get_tech_list 05");

	}
    logi("rho_nfc_get_tech_list 06");
	return array;
}

extern "C" void rho_nfc_tech_connect(const char* name) {
    logi("rho_nfc_tech_connect START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_connect", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objCallback = env->NewStringUTF(name);
    env->CallStaticVoidMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
    logi("rho_nfc_tech_connect FINISH");
}

extern "C" void rho_nfc_tech_close(const char* name) {
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_close", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objCallback = env->NewStringUTF(name);
    env->CallStaticVoidMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
}

extern "C" int rho_nfc_tech_is_connected(const char* name) {
    logi("rho_nfc_tech_is_connected START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_is_connected", "(Ljava/lang/String;)I");
    if (!mid) return 0;
    jstring objCallback = env->NewStringUTF(name);
    int res = env->CallStaticIntMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
    logi("rho_nfc_tech_is_connected FINISH");
    return res;    
}

extern "C" int rho_nfc_tech_MifareClassic_get_size() {
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_get_size", "()I");
    if (!mid) return 0;
    return env->CallStaticIntMethod(cls, mid);
}

extern "C" void rho_nfc_tech_MifareClassic_write_block(int index, VALUE block) {

}

extern "C" VALUE rho_nfc_tech_MifareClassic_read_block(int index) {

    logi("rho_nfc_tech_MifareClassic_read_block START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return rho_ruby_get_NIL();
    logi("rho_nfc_tech_MifareClassic_read_block 02");
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_read_block", "(I[B)I");
    if (!mid) return rho_ruby_get_NIL();
    logi("rho_nfc_tech_MifareClassic_read_block 03");

    VALUE val = rho_ruby_get_NIL();
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(16);
    int res = env->CallStaticIntMethod(cls, mid, index, buf_j);
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
    logi("rho_nfc_tech_MifareClassic_get_block_count START");    
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_get_block_count", "()I");
    if (!mid) return 0;
    int c = env->CallStaticIntMethod(cls, mid);
    logi("rho_nfc_tech_MifareClassic_get_block_count FINISH");    
    return c;
}



extern "C" int rho_nfc_tech_MifareClassic_get_sector_count() {
    logi("rho_nfc_tech_MifareClassic_get_sector_count START");    
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_get_sector_count", "()I");
    if (!mid) return 0;
    logi("rho_nfc_tech_MifareClassic_get_sector_count FINISH");    
    return env->CallStaticIntMethod(cls, mid);
}

extern "C" int rho_nfc_tech_MifareClassic_get_blocks_in_sector_count(int index) {
    logi("rho_nfc_tech_MifareClassic_get_blocks_in_sector_count START");    
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_get_blocks_in_sector_count", "(I)I");
    if (!mid) return 0;
    logi("rho_nfc_tech_MifareClassic_get_blocks_in_sector_count FINISH");    
    return env->CallStaticIntMethod(cls, mid, index);
}

extern "C" int rho_nfc_tech_MifareClassic_sector_to_block(int index) {
    logi("rho_nfc_tech_MifareClassic_sector_to_block START");    
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_sector_to_block", "(I)I");
    if (!mid) return 0;
    logi("rho_nfc_tech_MifareClassic_sector_to_block FINISH");    
    return env->CallStaticIntMethod(cls, mid, index);
}

extern "C" int rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A(int index, VALUE key) {

    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_authenticate_sector_with_key_A", "(I[B)I");
    if (!mid) return 0;
    
    int res = 0;
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(6);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(key, (unsigned char*)buf_p, 6);
    
    res = env->CallStaticIntMethod(cls, mid, index, buf_j);

    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    
    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_A FINISH");
    
    return res;
    
}

extern "C" int rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B(int index, VALUE key) {
    
    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/nfc/Nfc");
    if (!cls) return 0;
    jmethodID mid = env->GetStaticMethodID(cls, "tech_MifareClassic_authenticate_sector_with_key_B", "(I[B)I");
    if (!mid) return 0;
    
    int res = 0;
    jbyteArray buf_j = (jbyteArray)env->NewByteArray(6);
    jbyte* buf_p = env->GetByteArrayElements(buf_j, 0);
    
    rho_ruby_unpack_byte_array(key, (unsigned char*)buf_p, 6);
    
    res = env->CallStaticIntMethod(cls, mid, index, buf_j);
    
    env->ReleaseByteArrayElements(buf_j, buf_p, 0);
    env->DeleteLocalRef(buf_j);
    
    logi("rho_nfc_tech_MifareClassic_authenticate_sector_with_key_B FINISH");
    
    return res;
    
}




extern "C" void rho_nfc_tech_MifareUltralight_write_page(int index, VALUE block) {

}

extern "C" VALUE rho_nfc_tech_MifareUltralight_read_pages(int index) {
       return rho_ruby_get_NIL();
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




static jclass jclass_NfcMessagePack = NULL;
static jclass jclass_NfcMessage = NULL;
static jclass jclass_NfcRecord = NULL;

static jmethodID jmethod_NfcMessagePack_getItemCount = NULL;
static jmethodID jmethod_NfcMessagePack_getItem = NULL;

static jmethodID jmethod_NfcMessage_getItemCount = NULL;
static jmethodID jmethod_NfcMessage_getItem = NULL;
static jmethodID jmethod_NfcMessage_getByteArray = NULL;

static jmethodID jmethod_NfcRecord_getId = NULL;
static jmethodID jmethod_NfcRecord_getPayload = NULL;
static jmethodID jmethod_NfcRecord_getPayloadString = NULL;
static jmethodID jmethod_NfcRecord_getTnf = NULL;
static jmethodID jmethod_NfcRecord_getType = NULL;
static jmethodID jmethod_NfcRecord_getByteArray = NULL;
static jmethodID jmethod_NfcRecord_getSubrecords = NULL;




class CNFCMessage;

class CNFCRecord {
public:
	CNFCRecord(JNIEnv* env, jobject jrec); 

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

	VALUE makeRecordHash(CNFCRecord* record) {
        logi("            make record START");
		VALUE hash = rho_ruby_createHash();
		rho_ruby_add_to_hash(hash, rho_ruby_create_string("raw_record"), makeByteArray(record->mByteArray));
		rho_ruby_add_to_hash(hash, rho_ruby_create_string("id"), makeByteArray(record->mId));
		rho_ruby_add_to_hash(hash, rho_ruby_create_string("payload"), makeByteArray(record->mPayload));
		rho_ruby_add_to_hash(hash, rho_ruby_create_string("tnf"), rho_ruby_create_integer(record->mTnf));
		rho_ruby_add_to_hash(hash, rho_ruby_create_string("type"), makeByteArray(record->mType));
		rho_ruby_add_to_hash(hash, rho_ruby_create_string("payload_as_string"), makeString(record->mPayloadString));
		if (record->mSubrecords != NULL) {
			rho_ruby_add_to_hash(hash, rho_ruby_create_string("subrecords"), makeMessageHash(record->mSubrecords));
		}
        logi("            make record FINISH");
		return hash;
	}

	VALUE makeMessageHash(CNFCMessage* msg) {
        logi("        make message START");
		VALUE hash = rho_ruby_createHash();
		rho_ruby_add_to_hash(hash, rho_ruby_create_string("raw_message"), makeByteArray(msg->mByteArray));

		VALUE records = rho_ruby_create_array();
		int i;
        logi("        make message make records :");
		for (i = 0; i < msg->mRecordCount; i++) {
			rho_ruby_add_to_array(records, makeRecordHash(msg->mRecords[i]));
		}

		rho_ruby_add_to_hash(hash, rho_ruby_create_string("records"), records);

        logi("        make message FINISH");
		return hash;
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
			rho_ruby_add_to_array(messages_array, makeMessageHash(mMessagePack->mMessages[i]));	
		}

		logi("make Ruby value for callback FINISH");
		return messages_array;
	}

private:
	CNFCMessagePack*	mMessagePack;
};


//  private native void callCallback(String callback_url, NfcMessagePack msgpack);
extern "C" void JNICALL Java_com_rhomobile_nfc_Nfc_callCallback
(JNIEnv *env, jclass, jstring js_callback_url, jobject jo_msgpack)
{
	logi("native callback START");
	// fill java ids
	jclass_NfcMessagePack = env->FindClass("com/rhomobile/nfc/NfcMessagePack");
	jclass_NfcMessage = env->FindClass("com/rhomobile/nfc/NfcMessage");
	jclass_NfcRecord = env->FindClass("com/rhomobile/nfc/NfcRecord");

	jmethod_NfcMessagePack_getItemCount = env->GetMethodID( jclass_NfcMessagePack, "getItemCount", "()I");
	jmethod_NfcMessagePack_getItem = env->GetMethodID( jclass_NfcMessagePack, "getItem", "(I)Lcom/rhomobile/nfc/NfcMessage;");

	jmethod_NfcMessage_getItemCount = env->GetMethodID( jclass_NfcMessage, "getItemCount", "()I");
	jmethod_NfcMessage_getItem = env->GetMethodID( jclass_NfcMessage, "getItem", "(I)Lcom/rhomobile/nfc/NfcRecord;");
	jmethod_NfcMessage_getByteArray = env->GetMethodID( jclass_NfcMessage, "getByteArray", "()[B");

	jmethod_NfcRecord_getId = env->GetMethodID( jclass_NfcRecord, "getId", "()[B");
	jmethod_NfcRecord_getPayload = env->GetMethodID( jclass_NfcRecord, "getPayload", "()[B");
	jmethod_NfcRecord_getPayloadString = env->GetMethodID( jclass_NfcRecord, "getPayloadString", "()Ljava/lang/String;");
	jmethod_NfcRecord_getTnf = env->GetMethodID( jclass_NfcRecord, "getTnf", "()I");
	jmethod_NfcRecord_getType = env->GetMethodID( jclass_NfcRecord, "getType", "()[B");
	jmethod_NfcRecord_getByteArray = env->GetMethodID( jclass_NfcRecord, "getByteArray", "()[B");
	jmethod_NfcRecord_getSubrecords = env->GetMethodID( jclass_NfcRecord, "getSubrecords", "()Lcom/rhomobile/nfc/NfcMessage;");

	logi("native callback TMP 01 java IDs collected");

	CNFCMessagePack* messagePack = new CNFCMessagePack(env, jo_msgpack);

	logi("native callback TMP 02 C++ containers created");
	
	char url[2048];
	char body[2048];

	const char* jurl = env->GetStringUTFChars(js_callback_url,0);
	strcpy(url, jurl);
	env->ReleaseStringUTFChars(js_callback_url, jurl);

	logi("native callback TMP 03 url prepared");

	strcpy(body, "&rho_callback=1");
	strcat(body, "&");
	strcat(body, (RHODESAPP().addCallbackObject( new RhoCallbackNFCContainer(messagePack), "messages")).c_str());

	logi("native callback TMP 03+1 body prepared");

	rho_net_request_with_data(rho_http_normalizeurl(url), body);

	logi("native callback FINISH");
}



