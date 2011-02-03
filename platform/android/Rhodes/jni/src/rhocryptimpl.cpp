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
    jhobject dataInObj = jhobject(env->NewDirectByteBuffer(data, size)); 
    jhstring objPartition = rho_cast<jhstring>(szPartition);

    jhobject dataOutObj = jhobject(env->NewDirectByteBuffer(m_dataOut, size));
    jboolean result = env->CallBooleanMethod(m_obj, midDBDecrypt, objPartition.get(), dataInObj.get(), dataOutObj.get());

    memcpy( data, m_dataOut, size );

    //LOG(INFO) + "C:db_decrypt END";
    return result ? 1 : 0;

//    return 1;
}

int CRhoCryptImpl::db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut )
{
    //LOG(INFO) + "C:db_encrypt";
    JNIEnv *env = jnienv();
    jhobject dataInObj = jhobject(env->NewDirectByteBuffer(data, size)); 
    jhobject dataOutObj = jhobject(env->NewDirectByteBuffer(dataOut, size)); 
    jhstring objPartition = rho_cast<jhstring>(szPartition);
    jboolean result = env->CallBooleanMethod(m_obj, midDBEncrypt, objPartition.get(), dataInObj.get(), dataOutObj.get());

    //LOG(INFO) + "C:db_encrypt END";
    return result ? 1 : 0;
    //memcpy(dataOut, data, size);
    //return 1;
}

int CRhoCryptImpl::set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent )
{
    JNIEnv *env = jnienv();
    jhstring objPartition = rho_cast<jhstring>(szPartition);
    jhstring objKey = rho_cast<jhstring>(szKey);
    jboolean result = env->CallBooleanMethod(m_obj, midSetDBCryptKey, objPartition.get(), objKey.get(), (jboolean)bPersistent);

    return result ? 1 : 0;
}

} // namespace common
} // namespace rho

