#include "Barcode1.h"

#include "MethodResultJni.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Barcode1JNI"


namespace rhoelements {

//Barcode1FactorySingleton staff
const char* const CBarcode1Base::FACTORY_SINGLETON_CLASS = "com.rho.rhoelements.barcode1.Barcode1FactorySingleton";
jclass CBarcode1Base::s_clsFactorySingleton = 0;
jmethodID CBarcode1Base::s_midFactorySetInstance;
jmethodID CBarcode1Base::s_midFactoryGetInstance;

//Barcode1Factory staff
const char* const CBarcode1Base::IFACTORY_CLASS = "com.rho.rhoelements.barcode1.IBarcode1Factory";
jclass CBarcode1Base::s_clsIFactory = 0;
jmethodID CBarcode1Base::s_midGetApiSingleton;
jmethodID CBarcode1Base::s_midGetApiObject;

//Barcode1SingletonBase staff
//const char* const CBarcode1Base::SINGLETON_BASE_CLASS = "com.rho.rhoelements.barcode1.Barcode1SingletonBase";
//jclass CBarcode1Base::s_clsSingletonBase = 0;

//Barcode1Base staff
const char* const CBarcode1Base::OBJECT_BASE_CLASS = "com.rho.rhoelements.barcode1.Barcode1Base";
const char* const CBarcode1Base::GETPROPS_TASK_CLASS = "com.rho.rhoelements.barcode1.Barcode1Base$GetPropsTask";
const char* const CBarcode1Base::GETPROPS1_TASK_CLASS = "com.rho.rhoelements.barcode1.Barcode1Base$GetProps1Task";
const char* const CBarcode1Base::GETPROPS2_TASK_CLASS = "com.rho.rhoelements.barcode1.Barcode1Base$GetProps2Task";
const char* const CBarcode1Base::TAKEBARCODE_TASK_CLASS = "com.rho.rhoelements.barcode1.Barcode1Base$TakeBarcodeTask";
jclass CBarcode1Base::s_clsObjectBase = 0;
jclass CBarcode1Base::s_clsGetPropsTask = 0;
jclass CBarcode1Base::s_clsGetProps1Task = 0;
jclass CBarcode1Base::s_clsGetProps2Task = 0;
jclass CBarcode1Base::s_clsTakeBarcodeTask = 0;
jmethodID CBarcode1Base::s_midGetPropsTask;
jmethodID CBarcode1Base::s_midGetProps1Task;
jmethodID CBarcode1Base::s_midGetProps2Task;
jmethodID CBarcode1Base::s_midTakeBarcodeTask;

//Barcode1Singleton staff
const char* const CBarcode1Base::ISINGLETON_CLASS = "com.rho.rhoelements.IRhoApiSingleton";
jclass CBarcode1Base::s_clsISingleton = 0;
jmethodID CBarcode1Base::s_midEnumerate;
jmethodID CBarcode1Base::s_midGetDefaultID;
jmethodID CBarcode1Base::s_midSetDefaultID;

//Barcode1 staff
//const char* const CBarcode1Base::IOBJECT_CLASS = "com.rho.rhoelements.barcode1.IBarcode1";

//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CBarcode1Base::jniInit()
{
    JNIEnv *env = jnienv();
    if(!env)
    {
        RAWLOG_ERROR("JNI init failed: JNIEnv is null");
        return 0;
    }
    return jniInit(env);
}
//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CBarcode1Base::jniInit(JNIEnv* env)
{
    static bool initialized = false;
    env = MethodExecutorJni::jniInit(env);
    if (!env) {
        RAWLOG_ERROR("JNI init failed");
        return 0;
    }

    if(!initialized)
    {
        //init Barcode1FactorySingleton JNI
        s_clsFactorySingleton = loadClass(env, FACTORY_SINGLETON_CLASS);
        if (!s_clsFactorySingleton) return 0;

        s_midFactorySetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "setInstance", "(Lcom/rho/rhoelements/barcode1/IBarcode1Factory;)V");
        if(!s_midFactorySetInstance)
        {
            RAWLOG_ERROR1("Failed to get method 'setInstance' for java class %s", FACTORY_SINGLETON_CLASS);
            return NULL;
        }
        s_midFactoryGetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "getInstance", "()Lcom/rho/rhoelements/barcode1/IBarcode1Factory;");
        if(!s_midFactoryGetInstance)
        {
            RAWLOG_ERROR1("Failed to get method 'getInstance' for java class %s", FACTORY_SINGLETON_CLASS);
            return NULL;
        }

        //init IBarcode1Factory JNI
        s_clsIFactory = loadClass(env, IFACTORY_CLASS);
        if (!s_clsIFactory) return 0;
        s_midGetApiSingleton = env->GetMethodID(s_clsIFactory, "getApiSingleton", "()Lcom/rho/rhoelements/barcode1/IBarcode1Singleton;");
        if(!s_midGetApiSingleton)
        {
            RAWLOG_ERROR1("Failed to get method 'getApiSingleton' for java class %s", IFACTORY_CLASS);
            return NULL;
        }
        s_midGetApiObject = env->GetMethodID(s_clsIFactory, "getApiObject", "(Ljava/lang/String;)Lcom/rho/rhoelements/barcode1/IBarcode1;");
        if(!s_midGetApiObject)
        {
            RAWLOG_ERROR1("Failed to get method 'getApiObject' for java class %s", IFACTORY_CLASS);
            return NULL;
        }

//        s_clsSingletonBase = loadClass(env, SINGLETON_BASE_CLASS);
//        if (!s_clsSingletonBase) return 0;

        s_clsObjectBase = loadClass(env, OBJECT_BASE_CLASS);
        if (!s_clsObjectBase) return 0;
        s_clsGetPropsTask = loadClass(env, GETPROPS_TASK_CLASS);
        if (!s_clsGetPropsTask) return 0;
        s_clsGetProps1Task = loadClass(env, GETPROPS1_TASK_CLASS);
        if (!s_clsGetProps1Task) return 0;
        s_clsGetProps2Task = loadClass(env, GETPROPS2_TASK_CLASS);
        if (!s_clsGetProps2Task) return 0;
        s_clsTakeBarcodeTask = loadClass(env, TAKEBARCODE_TASK_CLASS);
        if (!s_clsTakeBarcodeTask) return 0;
        s_midGetPropsTask = env->GetMethodID(s_clsGetPropsTask, "<init>",
                        "(Lcom/rho/rhoelements/barcode1/IBarcode1;Lcom/rho/rhoelements/IMethodResult;)V");
        if(!s_midGetPropsTask)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS_TASK_CLASS);
            return NULL;
        }
        s_midGetProps1Task = env->GetMethodID(s_clsGetProps1Task, "<init>",
                        "(Lcom/rho/rhoelements/barcode1/IBarcode1;Ljava/lang/String;Lcom/rho/rhoelements/IMethodResult;)V");
        if(!s_midGetProps1Task)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS1_TASK_CLASS);
            return NULL;
        }
        s_midGetProps2Task = env->GetMethodID(s_clsGetProps2Task, "<init>",
                        "(Lcom/rho/rhoelements/barcode1/IBarcode1;Ljava/util/List;Lcom/rho/rhoelements/IMethodResult;)V");
        if(!s_midGetProps2Task)
        {
            RAWLOG_ERROR1("Failed to get constructor for java class %s", GETPROPS2_TASK_CLASS);
            return NULL;
        }

        s_clsISingleton = loadClass(env, ISINGLETON_CLASS);
        if (!s_clsISingleton) return 0;
        s_midEnumerate = env->GetMethodID(s_clsISingleton, "enumerate", "(Lcom/rho/rhoelements/IMethodResult;)V");
        if(!s_midEnumerate)
        {
            RAWLOG_ERROR1("Failed to get method 'enumerate' for java class %s", ISINGLETON_CLASS);
            return NULL;
        }
        s_midGetDefaultID = env->GetMethodID(s_clsISingleton, "getDefaultID", "()Ljava/lang/String;");
        if(!s_midGetDefaultID)
        {
            RAWLOG_ERROR1("Failed to get method 'getDefaultID' for java class %s", ISINGLETON_CLASS);
            return NULL;
        }
        s_midSetDefaultID = env->GetMethodID(s_clsISingleton, "setDefaultID", "(Ljava/lang/String;)V");
        if(!s_midSetDefaultID)
        {
            RAWLOG_ERROR1("Failed to get method 'setDefaultID' for java class %s", ISINGLETON_CLASS);
            return NULL;
        }

        initialized = true;
        RAWTRACE("CBarcode1 JNI init succeeded");
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1Base::setJavaFactory(JNIEnv* env, jobject jFactory)
{
    RAWTRACE("setJavaFactory");

    env = jniInit(env);
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    RAWTRACE1("s_clsFactorySingleton: 0x%.8X ------------------------------------", s_clsFactorySingleton);

    env->CallStaticVoidMethod(s_clsFactorySingleton, s_midFactorySetInstance, jFactory);

    RAWTRACE("setJavaFactory succeeded");
}
//----------------------------------------------------------------------------------------------------------------------

rho::String CBarcode1Base::getDefaultID()
{
    RAWTRACE("getDefaultID");

    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return rho::String();
    }

    jhobject jhSingleton = getSingleton(env);
    jhstring res = static_cast<jstring>(env->CallObjectMethod(jhSingleton.get(), s_midGetDefaultID));
    return rho_cast<rho::String>(env, res);
}
//----------------------------------------------------------------------------------------------------------------------

void CBarcode1Base::setDefaultID(const rho::String& id)
{
    RAWTRACE1("setDefaultID(id = \"%s\")", id.c_str());

    JNIEnv *env = jniInit();
    if (!env) {
        RAWLOG_ERROR("JNI initialization failed");
        return;
    }

    jhobject instance = getSingleton(env);
    jhstring jhId = rho_cast<jstring>(env, id);
    env->CallVoidMethod(instance.get(), s_midSetDefaultID, jhId.get());
}
//----------------------------------------------------------------------------------------------------------------------

jobject CBarcode1Base::getFactory(JNIEnv* env)
{
    jobject res = env->CallStaticObjectMethod(s_clsFactorySingleton, s_midFactoryGetInstance);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CBarcode1Base::getSingleton(JNIEnv* env)
{
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiSingleton);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CBarcode1Base::getObject(JNIEnv* env)
{
    jhstring jhId = rho_cast<jstring>(env, m_id);
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiObject, jhId.get());
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

}

