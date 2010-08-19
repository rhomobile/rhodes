#ifndef _RHOCONF_H_
#define _RHOCONF_H_

#ifdef __cplusplus

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
    String getPath(const char* szName);
    int    getInt(const char* szName);
    bool   getBool(const char* szName);

    void   setString(const char* szName, const String& str, boolean bSaveToFile);
    void   setInt(const char* szName, int nVal, boolean bSaveToFile);
    void   setBool(const char* szName, bool bVal, boolean bSaveToFile);

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

#else
#include "common/RhoPort.h"
#endif //__cplusplus

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

void  rho_conf_Init(const char* szRootPath);
int  rho_conf_getBool(const char* szName);
void  rho_conf_setBool(const char* szName, bool value);
int   rho_conf_getInt(const char* szName);
void  rho_conf_setInt(const char* szName, int value);
char* rho_conf_getString(const char* szName);
void  rho_conf_freeString(char* str);
void  rho_conf_setString(const char* szName, const char* value);
void  rho_conf_save();
int   rho_conf_is_property_exists(const char* name);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOCONF_H_
