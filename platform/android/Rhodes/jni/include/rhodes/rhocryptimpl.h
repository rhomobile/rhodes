#ifndef RHO_CRYPT_IMPL_36483A0BE5E44ef3B39915D41C54319B
#define RHO_CRYPT_IMPL_36483A0BE5E44ef3B39915D41C54319B

#include <jni.h>
#include "common/IRhoCrypt.h"
#include "logging/RhoLog.h"

namespace rho {
namespace common{

class CRhoCryptImpl : public IRhoCrypt
{
    DEFINE_LOGCLASS;
public:
    CRhoCryptImpl();
    ~CRhoCryptImpl();

    virtual int db_decrypt( const char* szPartition, int size, unsigned char* data );
    virtual int db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut );
    virtual int set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent );

private:
    jclass cls;
    jmethodID midConstructor;
    jmethodID midDBDecrypt;
    jmethodID midDBEncrypt;
    jmethodID midSetDBCryptKey;

    jobject m_obj;

    unsigned char* m_dataOut;
};

}
}

#endif // RHO_CRYPT_IMPL_36483A0BE5E44ef3B39915D41C54319B

