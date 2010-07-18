#ifndef _RHOFILE_H_
#define _RHOFILE_H_

#include "RhoStd.h"
#include "InputStream.h"

namespace rho{
namespace common{

class CRhoFile {
    FILE* m_file;
    String m_strPath;
    InputStream* m_pInputStream;

public:
    enum EOpenModes{ OpenForAppend = 1, OpenReadOnly = 2, OpenForWrite = 3, OpenForReadWrite = 4 };

    CRhoFile() : m_file(0), m_pInputStream(0){}
    ~CRhoFile(){close();}

    bool isOpened();
    bool open( const char* szFilePath, EOpenModes eMode );
    unsigned int write( const void* data, unsigned int len );
    void flush();
    void close();
    void movePosToStart();
    void movePosToEnd();
    void setPosTo(int nPos);
    unsigned int size();

    void readString(String& strData);
    void readStringW(StringW& strTextW);

    InputStream* getInputStream();
    int readByte();
    int readData(void* buffer, int bufOffset, int bytesToRead);

    static unsigned int getFileSize( const char* szFilePath );
    static bool         isFileExist( const char* szFilePath );
    static void         deleteFile( const char* szFilePath );
    static void         renameFile( const char* szOldFilePath, const char* szNewFilePath );
    static void         deleteFilesInFolder(const char* szFolderPath);
    static void         loadTextFile(const char* szFilePath, String& strFile);
    static void         createFolder(const char* szDirPath);
private:
    CRhoFile(const CRhoFile&);
    void operator=(const CRhoFile&);

};

}
}

extern "C" void delete_files_in_folder(const char *szFolderPath);

#endif //_RHOFILE_H_
