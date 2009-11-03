#include <com_rhomobile_rhodes_Platform.h>

JNIEXPORT jstring JNICALL Java_com_rhomobile_rhodes_Platform_getHomeUrlNative
  (JNIEnv *env, jobject)
{
	return env->NewStringUTF("http://127.0.0.1:8080");
}

