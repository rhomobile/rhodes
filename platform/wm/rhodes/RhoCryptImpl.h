#pragma once

#include "logging/RhoLog.h"
#include "common/IRhoCrypt.h"

#include <wincrypt.h>

namespace rho{
namespace common{

class CRhoCryptImpl : public IRhoCrypt
{
    DEFINE_LOGCLASS;

    HCRYPTPROV m_hCryptProv;
    HCRYPTKEY  m_hKey;
    DWORD      m_dwLastError;
    StringW    m_strDBPartition;

public:
    CRhoCryptImpl(void);
    ~CRhoCryptImpl(void);

    virtual int db_decrypt( const char* szPartition, int size, unsigned char* data );
    virtual int db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut );
    virtual int set_db_CryptKey( const char* szPartition, const char* szKey );
private:
    bool _checkError( BOOL bRes, const char* szFunc );
    unsigned int getErrorCode(){ return m_dwLastError; }

    void initContext(const char* szPartition);

    void readKeyFromStorage(DATA_BLOB& blobKey);
    void writeKeyToStorage();
    void saveKeyToStorage(DATA_BLOB& blobKey);
};

}
}

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void rho_crypt_aes_256_encrypt(int size, unsigned char *in, unsigned char *out);
void rho_crypt_aes_256_decrypt(int size, unsigned char *in, unsigned char *out);

#ifdef __cplusplus
};
#endif //__cplusplus