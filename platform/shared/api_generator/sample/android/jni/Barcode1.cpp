#include "Barcode1.h"

#include "rhodes/JNIRhodes.h"

namespace rhoelements {

const char const * CBarcode1::CLASS = "com/motorolasolutions/rhoelements/Barcode1";

jclass CBarcode1::s_class = NULL;
jmethodID CBarcode::s_midEnumerate;
jmethodID CBarcode::s_midGetDefaultID;
jmethodID CBarcode::s_midSetDefaultID;

//----------------------------------------------------------------------------------------------------------------------
JNIEnv* CBarcode1::jniInit()
{
    JNIEnv *env = jnienv();
    if(!s_class)
    {
        s_class = rho_find_class(env, CLASS);
        if(!s_class)
        {
            RAWLOG_ERROR1("Failed to find java class: %s", CLASS);
            s_class = 0;
            return NULL;
        }
        s_midEnumerate = env->GetStaticMethodID(s_class, "enumerate", "(V)java/util/List;");
        if(!s_midEnumerate)
        {
            RAWLOG_ERROR1("Failed to get method 'enumerate' for java class %s", CLASS);
            s_class = 0;
            return NULL;
        }
        s_midGetDefault = env->GetStaticMethodID(s_class, "getDefault", "()com/rhomobile/rhoelements/IBarcode1;");
        if(!s_midGetDefault)
        {
            RAWLOG_ERROR1("Failed to get method 'getDefault' for java class %s", CLASS);
            s_class = 0;
            return NULL;
        }
        s_midSetDefaultID = env->GetStaticMethodID(s_class, "setDefaultID", "(java/util/String;)V");
        if(!s_midSetDefaultID)
        {
            RAWLOG_ERROR1("Failed to get method 'setDefaultID' for java class %s", CLASS);
            s_class = 0;
            return NULL;
        }
        s_midCreate = env->GetStaticMethodID(s_class, "create", "(java/util/String;)com/rhomobile/rhoelements/IBarcode1;");
        if(!s_midCreate)
        {
            RAWLOG_ERROR1("Failed to get method 'create' for java class %s", CLASS);
            s_class = 0;
            return NULL;
        }
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

jhobject CBarcode1::enumerate()
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject res = env->CallStaticObjectMethod(s_class, s_midEnumerate);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jhobject CBarcode1::getDefault()
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhstring res = env->CallStaticObjectMethod(s_class, s_midGetDefault);
    return res;

}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1::setDefaultID(jhstring& jhId)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    env->CallStaticVoidMethod(s_class, s_midSetDefaultID, jhId.get());

}
//----------------------------------------------------------------------------------------------------------------------

jhobject CBarcode1::create(jhstring& jhId)
{
    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject res = env->CallStaticObjectMethod(s_class, s_midCreate, ghId.get());
}
//----------------------------------------------------------------------------------------------------------------------

}
