#pragma once

namespace rho{
namespace common{

struct IRhoCrypt
{
    virtual ~IRhoCrypt(void){}

    virtual int db_encrypt( const char* szPartition, int size, unsigned char* data, unsigned char* dataOut ) = 0;
    virtual int db_decrypt( const char* szPartition, int size, unsigned char* data ) = 0;
    virtual int set_db_CryptKey( const char* szPartition, const char* szKey, bool bPersistent ) = 0;
};

}
}
