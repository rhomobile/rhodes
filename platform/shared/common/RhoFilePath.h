#ifndef _RHOFILEPATH_H_
#define _RHOFILEPATH_H_

#include "RhoStd.h"

namespace rho{
namespace common{

class CFilePath{
public:
    CFilePath(const char* path) : m_szPath(path){}
    CFilePath(const String& str) : m_szPath(str.c_str()){}

    const char* getBaseName(){ 
        const char* base = findLastSlash();
        if (base)
            return base+1;

        return m_szPath;
    }

    String makeFullPath(const char* szFileName){
        String res = m_szPath;
        if ( res.length() > 0 && !findLastSlash() )
            res += "/";

        res += szFileName;
        
        return res;
    }

    String changeBaseName( const char* szFileName )
    {
        const char* base = findLastSlash();
        if ( base && *(base+1) ){
            String res( m_szPath, base-m_szPath+1);
            res += szFileName;

            return res;
        }

        return makeFullPath(szFileName);
    }

private:

    const char* findLastSlash(){
        const char* slash = strrchr(m_szPath, '/');
        if ( !slash )
            slash = strrchr(m_szPath, '\\');

        return slash;
    }

    const char* m_szPath;
};

}
}

#endif //_RHOFILEPATH_H_
