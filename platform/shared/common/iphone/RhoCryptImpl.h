#pragma once

#include "logging/RhoLog.h"
#include "common/IRhoCrypt.h"

namespace rho{
namespace common{

class CRhoCryptImpl : public IRhoCrypt
{
    DEFINE_LOGCLASS;

	unsigned char *  m_dbKeyData;
    int32_t   m_dwLastError;
    String    m_strDBPartition;

public:
    CRhoCryptImpl(void);
    ~CRhoCryptImpl(void);

    virtual int db_decrypt( const char* szPartition, int size, unsigned char* data );
    virtual int db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut );
    virtual int set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent );
private:
    bool _checkError( int32_t status, const char* szFunc );
    int32_t getErrorCode(){ return m_dwLastError; }

    void initContext(const char* szPartition);

	void generateNewKey();
    void readKeyFromStorage();
    void writeKeyToStorage();
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