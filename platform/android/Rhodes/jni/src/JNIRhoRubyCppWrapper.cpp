#include "rhodes/JNIRhodes.h"
#include "logging/RhoLog.h"
#include "rhoruby/api/RhoRuby.h"
#include "rhoruby/impl/MutableStringImpl.h"

#include <cassert>
#include <memory>

//#define AAR_DEBUG

#ifdef AAR_DEBUG
#define RHO_RUBY_JAVA_PACKAGE "com/example/n0men/myapplication/"
#else
#define RHO_RUBY_JAVA_PACKAGE "com/rhomobile/rhodes/"
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
    rho::ruby::IObject* executeRubyMethod(const char* full_class_name, const char* method_name, jobject params);
    rho::ruby::IObject* convertFromJToR(jobject param);
    rho::String getRubyServerURL();
private:
    RhoRubyWrapper(JNIEnv* e);

    JNIEnv* env = nullptr;
    jclass jRhoObject;
    jclass jIRhoRubyObject;
    jclass jRhoRubyArrayObjects;

    jmethodID midgetCount;
    jmethodID midgetItem;

    jmethodID midgetType;
    jmethodID midgetStringValue;
    jmethodID midgetIntValue;
    jmethodID midgetFloatValue;
    jmethodID midgetBooleanValue;
};

RhoRubyWrapper::RhoRubyWrapper(JNIEnv* e)
{
    if (!e) 
    {
       RAWLOG_ERROR("JNI init failed: jnienv is null");
       return;
    }

    env = e;
    jRhoObject = env->FindClass(RHO_RUBY_JAVA_PACKAGE "RhoRubyObject");
    if(!jRhoObject)
    {
        RAWLOG_ERROR("Class RhoRubyObject not found!");
        return;
    }

    midgetStringValue = getJNIClassMethod(env, jRhoObject, "getStringValue", "()Ljava/lang/String;");
    midgetIntValue = getJNIClassMethod(env, jRhoObject, "getIntValue", "()I");
    midgetBooleanValue = getJNIClassMethod(env, jRhoObject, "getBooleanValue", "()Z");
    midgetFloatValue = getJNIClassMethod(env, jRhoObject, "getFloatValue", "()F");

    jIRhoRubyObject = env->FindClass(RHO_RUBY_JAVA_PACKAGE "IRhoRubyObject");
    if(!jIRhoRubyObject)
    {
        RAWLOG_ERROR("Class IRhoRubyObject not found!");
        return;
    }

    midgetType = getJNIClassMethod(env, jIRhoRubyObject, "getType", "()I");

    jRhoRubyArrayObjects = env->FindClass(RHO_RUBY_JAVA_PACKAGE "RhoRubyArrayObjects");
    if(!jRhoRubyArrayObjects)
    {
        RAWLOG_ERROR("Class RhoRubyArrayObjects not found!");
        return;
    }
    
    midgetCount = getJNIClassMethod(env, jRhoRubyArrayObjects, "getCount", "()I");
    midgetItem = getJNIClassMethod(env, jRhoRubyArrayObjects, "getItem", "(I)" "L" RHO_RUBY_JAVA_PACKAGE "IRhoRubyObject;");
}

int RhoRubyWrapper::getCount(jobject array) const
{
    if(!array) return 0;
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
    rr->loadModel(full_class_name);
    rho::ruby::IString* ruby_string = rr->executeRubyMethodWithJSON(full_class_name, method_name, parameters);
    if(!ruby_string) return "(null)";
    
    std::string result = ruby_string->getUTF8();
    RAWLOG_INFO1("result: %s", result.c_str());
    ruby_string->release();
    return result;
}

rho::String RhoRubyWrapper::getRubyServerURL()
{
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IString* ruby_string = rr->getRubyServerURL();
    if(!ruby_string) return "(null)";

    std::string result = ruby_string->getUTF8();
    RAWLOG_INFO1("getRubyServerURL: %s", result.c_str());
    ruby_string->release();
    return result;
}

rho::ruby::IObject* RhoRubyWrapper::convertFromJToR(jobject param)
{
    jint result = env->CallIntMethod(param, midgetType);
    int type = rho_cast<int>(env, result);
    RAWLOG_INFO1("rhoruby type: %d", type);

    rho::ruby::BASIC_TYPES _type = (rho::ruby::BASIC_TYPES)type;

    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();

    switch (_type)
    {
        case rho::ruby::BASIC_TYPES::String:
        {
            jstring jstr = static_cast<jstring>(env->CallObjectMethod(param, midgetStringValue));
            rho::String str = rho_cast<rho::String>(env, jstr);
            RAWLOG_INFO1("rhoruby string: %s", str.c_str());
            rho::ruby::IMutableString* o_str = (rho::ruby::IMutableString*)rr->makeBaseTypeObject(rho::ruby::BASIC_TYPES::MutableString);
            o_str->setUTF8(str.c_str());
            return o_str;
        }

        case rho::ruby::BASIC_TYPES::Integer:
        {
            jint jint_value = env->CallIntMethod(param, midgetIntValue);
            int value = rho_cast<int>(env, jint_value);
            RAWLOG_INFO1("rhoruby int value: %d", value);
            rho::ruby::IMutableInteger* o_int = (rho::ruby::IMutableInteger*)rr->makeBaseTypeObject(rho::ruby::BASIC_TYPES::Integer);
            o_int->setLong(value);
            return o_int;
        }

        case rho::ruby::BASIC_TYPES::Float:
        {
            jfloat jfloat_value = env->CallFloatMethod(param, midgetFloatValue);
            float value = static_cast<float>(jfloat_value);
            RAWLOG_INFO1("rhoruby float value: %f", value);
            rho::ruby::IMutableFloat* o_float = (rho::ruby::IMutableFloat*)rr->makeBaseTypeObject(rho::ruby::BASIC_TYPES::Float);
            o_float->setDouble((float)value);
            return o_float;
        }

        case rho::ruby::BASIC_TYPES::Boolean:
        {
            jboolean jbool_value = env->CallBooleanMethod(param, midgetBooleanValue);
            bool value = static_cast<bool>(jbool_value);
            RAWLOG_INFO1("rhoruby bool value: %d", (int)value);
            rho::ruby::IMutableBoolean* o_bool = (rho::ruby::IMutableBoolean*)rr->makeBaseTypeObject(rho::ruby::BASIC_TYPES::Boolean);
            o_bool->setBool(value);
            return o_bool;
        }
        
        case rho::ruby::BASIC_TYPES::RubyNil:
        {
            RAWLOG_INFO("rhoruby nil value!");
            rho::ruby::INil* o_nill = (rho::ruby::INil*)rr->makeBaseTypeObject(rho::ruby::BASIC_TYPES::RubyNil);
            return o_nill;
        }

        case rho::ruby::BASIC_TYPES::Array:
        {
            int count = getCount(param);
            RAWLOG_INFO1("count element in array: %d", count);
            rho::ruby::IMutableArray* new_param_array = (rho::ruby::IMutableArray*)rr->makeBaseTypeObject(rho::ruby::BASIC_TYPES::Array);

            for (int i = 0; i < count; ++i)
            {
                jobject jRhoRubyObj = env->CallObjectMethod(param, midgetItem, rho_cast<jint>(env, i));
                rho::ruby::IObject* obj = convertFromJToR(jRhoRubyObj);
                if(!obj)
                {
                    return nullptr;
                }

                new_param_array->addItem(obj);
           }

           return new_param_array;
        }
    
        default:
           RAWLOG_ERROR1("Unsupported type: %d", type);
           break;
    }

    return nullptr;
}

rho::ruby::IObject* RhoRubyWrapper::executeRubyMethod(const char* full_class_name, const char* method_name, jobject params)
{
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();

    if(params)
    {
        int count = getCount(params);
        RAWLOG_INFO1("count params: %d", count);

        rho::ruby::IMutableArray* new_param_array = (rho::ruby::IMutableArray*)rr->makeBaseTypeObject(rho::ruby::BASIC_TYPES::Array);

        for (int i = 0; i < count; ++i)
        {
            jobject jRhoRubyObj = env->CallObjectMethod(params, midgetItem, rho_cast<jint>(env, i));
            rho::ruby::IObject* obj = convertFromJToR(jRhoRubyObj);
            if(!obj)
            {
                return nullptr;
            }

            new_param_array->addItem(obj);
        }

        rho::ruby::IObject* result = rr->executeRubyMethod(full_class_name, method_name, new_param_array);
    }
    else
    {
        rho::ruby::IObject* result = rr->executeRubyMethod(full_class_name, method_name, nullptr);
    }

    return nullptr;
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
 RHO_GLOBAL jstring JNICALL Java_com_example_n0men_myapplication_RhoRubySingleton_getRubyServerURL
#else
 RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoRubySingleton_getRubyServerURL
#endif
(JNIEnv* env, jclass)
{
    RhoRubyWrapper* pwrapper = RhoRubyWrapper::instance(env);
    rho::String result = pwrapper->getRubyServerURL();
    return rho_cast<jstring>(env, result);
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

#ifdef AAR_DEBUG
 RHO_GLOBAL jobject JNICALL Java_com_example_n0men_myapplication_RhoRubyClassObject_NativeExecuteRubyObjectMethod
#else
 RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhodes_RhoRubyClassObject_NativeExecuteRubyObjectMethod
#endif
  (JNIEnv* env, jclass, jstring class_name, jstring method_name, jobject parameters)
  {
      
      rho::String _class_name = rho_cast<rho::String>(env, class_name);
      rho::String _method_name = rho_cast<rho::String>(env, method_name);

      RAWLOG_INFO2("jni NativeExecuteRubyObjectMethod: %s, %s", _class_name.c_str(), _method_name.c_str());
      RhoRubyWrapper* pwrapper = RhoRubyWrapper::instance(env);
      pwrapper->executeRubyMethod(_class_name.c_str(), _method_name.c_str(), parameters);
      return nullptr;

  }