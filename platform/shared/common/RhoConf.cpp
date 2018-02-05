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

#include "RhoConf.h"
#include "RhoFile.h"
#include "RhoFilePath.h"
#include "StringConverter.h"
#ifdef RHODES_EMULATOR
static const char* CONF_FILENAME = "rhoconfig.txt";
#else
static const char* CONF_FILENAME = "apps/rhoconfig.txt";
#endif
static const char* CONF_SHARED_FILENAME = "rhoconfig.txt";
static const char* CONF_CHANGES = ".changes";
static const char* CONF_TIMESTAMP = ".timestamp";
static const char* CONF_TIMESTAMP_PROP = "rho_conf_timestamp";

namespace rho{
namespace common{

RhoSettings g_RhoSettings;

void RhoSettings::saveToFile(const char* szName, boolean bRemove/* = false*/)
{
    if ( !bRemove )
        m_mapChangedValues.put(szName, getString(szName) );
    else
        m_mapChangedValues.remove(szName);

    String strData;
    saveChangesToString(strData);

    CRhoFile oFile;
    oFile.open( (getAppConfUserFilePath()+CONF_CHANGES).c_str(), common::CRhoFile::OpenForWrite);
    oFile.write( strData.c_str(), strData.size() );
}

void RhoSettings::checkConflicts()
{
    m_mapConflictedValues.clear();
    for ( Hashtable<String,String>::iterator it=m_mapChangedValues.begin() ; it != m_mapChangedValues.end(); it++ ) 
    {
        String key = it->first;
        String valueChanged = it->second;
	    Hashtable<String,String>::iterator itValues = m_mapValues.find(key);
        if ( itValues == m_mapValues.end() )
            continue;

        String strValue = itValues->second;
        if ( strValue.compare(valueChanged) != 0 )
        {
            Vector<String>* values = new Vector<String>();
            (*values).addElement(valueChanged);
            (*values).addElement(strValue);
            m_mapConflictedValues.put(key, values);
        }
    }
}

void RhoSettings::conflictsResolved()
{
    if (m_mapConflictedValues.size() == 0 )
        return;

    String strTimestamp;
    CRhoFile::readStringFromFile((getConfFilePath()+CONF_TIMESTAMP).c_str(), strTimestamp);

    setString(CONF_TIMESTAMP_PROP, strTimestamp, true);
    m_mapConflictedValues.clear();
}

void RhoSettings::readChanges()
{
    String strTimestamp;
    CRhoFile::readStringFromFile((getConfFilePath()+CONF_TIMESTAMP).c_str(), strTimestamp);

    if ( CRhoFile::isFileExist((getAppConfUserFilePath()+CONF_CHANGES).c_str()) )
    {
        String strSettings;
        CRhoFile::readStringFromFile((getAppConfUserFilePath()+CONF_CHANGES).c_str(), strSettings);
        loadFromString( strSettings.c_str(), m_mapChangedValues );

        String strOldTimestamp = "";
	    Hashtable<String,String>::iterator it = m_mapChangedValues.find(CONF_TIMESTAMP_PROP);
        if ( it != m_mapChangedValues.end() )
            strOldTimestamp = it->second;
        
        if ( strTimestamp.compare(strOldTimestamp) != 0 )
            checkConflicts();

        loadFromString( strSettings.c_str(), m_mapValues );
    }else
    {
        m_mapChangedValues.put(CONF_TIMESTAMP_PROP,strTimestamp);
    }
}

void RhoSettings::loadFromFile()
{
    m_mapValues.clear();
    m_mapChangedValues.clear();

    String strSettings;
    CRhoFile::readStringFromFile(getAppConfFilePath().c_str(), strSettings);
    loadFromString( strSettings.c_str(), m_mapValues );

    readChanges();
}

void RhoSettings::loadFromString(const char* szSettings, Hashtable<String,String>& mapValues)
{
    if ( !szSettings && !*szSettings )
        return;

    const char* start = szSettings;

    while(start!=0){
        int len = 0;

        const char* end = strchr(start,'\n');
        if (end)
        {
            len = end-start;
            while(start[len-1] == '\r' && len > 0 )
                len--;
        }else {
            len = (int)strlen(start);
        }

        loadProperty( start, len, mapValues );

        start = end ? end + 1 : end;
    }
}

void RhoSettings::loadProperty( const char* start, int len, Hashtable<String,String>& mapValues )
{
    int nNameLen = 0;
    while(*start==' '){ start++; len--;}

    int i = 0;
    for( i = 0; i < len; i++ ){
        if ( start[i] == '=' ){
            if ( i > 0 ){
                int s = i-1;
                for(; s >= 0 && start[s]==' '; s-- );

                nNameLen = s+1;
                break;
            }else 
                break;
        }
    }

    if ( nNameLen == 0 )
        return;

    const char* szValue = start + i+1;
    int nValueLen = len - (i+1);

    while((*szValue==' ' || *szValue=='\'' || *szValue=='"') && nValueLen >= 0 ){ szValue++; nValueLen--;}
    while(nValueLen > 0 && (szValue[nValueLen-1]==' ' || szValue[nValueLen-1]=='\'' || szValue[nValueLen-1]=='"')) nValueLen--;

    setPropertyByName(start, nNameLen, szValue, nValueLen, mapValues );
}

void RhoSettings::setPropertyByName(const char* szName, int nNameLen, const char* szValue, int nValueLen, Hashtable<String,String>& mapValues )
{
    String name(szName,nNameLen);
    String value(szValue,nValueLen);
	//printf("name: %s, value: %s\n", name.c_str(), value.c_str());
    mapValues[name] = value;
}

void RhoSettings::saveChangesToString(String& strData)
{
    for ( std::map<String,String>::iterator it=m_mapChangedValues.begin() ; it != m_mapChangedValues.end(); it++ ) {
        strData += it->first;
        strData += "='";
        strData += it->second;
        strData += "'";
        strData += LOG_NEWLINE;
    }
}

const String& RhoSettings::getString(const char* szName){
	std::map<String,String>::iterator it = m_mapValues.find(szName);
	if ( it != m_mapValues.end() )
        return it->second;

    return m_EmptyString;
}

String RhoSettings::getPath(const char* szName){
    String strPath = getString(szName);
    if ( strPath.length() == 0 )
        return strPath;

    return CFilePath::join(strPath, "/");
}

int RhoSettings::getInt(const char* szName){
	std::map<String,String>::iterator it = m_mapValues.find(szName);
	if ( it != m_mapValues.end() )
        return atoi(it->second.c_str());
    return 0;
}

bool   RhoSettings::getBool(const char* szName){
    return getInt(szName) == 0 ? false : true;
}

void   RhoSettings::setString(const char* szName, const String& str, boolean bSaveToFile){
    m_mapValues[szName] = str;
    notifyListeners( szName, str );

    if ( bSaveToFile )
        saveToFile(szName);
}

void   RhoSettings::setInt(const char* szName, int nVal, boolean bSaveToFile){
    const String sVal = common::convertToStringA(nVal);
    m_mapValues[szName] = sVal;
    notifyListeners( szName, sVal );

    if ( bSaveToFile )
        saveToFile(szName);
}

void   RhoSettings::setBool(const char* szName, bool bVal, boolean bSaveToFile){
    setInt(szName, bVal?1:0, bSaveToFile);
}

bool   RhoSettings::isExist(const char* szName){
	std::map<String,String>::iterator it = m_mapValues.find(szName);
	return it != m_mapValues.end();
}

void  RhoSettings::removeProperty(const char* szName, boolean bSaveToFile)
{
    m_mapValues.remove(szName);
    if ( bSaveToFile ) 
        saveToFile( szName, true );
}

void RhoSettings::notifyListeners( const String& name, const String& newVal )
{
    for ( int i = 0; i < m_listeners.size(); ++i )
    {
        m_listeners[i]->onSettingUpdated( name, newVal );
    }
}

}
}

extern "C" {
	
void rho_conf_Init(const char* szRootPath)
{
    rho_conf_Init_with_separate_user_path(szRootPath, szRootPath);
}
    
void  rho_conf_Init_with_separate_user_path(const char* szRootPath, const char* szUserPath)
{
	rho::common::CFilePath oRhoPath( szRootPath );
	rho::common::CFilePath oUserPath( szUserPath );
    
    RHOCONF().setAppConfFilePath(oRhoPath.makeFullPath(CONF_FILENAME).c_str());
    RHOCONF().setAppConfUserFilePath(oUserPath.makeFullPath(CONF_FILENAME).c_str());
#ifdef RHODES_EMULATOR
    rho::String strPath = rho::common::CFilePath::join( szRootPath, RHO_EMULATOR_DIR);
    strPath = rho::common::CFilePath::join( strPath, CONF_FILENAME);
    RHOCONF().setConfFilePath(strPath.c_str());
#else
    RHOCONF().setConfFilePath(oRhoPath.makeFullPath(CONF_FILENAME).c_str());
#endif
    
    RHOCONF().loadFromFile();
}

void rho_conf_Init_from_shared_path(const char* szSharedRootPath)
{
    rho::common::CFilePath oRhoPath( szSharedRootPath );
    rho::common::CFilePath oUserPath( szSharedRootPath );
    
    RHOCONF().setAppConfFilePath(oRhoPath.makeFullPath(CONF_SHARED_FILENAME).c_str());
    RHOCONF().setAppConfUserFilePath(oUserPath.makeFullPath(CONF_SHARED_FILENAME).c_str());
    RHOCONF().setConfFilePath(oRhoPath.makeFullPath(CONF_SHARED_FILENAME).c_str());

    RHOCONF().loadFromFile();
}

int rho_conf_getBool(const char* szName) {
    return RHOCONF().getBool(szName) ? 1 : 0;
}

void rho_conf_setBool(const char* szName, bool value) {
	RHOCONF().setBool(szName,value,true);
}

int rho_conf_getInt(const char* szName) {
	return RHOCONF().getInt(szName);
}
	
void rho_conf_setInt(const char* szName, int value) {
	RHOCONF().setInt(szName,value,true);
}
	
char* rho_conf_getString(const char* szName) {
	return strdup(RHOCONF().getString(szName).c_str());
}

int rho_conf_is_property_exists(const char* name)
{
    return RHOCONF().isExist(name) ? 1 : 0;
}

void  rho_conf_remove_property(const char* name)
{
    RHOCONF().removeProperty(name, true);
}

void rho_conf_freeString(char* str) {
	if ( str )
		free(str);
}

void rho_conf_setString(const char* szName, const char* value){
    RHOCONF().setString(szName,value ? value : "", true);
}

//void rho_conf_save() {
//	RHOCONF().saveToFile();
//}

char* str_assign_ex( char* data, int len) 
{
	if (data) 
	{
		char* a = (char*)malloc(len+1);
		strncpy(a,data,len);
		a[len] = 0;
		return a;
	}
	return 0;
}

char* str_assign(char* data) 
{
	if (data) 
	{
		int len = strlen(data);
		return str_assign_ex(data,len);
	}
	return 0;
}
	
}
