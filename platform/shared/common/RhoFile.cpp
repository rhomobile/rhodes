#include "RhoFile.h"
#include "common/StringConverter.h"

namespace rho{
namespace common{

class CFileInputStream : public InputStream
{
    CRhoFile& m_oFile;
public:
    CFileInputStream(CRhoFile& oFile) : m_oFile(oFile){}

    virtual  int available(){ return m_oFile.size(); }
    virtual  int read(){ return m_oFile.readByte(); }
    virtual  int read(void* buffer, int bufOffset, int bytesToRead){ return m_oFile.readData(buffer, bufOffset, bytesToRead); }
    virtual  void reset(){ m_oFile.movePosToStart(); }
};

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

int CRhoFile::readByte()
{ 
    unsigned char buf[1];
    int nSize = fread(buf, 1, 1, m_file);

    return nSize > 0 ? buf[0] : -1;
}

int CRhoFile::readData(void* buffer, int bufOffset, int bytesToRead)
{ 
    int nSize = fread(((char*)buffer)+bufOffset, 1, bytesToRead, m_file);
    return nSize > 0 ? nSize : -1;
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
    common::convertToStringW(buf,strTextW);
    free(buf);
}

InputStream* CRhoFile::getInputStream()
{
    if ( m_pInputStream )
        delete m_pInputStream;

    m_pInputStream = new CFileInputStream(*this);
    return m_pInputStream;
}

void CRhoFile::flush(){
    if ( !isOpened() )
        return;

    fflush(m_file);
}

void CRhoFile::close(){
    if ( !isOpened() )
        return;

    if ( m_pInputStream )
        delete m_pInputStream;

    m_pInputStream = 0;

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

void CRhoFile::loadTextFile(const char* szFilePath, String& strFile)
{
    common::CRhoFile oFile;
    if ( oFile.open( szFilePath, common::CRhoFile::OpenReadOnly) )
        oFile.readString(strFile);
}

void CRhoFile::deleteFile( const char* szFilePath ){
#if defined(OS_WINDOWS) || defined(OS_WINCE)
    StringW wFileName;
    common::convertToStringW(szFilePath,wFileName);
    DeleteFile(wFileName.c_str());
#else
    remove(szFilePath);
#endif
}

void CRhoFile::deleteFilesInFolder(const char* szFolderPath)
{
#if defined(OS_WINDOWS) || defined(OS_WINCE)
    StringW wFolderName;
    common::convertToStringW(szFolderPath,wFolderName);
    StringW wFolderMask = wFolderName + L"/*";

    WIN32_FIND_DATAW FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFileW(wFolderMask.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) 
        return;

    while (FindNextFileW(hFind, &FindFileData) != 0) 
    {
        if ( FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
            continue;

        StringW wFileName = wFolderName + L"/" + FindFileData.cFileName;
        DeleteFileW(wFileName.c_str());
    }

    FindClose(hFind);

#else
    delete_files_in_folder(szFolderPath);
#endif
}

/*static*/ void CRhoFile::renameFile( const char* szOldFilePath, const char* szNewFilePath )
{
#if defined(OS_WINDOWS) || defined(OS_WINCE)
    StringW wNewFileName, wOldFileName;
    common::convertToStringW(szNewFilePath,wNewFileName);
    common::convertToStringW(szOldFilePath,wOldFileName);

	BOOL res = MoveFileW( wOldFileName.c_str(), wNewFileName.c_str());
#else
    rename( szOldFilePath, szNewFilePath );
#endif

}

}
}
