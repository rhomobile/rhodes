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

#ifndef _RHOCONF_H_
#define _RHOCONF_H_

#ifdef __cplusplus

#include "RhoStd.h"

namespace rho{
namespace common{

#define RHOVERSION "Version"
#define RHODBVERSION "DBVersion"

class IRhoSettingsListener {
public:
    virtual void onSettingUpdated( const String& name, const String& newVal ) = 0;
};

class RhoSettings{

    Vector<IRhoSettingsListener*> m_listeners;

    String      m_strConfFilePath, m_strAppConfFilePath, m_strAppConfUserPath;
    Hashtable<String,String> m_mapValues;
    Hashtable<String,String> m_mapChangedValues;
    HashtablePtr<String,Vector<String>* > m_mapConflictedValues;
    String m_EmptyString;

public:
    const String& getString(const char* szName);
    String getPath(const char* szName);
    int    getInt(const char* szName);
    bool   getBool(const char* szName);

    void   setString(const char* szName, const String& str, boolean bSaveToFile);
    void   setInt(const char* szName, int nVal, boolean bSaveToFile);
    void   setBool(const char* szName, bool bVal, boolean bSaveToFile);

    bool   isExist(const char* szName);
    void   removeProperty(const char* szName, boolean bSaveToFile);

    const String& getConfFilePath()const{ return m_strConfFilePath;}
    void setConfFilePath(const char* szConfFilePath){ m_strConfFilePath = szConfFilePath; }
    
    const String& getAppConfFilePath()const{ return m_strAppConfFilePath;}
    void setAppConfFilePath(const char* szConfFilePath){ m_strAppConfFilePath = szConfFilePath; }
    
    const String& getAppConfUserFilePath()const{ return m_strAppConfUserPath;}
    void setAppConfUserFilePath(const char* szUserPath){ m_strAppConfUserPath = szUserPath; }

    void loadFromFile();

    HashtablePtr<String,Vector<String>* >& getConflicts(){ return m_mapConflictedValues;}
    void conflictsResolved();

    void addListener( IRhoSettingsListener* l ) { m_listeners.push_back(l); }
    void removeListener( IRhoSettingsListener* l ) { m_listeners.removeElement(l); }

protected:
    void saveChangesToString(String& strData);
    void loadFromString(const char* szSettings, Hashtable<String,String>& mapValues);

    void setPropertyByName(const char* szName, int nNameLen, const char* szValue, int nValueLen, Hashtable<String,String>& mapValues );
    void loadProperty( const char* start, int len, Hashtable<String,String>& mapValues );

    void saveToFile(const char* szName, boolean bRemove = false );
    void readChanges();
    void checkConflicts();

private:    
    void notifyListeners( const String& name, const String& newVal );
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
void  rho_conf_Init_with_separate_user_path(const char* szRootPath, const char* szUserPath);
void  rho_conf_Init_from_shared_path(const char* szSharedRootPath);
int  rho_conf_getBool(const char* szName);
#ifdef __cplusplus
//In c bool is not available without loading standard library #include <stdbool.h> (C99)
void  rho_conf_setBool(const char* szName, bool value);
#endif //__cplusplus
int   rho_conf_getInt(const char* szName);
void  rho_conf_setInt(const char* szName, int value);
char* rho_conf_getString(const char* szName);
void  rho_conf_freeString(char* str);
void  rho_conf_setString(const char* szName, const char* value);
//void  rho_conf_save();
int   rho_conf_is_property_exists(const char* name);
void  rho_conf_remove_property(const char* name);

#ifdef __cplusplus
}
#endif //__cplusplus

#ifdef RHODES_EMULATOR
#include "RhoSimConf.h"
#endif //RHODES_EMULATOR

#endif //_RHOCONF_H_
