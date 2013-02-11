#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoJS.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JNIRhoJS"

namespace details {

jstring rho_cast_helper<jstring, CJSONEntry>::operator ()(JNIEnv *env, CJSONEntry jsonEntry)
{
    return NULL;
}

    jobject rho_cast_helper<jobject, CJSONEntry>::operator ()(JNIEnv *env, CJSONEntry jsonEntry)
{
    return NULL;
}

jobjectArray rho_cast_helper<jobjectArray, CJSONArray>::operator ()(JNIEnv *env, const CJSONArray& jsonArray)
{
    return NULL;
}

}
