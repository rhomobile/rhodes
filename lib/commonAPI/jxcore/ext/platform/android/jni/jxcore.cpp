/*
JXcore Java bindings
The MIT License (MIT)

Copyright (c) 2015 Oguz Bastemur

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define __STDC_LIMIT_MACROS
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "jxcore_droid.h"
#include "JniHelper.h"
#include <android/log.h>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include <assert.h>

#include "logging/RhoLog.h"

extern void store_thr_jnienv(JNIEnv *env);

static JXValue *cb_values = NULL;
static JXValue eventCB = {0};

void ConvertResult(JXValue *result, std::string &to_result) {
  switch (result->type_) {
    case RT_Null:
      to_result = "null";
      break;
    case RT_Undefined:
      to_result = "undefined";
      break;
    case RT_Boolean:
      to_result = JX_GetBoolean(result) ? "true" : "false";
      break;
    case RT_Int32: {
      std::stringstream ss;
      ss << JX_GetInt32(result);
      to_result = ss.str();
    } break;
    case RT_Double: {
      std::stringstream ss;
      ss << JX_GetDouble(result);
      to_result = ss.str();
    } break;
    case RT_Buffer:
    case RT_Object:
    case RT_String:
    case RT_Error: {
      char *chr = JX_GetString(result);
      to_result = chr;
      free(chr);
    } break;
    default:
      to_result = "null";
      return;
  }
}

static void callback(JXValue *results, int argc) {
  if (argc != 3) {
    error_console(
        "wrong callback received. expected number of parameters was 3. "
        "received %d",
        argc);
    return;
  }

  if (!JX_IsString(results + 2)) {
    error_console(
        "JXcore-Cordova: Unexpected callback received. Third parameter must "
        "be a String");
    return;
  }

  cb_values = results;

  // results+1 -> err_val
  jxcore::Callback((JX_IsNull(results + 1) || JX_IsUndefined(results + 1)) ? 0
                                                                           : 1);

  cb_values = NULL;
}

static void callJXcoreNative(JXValue *results, int argc) {
  JNIEnv *env = jxcore::JniHelper::getEnv();

  jclass arrClass = env->FindClass("java/util/ArrayList");
  jmethodID arrInit = env->GetMethodID(arrClass, "<init>", "()V");
  jobject objArray = env->NewObject(arrClass, arrInit);
  jmethodID addObject =
      env->GetMethodID(arrClass, "add", "(Ljava/lang/Object;)Z");

  jclass boolClass = NULL;
  jmethodID boolMethod;

  jclass intClass = NULL;
  jmethodID intMethod;

  jclass doubleClass = NULL;
  jmethodID doubleMethod;

  jclass strClass = env->FindClass("java/lang/String");

  for (int i = 0; i < argc; i++) {
    JXValue *result = results + i;
    jobject objValue = NULL;

    switch (result->type_) {
      case RT_Boolean: {
        if (boolClass == NULL) {
          boolClass = env->FindClass("java/lang/Boolean");
          boolMethod = env->GetMethodID(boolClass, "<init>", "(Z)V");
        }
        jboolean bl = JX_GetBoolean(result) ? JNI_TRUE : JNI_FALSE;
        objValue = env->NewObject(boolClass, boolMethod, bl);
      } break;
      case RT_Int32: {
        if (intClass == NULL) {
          intClass = env->FindClass("java/lang/Integer");
          intMethod = env->GetMethodID(intClass, "<init>", "(I)V");
        }

        jint nt = JX_GetInt32(result);
        objValue = env->NewObject(intClass, intMethod, nt);
      } break;
      case RT_Double: {
        if (doubleClass == NULL) {
          doubleClass = env->FindClass("java/lang/Double");
          doubleMethod = env->GetMethodID(doubleClass, "<init>", "(D)V");
        }
        jdouble db = JX_GetDouble(result);
        objValue = env->NewObject(doubleClass, doubleMethod, db);
      } break;
      case RT_Buffer: {
        char *arr = JX_GetString(result);
        int length = JX_GetDataLength(result);

        jbyteArray ret = env->NewByteArray(length);
        env->SetByteArrayRegion(ret, 0, length, (jbyte *)arr);
        objValue = (jobject)ret;
        free(arr);
      } break;
      case RT_Object: {
        std::string str_result;
        ConvertResult(result, str_result);
        const char *data = str_result.c_str();
        int ln = JX_GetDataLength(result);
        if (ln > 0 && *data != '{' && *data != '[') {
          objValue = (jobject)env->NewStringUTF(str_result.c_str());
        } else {
          jobjectArray ret = (jobjectArray)env->NewObjectArray(
              1, strClass, env->NewStringUTF(""));

          jstring jstr = env->NewStringUTF(str_result.c_str());
          env->SetObjectArrayElement(ret, 0, jstr);

          objValue = (jobject)ret;
          env->DeleteLocalRef(jstr);
        }
      } break;
      case RT_Error:
      case RT_String: {
        std::string str_result;
        ConvertResult(result, str_result);

        objValue = env->NewStringUTF(str_result.c_str());
      } break;
      default:
        break;
    }

    env->CallBooleanMethod(objArray, addObject, objValue);
    env->DeleteLocalRef(objValue);
  }

  jxcore::CallJava(objArray);

  env->DeleteLocalRef(objArray);
  env->DeleteLocalRef(arrClass);
  env->DeleteLocalRef(strClass);

  if (doubleClass != NULL)
    env->DeleteLocalRef(doubleClass);

  if (intClass != NULL)
    env->DeleteLocalRef(intClass);

  if (boolClass != NULL)
    env->DeleteLocalRef(boolClass);
}

AAssetManager *assetManager;
static void assetExistsSync(JXValue *results, int argc) {
  char *filename = JX_GetString(&results[0]);

  bool found = false;
  AAsset *asset =
      AAssetManager_open(assetManager, filename, AASSET_MODE_UNKNOWN);
  if (asset) {
    found = true;
    AAsset_close(asset);
  }



RAWTRACE2( ">>>>>>>>>>>>>>>>>>>>>>>> assetExistsSync: %s, found: %d", filename, found );



  JX_SetBoolean(&results[argc], found);
  free(filename);
}

static void assetReadSync(JXValue *results, int argc) {
  char *filename = JX_GetString(&results[0]);

RAWTRACE1( ">>>>>>>>>>>>>>>>>>>>>>>> assetReadSync: %s", filename );


  AAsset *asset =
      AAssetManager_open(assetManager, filename, AASSET_MODE_UNKNOWN);

  free(filename);
  if (asset) {
    off_t fileSize = AAsset_getLength(asset);
    void *data = malloc(fileSize);
    int read_len = AAsset_read(asset, data, fileSize);

RAWTRACE3( ">>>>>>>>>>>>>>>>>>>>>>>> assetReadSync: %s, len: %d, data: %s", 
  filename, read_len, (char*)data );


    JX_SetBuffer(&results[argc], (char *)data, read_len);
    free(data);
    AAsset_close(asset);
    return;
  }

  const char *err = "File doesn't exist";
  JX_SetError(&results[argc], err, strlen(err));
}

static void defineEventCB(JXValue *results, int argc) {
  if (!JX_IsFunction(results + 1)) {
    error_console("defineEventCB expects a function");
    return;
  }

  JX_MakePersistent(results+1);
  eventCB = *(results + 1);
}

std::string files_json;
static void assetReadDirSync(JXValue *results, int argc) {

RAWTRACE1( ">>>>>>>>>>>>>>>>>>>>>>>> assetReadDirSync: %s", files_json.c_str() );


  JX_SetJSON(&results[argc], files_json.c_str(), files_json.length());
}

extern "C" {

//jint JNI_OnLoad(JavaVM *vm, void *reserved) { return jxcore::Initialize(vm); }

JNIEXPORT jlong JNICALL
Java_io_jxcore_node_jxcore_callCBString(JNIEnv *env, jobject thiz,
                                        jstring ev_name, jstring param,
                                        jint json) {

  if (eventCB.type_ == 0) {
    error_console("event callback is not ready yet.");
    return 0;
  }

  const char *str_ev = env->GetStringUTFChars(ev_name, 0);
  const char *str_param = env->GetStringUTFChars(param, 0);

  long ret_val = 0;
  JXValue args[2];
  if (JX_CreateEmptyObject(&args[1])) {
    JXValue jx_str_param;
    JX_New(&args[0]);
    JX_New(&jx_str_param);

    JX_SetString(&args[0], str_ev, 0);
    JX_SetString(&jx_str_param, str_param, 0);

    if (json != 1)
      JX_SetNamedProperty(&args[1], "str", &jx_str_param);
    else
      JX_SetNamedProperty(&args[1], "json", &jx_str_param);

    JX_Free(&jx_str_param);

    JXValue out;
    JX_CallFunction(&eventCB, args, 2, &out);

    JX_Free(&args[0]);
    JX_Free(&args[1]);

    if (!JX_IsNull(&out) && !JX_IsUndefined(&out))
      ret_val = JX_StoreValue(&out);
    else
      ret_val = -1;
  } else {
    error_console("couldn't create JXValue Object");
  }

  env->ReleaseStringUTFChars(ev_name, str_ev);
  env->ReleaseStringUTFChars(param, str_param);

  return ret_val;
}

JNIEXPORT jlong JNICALL
Java_io_jxcore_node_jxcore_callCBArray(JNIEnv *env, jobject thiz,
                                       jstring ev_name, jobjectArray params,
                                       jint size) {
  if (eventCB.type_ == 0) {
    error_console("event callback is not ready yet.");
    return 0;
  }

  const char *str_ev = env->GetStringUTFChars(ev_name, 0);

  long ret_val = 0;
  JXValue args[2];
  if (JX_CreateArrayObject(&args[1])) {
    JX_New(&args[0]);

    JX_SetString(&args[0], str_ev, 0);

    jclass boolClass = env->FindClass("java/lang/Boolean");
    jclass doubleClass = env->FindClass("java/lang/Double");
    jclass intClass = env->FindClass("java/lang/Integer");
    jclass strClass = env->FindClass("java/lang/String");
    jclass barrClass = env->FindClass("[B");

    for (int i = 0; i < (int)size; i++) {
      jobject elm = (jobject)env->GetObjectArrayElement(params, i);

      JXValue val;
      JX_New(&val);
      if (elm == NULL) {
        JX_SetNull(&val);
      } else if (env->IsInstanceOf(elm, boolClass) == JNI_TRUE) {
        jmethodID bvalID = env->GetMethodID(boolClass, "booleanValue", "()Z");
        bool nval = (bool)env->CallBooleanMethod(elm, bvalID);
        JX_SetBoolean(&val, nval);
      } else if (env->IsInstanceOf(elm, intClass) == JNI_TRUE) {
        jmethodID bvalID = env->GetMethodID(intClass, "intValue", "()I");
        int nval = (int)env->CallIntMethod(elm, bvalID);
        JX_SetInt32(&val, nval);
      } else if (env->IsInstanceOf(elm, doubleClass) == JNI_TRUE) {
        jmethodID bvalID = env->GetMethodID(doubleClass, "doubleValue", "()D");
        double nval = (double)env->CallDoubleMethod(elm, bvalID);
        JX_SetDouble(&val, nval);
      } else if (env->IsInstanceOf(elm, strClass) == JNI_TRUE) {
        jstring jstr = (jstring)elm;
        const char *str = env->GetStringUTFChars(jstr, 0);
        JX_SetString(&val, str, strlen(str));
        env->ReleaseStringUTFChars(jstr, str);
      } else if (env->IsInstanceOf(elm, barrClass) == JNI_TRUE) {
        jbyteArray jarr = (jbyteArray)elm;
        int len = env->GetArrayLength(jarr);
        jbyte *barr = env->GetByteArrayElements(jarr, 0);
        JX_SetBuffer(&val, (char *)barr, len);

        env->ReleaseByteArrayElements(jarr, barr, JNI_ABORT);
      }

      JX_SetIndexedProperty(&args[1], i, &val);
      JX_Free(&val);

      env->DeleteLocalRef(elm);
    }

    env->DeleteLocalRef(boolClass);
    env->DeleteLocalRef(doubleClass);
    env->DeleteLocalRef(intClass);
    env->DeleteLocalRef(strClass);
    env->DeleteLocalRef(barrClass);

    JXValue out;
    JX_CallFunction(&eventCB, args, 2, &out);

    JX_Free(&args[0]);
    JX_Free(&args[1]);

    if (!JX_IsNull(&out) && !JX_IsUndefined(&out))
      ret_val = JX_StoreValue(&out);
    else
      ret_val = -1;
  } else {
    error_console("couldn't create JXValue Object");
  }

  env->ReleaseStringUTFChars(ev_name, str_ev);

  return ret_val;
}

JNIEXPORT void JNICALL
Java_io_jxcore_node_jxcore_prepareEngine(JNIEnv *env, jobject thiz,
                                         jstring home, jstring files) {

  static bool initialized = false;

  const char *hfiles = env->GetStringUTFChars(files, 0);
  const char *hfolder = env->GetStringUTFChars(home, 0);

  if (!initialized) {  // silly but does the job
    initialized = true;
    JX_Initialize(hfolder, callback);
  }

  JX_InitializeNewEngine();
  files_json = hfiles;

  env->ReleaseStringUTFChars(home, hfolder);
  env->ReleaseStringUTFChars(files, hfiles);

  JX_DefineExtension("assetExistsSync", assetExistsSync);
  JX_DefineExtension("assetReadSync", assetReadSync);
  JX_DefineExtension("assetReadDirSync", assetReadDirSync);
  JX_DefineExtension("defineEventCB", defineEventCB);
  JX_DefineExtension("callJXcoreNative", callJXcoreNative);
}

JNIEXPORT jlong JNICALL
Java_io_jxcore_node_jxcore_evalEngine(JNIEnv *env, jobject thiz,
                                      jstring contents) {
  const char *data = env->GetStringUTFChars(contents, 0);

  JXValue result;
  JX_Evaluate(data, 0, &result);

  env->ReleaseStringUTFChars(contents, data);

  if (!JX_IsNull(&result) && !JX_IsUndefined(&result))
    return JX_StoreValue(&result);
  else
    return -1;
}

JNIEXPORT jstring JNICALL
Java_io_jxcore_node_jxcore_convertToString(JNIEnv *env, jobject thiz,
                                           jlong id) {
  JXValue *val;
  if (id < 0)
    val = cb_values + (id + 3);
  else
    val = JX_RemoveStoredValue(0, id);

  std::string str_result;
  ConvertResult(val, str_result);
  if (id >= 0) JX_Free(val);

  return env->NewStringUTF(str_result.c_str());
}

JNIEXPORT jint JNICALL
Java_io_jxcore_node_jxcore_getType(JNIEnv *env, jobject thiz, jlong id) {
  if (id < 0 && cb_values != NULL) return cb_values[id + 3].type_;

  if (id < 0) return RT_Undefined;

  return JX_GetStoredValueType(0, id);
}

JNIEXPORT jint JNICALL
Java_io_jxcore_node_jxcore_getInt32(JNIEnv *env, jobject thiz, jlong id) {
  if (id < 0) return JX_GetInt32(cb_values + (id + 3));

  JXValue *val = JX_RemoveStoredValue(0, id);
  int n = JX_GetInt32(val);
  JX_Free(val);
  return n;
}

JNIEXPORT jdouble JNICALL
Java_io_jxcore_node_jxcore_getDouble(JNIEnv *env, jobject thiz, jlong id) {
  if (id < 0) return JX_GetDouble(cb_values + (id + 3));

  JXValue *val = JX_RemoveStoredValue(0, id);
  int n = JX_GetDouble(val);
  JX_Free(val);
  return n;
}

JNIEXPORT jint JNICALL
Java_io_jxcore_node_jxcore_getBoolean(JNIEnv *env, jobject thiz, jlong id) {
  if (id < 0) return JX_GetBoolean(cb_values + (id + 3)) ? 1 : 0;

  JXValue *val = JX_RemoveStoredValue(0, id);
  int n = JX_GetBoolean(val) ? 1 : 0;
  JX_Free(val);
  return n;
}

JNIEXPORT jstring JNICALL
Java_io_jxcore_node_jxcore_getString(JNIEnv *env, jobject thiz, jlong id) {
  JXValue *val;
  if (id < 0)
    val = cb_values + (id + 3);
  else
    val = JX_RemoveStoredValue(0, id);

  char *chrp = JX_GetString(val);
  if (id >= 0) JX_Free(val);

  jstring js = env->NewStringUTF(chrp);
  free(chrp);

  return js;
}

JNIEXPORT jbyteArray JNICALL
Java_io_jxcore_node_jxcore_getBuffer(JNIEnv *env, jobject thiz, jlong id) {
  JXValue *val;
  if (id < 0)
    val = cb_values + (id + 3);
  else
    val = JX_RemoveStoredValue(0, id);

  char *arr = JX_GetString(val);
  int length = JX_GetDataLength(val);

  jbyteArray ret = env->NewByteArray(length);
  env->SetByteArrayRegion(ret, 0, length, (jbyte *)arr);
  free(arr);
  if (id >= 0) JX_Free(val);
  return ret;
}

JNIEXPORT void JNICALL
Java_io_jxcore_node_jxcore_defineMainFile(JNIEnv *env, jobject obj,
                                          jstring contents) {
  const char *data = env->GetStringUTFChars(contents, 0);
  JX_DefineMainFile(data);
  env->ReleaseStringUTFChars(contents, data);
}

JNIEXPORT void JNICALL
Java_io_jxcore_node_jxcore_defineFile(JNIEnv *env, jobject obj,
                                      jstring filename, jstring data) {
  const char *name = env->GetStringUTFChars(filename, 0);
  const char *file = env->GetStringUTFChars(data, 0);

  JX_DefineFile(name, file);

  env->ReleaseStringUTFChars(filename, name);
  env->ReleaseStringUTFChars(data, file);
}

JNIEXPORT void JNICALL
Java_io_jxcore_node_jxcore_startEngine(JNIEnv *env, jobject thiz) {
  JX_StartEngine();
}

JNIEXPORT jint JNICALL
Java_io_jxcore_node_jxcore_loopOnce(JNIEnv *env, jobject thiz) {
  return JX_LoopOnce();
}

JNIEXPORT void JNICALL
Java_io_jxcore_node_jxcore_stopEngine(JNIEnv *env, jobject thiz) {
  JX_ClearPersistent(&eventCB);
  JX_Free(&eventCB);
  JX_StopEngine();
}

JNIEXPORT void JNICALL
Java_io_jxcore_node_jxcore_setNativeContext(JNIEnv *env, jobject thiz,
                                            jobject context,
                                            jobject j_assetManager) {

  store_thr_jnienv(env);

  JavaVM* vm = 0;
  env->GetJavaVM(&vm);
  jxcore::Initialize(vm);

  jxcore::JniHelper::setClassLoaderFrom(context);
  assetManager = AAssetManager_fromJava(env, j_assetManager);
}
}