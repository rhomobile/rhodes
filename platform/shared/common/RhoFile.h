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
    static unsigned int deleteFile( const char* szFilePath );
    static unsigned int renameFile( const char* szOldFilePath, const char* szNewFilePath );
    static void         deleteFilesInFolder(const char* szFolderPath);
    static void         loadTextFile(const char* szFilePath, String& strFile);
    static unsigned int createFolder(const char* szDirPath);
    static void         recursiveCreateDir(const char* szFolderPath, const char* szBasePath);
    static void         readStringFromFile( const char* szFilePath, String& strData );
    static void         writeStringToFile( const char* szFilePath, String& strData );
    
    static unsigned int copyFile(const char* szSrcFile, const char* szDstFile);
    static unsigned int deleteFolder(const char* szFolderPath);
    static unsigned int copyFoldersContentToAnotherFolder(const char* szSrcFolderPath, const char* szDstFolderPath);
    static unsigned int moveFoldersContentToAnotherFolder(const char* szSrcFolderPath, const char* szDstFolderPath);

private:
    CRhoFile(const CRhoFile&);
    void operator=(const CRhoFile&);

};

}
}

extern "C" void rho_file_impl_delete_files_in_folder(const char *szFolderPath);

extern "C" void rho_file_impl_delete_folder(const char* szFolderPath);
extern "C" void rho_file_impl_copy_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath);
extern "C" void rho_file_impl_move_folders_content_to_another_folder(const char* szSrcFolderPath, const char* szDstFolderPath);

#endif //_RHOFILE_H_
