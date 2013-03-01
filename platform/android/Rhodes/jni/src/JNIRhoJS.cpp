#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoJS.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JNIRhoJS"

namespace details {

jstring rho_cast_helper<jstring, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return NULL;
}

jobject rho_cast_helper<jobject, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return NULL;
}

jobjectArray rho_cast_helper<jobjectArray, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonArray)
{
    return NULL;
}

jint rho_cast_helper<jint, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return 0;
}

jboolean rho_cast_helper<jboolean, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return false;
}

jdouble rho_cast_helper<jdouble, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return 0.0;
}

}
