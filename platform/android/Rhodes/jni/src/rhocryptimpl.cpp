#include "rhodes/rhocryptimpl.h"
#include "rhodes/JNIRhodes.h"

namespace rho
{
namespace common
{
IMPLEMENT_LOGCLASS(CRhoCryptImpl,"RhoCrypt");

CRhoCryptImpl::CRhoCryptImpl() : m_obj(0), m_dataOut(0)
{
    JNIEnv *env = jnienv();
    if (!env) {
       return;
    }
    cls = getJNIClass(RHODES_JAVA_CLASS_RHOCRYPTIMPL);
    if (!cls) return;
    midConstructor = getJNIClassMethod(env, cls, "<init>", "()V");
    if (!midConstructor) return;
    midDBDecrypt = getJNIClassMethod(env, cls, "db_decrypt", "(Ljava/lang/String;Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;)Z");
    if (!midDBDecrypt) return;
    midDBEncrypt = getJNIClassMethod(env, cls, "db_encrypt", "(Ljava/lang/String;Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;)Z");
    if (!midDBEncrypt) return;
    midSetDBCryptKey = getJNIClassMethod(env, cls, "set_db_CryptKey", "(Ljava/lang/String;Ljava/lang/String;Z)Z");
    if (!midSetDBCryptKey) return;

    m_obj = env->NewObject(cls, midConstructor);
    env->NewGlobalRef(m_obj);

    m_dataOut = (unsigned char*)malloc(1024); //sqlite page size
}

CRhoCryptImpl::~CRhoCryptImpl()
{
    if (m_obj)
        jnienv()->DeleteGlobalRef(m_obj);

    if (m_dataOut)
        free(m_dataOut);
}

int CRhoCryptImpl::db_decrypt( const char* szPartition, int size, unsigned char* data )
{
    //LOG(INFO) + "C:db_decrypt";
    JNIEnv *env = jnienv();
    jobject dataInObj = env->NewDirectByteBuffer(data, size); 
    jstring objPartition = rho_cast<jstring>(szPartition);

    jobject dataOutObj = env->NewDirectByteBuffer(m_dataOut, size);
    jboolean result = env->CallBooleanMethod(m_obj, midDBDecrypt, objPartition, dataInObj, dataOutObj);

    memcpy( data, m_dataOut, size );

    env->DeleteLocalRef(objPartition);
    env->DeleteLocalRef(dataOutObj);
    env->DeleteLocalRef(dataInObj);

    //LOG(INFO) + "C:db_decrypt END";
    return result ? 1 : 0;

//    return 1;
}

int CRhoCryptImpl::db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut )
{
    //LOG(INFO) + "C:db_encrypt";
    JNIEnv *env = jnienv();
    jobject dataInObj = env->NewDirectByteBuffer(data, size); 
    jobject dataOutObj = env->NewDirectByteBuffer(dataOut, size); 
    jstring objPartition = rho_cast<jstring>(szPartition);
    jboolean result = env->CallBooleanMethod(m_obj, midDBEncrypt, objPartition, dataInObj, dataOutObj);

    env->DeleteLocalRef(objPartition);
    env->DeleteLocalRef(dataOutObj);
    env->DeleteLocalRef(dataInObj);

    //LOG(INFO) + "C:db_encrypt END";
    return result ? 1 : 0;
    //memcpy(dataOut, data, size);
    //return 1;
}

int CRhoCryptImpl::set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent )
{
    JNIEnv *env = jnienv();
    jstring objPartition = rho_cast<jstring>(szPartition);
    jstring objKey = rho_cast<jstring>(szKey);
    jboolean result = env->CallBooleanMethod(m_obj, midSetDBCryptKey, objPartition, objKey, (jboolean)bPersistent);

    env->DeleteLocalRef(objPartition);
    env->DeleteLocalRef(objKey);

    return result ? 1 : 0;
}

} // namespace common
} // namespace rho

