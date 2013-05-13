#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoJS.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "JNIRhoJS"

namespace details {

using rho::json::CJSONArray;
using rho::json::CJSONStructIterator;

//----------------------------------------------------------------------------------------------------------------------

jstring rho_cast_helper<jstring, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return rho_cast<jstring>(env, jsonEntry.getString());
}
//----------------------------------------------------------------------------------------------------------------------

jobject rho_cast_helper<jobject, CJSONEntry>::convertJsonEntryToJavaCollection(const CJSONEntry& jsonEntry)
{
    jobject jList = m_env->NewObject(clsArrayList, midArrayList);
    if(!jList)
    {
        return 0;
    }
    CJSONArray jsonArray(jsonEntry);
    int size = jsonArray.getSize();

    for(int i = 0; i < size; ++i)
    {
        jhstring jhElement = rho_cast<jstring>(m_env, jsonArray.getItem(i).getString());
        m_env->CallBooleanMethod(jList, RhoJniConvertor::midArrayListAdd, jhElement.get());
    }

    return jList;
}
//----------------------------------------------------------------------------------------------------------------------

jobject rho_cast_helper<jobject, CJSONEntry>::convertJsonEntryToJavaMap(const CJSONEntry& jsonEntry)
{
    jobject jMap = m_env->NewObject(clsHashMap, midHashMap);
    if(!jMap)
    {
        return 0;
    }

    CJSONStructIterator jsonObjectIt(jsonEntry);
    for( ; !jsonObjectIt.isEnd(); jsonObjectIt.next() )
    {
        jhstring jhKey = rho_cast<jstring>(m_env, jsonObjectIt.getCurKey());
        jhstring jhVal = rho_cast<jstring>(m_env, jsonObjectIt.getCurString());

        jhobject jhPrev = m_env->CallObjectMethod(jMap, RhoJniConvertor::midHashMapPut, jhKey.get(), jhVal.get());
    }

    return jMap;

}
//----------------------------------------------------------------------------------------------------------------------

jobject rho_cast_helper<jobject, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    if(jsonEntry.isEmpty() || jsonEntry.isNull()) {
        return 0;
    }
    if (!initConvertor(env))
    {
        return 0;
    }
    if(jsonEntry.isString())
    {
        RAWTRACE("Convert to String object");
        return rho_cast<jstring>(env, jsonEntry.getString());
    }
    if(jsonEntry.isArray())
    {
        RAWTRACE("Convert to Collection object");
        return convertJsonEntryToJavaCollection(jsonEntry);
    }
    if(jsonEntry.isObject())
    {
        RAWTRACE("Convert to Map object");
        return convertJsonEntryToJavaMap(jsonEntry);
    }
    if(jsonEntry.isBoolean())
    {
        RAWTRACE("Convert to Boolean object");
        return RhoJniConvertor::getBooleanObject(jsonEntry.getBoolean());
    }
    if(jsonEntry.isInteger())
    {
        RAWTRACE("Convert to Integer object");
        return RhoJniConvertor::getIntegerObject(jsonEntry.getInt());
    }
    if(jsonEntry.isFloat())
    {
        RAWTRACE("Convert to Double object");
        return RhoJniConvertor::getDoubleObject(jsonEntry.getDouble());
    }

    RAWTRACE("Convert to null");
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------

jobjectArray rho_cast_helper<jobjectArray, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    if (!jsonEntry.isArray())
    {
        return 0;
    }
    if (!initConvertor(env))
    {
        return 0;
    }

    CJSONArray jsonArray(jsonEntry);
    int size = jsonArray.getSize();
    jobjectArray jArray = env->NewObjectArray(size, clsString, 0);

    for(int i = 0; i < size; ++i)
    {
        jhstring jhElement = rho_cast<jstring>(env, jsonArray.getItem(i).getString());
        env->SetObjectArrayElement(jArray, i, jhElement.get());
    }
    return jArray;
}
//----------------------------------------------------------------------------------------------------------------------

jint rho_cast_helper<jint, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return static_cast<jint>(jsonEntry.getInt());
}
//----------------------------------------------------------------------------------------------------------------------

jboolean rho_cast_helper<jboolean, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return static_cast<jboolean>(jsonEntry.getBoolean());
}
//----------------------------------------------------------------------------------------------------------------------

jdouble rho_cast_helper<jdouble, CJSONEntry>::operator ()(JNIEnv *env, const CJSONEntry& jsonEntry)
{
    return static_cast<jdouble>(jsonEntry.getDouble());
}
//----------------------------------------------------------------------------------------------------------------------

}
