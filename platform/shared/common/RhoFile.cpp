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

#include "RhoFile.h"
#include "common/StringConverter.h"
#include "common/Tokenizer.h"
#include "common/RhoFilePath.h"

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

        if ( !m_file && !isFileExist(szFilePath) )
            m_file = fopen(szFilePath,"wb");

        if ( eMode == OpenForAppend )
            movePosToEnd();

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

    int nRes = fseek(m_file,0,SEEK_END);
    fpos_t pos;
    int nRes2 = fgetpos(m_file, &pos );

    if ( !nRes2 )
        nRes2 = 0;
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
    DeleteFileW(wFileName.c_str());
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

#if defined(WINDOWS_PLATFORM)
    extern "C" int _mkdir(const char * dir);
#endif

/*static*/ void CRhoFile::createFolder(const char* szFolderPath)
{
#if defined(WINDOWS_PLATFORM)
    _mkdir(szFolderPath);
#else
    mkdir(szFolderPath, S_IRWXU);
#endif
}

/*static*/ void CRhoFile::recursiveCreateDir(const char* szFolderPath, const char* szBasePath)
{
    String strRelPath = String(szFolderPath).substr(strlen(szBasePath), strlen(szFolderPath) );
    String strPath = szBasePath;
    CTokenizer oTokenizer( strRelPath, "/\\" );
	while (oTokenizer.hasMoreTokens()) 
    {
		String tok = oTokenizer.nextToken();
        strPath = CFilePath::join(strPath, tok);
        createFolder(strPath.c_str());
    }
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

/*static*/ void  CRhoFile::readStringFromFile( const char* szFilePath, String& strData )
{
    common::CRhoFile oFile;
    if ( oFile.open( szFilePath, common::CRhoFile::OpenReadOnly) )
    {
        oFile.readString(strData);
        oFile.close();
    }
}

/*static*/ void  CRhoFile::writeStringToFile( const char* szFilePath, String& strData )
{
    common::CRhoFile oFile;
    if ( oFile.open( szFilePath, common::CRhoFile::OpenForWrite) )
    {
        oFile.write(strData.c_str(), strData.length() );
        oFile.close();
    }
}
    
    /*static*/ void CRhoFile::copyFile(const char* szSrcFile, const char* szDstFile) {
        
        CRhoFile src;
        CRhoFile dst;
        
        if (!src.open(szSrcFile, OpenReadOnly)) {
            return;
        }
        if (!dst.open(szDstFile, OpenForWrite)) {
            src.close();
        }
        
        int buf_size = 1 << 16;
        unsigned char* buf = new unsigned char[buf_size];
        
        unsigned int to_copy = src.size();
        
        while (to_copy > 0) {
            int portion_size = buf_size;
            if (to_copy < portion_size) {
                portion_size = to_copy;
            }
            src.readData(buf, 0, portion_size);
            dst.write(buf, portion_size);
            
            to_copy -= portion_size;
        }
        
        src.close();
        dst.flush();
        dst.close();
        
        delete buf;
    }
    
    
    /*static*/ void  CRhoFile::deleteFolder(const char* szFolderPath) {
#if defined(OS_WINDOWS) || defined(OS_WINCE)
#else
        rho_delete_folder(szFolderPath);
#endif
    }
    
    /*static*/ void  CRhoFile::copyFoldersContentToAnotherFolder(const char* szSrcFolderPath, const char* szDstFolderPath) {
#if defined(OS_WINDOWS) || defined(OS_WINCE)
#else
        rho_copy_folders_content_to_another_folder(szSrcFolderPath, szDstFolderPath);
#endif
    }

}
}

#if defined(OS_MACOSX) || defined(OS_ANDROID)
// implemented in platform code
#else
extern "C" {
    
    void rho_delete_folder(const char* szFolderPath) {
        
    }
    
    void rho_copy_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath) {
        
    }
    
    
}
#endif

