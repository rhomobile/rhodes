#include "net/INetRequest.h"

class CNetData : public rho::net::INetData
{
    const char* m_szData;
public:
    CNetData(const char* szData) : m_szData(szData){}

    virtual const char* getCharData(){ return m_szData; }
    virtual unsigned int getDataSize(){ return strlen(m_szData); }

};

