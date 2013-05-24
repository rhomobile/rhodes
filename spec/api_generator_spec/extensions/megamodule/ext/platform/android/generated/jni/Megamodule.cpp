#include "Megamodule.h"


namespace rho {
namespace examples {

IMPLEMENT_LOGCLASS(CMegamoduleBase, "MegamoduleJNI");

//MegamoduleFactorySingleton staff
const char* const CMegamoduleBase::FACTORY_SINGLETON_CLASS = "com.rho.examples.megamodule.MegamoduleFactorySingleton";
jclass CMegamoduleBase::s_clsFactorySingleton = 0;
jmethodID CMegamoduleBase::s_midFactorySetInstance;
jmethodID CMegamoduleBase::s_midFactoryGetInstance;

//MegamoduleFactory staff
const char* const CMegamoduleBase::IFACTORY_CLASS = "com.rho.examples.megamodule.IMegamoduleFactory";
jclass CMegamoduleBase::s_clsIFactory = 0;
jmethodID CMegamoduleBase::s_midGetApiSingleton;
jmethodID CMegamoduleBase::s_midGetApiObject;

//MegamoduleSingletonBase staff
const char* const CMegamoduleBase::SINGLETON_BASE_CLASS = "com.rho.examples.megamodule.MegamoduleSingletonBase";
jclass CMegamoduleBase::s_clsSingletonBase = 0;

//MegamoduleBase staff
const char* const CMegamoduleBase::OBJECT_BASE_CLASS = "com.rho.examples.megamodule.MegamoduleBase";
jclass CMegamoduleBase::s_clsObjectBase = 0;


//IRhoApiDefaultId staff
const char* const CMegamoduleBase::IDEFAULTID_CLASS = "com.rhomobile.rhodes.api.IRhoApiDefaultId";
jclass CMegamoduleBase::s_clsIDefaultId = 0;
jmethodID CMegamoduleBase::s_midGetDefaultID;
jmethodID CMegamoduleBase::s_midSetDefaultID;


//Method tasks

const char* const CMegamoduleBase::GETSTRINGPROPERTY_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$getStringPropertyTask";

jclass CMegamoduleBase::s_clsgetStringPropertyTask = 0;
jmethodID CMegamoduleBase::s_midgetStringPropertyTask;

const char* const CMegamoduleBase::GETINTEGERPROPERTY_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$getIntegerPropertyTask";

jclass CMegamoduleBase::s_clsgetIntegerPropertyTask = 0;
jmethodID CMegamoduleBase::s_midgetIntegerPropertyTask;

const char* const CMegamoduleBase::SETINTEGERPROPERTY_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$setIntegerPropertyTask";

jclass CMegamoduleBase::s_clssetIntegerPropertyTask = 0;
jmethodID CMegamoduleBase::s_midsetIntegerPropertyTask;

const char* const CMegamoduleBase::TYPESTEST_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$typesTestTask";

jclass CMegamoduleBase::s_clstypesTestTask = 0;
jmethodID CMegamoduleBase::s_midtypesTestTask;

const char* const CMegamoduleBase::PRODUCEARRAY_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$produceArrayTask";

jclass CMegamoduleBase::s_clsproduceArrayTask = 0;
jmethodID CMegamoduleBase::s_midproduceArrayTask;

const char* const CMegamoduleBase::PRODUCEHASH_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$produceHashTask";

jclass CMegamoduleBase::s_clsproduceHashTask = 0;
jmethodID CMegamoduleBase::s_midproduceHashTask;

const char* const CMegamoduleBase::PRODUCECOMPLICATEDRESULT_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$produceComplicatedResultTask";

jclass CMegamoduleBase::s_clsproduceComplicatedResultTask = 0;
jmethodID CMegamoduleBase::s_midproduceComplicatedResultTask;

const char* const CMegamoduleBase::GETOBJECTSCOUNT_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleSingletonBase$getObjectsCountTask";

jclass CMegamoduleBase::s_clsgetObjectsCountTask = 0;
jmethodID CMegamoduleBase::s_midgetObjectsCountTask;

const char* const CMegamoduleBase::GETOBJECTBYINDEX_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleSingletonBase$getObjectByIndexTask";

jclass CMegamoduleBase::s_clsgetObjectByIndexTask = 0;
jmethodID CMegamoduleBase::s_midgetObjectByIndexTask;

const char* const CMegamoduleBase::SHOWALERTFROMUITHREAD_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$showAlertFromUIThreadTask";

jclass CMegamoduleBase::s_clsshowAlertFromUIThreadTask = 0;
jmethodID CMegamoduleBase::s_midshowAlertFromUIThreadTask;

const char* const CMegamoduleBase::SETPERIODICALLYCALLBACK_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$setPeriodicallyCallbackTask";

jclass CMegamoduleBase::s_clssetPeriodicallyCallbackTask = 0;
jmethodID CMegamoduleBase::s_midsetPeriodicallyCallbackTask;

const char* const CMegamoduleBase::ISPERIODICALLYCALLBACKSETTED_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$isPeriodicallyCallbackSettedTask";

jclass CMegamoduleBase::s_clsisPeriodicallyCallbackSettedTask = 0;
jmethodID CMegamoduleBase::s_midisPeriodicallyCallbackSettedTask;

const char* const CMegamoduleBase::STOPPERIODICALLYCALLBACK_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$stopPeriodicallyCallbackTask";

jclass CMegamoduleBase::s_clsstopPeriodicallyCallbackTask = 0;
jmethodID CMegamoduleBase::s_midstopPeriodicallyCallbackTask;

const char* const CMegamoduleBase::COMPLICATEDTYPESTEST1_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$complicatedTypesTest1Task";

jclass CMegamoduleBase::s_clscomplicatedTypesTest1Task = 0;
jmethodID CMegamoduleBase::s_midcomplicatedTypesTest1Task;

const char* const CMegamoduleBase::GETPROPERTY_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$getPropertyTask";

jclass CMegamoduleBase::s_clsgetPropertyTask = 0;
jmethodID CMegamoduleBase::s_midgetPropertyTask;

const char* const CMegamoduleBase::GETPROPERTIES_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$getPropertiesTask";

jclass CMegamoduleBase::s_clsgetPropertiesTask = 0;
jmethodID CMegamoduleBase::s_midgetPropertiesTask;

const char* const CMegamoduleBase::GETALLPROPERTIES_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$getAllPropertiesTask";

jclass CMegamoduleBase::s_clsgetAllPropertiesTask = 0;
jmethodID CMegamoduleBase::s_midgetAllPropertiesTask;

const char* const CMegamoduleBase::SETPROPERTY_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$setPropertyTask";

jclass CMegamoduleBase::s_clssetPropertyTask = 0;
jmethodID CMegamoduleBase::s_midsetPropertyTask;

const char* const CMegamoduleBase::SETPROPERTIES_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$setPropertiesTask";

jclass CMegamoduleBase::s_clssetPropertiesTask = 0;
jmethodID CMegamoduleBase::s_midsetPropertiesTask;

const char* const CMegamoduleBase::CLEARALLPROPERTIES_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleBase$clearAllPropertiesTask";

jclass CMegamoduleBase::s_clsclearAllPropertiesTask = 0;
jmethodID CMegamoduleBase::s_midclearAllPropertiesTask;

const char* const CMegamoduleBase::ENUMERATE_TASK_CLASS = 
        "com.rho.examples.megamodule.MegamoduleSingletonBase$enumerateTask";

jclass CMegamoduleBase::s_clsenumerateTask = 0;
jmethodID CMegamoduleBase::s_midenumerateTask;


//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CMegamoduleBase::jniInit()
{
    JNIEnv *env = jnienv();
    if(!env)
    {
        LOG(FATAL) + "JNI init failed: JNIEnv is null";
        return 0;
    }
    return jniInit(env);
}
//----------------------------------------------------------------------------------------------------------------------

JNIEnv* CMegamoduleBase::jniInit(JNIEnv* env)
{
    static bool initialized = false;
    env = MethodExecutorJni::jniInit(env);
    if (!env) {
        LOG(FATAL) + "JNI init failed";
        return 0;
    }

    if(!initialized)
    {
        //init MegamoduleFactorySingleton JNI
        s_clsFactorySingleton = loadClass(env, FACTORY_SINGLETON_CLASS);
        if (!s_clsFactorySingleton) return 0;

        s_midFactorySetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "setInstance", "(Lcom/rho/examples/megamodule/IMegamoduleFactory;)V");
        if(!s_midFactorySetInstance)
        {
            LOG(FATAL) + "Failed to get method 'setInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }
        s_midFactoryGetInstance = env->GetStaticMethodID(s_clsFactorySingleton, "getInstance", "()Lcom/rho/examples/megamodule/IMegamoduleFactory;");
        if(!s_midFactoryGetInstance)
        {
            LOG(FATAL) + "Failed to get method 'getInstance' for java class " + FACTORY_SINGLETON_CLASS;
            return NULL;
        }

        //init IMegamoduleFactory JNI
        s_clsIFactory = loadClass(env, IFACTORY_CLASS);
        if (!s_clsIFactory) return 0;
        s_midGetApiSingleton = env->GetMethodID(s_clsIFactory, "getApiSingleton", "()Lcom/rho/examples/megamodule/IMegamoduleSingleton;");
        if(!s_midGetApiSingleton)
        {
            LOG(FATAL) + "Failed to get method 'getApiSingleton' for java class " + IFACTORY_CLASS;
            return NULL;
        }
        s_midGetApiObject = env->GetMethodID(s_clsIFactory, "getApiObject", "(Ljava/lang/String;)Lcom/rho/examples/megamodule/IMegamodule;");
        if(!s_midGetApiObject)
        {
            LOG(FATAL) + "Failed to get method 'getApiObject' for java class " + IFACTORY_CLASS;
            return NULL;
        }

        s_clsSingletonBase = loadClass(env, SINGLETON_BASE_CLASS);
        if (!s_clsSingletonBase) return 0;
        s_clsObjectBase = loadClass(env, OBJECT_BASE_CLASS);
        if (!s_clsObjectBase) return 0;



        s_clsgetStringPropertyTask = loadClass(env, GETSTRINGPROPERTY_TASK_CLASS);
        if (!s_clsgetStringPropertyTask) return 0;
        s_midgetStringPropertyTask = env->GetMethodID(s_clsgetStringPropertyTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetStringPropertyTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETSTRINGPROPERTY_TASK_CLASS;
            return NULL;
        }

        s_clsgetIntegerPropertyTask = loadClass(env, GETINTEGERPROPERTY_TASK_CLASS);
        if (!s_clsgetIntegerPropertyTask) return 0;
        s_midgetIntegerPropertyTask = env->GetMethodID(s_clsgetIntegerPropertyTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetIntegerPropertyTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETINTEGERPROPERTY_TASK_CLASS;
            return NULL;
        }

        s_clssetIntegerPropertyTask = loadClass(env, SETINTEGERPROPERTY_TASK_CLASS);
        if (!s_clssetIntegerPropertyTask) return 0;
        s_midsetIntegerPropertyTask = env->GetMethodID(s_clssetIntegerPropertyTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;ILcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midsetIntegerPropertyTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + SETINTEGERPROPERTY_TASK_CLASS;
            return NULL;
        }

        s_clstypesTestTask = loadClass(env, TYPESTEST_TASK_CLASS);
        if (!s_clstypesTestTask) return 0;
        s_midtypesTestTask = env->GetMethodID(s_clstypesTestTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Ljava/lang/String;ZIDLjava/util/List;Ljava/util/Map;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midtypesTestTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + TYPESTEST_TASK_CLASS;
            return NULL;
        }

        s_clsproduceArrayTask = loadClass(env, PRODUCEARRAY_TASK_CLASS);
        if (!s_clsproduceArrayTask) return 0;
        s_midproduceArrayTask = env->GetMethodID(s_clsproduceArrayTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midproduceArrayTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + PRODUCEARRAY_TASK_CLASS;
            return NULL;
        }

        s_clsproduceHashTask = loadClass(env, PRODUCEHASH_TASK_CLASS);
        if (!s_clsproduceHashTask) return 0;
        s_midproduceHashTask = env->GetMethodID(s_clsproduceHashTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midproduceHashTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + PRODUCEHASH_TASK_CLASS;
            return NULL;
        }

        s_clsproduceComplicatedResultTask = loadClass(env, PRODUCECOMPLICATEDRESULT_TASK_CLASS);
        if (!s_clsproduceComplicatedResultTask) return 0;
        s_midproduceComplicatedResultTask = env->GetMethodID(s_clsproduceComplicatedResultTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midproduceComplicatedResultTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + PRODUCECOMPLICATEDRESULT_TASK_CLASS;
            return NULL;
        }

        s_clsgetObjectsCountTask = loadClass(env, GETOBJECTSCOUNT_TASK_CLASS);
        if (!s_clsgetObjectsCountTask) return 0;
        s_midgetObjectsCountTask = env->GetMethodID(s_clsgetObjectsCountTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamoduleSingleton;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetObjectsCountTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETOBJECTSCOUNT_TASK_CLASS;
            return NULL;
        }

        s_clsgetObjectByIndexTask = loadClass(env, GETOBJECTBYINDEX_TASK_CLASS);
        if (!s_clsgetObjectByIndexTask) return 0;
        s_midgetObjectByIndexTask = env->GetMethodID(s_clsgetObjectByIndexTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamoduleSingleton;ILcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetObjectByIndexTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETOBJECTBYINDEX_TASK_CLASS;
            return NULL;
        }

        s_clsshowAlertFromUIThreadTask = loadClass(env, SHOWALERTFROMUITHREAD_TASK_CLASS);
        if (!s_clsshowAlertFromUIThreadTask) return 0;
        s_midshowAlertFromUIThreadTask = env->GetMethodID(s_clsshowAlertFromUIThreadTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Ljava/lang/String;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midshowAlertFromUIThreadTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + SHOWALERTFROMUITHREAD_TASK_CLASS;
            return NULL;
        }

        s_clssetPeriodicallyCallbackTask = loadClass(env, SETPERIODICALLYCALLBACK_TASK_CLASS);
        if (!s_clssetPeriodicallyCallbackTask) return 0;
        s_midsetPeriodicallyCallbackTask = env->GetMethodID(s_clssetPeriodicallyCallbackTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;ILcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midsetPeriodicallyCallbackTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + SETPERIODICALLYCALLBACK_TASK_CLASS;
            return NULL;
        }

        s_clsisPeriodicallyCallbackSettedTask = loadClass(env, ISPERIODICALLYCALLBACKSETTED_TASK_CLASS);
        if (!s_clsisPeriodicallyCallbackSettedTask) return 0;
        s_midisPeriodicallyCallbackSettedTask = env->GetMethodID(s_clsisPeriodicallyCallbackSettedTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midisPeriodicallyCallbackSettedTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + ISPERIODICALLYCALLBACKSETTED_TASK_CLASS;
            return NULL;
        }

        s_clsstopPeriodicallyCallbackTask = loadClass(env, STOPPERIODICALLYCALLBACK_TASK_CLASS);
        if (!s_clsstopPeriodicallyCallbackTask) return 0;
        s_midstopPeriodicallyCallbackTask = env->GetMethodID(s_clsstopPeriodicallyCallbackTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midstopPeriodicallyCallbackTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + STOPPERIODICALLYCALLBACK_TASK_CLASS;
            return NULL;
        }

        s_clscomplicatedTypesTest1Task = loadClass(env, COMPLICATEDTYPESTEST1_TASK_CLASS);
        if (!s_clscomplicatedTypesTest1Task) return 0;
        s_midcomplicatedTypesTest1Task = env->GetMethodID(s_clscomplicatedTypesTest1Task, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Ljava/util/List;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midcomplicatedTypesTest1Task)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + COMPLICATEDTYPESTEST1_TASK_CLASS;
            return NULL;
        }

        s_clsgetPropertyTask = loadClass(env, GETPROPERTY_TASK_CLASS);
        if (!s_clsgetPropertyTask) return 0;
        s_midgetPropertyTask = env->GetMethodID(s_clsgetPropertyTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Ljava/lang/String;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetPropertyTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETPROPERTY_TASK_CLASS;
            return NULL;
        }

        s_clsgetPropertiesTask = loadClass(env, GETPROPERTIES_TASK_CLASS);
        if (!s_clsgetPropertiesTask) return 0;
        s_midgetPropertiesTask = env->GetMethodID(s_clsgetPropertiesTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Ljava/util/List;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetPropertiesTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETPROPERTIES_TASK_CLASS;
            return NULL;
        }

        s_clsgetAllPropertiesTask = loadClass(env, GETALLPROPERTIES_TASK_CLASS);
        if (!s_clsgetAllPropertiesTask) return 0;
        s_midgetAllPropertiesTask = env->GetMethodID(s_clsgetAllPropertiesTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midgetAllPropertiesTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + GETALLPROPERTIES_TASK_CLASS;
            return NULL;
        }

        s_clssetPropertyTask = loadClass(env, SETPROPERTY_TASK_CLASS);
        if (!s_clssetPropertyTask) return 0;
        s_midsetPropertyTask = env->GetMethodID(s_clssetPropertyTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Ljava/lang/String;Ljava/lang/String;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midsetPropertyTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + SETPROPERTY_TASK_CLASS;
            return NULL;
        }

        s_clssetPropertiesTask = loadClass(env, SETPROPERTIES_TASK_CLASS);
        if (!s_clssetPropertiesTask) return 0;
        s_midsetPropertiesTask = env->GetMethodID(s_clssetPropertiesTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Ljava/util/Map;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midsetPropertiesTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + SETPROPERTIES_TASK_CLASS;
            return NULL;
        }

        s_clsclearAllPropertiesTask = loadClass(env, CLEARALLPROPERTIES_TASK_CLASS);
        if (!s_clsclearAllPropertiesTask) return 0;
        s_midclearAllPropertiesTask = env->GetMethodID(s_clsclearAllPropertiesTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamodule;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midclearAllPropertiesTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + CLEARALLPROPERTIES_TASK_CLASS;
            return NULL;
        }

        s_clsenumerateTask = loadClass(env, ENUMERATE_TASK_CLASS);
        if (!s_clsenumerateTask) return 0;
        s_midenumerateTask = env->GetMethodID(s_clsenumerateTask, "<init>",
                        "(Lcom/rho/examples/megamodule/IMegamoduleSingleton;Lcom/rhomobile/rhodes/api/IMethodResult;)V");
        if(!s_midenumerateTask)
        {
            LOG(FATAL) + "Failed to get constructor for java class " + ENUMERATE_TASK_CLASS;
            return NULL;
        }


        s_clsIDefaultId = loadClass(env, IDEFAULTID_CLASS);
        if (!s_clsIDefaultId) return 0;
        s_midGetDefaultID = env->GetMethodID(s_clsIDefaultId, "getDefaultID", "()Ljava/lang/String;");
        if(!s_midGetDefaultID)
        {
            LOG(FATAL) + "Failed to get method 'getDefaultID' for java class " + IDEFAULTID_CLASS;
            return NULL;
        }
        s_midSetDefaultID = env->GetMethodID(s_clsIDefaultId, "setDefaultID", "(Ljava/lang/String;)V");
        if(!s_midSetDefaultID)
        {
            LOG(FATAL) + "Failed to get method 'setDefaultID' for java class " + IDEFAULTID_CLASS;
            return NULL;
        }


        initialized = true;
        LOG(TRACE) + "CMegamodule JNI init succeeded";
    }
    return env;
}
//----------------------------------------------------------------------------------------------------------------------

void CMegamoduleBase::setJavaFactory(JNIEnv* env, jobject jFactory)
{
    LOG(TRACE) + "setJavaFactory";

    env = jniInit(env);
    if (!env) {
        LOG(FATAL) + "JNI initialization failed";
        return;
    }

    env->CallStaticVoidMethod(s_clsFactorySingleton, s_midFactorySetInstance, jFactory);

    LOG(TRACE) + "setJavaFactory succeeded";
}
//----------------------------------------------------------------------------------------------------------------------

rho::String CMegamoduleBase::getDefaultID()
{
    LOG(TRACE) + "getDefaultID";

    JNIEnv *env = jniInit();
    if (!env) {
        LOG(FATAL) + "JNI initialization failed";
        return rho::String();
    }

    jhobject jhSingleton = getSingleton(env);
    jhstring res = static_cast<jstring>(env->CallObjectMethod(jhSingleton.get(), s_midGetDefaultID));
    return rho_cast<rho::String>(env, res);
}
//----------------------------------------------------------------------------------------------------------------------

void CMegamoduleBase::setDefaultID(const rho::String& id)
{
    LOG(TRACE) + "setDefaultID: " + id;

    JNIEnv *env = jniInit();
    if (!env) {
        LOG(FATAL) + "JNI initialization failed";
        return;
    }

    jhobject instance = getSingleton(env);
    jhstring jhId = rho_cast<jstring>(env, id);
    env->CallVoidMethod(instance.get(), s_midSetDefaultID, jhId.get());
}
//----------------------------------------------------------------------------------------------------------------------

jobject CMegamoduleBase::getFactory(JNIEnv* env)
{
    jobject res = env->CallStaticObjectMethod(s_clsFactorySingleton, s_midFactoryGetInstance);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CMegamoduleBase::getSingleton(JNIEnv* env)
{
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiSingleton);
    return res;
}
//----------------------------------------------------------------------------------------------------------------------

jobject CMegamoduleBase::getObject(JNIEnv* env)
{
    jhstring jhId = rho_cast<jstring>(env, m_id);
    jhobject jhFactory = getFactory(env);
    jobject res = env->CallObjectMethod(jhFactory.get(), s_midGetApiObject, jhId.get());
    return res;
}
//----------------------------------------------------------------------------------------------------------------------


}
}
