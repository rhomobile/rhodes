#ifndef _RHOCONF_H_
#define _RHOCONF_H_

#include "RhoStd.h"
#include <map>

namespace rho{
namespace common{

#define RHOVERSION "Version"
#define RHODBVERSION "DBVersion"

class RhoSettings{
    String      m_strConfFilePath;
    std::map<String,String> m_mapValues;

public:
    String getString(const char* szName);
    int    getInt(const char* szName);
    bool   getBool(const char* szName);
    void   setString(const char* szName, const String& str);
    void   setInt(const char* szName, int nVal);
    void   setBool(const char* szName, bool bVal);

    bool   isExist(const char* szName);

    const String& getConfFilePath()const{ return m_strConfFilePath;}
    void setConfFilePath(const char* szConfFilePath){ m_strConfFilePath = szConfFilePath; }

    void saveToFile();
    void loadFromFile();

    void loadFromString(const char* szSettings);

protected:
    void saveToString(String& strData);

    void setPropertyByName(const char* szName, int nNameLen, const char* szValue, int nValueLen );
    void loadProperty( const char* start, int len );
};

extern RhoSettings g_RhoSettings;

}
}

inline rho::common::RhoSettings& RHOCONF(){ return rho::common::g_RhoSettings; }
extern "C" void InitRhoConf(const char* szRootPath);

#endif //_RHOCONF_H_
