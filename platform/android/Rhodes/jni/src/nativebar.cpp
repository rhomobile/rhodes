#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeBar"

RHO_GLOBAL void create_nativebar(int bar_type, int nparams, char** params)
{
	JNIEnv *env = jnienv();
	jclass clsVector = getJNIClass(RHODES_JAVA_CLASS_VECTOR);
	if (!clsVector) return;
	jclass clsNativeBar = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
	if (!clsNativeBar) return;
	jmethodID midConstructor = getJNIClassMethod(clsVector, "<init>", "(I)V");
	if (!midConstructor) return;
	jobject vectorObj = env->NewObject(clsVector, midConstructor, nparams);
	if (!vectorObj) return;
	jmethodID midAddElement = getJNIClassMethod(clsVector, "addElement", "(Ljava/lang/Object;)V");
	if (!midAddElement) return;
	jmethodID midCreate = getJNIClassStaticMethod(clsNativeBar, "create", "(ILjava/util/Vector;)V");
	if (!midCreate) return;

	for (int i = 0; i != nparams; ++i) {
		char const *s = params[i] ? params[i] : "";
		jstring strObj = env->NewStringUTF(s);
		env->CallVoidMethod(vectorObj, midAddElement, strObj);
		env->DeleteLocalRef(strObj);
	}

	env->CallStaticVoidMethod(clsNativeBar, midCreate, bar_type, vectorObj);
	env->DeleteLocalRef(vectorObj);
}

RHO_GLOBAL void remove_nativebar()
{
	jclass cls = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
	if (!cls) return;
	jmethodID mid = getJNIClassStaticMethod(cls, "remove", "()V");
	if (!mid) return;

	jnienv()->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void nativebar_switch_tab(int index)
{
	jclass cls = getJNIClass(RHODES_JAVA_CLASS_NATIVEBAR);
	if (!cls) return;
	jmethodID mid = getJNIClassStaticMethod(cls, "switchTab", "(I)V");
	if (!mid) return;

	jnienv()->CallStaticVoidMethod(cls, mid, index);
}
