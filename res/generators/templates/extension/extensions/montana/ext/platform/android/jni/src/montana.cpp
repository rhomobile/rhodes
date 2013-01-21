#include <rhodes.h>
#include "rubyext/WebView.h"
#include <stdlib.h>
#include "ruby/ext/rho/rhoruby.h"



extern "C" VALUE <%= name.downcase %>_native_process_string(const char* str) {

    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/<%= name.downcase %>/<%= name.camel_case %>");
    if (!cls) return rho_ruby_get_NIL();;
    jmethodID mid = env->GetStaticMethodID( cls, "processString", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!mid) return rho_ruby_get_NIL();;
    jstring objStr = env->NewStringUTF(str);
    jstring jstr = (jstring)env->CallStaticObjectMethod(cls, mid, objStr);
    env->DeleteLocalRef(objStr);
    const char* buf = env->GetStringUTFChars(jstr,0);
    VALUE result = rho_ruby_create_string(buf);
    env->ReleaseStringUTFChars(jstr, buf);
    return result;
}


extern "C" int <%= name.downcase %>_calc_summ(int x, int y) {
	return (x+y);
}

