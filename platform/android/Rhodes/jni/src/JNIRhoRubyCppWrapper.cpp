#include "rhodes/JNIRhodes.h"
#include "logging/RhoLog.h"
#include "rhoruby/api/RhoRuby.h"

#include <cassert>
#include <memory>

#define AAR_DEBUG

#ifdef AAR_DEBUG
#define RHO_RUBY_JAVA_PACKAGE "com.example.n0men.myapplication"
#else
#define RHO_RUBY_JAVA_PACKAGE "com.rhomobile.rhodes"
#endif

class RhoRubyWrapper
{
public:
    static RhoRubyWrapper* instance(JNIEnv* e)
    {
        static RhoRubyWrapper wrapper(e);
        return &wrapper;
    }

    int getCount(jobject array) const;

    bool makeRubyClassObject(rho::String name);
    rho::String executeRubyMethodWithJSON(const char* full_class_name, const char* method_name, const char* parameters);
private:
    RhoRubyWrapper(JNIEnv* e);

    JNIEnv* env = nullptr;
    jclass jRhoObject;
    jclass jIRhoRubyObject;
    jclass jRhoRubyArrayObjects;
    jmethodID midgetCount;
};

RhoRubyWrapper::RhoRubyWrapper(JNIEnv* e)
{
    if (!e) 
    {
       RAWLOG_ERROR("JNI init failed: jnienv is null");
       return;
    }

    env = e;
    jRhoObject = env->FindClass("com/example/n0men/myapplication/RhoRubyObject");
    if(!jRhoObject)
    {
        RAWLOG_ERROR("Class RhoRubyObject not found!");
        return;
    }

    jIRhoRubyObject = env->FindClass("com/example/n0men/myapplication/IRhoRubyObject");
    if(!jIRhoRubyObject)
    {
        RAWLOG_ERROR("Class IRhoRubyObject not found!");
        return;
    }

    jRhoRubyArrayObjects = env->FindClass("com/example/n0men/myapplication/RhoRubyArrayObjects");
    if(!jRhoRubyArrayObjects)
    {
        RAWLOG_ERROR("Class RhoRubyArrayObjects not found!");
        return;
    }
    
    midgetCount = getJNIClassMethod(env, jRhoRubyArrayObjects, "getCount", "()I");
}

int RhoRubyWrapper::getCount(jobject array) const
{
    jint result = env->CallIntMethod(array, midgetCount);
    return rho_cast<int>(env, result);
}

bool RhoRubyWrapper::makeRubyClassObject(rho::String name)
{
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    //rho::ruby::IObject* class_object = ;
    std::unique_ptr<rho::ruby::IObject> class_object(rr->makeRubyClassObject(name.c_str()));
    if(!class_object) return false;
    //class_object->release();
    return true;
}

rho::String RhoRubyWrapper::executeRubyMethodWithJSON(const char* full_class_name, const char* method_name, const char* parameters)
{
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IString* ruby_string = rr->executeRubyMethodWithJSON(full_class_name, method_name, parameters);
    RAWLOG_INFO("line:86");
    std::string result = ruby_string->getUTF8();
    RAWLOG_INFO1("result: %s", result.c_str());
    ruby_string->release();
    return result;
}

#ifdef AAR_DEBUG
RHO_GLOBAL jboolean JNICALL Java_com_example_n0men_myapplication_RhoRubyClassObject_NativeMakeRubyClassObject
#else
RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhoRubyClassObject_NativeMakeRubyClassObject
#endif
  (JNIEnv * env, jclass, jstring name)
  {
      RhoRubyWrapper* pwrapper = RhoRubyWrapper::instance(env);
      
      if(!pwrapper->makeRubyClassObject(rho_cast<rho::String>(env, name)))
          return rho_cast<jboolean>(env, false);

      if(env->ExceptionCheck() == JNI_TRUE)
      {
          rho::String message = rho::common::clearException(env);
          RAWLOG_ERROR(message.c_str());
          return rho_cast<jboolean>(env, false);
      }
      return rho_cast<jboolean>(env, true);
  }

#ifdef AAR_DEBUG
 RHO_GLOBAL jstring JNICALL Java_com_example_n0men_myapplication_RhoRubySingleton_executeRubyMethodWithJSON
#else
 RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoRubySingleton_executeRubyMethodWithJSON
#endif
  (JNIEnv* env, jclass, jstring full_class_name, jstring method_name, jstring parameters)
  {
      static rho::String strErr = "(null)"; 
      if(!full_class_name || !method_name)
      {
          RAWLOG_ERROR("Invalid args!!!");
          return  rho_cast<jstring>(env, strErr);;
      }
      rho::String _full_class_name = rho_cast<rho::String>(env, full_class_name);
      rho::String _method_name = rho_cast<rho::String>(env, method_name);
      rho::String _parameters = parameters ? rho_cast<rho::String>(env, parameters) : strErr;

      RAWLOG_INFO3("jni executeRubyMethodWithJSON: %s, %s, %s", _full_class_name.c_str(), _method_name.c_str(), 
          _parameters.c_str());
      RhoRubyWrapper* pwrapper = RhoRubyWrapper::instance(env);

      rho::String result = pwrapper->executeRubyMethodWithJSON(
          _full_class_name.c_str(),
          _method_name.c_str(),
          parameters ? _parameters.c_str() : nullptr);

      if(env->ExceptionCheck() == JNI_TRUE)
      {
          rho::String message = rho::common::clearException(env);
          RAWLOG_ERROR(message.c_str());
          return rho_cast<jstring>(env, strErr);
      }

      RAWLOG_INFO1("result: %s", result.c_str());
      return rho_cast<jstring>(env, result);

  }