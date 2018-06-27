/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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

    jobject obj = env->NewObject(cls, midConstructor);
    m_obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);

    m_dataOut = (unsigned char*)malloc(RHO_DEFAULT_CRYPTO_PAGE_SIZE); //sqlite page size
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
    if(RHO_DEFAULT_CRYPTO_PAGE_SIZE < (unsigned int)size)
    {
        LOG(ERROR) + "Output buffer to decrypted is small!!!";
        return 0;
    }
    //LOG(INFO) + "C:db_decrypt";
    JNIEnv *env = jnienv();
    jhobject dataInObj = env->NewDirectByteBuffer(data, size);
    jhstring objPartition = rho_cast<jstring>(env, szPartition);

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
    jhobject dataInObj = env->NewDirectByteBuffer(data, size);
    jhobject dataOutObj = env->NewDirectByteBuffer(dataOut, size);
    jhstring objPartition = rho_cast<jstring>(env, szPartition);
    jboolean result = env->CallBooleanMethod(m_obj, midDBEncrypt, objPartition.get(), dataInObj.get(), dataOutObj.get());

    //LOG(INFO) + "C:db_encrypt END";
    return result ? 1 : 0;
    //memcpy(dataOut, data, size);
    //return 1;
}

int CRhoCryptImpl::set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent )
{
    JNIEnv *env = jnienv();
    jhstring objPartition = rho_cast<jstring>(env, szPartition);
    jhstring objKey = rho_cast<jstring>(env, szKey);
    jboolean result = env->CallBooleanMethod(m_obj, midSetDBCryptKey, objPartition.get(), objKey.get(), (jboolean)bPersistent);

    return result ? 1 : 0;
}

} // namespace common
} // namespace rho

