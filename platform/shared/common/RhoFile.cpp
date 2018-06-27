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
#include "logging/RhoLog.h"

#if defined(OS_WP8) || defined(OS_UWP)
extern "C" void recursiveDeleteDirectory(const std::wstring &path);
#endif

#if defined(WINDOWS_PLATFORM) && !defined(OS_WP8) && !defined(OS_UWP)
#include <windows.h>
#include <shellapi.h>
#endif


//#if !defined(WINDOWS_PLATFORM)
#if !defined(OS_UWP) && !defined(OS_WINCE)
#include <dirent.h>
#elif !defined(OS_WINCE)
#include "../win32/include/dirent.h"
#endif
//#else

#  ifndef S_ISDIR
#    define S_ISDIR(m) ((_S_IFDIR & m) == _S_IFDIR)
#  endif
#ifndef S_ISREG
#   define S_ISREG(m) ((m & S_IFMT) == S_IFREG)
#endif

#ifdef _WIN32_WCE
extern "C"{
#define rb_encoding void
#include "win32/dir.h"
}
#define dirent direct
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoFile"

#if defined(WINDOWS_PLATFORM)
    extern "C" int _mkdir(const char * dir);
    extern "C" int _rmdir(const char * dir);

#if !defined(OS_WP8) && !defined(OS_UWP) && !defined(RHODES_QT_PLATFORM)
    extern "C" int  _unlink(const char *path);
#endif

#endif

#if defined(OS_WINCE)
    extern "C" int  _rename(const char *, const char *);
#elif defined(OS_WINDOWS_DESKTOP) && !defined(RHODES_QT_PLATFORM)
    extern "C" int  rename(const char *, const char *);
#endif

namespace rho{
namespace common{

// Below is cross platform folder tree iteration algo implementation
// but it tested for Android only and removed from other platforms unlil tested
#ifdef OS_ANDROID

template <typename FileFunctor>
unsigned iterateFolderTree(const String& path, const FileFunctor& functor)
{
    unsigned res = 0;
    struct dirent *dent;
    DIR *dir;

    if (!(dir = opendir(path.c_str())))
    {
        LOG(ERROR) + "Can't open dir: " + path;
        return static_cast<unsigned>(errno);
    }

    while ((dent = readdir(dir))) {
        if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
            continue;

        String child = CFilePath::join(path, dent->d_name);
        if(child.length() > FILENAME_MAX)
        {
            LOG(ERROR) + "Path is too long: " + child;
            res = ENAMETOOLONG;
            continue;
        }

        unsigned res1 = functor(child.c_str() + strlen(functor.m_root));
        if(res1 != 0) res = res1;
    }

    if (dir) closedir(dir);

    return res;
}

struct RemoveFileFunctor {
    const char* m_root;
    RemoveFileFunctor(const char* root) : m_root(root) {}

    unsigned operator()(const String& rel_path) const
    {
        unsigned res = 0;
        String path = CFilePath::join(m_root, rel_path);
        if(CRhoFile::isDirectory(path.c_str()))
        {
            res = iterateFolderTree(path, *this);
            unsigned res1 = CRhoFile::deleteEmptyFolder(path.c_str());
            if(res1 != 0) res = res1;
        }
        else
        {
            res = CRhoFile::deleteFile(path.c_str());
        }
        return res;
    }
};

struct CopyFileFunctor {
    const char* m_root;
    const char* m_dst_root;
    CopyFileFunctor(const char* src_root, const char* dst_root)
        : m_root(src_root), m_dst_root(dst_root) {}

    unsigned operator()(const String& rel_path) const
    {
        unsigned res = 0;
        String path = CFilePath::join(m_root, rel_path);
        String dst_path = CFilePath::join(m_dst_root, rel_path);

        if(CRhoFile::isDirectory(path.c_str()))
        {
            if((res = CRhoFile::createFolder(dst_path.c_str())) == 0)
            {
                res = iterateFolderTree(path, *this);
            }
        }
        else
        {
            res = CRhoFile::copyFile(path.c_str(), dst_path.c_str());
        }
        return res;
    }
};

struct MoveFileFunctor {
    const char* m_root;
    const char* m_dst_root;
    MoveFileFunctor(const char* src_root, const char* dst_root)
        : m_root(src_root), m_dst_root(dst_root) {}

    unsigned operator()(const String& rel_path) const
    {
        unsigned res = 0;
        String path = CFilePath::join(m_root, rel_path);
        String dst_path = CFilePath::join(m_dst_root, rel_path);

        if(CRhoFile::isDirectory(path.c_str()))
        {
            if((res = CRhoFile::createFolder(dst_path.c_str())) == 0)
            {
                if((res = iterateFolderTree(path, *this)) == 0)
                {
                    CRhoFile::deleteEmptyFolder(path.c_str());
                }
            }
        }
        else
        {
            res = CRhoFile::renameFile(path.c_str(), dst_path.c_str());
        }
        return res;
    }
};

#endif

static wchar_t * translate_wchar(wchar_t *p, int from, int to)
{
    for (; *p; p++) {
	    if (*p == from)
	        *p = to;
        }
    return p;
}

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

bool CRhoFile::isOpened() const{
    return m_file!=0;
}

bool CRhoFile::open( const char* szFilePath, EOpenModes eMode ){
    m_strPath = szFilePath;
    if ( eMode == OpenForAppend || eMode == OpenForReadWrite ){
        m_file = fopen(szFilePath,"r+bc");

        if ( !m_file && !isFileExist(szFilePath) )
            m_file = fopen(szFilePath,"wbc");

        if ( eMode == OpenForAppend )
            movePosToEnd();

    }else if ( eMode == OpenReadOnly )
        m_file = fopen(szFilePath,"rbc");
    else if ( eMode == OpenForWrite )
        m_file = fopen(szFilePath,"wbc");
    
    return isOpened();
}

unsigned int CRhoFile::write( const void* data, unsigned int len ){
    if ( !isOpened() )
        return 0;

    return static_cast<unsigned int>( fwrite(data,1, len, m_file) );
}

int CRhoFile::readByte()
{
    if ( !isOpened() )
        return -1;
    
    unsigned char buf[1];
    int nSize = fread(buf, 1, 1, m_file);

    return nSize > 0 ? buf[0] : -1;
}

int CRhoFile::readData(void* buffer, int bufOffset, int bytesToRead)
{
    if ( !isOpened() )
        return -1;
    
    int nSize = fread(((char*)buffer)+bufOffset, 1, bytesToRead, m_file);
    return nSize > 0 ? nSize : -1;
}

void CRhoFile::readString(String& strData){
    if ( !isOpened() )
        return;
    
    unsigned int nSize = size();
    if (nSize > 0) {
        Vector<char> buffer;
        buffer.resize(nSize);
        nSize = fread(&buffer[0], 1, nSize, m_file);
        strData.assign(&buffer[0], nSize);
    } else {
        strData.clear();
    }
}

void CRhoFile::readStringW(StringW& strTextW)
{
    if ( !isOpened() )
        return;
    
    unsigned int nSize = size();
    if (nSize > 0) {
        Vector<char> buffer;
        buffer.resize(nSize+1);
        nSize = fread(&buffer[0], 1, nSize, m_file);
        buffer[nSize] = 0;
        // TODO: update converting function to use buffer size instead of \0
        common::convertToStringW(&buffer[0],strTextW);
    } else {
        strTextW.clear();
    }
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

int CRhoFile::getPos() const {
    if ( !isOpened() )
        return -1;

    #ifdef OS_LINUX
    off_t pos;
    if((pos = ftello(m_file))!= -1)
    #else
    fpos_t pos;
    if(fgetpos(m_file, &pos) == 0)
    #endif
    {
        return (int)pos;
    } else
    {
        return -1;
    }
}

bool CRhoFile::isEnd() const
{
    if(!isOpened())
        return false;

    return feof(m_file) != 0;
}

unsigned int CRhoFile::size() const {
    if ( !isOpened() )
        return 0;

    return getFileSize( m_strPath.c_str() );
}

void CRhoFile::truncate(unsigned int size) {
    if ( !isOpened() )
        return;

#ifdef WINDOWS_PLATFORM
    //_chsize(fileno(m_file), size);
#else
    ftruncate(fileno(m_file), size);
#endif

}

bool CRhoFile::isFileExist( const char* szFilePath ){
    struct stat st;
    memset(&st,0, sizeof(st));
#if defined(WINDOWS_PLATFORM)
    String strTemp;
    if ( szFilePath && *szFilePath && (szFilePath[strlen(szFilePath)-1] == '/' || szFilePath[strlen(szFilePath)-1] == '\\') )
    {
        strTemp = szFilePath;
        strTemp.erase(strTemp.size()-1);
        szFilePath = strTemp.c_str();
    }
#endif
    return stat(szFilePath, &st) == 0;
}

bool CRhoFile::isDirectory( const char* szFilePath ){
    bool res = false;
    struct stat st;
    memset(&st,0, sizeof(st));

#if defined(WINDOWS_PLATFORM)
    String strTemp;
    if ( szFilePath && *szFilePath && (szFilePath[strlen(szFilePath)-1] == '/' || szFilePath[strlen(szFilePath)-1] == '\\') )
    {
        strTemp = szFilePath;
        strTemp.erase(strTemp.size()-1);
        szFilePath = strTemp.c_str();
    }
#endif

    if (stat(szFilePath, &st) == 0)
    {
        return S_ISDIR(st.st_mode);
    }
    return res;
}
    
bool CRhoFile::isFile( const char* szFilePath ){
    bool res = false;
    struct stat st;
    memset(&st,0, sizeof(st));
    if (stat(szFilePath, &st) == 0)
    {
        return S_ISREG(st.st_mode);
    }

    return res;
}

unsigned int CRhoFile::getFileSize( const char* szFilePath ){
    struct stat st;
    memset(&st,0, sizeof(st));
    int rc = stat(szFilePath, &st);
    if ( rc == 0 && st.st_size > 0 )
        if (!S_ISDIR(st.st_mode))
            return st.st_size;

    return 0;
}

bool CRhoFile::loadTextFile(const char* szFilePath, String& strFile)
{
    common::CRhoFile oFile;
    bool result = false;
    if ( oFile.open( szFilePath, common::CRhoFile::OpenReadOnly) ) {
        oFile.readString(strFile);
        result = true;
    } else {
        strFile.clear();
    }
    return result;
}

unsigned int CRhoFile::deleteFile( const char* szFilePath ){
#if defined(WINDOWS_PLATFORM)
    return (unsigned int)_unlink(szFilePath);
#else
    return (unsigned int)remove(szFilePath);
#endif
}

unsigned int CRhoFile::deleteEmptyFolder( const char* szFilePath ){
#if defined(WINDOWS_PLATFORM)
    return (unsigned int)_rmdir(szFilePath);
#else
    return (unsigned int)rmdir(szFilePath);
#endif
}


void CRhoFile::deleteFilesInFolder(const char* szFolderPath)
{
#if defined(WINDOWS_PLATFORM)
    StringW wFolderName;
    common::convertToStringW(szFolderPath,wFolderName);
    StringW wFolderMask = wFolderName + L"/*";

    WIN32_FIND_DATAW FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFileW(wFolderMask.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE) 
        return;

    do{
        if ( FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
            continue;

        StringW wFileName = wFolderName + L"/" + FindFileData.cFileName;
        DeleteFileW(wFileName.c_str());
    }while (FindNextFileW(hFind, &FindFileData) != 0); 

    FindClose(hFind);

#else
    rho_file_impl_delete_files_in_folder(szFolderPath);
#endif
}

/*static*/ unsigned int CRhoFile::createFolder(const char* szFolderPath)
{
#if defined(WINDOWS_PLATFORM)
    return _mkdir(szFolderPath);
#else
    return mkdir(szFolderPath, S_IRWXU);
#endif
}

/*static*/ void CRhoFile::recursiveCreateDir(const char* szFolderPath, const char* szBasePath)
{
    String strRelPath = String(szFolderPath).substr(strlen(szBasePath), strlen(szFolderPath) );
    String strPath = szBasePath;
    // handle case when szBasePath is empty and szFolderPath is starting with /
    if (strPath.length() == 0 && strRelPath.length() > 0 && strRelPath[0] == '/')
    {
        strPath += '/';
    }
    
    CTokenizer oTokenizer( strRelPath, "/\\" );
	while (oTokenizer.hasMoreTokens()) 
    {
		String tok = oTokenizer.nextToken();
        strPath = CFilePath::join(strPath, tok);
        
        if (!isDirectory(strPath.c_str()))
        {
            createFolder(strPath.c_str());
        }
    }
}

/*static*/ unsigned int CRhoFile::renameFile( const char* szOldFilePath, const char* szNewFilePath )
{
#if defined(OS_WINCE)
    return _rename( szOldFilePath, szNewFilePath );

#else
    return rename( szOldFilePath, szNewFilePath );
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
    
/*static*/ unsigned int CRhoFile::copyFile(const char* szSrcFile, const char* szDstFile) {
    
    CRhoFile src;
    CRhoFile dst;
    
    if (!src.open(szSrcFile, OpenReadOnly)) {
        return -1;
    }
    if (!dst.open(szDstFile, OpenForWrite)) {
        return -1;
    }
    
    unsigned int buf_size = 1 << 16;
    unsigned char* buf = new unsigned char[buf_size];
    
    unsigned int to_copy = src.size();
    
    while (to_copy > 0) {
        unsigned int portion_size = buf_size;
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
    
    delete[] buf;

    return 0;
}
    
/*static*/ unsigned int CRhoFile::deleteFolder(const char* szFolderPath) 
{
#if defined(WINDOWS_PLATFORM) && !defined(OS_WP8) && !defined(OS_UWP)

	StringW  swPath;
    convertToStringW(szFolderPath, swPath);
	wchar_t* name = new wchar_t[ swPath.length() + 2];
    swprintf(name, L"%s%c", swPath.c_str(), '\0');
    translate_wchar(name, L'/', L'\\');

    SHFILEOPSTRUCTW fop = {0};

	fop.hwnd = NULL;
	fop.wFunc = FO_DELETE;		
        fop.pFrom = name;
	fop.pTo = NULL;
	fop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR 
#if defined(OS_WINDOWS_DESKTOP) || defined(OS_WINCE)
                 | FOF_NOERRORUI
#endif        
        ;
        int result = SHFileOperationW(&fop);

    delete name;

    return result == 0 ? 0 : (unsigned int)-1;
#elif defined(OS_WP8) || defined(OS_UWP)
	StringW  swPath;
    convertToStringW(szFolderPath, swPath);
	recursiveDeleteDirectory(swPath);
	return 0;
#elif defined (OS_ANDROID)
    
    RemoveFileFunctor funct(szFolderPath);
    return funct("");

#else
    rho_file_impl_delete_folder(szFolderPath);
    return 0;
#endif
}

bool CRhoFile::listFolderEntries( const String& path, Vector<String> &entries)
{
    bool ret = false;
//#ifndef RHO_NO_RUBY
    DIR *dp;
    struct dirent *ep;
    dp = opendir (path.c_str());

    
    if (dp != 0)
    {
        ret = true;
#ifdef OS_WINCE
        entries.push_back(".");
        entries.push_back("..");
#endif

        while ( (ep = readdir (dp)) != 0 ) 
        {
            if ( ep->d_name && *ep->d_name)
                entries.push_back(ep->d_name);
        }
        
        closedir(dp);
    }
//#endif //RHO_NO_RUBY
    return ret;
}
    
#if defined(WINDOWS_PLATFORM)
static unsigned int copyFolder(const StringW& strSrc, const StringW& strDst, boolean bMove)
{
    unsigned int nErr = 0;

    CRhoFile::createFolder(convertToStringA(strDst).c_str());

    StringW wFolderMask = CFilePath::join(strSrc, L"*");

    WIN32_FIND_DATAW FindFileData = {0};
    HANDLE hFind = INVALID_HANDLE_VALUE;

    hFind = FindFirstFileW(wFolderMask.c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        DWORD dwErr = GetLastError();
        if (dwErr == ERROR_NO_MORE_FILES)
            return 0;

        return dwErr;
    }

    do{
        if (!wcscmp(FindFileData.cFileName , L".")) continue ;
		if (!wcscmp(FindFileData.cFileName , L"..")) continue ;

        if ( FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
        {
            int i = 0;
            nErr = copyFolder( CFilePath::join(strSrc,FindFileData.cFileName), CFilePath::join(strDst,FindFileData.cFileName), bMove );
        }
        else
        {
            if ( bMove )
            {
                CRhoFile::deleteFile(convertToStringA(CFilePath::join(strDst,FindFileData.cFileName)).c_str());
                nErr = CRhoFile::renameFile(convertToStringA(CFilePath::join(strSrc,FindFileData.cFileName)).c_str(), convertToStringA(CFilePath::join(strDst,FindFileData.cFileName)).c_str());
            }
            else
                nErr = CRhoFile::copyFile( convertToStringA(CFilePath::join(strSrc,FindFileData.cFileName)).c_str(), convertToStringA(CFilePath::join(strDst,FindFileData.cFileName)).c_str() );
        }

        if ( nErr != 0 )
            return nErr;
    }while (FindNextFileW(hFind, &FindFileData) != 0); 

    FindClose(hFind);

    return 0;
}
#endif

/*static*/ unsigned int CRhoFile::copyFoldersContentToAnotherFolder(const char* szSrcFolderPath, const char* szDstFolderPath) 
{
#if defined(WINDOWS_PLATFORM)

    StringW strSrcW, strDstW;
    common::convertToStringW(szSrcFolderPath,strSrcW);
    common::convertToStringW(szDstFolderPath,strDstW);
    String_replace(strSrcW, L'/', L'\\' );
    String_replace(strDstW, L'/', L'\\' );

    return copyFolder(strSrcW, strDstW, false);
#elif defined (OS_ANDROID)

    return iterateFolderTree(String(szSrcFolderPath), CopyFileFunctor(szSrcFolderPath, szDstFolderPath));

#else
    rho_file_impl_copy_folders_content_to_another_folder(szSrcFolderPath, szDstFolderPath);
    return 0;
#endif
}

/*static*/ unsigned int CRhoFile::moveFoldersContentToAnotherFolder(const char* szSrcFolderPath, const char* szDstFolderPath) 
{
#if defined(WINDOWS_PLATFORM)

    StringW strSrcW, strDstW;
    common::convertToStringW(szSrcFolderPath,strSrcW);
    common::convertToStringW(szDstFolderPath,strDstW);
    String_replace(strSrcW, L'/', L'\\' );
    String_replace(strDstW, L'/', L'\\' );

    return copyFolder(strSrcW, strDstW, true);
#elif defined (OS_ANDROID)

    return iterateFolderTree(String(szSrcFolderPath), MoveFileFunctor(szSrcFolderPath, szDstFolderPath));

#else
    rho_file_impl_move_folders_content_to_another_folder(szSrcFolderPath, szDstFolderPath);
    return 0;
#endif
}

}
}

#ifndef OS_ANDROID
extern "C" void rho_file_set_fs_mode(int mode) {
}
#endif

#if defined(OS_MACOSX) || defined(OS_ANDROID) || defined(OS_LINUX)
    void rho_file_impl_move_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath) {
        
    }
#elif !defined(RHODES_QT_PLATFORM)
extern "C" {
    
    void rho_file_impl_delete_folder(const char* szFolderPath) {
        
    }
    
    void rho_file_impl_copy_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath) {
        
    }
    
    void rho_file_impl_move_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath) {
        
    }
    
}
#endif
