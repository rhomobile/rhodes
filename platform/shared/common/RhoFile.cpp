#include "RhoFile.h"
#include "common/StringConverter.h"

#if defined(OS_WINCE)
#include "wince/sys/types.h"
#include "wince/sys/stat.h"
#else
#include <sys/stat.h>
#endif

namespace rho{
namespace general{

    bool CRhoFile::isOpened(){
        return m_file!=0;
    }

    bool CRhoFile::open( const char* szFilePath, EOpenModes eMode ){
        m_strPath = szFilePath;
        if ( eMode == OpenForAppend || eMode == OpenForReadWrite ){
            m_file = fopen(szFilePath,"r+b");

            if ( eMode == OpenForAppend )
                movePosToEnd();

            if ( !m_file && !isFileExist(szFilePath) )
                m_file = fopen(szFilePath,"wb");
        }else if ( eMode == OpenReadOnly )
            m_file = fopen(szFilePath,"rb");
        else if ( eMode == OpenForWrite )
            m_file = fopen(szFilePath,"wb");
        
        return isOpened();
    }

    unsigned int CRhoFile::write( const void* data, unsigned int len ){
        if ( !isOpened() )
            return 0;

        return static_cast<unsigned int>( fwrite(data,1, len, m_file) );
    }

    void CRhoFile::readString(String& strData){
        if ( !isOpened() )
            return;

        int nSize = size();
        strData.resize(nSize);
        nSize = fread(&strData[0], 1, nSize, m_file);
        strData[nSize] = 0;
    }

    void CRhoFile::readStringW(StringW& strTextW)
    {
        if ( !isOpened() )
            return;

        int nSize = size();
        char* buf = (char*)malloc(nSize+1);
        nSize = fread(buf, 1, nSize, m_file);
        buf[nSize] = 0;
        general::convertToStringW(buf,strTextW);
        free(buf);
    }

    void CRhoFile::flush(){
        if ( !isOpened() )
            return;

        fflush(m_file);
    }

    void CRhoFile::close(){
        if ( !isOpened() )
            return;

        fclose(m_file);
        m_file = 0;
    }

    void CRhoFile::movePosToStart(){
        if ( !isOpened() )
            return;

        fseek(m_file,0,SEEK_SET);
    }

    void CRhoFile::movePosToEnd(){
        if ( !isOpened() )
            return;

        fseek(m_file,0,SEEK_END);
    }

    void CRhoFile::setPosTo(int nPos){
        if ( !isOpened() || nPos < 0 )
            return;

        fseek(m_file,nPos,SEEK_SET);
    }

    unsigned int CRhoFile::size(){
        if ( !isOpened() )
            return 0;

        return getFileSize( m_strPath.c_str() );
    }

    bool CRhoFile::isFileExist( const char* szFilePath ){
        struct stat st;
        memset(&st,0, sizeof(st));
        return stat(szFilePath, &st) == 0;
    }

    unsigned int CRhoFile::getFileSize( const char* szFilePath ){
        struct stat st;
        memset(&st,0, sizeof(st));
        int rc = stat(szFilePath, &st);
        if ( rc == 0 && st.st_size > 0 )
            return st.st_size;

        return 0;
    }
}
}
