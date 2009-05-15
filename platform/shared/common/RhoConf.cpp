#include "RhoConf.h"
#include "RhoFile.h"
#include "RhoFilePath.h"
#include "StringConverter.h"

static const char* CONF_FILENAME = "apps/rhoconfig.txt";

namespace rho{
namespace common{

RhoSettings g_RhoSettings;

void RhoSettings::saveToFile(){
    String strData;
    saveToString(strData);

    common::CRhoFile oFile;
    oFile.open(  getConfFilePath().c_str(), common::CRhoFile::OpenForWrite);

    oFile.write( strData.c_str(), strData.size() );
}

void RhoSettings::loadFromFile(){
    common::CRhoFile oFile;
    if ( oFile.open( getConfFilePath().c_str(), common::CRhoFile::OpenReadOnly) ){
        String strSettings;
        oFile.readString(strSettings);
        loadFromString( strSettings.c_str() );
    }
}

void RhoSettings::loadFromString(const char* szSettings){
    if ( !szSettings && !*szSettings )
        return;

    const char* start = szSettings;

    while(start!=0){
        int len = 0;

        const char* end = end = strchr(start,'\n');
        if (end){
            if ( end > start && *(end-1) == '\r' )
                len = end-start-1;
            else
                len = end-start;
        }else {
            len = (int)strlen(start);
        }

        loadProperty( start, len );

        start = end ? end + 1 : end;
    }
}

void RhoSettings::loadProperty( const char* start, int len ){
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

    while(*szValue==' ' || *szValue=='\'' || *szValue=='"' && nValueLen >= 0 ){ szValue++; nValueLen--;}
    while(nValueLen > 0 && (szValue[nValueLen-1]==' ' || szValue[nValueLen-1]=='\'' || szValue[nValueLen-1]=='"')) nValueLen--;

    setPropertyByName(start, nNameLen, szValue, nValueLen );
}

void RhoSettings::setPropertyByName(const char* szName, int nNameLen, const char* szValue, int nValueLen ){
    String name(szName,nNameLen);
    String value(szValue,nValueLen);
	printf("name: %s, value: %s\n", name.c_str(), value.c_str());
    m_mapValues[name] = value;
}

void RhoSettings::saveToString(String& strData){
	for ( std::map<String,String>::iterator it=m_mapValues.begin() ; it != m_mapValues.end(); it++ ) {
        strData += it->first;
        strData += "='";
        strData += it->second;
        strData += "'";
        strData += LOG_NEWLINE;
	}
}

String RhoSettings::getString(const char* szName){
	std::map<String,String>::iterator it = m_mapValues.find(szName);
	if ( it != m_mapValues.end() )
        return it->second;

    return String();
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

void   RhoSettings::setString(const char* szName, const String& str){
    m_mapValues[szName] = str;
}

void   RhoSettings::setInt(const char* szName, int nVal){
    m_mapValues[szName] = common::convertToStringA(nVal);
}

void   RhoSettings::setBool(const char* szName, bool bVal){
    setInt(szName, bVal?1:0);
}

bool   RhoSettings::isExist(const char* szName){
	std::map<String,String>::iterator it = m_mapValues.find(szName);
	return it != m_mapValues.end();
}

}
}

extern "C" void InitRhoConf(const char* szRootPath){
	rho::common::CFilePath oRhoPath( szRootPath );

    RHOCONF().setConfFilePath(oRhoPath.makeFullPath(CONF_FILENAME).c_str());
}