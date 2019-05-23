#include "rhodes/SecurityTokenGenerator.h"
#include "rhodes/JNIRhodes.h"
#include <unistd.h>


namespace rho
{
namespace common
{

    SecurityTokenGenerator::SecurityTokenGenerator()
    {
        JNIEnv *env = jnienv();    
        if (!env) {      
            return;    
        }

        size_t tryCount = 5;
        do
        {
            cls = env->FindClass("com/rhomobile/rhodes/SecurityTokenGenerator");
            if(cls) break;
            usleep(1000);
        }while(--tryCount);

        if (!cls) return;

        jmethodID midGetInstance = getJNIClassStaticMethod(env, cls, "getInstance", "()Lcom/rhomobile/rhodes/SecurityTokenGenerator;");
        jobject obj = env->CallStaticObjectMethod(cls, midGetInstance);

        mibGetTokenString = getJNIClassMethod(env, cls, "getTokenString", "()Ljava/lang/String;");
        if(!mibGetTokenString) return;
        mibGetSecureAgent = getJNIClassMethod(env, cls, "getSecureAgent", "()Ljava/lang/String;");
        if(!mibGetSecureAgent) return;

        generator_object = env->NewGlobalRef(obj);
        env->DeleteLocalRef(obj);
    }

    SecurityTokenGenerator::~SecurityTokenGenerator()
    {
        if (generator_object)        
           jnienv()->DeleteGlobalRef(generator_object);

    }

    const rho::String& SecurityTokenGenerator::getSecurityToken() const
    {
        JNIEnv *env = jnienv();
        if(currentTokens.empty())
        {
            jhstring result = static_cast<jstring>(env->CallObjectMethod(generator_object, mibGetTokenString));
            currentTokens = rho_cast<rho::String>(env, result);
        }

        return currentTokens;
    }

}
}