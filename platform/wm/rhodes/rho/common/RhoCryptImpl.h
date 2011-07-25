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
    virtual int set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent );
private:
    bool _checkError( BOOL bRes, const char* szFunc );
    unsigned int getErrorCode(){ return m_dwLastError; }

    void initContext(const char* szPartition);
    void initCryptProvider();

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