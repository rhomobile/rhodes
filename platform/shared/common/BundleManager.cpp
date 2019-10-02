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

#include "RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoFile.h"
#include "common/RhoConf.h"
#include "common/RhoFilePath.h"
#include "common/Tokenizer.h"

#include "ruby/ext/rho/rhoruby.h"
#include <algorithm>


#include "net/INetRequest.h"

#if defined(OS_WINCE)
#include <net/CompatWince.h>
#endif

using namespace rho;
using namespace rho::common;
extern "C" void rho_sys_app_exit();
extern "C" void rho_sys_impl_exit_with_errormessage(const char* szTitle, const char* szMsg);

#if defined(OS_MACOSX) && !defined(RHODES_EMULATOR)
extern "C" int rho_prepare_folder_for_upgrade(const char* szPath);
#endif

#if defined OS_ANDROID
extern "C" void rho_android_file_reload_stat_table();
extern "C" void rho_android_force_all_files();
extern "C" int rho_android_remove_item(const char* path);

extern "C" void rho_file_set_fs_mode(int mode);
extern "C" int rho_file_get_fs_mode();

#endif


#if !defined(OS_WINDOWS_DESKTOP) && !defined(OS_WINCE) && !defined(OS_MACOSX) && !defined(OS_LINUX)
void rho_sys_impl_exit_with_errormessage(const char* szTitle, const char* szMsg)
{
}
#endif

extern "C" void rho_startup_logging(const char* message);


extern "C" void rho_file_operation_test() {
    
}





namespace rho {
namespace common {

class CFileTransaction
{
    unsigned int m_nError;
    String m_strError;
    String m_strFolder;
    boolean m_bRollbackInDestr;
public:
    CFileTransaction(const String& strFolder, boolean bRollbackInDestr = true);
    ~CFileTransaction();
    unsigned int start();
    void commit();
    unsigned int rollback();

    boolean isError(){ return m_nError != 0; }
    unsigned int getError(){ return m_nError; }
    String getErrorText(){ return m_strError; }

};
    
    
    String rho_normalize_path_string(String& p) {
        char* buf = NULL;
        int i;
        int res_len = 0;
        int len = (int)p.length();
        const char* str = p.c_str();
        buf = new char[len+1];
        for (i = 0; i < len; i++) {
            char c = str[i];
            
            if ((c == 0x0D) || (c == 0x0A)) {
                c = 0;
            }
            if (c == '\\') {
                c = '/';
            }
            if (c != 0) {
                buf[res_len] = c;
                res_len++;
            }
        }
        
        if (buf != NULL) {
            buf[res_len] = 0;
            String res_s = buf;
            delete buf;
            return res_s;
        }
        return p;
    }
    

    
class CFileList 
{
public:
    CFileList();
    virtual ~CFileList();
    
    bool removeFile(const String& path); 
    void removeFolder(const String& path); 
    void addFile(const String& full_line); 
    void addFolder(const String& full_line);
    
    int getItemCount();
    String getItem(int index);
    int getItemDetails(int index, String* path, String* type, String* size, String* mtime, String* crc);
    int findItemByPath(String path);
    
    void loadFromFile(const String& filelist_path, const String& prefix);
    void saveToFile();

private:
    rho::Vector<String> mLines;
    
    String mFilePath;
    
    String mPrefix;

};

    CFileList::CFileList() {
    }
    
    CFileList::~CFileList() {
        
    }
    
    int CFileList::getItemCount() {
        return mLines.size();
    }
    
    String CFileList::getItem(int index) {
        return mLines.at(index);
    }

    
    int CFileList::getItemDetails(int index, String* path, String* type, String* size, String* mtime, String* crc) {
        String line = mLines.at(index);
        int count = 0;
        CTokenizer oTokenizer( line, "|" );
        if (oTokenizer.hasMoreTokens()) {
            String v = oTokenizer.nextToken();
            if (path != NULL) {
                *path = v;
            }
            count++;
        }
        if (oTokenizer.hasMoreTokens()) {
            String v = oTokenizer.nextToken();
            if (type != NULL) {
                *type = v;
            }
            count++;
        }
        if (oTokenizer.hasMoreTokens()) {
            String v = oTokenizer.nextToken();
            if (size != NULL) {
                *size = v;
            }
            count++;
        }
        if (oTokenizer.hasMoreTokens()) {
            String v = oTokenizer.nextToken();
            if (mtime != NULL) {
                *mtime = v;
            }
            count++;
        }
        if (oTokenizer.hasMoreTokens()) {
            String v = oTokenizer.nextToken();
            if (crc != NULL) {
                *crc = v;
            }
            count++;
        }
        return count;
    }
    
    int CFileList::findItemByPath(String path) {
        int i;
        for (i = 0; i < getItemCount(); i++) {
            String ipath = "";
            getItemDetails(i, &ipath, NULL,NULL,NULL,NULL);
            if (path.compare(ipath) == 0) {
                return i;
            }
        }
        return -1;
    }
    
    
    bool CFileList::removeFile(const String& path) {
        String f = mPrefix + path;
        LOG(TRACE) + "CFileList::removeFile( " + f + ") :";
        bool founded = false;
        Vector<String>::iterator it;
        String to_find = f + "|";
        
        int i;
        for (i = 0; i < mLines.size(); i++) {
            String& line = mLines.at(i);
            if (rho::String_startsWith(line, to_find)) {
                mLines.removeElementAt(i);
                i--;
                founded = true;
            }
        }
        return founded;
    }
    
    void CFileList::removeFolder(const String& path) {
        String f = mPrefix + path;
        LOG(TRACE) + "CFileList::removeFolder( " + f + ") :";
        removeFile(path);
        String to_find = CFilePath::join(f, "");
        Vector<String>::iterator it;
        int i;
        for (i = 0; i < mLines.size(); i++) {
            String& line = mLines.at(i);
            if (rho::String_startsWith(line, to_find)) {
                mLines.removeElementAt(i);
                i--;
            }
        }
    }
    
    void CFileList::addFile(const String& full_line) {
        String f = mPrefix + full_line;
        LOG(TRACE) + "CFileList::addFile( " + f + ") :";
        mLines.push_back(f);
    }
    
    void CFileList::addFolder(const String& full_line) {
        String f = mPrefix + full_line;
        LOG(TRACE) + "CFileList::addFolder( " + f + ") :";
        mLines.push_back(f);
    }
    
    void CFileList::saveToFile() {
        LOG(TRACE) + "CFileList::saveToFile( " + mFilePath + ") :";

        common::CRhoFile oFile;
        String n = "\n";
        if ( oFile.open( mFilePath.c_str(), common::CRhoFile::OpenForWrite) )
        {
            std::sort(mLines.begin(), mLines.end());
            int i;
            for (i = 0; i < mLines.size(); i++) {
                String& line = mLines.at(i);
                oFile.write(line.c_str(),line.length());
                oFile.write(n.c_str(), n.length());
                LOG(TRACE) + "                " + line;
            }
            oFile.close();
        }    
    }
    
    void CFileList::loadFromFile(const String& filelist_path, const String& prefix) {
        mPrefix = prefix;
        mFilePath = filelist_path;
        LOG(TRACE) + "CFileList::loadFromFile( " + mFilePath + ") :";

        String strList;
        CRhoFile::loadTextFile(mFilePath.c_str(), strList);
        
        CTokenizer oTokenizer( strList, "\n" );
        while (oTokenizer.hasMoreTokens()) 
        {
            String strLine = oTokenizer.nextToken();
            if (strLine.length() > 3) {
                strLine = rho_normalize_path_string(strLine);
                mLines.push_back(strLine);
                LOG(TRACE) + "                " + strLine;
            }
        }
    }
    
    
    
common::CMutex   m_mxBundleReplaceMutex;
    
    
    
    
    

class CReplaceBundleThread : public rho::common::CRhoThread
{
    DEFINE_LOGCLASS;

    String m_bundle_path;
    static String m_finish_callback;
    bool mDoNotRestartApp;
    
    static bool mCheckFileList;
    static bool mAllViaCallback;
    
    bool* m_is_finished_flag;

    void doReplaceBundle();
    
    void sendRequestFullUpdate();
    
    bool is_need_full_update(CFileList* old_filelist, CFileList* new_filelist, CFileList* add_list, CFileList* remove_list);
public:

    CReplaceBundleThread(const char* root_path, const char* finish_callback, bool do_not_restart_app, bool* is_finished_flag, bool is_check_filelist, bool is_all_responce_only_via_callback)
    {
        m_bundle_path = root_path;
        if (finish_callback != NULL) {
            m_finish_callback = finish_callback;
        }
        else {
            m_finish_callback = "";
        }
        mDoNotRestartApp = do_not_restart_app;
        m_is_finished_flag = is_finished_flag;
        mCheckFileList = is_check_filelist;
        mAllViaCallback = is_all_responce_only_via_callback;
        start(rho::common::IRhoRunnable::epNormal);
    }
    
    virtual ~CReplaceBundleThread() {}
    
    virtual void run();

    static void showError(int nError, const String& strError );

    static unsigned int removeFilesByList( const String& strListPath, const String& strSrcFolder, bool skipNotExist );
    static unsigned int moveFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDstFolder );
    
    static unsigned int partialAddFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDstFolder, CFileList* filelist, CFileList* filelist_apps);
    static unsigned int partialRemoveItemsByList( const String& strListPath, const String& strSrcFolder, CFileList* filelist, CFileList* filelist_apps, String tmp_remove_folder);
    
};
IMPLEMENT_LOGCLASS(CReplaceBundleThread,"RhoBundle");
    
String CReplaceBundleThread::m_finish_callback;
bool CReplaceBundleThread::mCheckFileList;
bool CReplaceBundleThread::mAllViaCallback;
   

CFileTransaction::CFileTransaction(const String& strFolder, boolean bRollbackInDestr) : m_strFolder(strFolder), m_bRollbackInDestr(bRollbackInDestr)
{
}

unsigned int CFileTransaction::start()
{
    LOG(TRACE) + "FileTransaction is starting: " + m_strFolder;
    LOG(TRACE) + "Creating folder" + m_strFolder + "_temp_journal";

    CRhoFile::createFolder((m_strFolder + "_temp_journal").c_str());

    LOG(TRACE) + "Coping: " + CFilePath::join(m_strFolder, "") + " -> " + m_strFolder + "_temp_journal";

    m_nError = CRhoFile::copyFoldersContentToAnotherFolder( CFilePath::join(m_strFolder, "").c_str(), (m_strFolder + "_temp_journal").c_str() );
    if (m_nError)
    {
        m_strError = "Unable to copy folder: " + m_strFolder + " to : " + m_strFolder + "_temp_journal";
        return m_nError;
    }

    LOG(TRACE) + "Renaming: " + m_strFolder + "_temp_journal" + " -> " + m_strFolder + "_journal";

    m_nError = CRhoFile::renameFile( (m_strFolder + "_temp_journal").c_str(), (m_strFolder + "_journal").c_str());
    if (m_nError)
    {
        m_strError = "Unable to rename folder: " + m_strFolder + "_temp_journal to : " + m_strFolder + "_journal";
        return m_nError;
    }

    LOG(TRACE) + "FileTransaction has started";

    return m_nError;
}

CFileTransaction::~CFileTransaction()
{
    if (m_bRollbackInDestr) {
        LOG(INFO) + "rollback() in ~CFileTransaction";
        rollback();
    }
}

void CFileTransaction::commit()
{
    LOG(TRACE) + "FileTransaction is committing: " + m_strFolder;
    String strFolder = m_strFolder;
    
    LOG(TRACE) + "Renaming: " + strFolder + "_journal" + " -> " + strFolder + "_temp_journal";

    m_nError = CRhoFile::renameFile( (strFolder + "_journal").c_str(), (strFolder + "_temp_journal").c_str() );
    if (m_nError)
    {
        m_strError = "Unable to rename folder: " + strFolder + "_journal to : " + strFolder + "_temp_journal";
        return;
    }
    m_strFolder = "";

    LOG(TRACE) + "Deleting: " + strFolder + "_temp_journal";

    m_nError = CRhoFile::deleteFolder( (strFolder + "_temp_journal").c_str() );
    if (m_nError)
    {
        m_strError = "Unable to delete folder: " + strFolder + "_temp_journal";
        return;
    }

    LOG(TRACE) + "FileTransaction has committed";
}

unsigned int CFileTransaction::rollback()
{
    if (rho_log_system_is_ready()) {
        LOG(TRACE) + "FileTransaction is rolling back: " + m_strFolder;
    }
    String strFolder = m_strFolder;
    m_strFolder = "";

    if ( strFolder.length() == 0 ) {
        if (rho_log_system_is_ready()) {
            LOG(TRACE) + "FileTransaction rolling back: empty transaction" + m_strFolder;
        }
        return 0;
    }

    CRhoFile::deleteFolder( CFilePath(strFolder).changeBaseName("RhoBundle").c_str() );
    CRhoFile::deleteFolder( (strFolder + "_temp_journal").c_str() );

    if ( !CRhoFile::isFileExist((strFolder + "_journal").c_str()) )
        if (rho_log_system_is_ready()) {
            LOG(TRACE) + "FileTransaction stop: file not exist : " + (strFolder + "_journal");
        }
        return 0;

    m_nError = CRhoFile::deleteFolder( (strFolder).c_str() );
    if (m_nError)
    {
        m_strError = "Unable to delete folder: " + strFolder;
        CReplaceBundleThread::showError(m_nError, m_strError);
        return m_nError;
    }

    m_nError = CRhoFile::renameFile( (strFolder + "_journal").c_str(), (strFolder).c_str() );
    if (m_nError)
    {
        m_strError = "Unable to rename folder: " + strFolder + "_journal" + " to : " + strFolder;
        CReplaceBundleThread::showError(m_nError, m_strError);
        return m_nError;
    }

    if (rho_log_system_is_ready()) {
        LOG(TRACE) + "FileTransaction has rolled back";
    }

    return m_nError;
}

unsigned int CReplaceBundleThread::removeFilesByList( const String& strListPath, const String& strSrcFolder, bool skipNotExist )
{
	LOG(TRACE) + "Removing files by list: " + strSrcFolder + ", list: " + strListPath;

    unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);


    CTokenizer oTokenizer( strList, "\n" );
	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();
        strLine = rho_normalize_path_string(strLine);
        
        if (strLine.length() < 4)
            continue;

        CTokenizer oLineTok( strLine, "|" );
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strPath = oLineTok.nextToken();
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strType = oLineTok.nextToken();

        if ( strType.compare("file") == 0)
        {
            LOG(TRACE) + "Deleting file: " + CFilePath::join( strSrcFolder,strPath);

            if (CRhoFile::isFileExist(CFilePath::join( strSrcFolder,strPath).c_str()) ) {
                nError = CRhoFile::deleteFile( CFilePath::join( strSrcFolder,strPath).c_str() );
                if (nError != 0)
                {
                    LOG(ERROR) + "Cannot remove file: " + CFilePath::join( strSrcFolder,strPath);
                    break;
                }
            }
            else {
                if (!skipNotExist) {
                    LOG(ERROR) + "Cannot remove file (not found): " + CFilePath::join( strSrcFolder,strPath);
                    nError = 1;
                    break;
                }
                else {
                    LOG(TRACE) + "Cannot remove file (not found): " + CFilePath::join( strSrcFolder,strPath);
                }
            }
        }
    }

    return nError;
}

unsigned int CReplaceBundleThread::moveFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDestFolder )
{
	LOG(TRACE) + "Moving files by list: " + strSrcFolder + " -> " + strDestFolder;

	unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);

    CTokenizer oTokenizer( strList, "\n" );
	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();
        strLine = rho_normalize_path_string(strLine);
        
        if (strLine.length() < 4)
            continue;

        CTokenizer oLineTok( strLine, "|" );
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strPath = oLineTok.nextToken();
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strType = oLineTok.nextToken();
        String strSrcPath = CFilePath::join( strSrcFolder,strPath);
        String strDstPath = CFilePath::join( strDestFolder, strPath );

        if ( strType.compare("dir") == 0)
        {
            LOG(TRACE) + "Creating folder: " + strDstPath;
            if (!CRhoFile::isFileExist(strDstPath.c_str())) {
                CRhoFile::createFolder(strDstPath.c_str());
            }
        }else if ( strType.compare("file") == 0)
        {
            LOG(TRACE) + "Renaming file: " + strSrcPath + " -> " + strDstPath;
            nError = CRhoFile::renameFile( strSrcPath.c_str(), strDstPath.c_str() );
            if (nError != 0)
            {
                LOG(ERROR) + "Cannot rename file from : " + strSrcPath + "; to: " + strDstPath;
                break;
            }
        }
    }

    return nError;
}

    
unsigned int CReplaceBundleThread::partialAddFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDstFolder, CFileList* filelist, CFileList* filelist_apps) {
	LOG(TRACE) + "Partial Moving files by list: " + strSrcFolder + " -> " + strDstFolder;
    
	unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);
    const char nl[] = "\n";
    CTokenizer oTokenizer( strList, nl );
	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();
        strLine = rho_normalize_path_string(strLine);
        
        if (strLine.length() > 0) {
            if ((strLine[0] == 0xD) || (strLine[0] == 0xA)) {
                strLine = strLine.substr(1, strLine.length()-1);
            }
            if ((strLine[strLine.length()-1] == 0xD) || (strLine[strLine.length()-1] == 0xA)) {
                strLine = strLine.substr(0, strLine.length()-1);
            }
        }
        if (strLine.length() < 4)
            continue;
        
        CTokenizer oLineTok( strLine, "|" );
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strPath = oLineTok.nextToken();
        if ( !oLineTok.hasMoreTokens() )
            continue;
        
        if ((strPath.length() <= 0) || (strPath.compare("upgrade_package_add_files.txt") == 0)) {
            continue;
        }
        
        String strType = oLineTok.nextToken();
        String strSrcPath = CFilePath::join( strSrcFolder,strPath);
        String strDstPath = CFilePath::join( strDstFolder, strPath );
        
        if ( strType.compare("dir") == 0)
        {
            LOG(TRACE) + "Creating folder: " + strDstPath;
            if (!CRhoFile::isFileExist(strDstPath.c_str())) {
                CRhoFile::createFolder(strDstPath.c_str());
                filelist->addFolder(strLine);
                filelist_apps->addFolder(strLine);
            }
        }else if ( strType.compare("file") == 0)
        {
            LOG(TRACE) + "Renaming file: " + strSrcPath + " -> " + strDstPath;

            bool must_be_exist = filelist->removeFile(strPath);
            filelist_apps->removeFile(strPath);

#ifdef OS_ANDROID
            must_be_exist = false;
#endif

            if (CRhoFile::isFileExist( strDstPath.c_str()) ) {
                
                nError = CRhoFile::deleteFile( strDstPath.c_str() );
#ifdef OS_ANDROID
                nError = 0;
#endif                
                if (nError != 0)
                {
                    LOG(ERROR) + "Cannot remove file (for replace): " + strDstPath;
                    break;
                }
            }
            else {
                if (must_be_exist) {
                    LOG(ERROR) + "Cannot remove file (not found for replace): " + strDstPath;
                    break;
                }
            }
            
            nError = CRhoFile::renameFile( strSrcPath.c_str(), strDstPath.c_str() );
            if (nError != 0)
            {
                LOG(ERROR) + "Cannot rename file from : " + strSrcPath + "; to: " + strDstPath;
                break;
            }
            else {
                filelist->addFile(strLine);
                filelist_apps->addFile(strLine);
            }
        }
    }
    
    return nError;
}
    
unsigned int CReplaceBundleThread::partialRemoveItemsByList( const String& strListPath, const String& strSrcFolder, CFileList* filelist, CFileList* filelist_apps, String tmp_remove_folder) {
	LOG(TRACE) + "Partial Removing items by list: " + strSrcFolder + ", list: " + strListPath;
    
    unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);
    
    
    const char nl[] = "\n";
    CTokenizer oTokenizer( strList, nl );

	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();
        strLine = rho_normalize_path_string(strLine);

        if (strLine.length() > 0) {
            if ((strLine[0] == 0xD) || (strLine[0] == 0xA)) {
                strLine = strLine.substr(1, strLine.length()-1);
            }
            if ((strLine[strLine.length()-1] == 0xD) || (strLine[strLine.length()-1] == 0xA)) {
                strLine = strLine.substr(0, strLine.length()-1);
            }
        }
        
        if (strLine.length() < 4)
            continue;
        CTokenizer oLineTok( strLine, "|" );
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strPath = oLineTok.nextToken();

        if (strPath.length() <= 0) {
            continue;
        }
        
        bool is_dir = CRhoFile::isDirectory(CFilePath::join( strSrcFolder,strPath).c_str());
        
        
        if ( is_dir)
        {
            LOG(ERROR) + "Deleting item: " + CFilePath::join( strSrcFolder,strPath);
            
            if (CRhoFile::isFileExist(CFilePath::join( strSrcFolder,strPath).c_str()) ) {
                
//#ifdef OS_ANDROID
                //nError = rho_android_remove_item(CFilePath::join( strSrcFolder,strPath).c_str());
//#else
                nError = CRhoFile::deleteFolder( CFilePath::join( strSrcFolder,strPath).c_str() );
//#endif
                
                if (nError != 0)
                {
                    LOG(ERROR) + "Cannot remove folder: " + CFilePath::join( strSrcFolder,strPath);
                    break;
                }
                else {
                    filelist->removeFolder(strPath);
                    filelist_apps->removeFolder(strPath);
                }
            }
            else {
                LOG(ERROR) + "Cannot remove folder (not found): " + CFilePath::join( strSrcFolder,strPath);
                filelist->removeFolder(strPath);
                filelist_apps->removeFolder(strPath);                // folder can be removed manually etc. It is not real issue - after this cammand files not exist, how it was removed is does not matter
                //nError = 1;
                //break;
            }
        }
        else
        {
            LOG(ERROR) + "Deleting file: " + CFilePath::join( strSrcFolder,strPath);
            
            if (CRhoFile::isFileExist(CFilePath::join( strSrcFolder,strPath).c_str()) ) {

//#ifdef OS_ANDROID
                
//                nError = CRhoFile::renameFile( CFilePath::join( strSrcFolder,strPath).c_str(),  CFilePath::join( tmp_remove_folder, "to_remove.bin").c_str());
//                if (nError != 0)
//                {
//                    LOG(ERROR) + "Cannot rename file (for deleting)  from : " + CFilePath::join( strSrcFolder,strPath) + "; to: " + CFilePath::join( tmp_remove_folder, "to_remove.bin");
//                    break;
//                }
                
                
                
                //nError = rho_android_remove_item(CFilePath::join( strSrcFolder,strPath).c_str());
//#else
                nError = CRhoFile::deleteFile( CFilePath::join( strSrcFolder,strPath).c_str() );
//#endif
                if (nError != 0)
                {
                    LOG(ERROR) + "Cannot remove file: " + CFilePath::join( strSrcFolder,strPath);
                    break;
                }
                else {
                    filelist->removeFile(strPath);
                    filelist_apps->removeFile(strPath);
                }
            }
            else {
                LOG(ERROR) + "Cannot remove file (not found): " + CFilePath::join( strSrcFolder,strPath);
                filelist->removeFile(strPath);
                filelist_apps->removeFile(strPath);                // file can be removed manually etc. It is not real issue - after this cammand files not exist, how it was removed is does not matter
                //nError = 1;
                //break;
            }
        }
    }
    
    return nError;
}
    
    
    
    
    
void CReplaceBundleThread::sendRequestFullUpdate()
{
    if (m_finish_callback.size() > 0) {
        char* norm_url = rho_http_normalizeurl(m_finish_callback.c_str());
        String query = "&rho_callback=1&status=need_sync";
        rho_net_request_with_data(norm_url, query.c_str());
        rho_http_free(norm_url);
        m_finish_callback = "";
    }
}
    
    
void CReplaceBundleThread::showError(int nError, const String& strError )
{
    LOG(ERROR) + "Error happen when replace bundle: " + strError + "; Code: " + LOGFMT("%d") + nError;

    String strMsg = "Error happen when replace bundle: " + strError;
    if (mAllViaCallback) {
        if (m_finish_callback.size() > 0) {
            char* norm_url = rho_http_normalizeurl(m_finish_callback.c_str());
            String query = "&rho_callback=1&status=error&message=";
            query = query + strMsg.c_str();
            rho_net_request_with_data(norm_url, query.c_str());
            rho_http_free(norm_url);
            m_finish_callback = "";
        }
    }
    else {
        rho_sys_impl_exit_with_errormessage("Bundle update", strMsg.c_str());
    }
}

void CReplaceBundleThread::run()
{
    synchronized(m_mxBundleReplaceMutex)
    {
        //Stop application
        if (!mDoNotRestartApp) {
            rho_rhodesapp_callAppActiveCallback(0);
            rho_rhodesapp_callUiDestroyedCallback();
            RHODESAPP().stopApp();
        }
        doReplaceBundle();
        
        if (mDoNotRestartApp /*&& (m_finish_callback.size() > 0)*/) {
            // call callback
            if (m_finish_callback.size() > 0) {
                char* norm_url = rho_http_normalizeurl(m_finish_callback.c_str());
                rho_net_request_with_data(norm_url, "&rho_callback=1&status=ok");
                rho_http_free(norm_url);
            }
        }
        else {
            rho_platform_restart_application();
            rho_sys_app_exit();
        }
        
        if (m_is_finished_flag != NULL) {
            *m_is_finished_flag = true;
        }
    }
}

    
bool CReplaceBundleThread::is_need_full_update(CFileList* old_filelist, CFileList* new_filelist, CFileList* add_list, CFileList* remove_list) {
    // 1. enumerate new list
    int i;
    
    for (i = 0; i < new_filelist->getItemCount(); i++) {
        String new_item = new_filelist->getItem(i);
        String new_item_path;
        new_filelist->getItemDetails(i, &new_item_path, NULL, NULL, NULL, NULL);
        int old_index = old_filelist->findItemByPath(new_item_path);
        if (old_index >= 0) {
            // exist in old
            String old_item = old_filelist->getItem(old_index);
            // extract type and crc
            String old_type = "";
            String new_type = "";
            String old_crc = "";
            String new_crc = "";
            String old_size = "";
            String new_size = "";
            new_filelist->getItemDetails(i, NULL, &new_type, &new_size, NULL, &new_crc);
            old_filelist->getItemDetails(old_index, NULL, &old_type, &old_size, NULL, &old_crc);
            if ((new_type.length() > 0) && (new_size.length() > 0) && (new_crc.length() > 0) && (new_type.compare(old_type) == 0) && (new_size.compare(old_size) == 0) && (new_crc.compare(old_crc) == 0)) {
            //if (new_item.compare(old_item) == 0) {
                // equal
                // good - skip it and go forward
            }
            else {
                // not equal
                // check for dir type
                if ((new_type.compare("dir") == 0) && (new_type.compare(old_type) == 0)) {
                    // OK - just skip it
                    // dir can has differetn hashes (platform depended)
                }
                else {
                    // check for contain in add list
                    int add_index = add_list->findItemByPath(new_item_path);
                    if (add_index >= 0) {
                        // exist in add_list
                        // good - skip it and go forward
                    }
                    else {
                        // not exist
                        // sync issue founded !
                        if ((new_item_path.compare("rhofilelist.txt") != 0) &&
                            (new_item_path.compare("rhoconfig.txt") != 0) &&
                            (new_item_path.compare("rhoconfig.txt.timestamp") != 0) /*&&
                            (new_item_path.compare("app_manifest.txt") != 0) &&
                            (!rho::String_startsWith(new_item_path, "public/api/"))*/ ) {
                            LOG(ERROR) + "SYNC ISSUE FOUNDED ! Item ["+new_item_path+"] is different with current server version !  value [new/old] size["+new_size+"/"+old_size+"] crc["+new_crc+"/"+old_crc+"]";
                            return true;
                        }
                    }
                }
            }
        }
        else {
            // not exist in old
            // check in add list
            int add_index = add_list->findItemByPath(new_item_path);
            if (add_index >= 0) {
                // exist in add_list
                // good - skip it and go forward
            }
            else {
                // not exist
                // sync issue founded !
                if ((new_item_path.compare("rhofilelist.txt") != 0) &&
                    (new_item_path.compare("rhoconfig.txt") != 0) &&
                    (new_item_path.compare("rhoconfig.txt.timestamp") != 0) /*&&
                    (new_item_path.compare("app_manifest.txt") != 0) &&
                    (!rho::String_startsWith(new_item_path, "public/api/"))*/ ) {
                    LOG(ERROR) + "SYNC ISSUE FOUNDED ! Item ["+new_item_path+"] exist in current version on server but not exist on device !";
                    return true;
                }
            }
            
        }
    }
    
    // enumerate old list
    for (i = 0; i < old_filelist->getItemCount(); i++) {
        String old_item = old_filelist->getItem(i);
        String old_item_path;
        old_filelist->getItemDetails(i, &old_item_path, NULL, NULL, NULL, NULL);
        int new_index = new_filelist->findItemByPath(old_item_path);
        if (new_index >= 0) {
            // none - already checked when enumerate new items
        }
        else {
            // check remove list
            int remove_index = remove_list->findItemByPath(old_item_path);
            if (remove_index >= 0) {
                // exist in removed
                // good - skip and go forward
            }
            else {
               // not in removed not in new
               // sync issue founded !
                if ((old_item_path.compare("rhofilelist.txt") != 0) &&
                    (old_item_path.compare("rhoconfig.txt") != 0) &&
                    (old_item_path.compare("rhoconfig.txt.timestamp") != 0) /*&&
                    (old_item_path.compare("app_manifest.txt") != 0) &&
                    (!rho::String_startsWith(old_item_path, "public/api/"))*/ ) {
                    LOG(ERROR) + "SYNC ISSUE FOUNDED ! Item ["+old_item_path+"] exist on device but not exist on server !";
                    return true;
                }
            }
        }
    }
    
    
    return false;
}
    
    
    
void CReplaceBundleThread::doReplaceBundle()
{
    
#ifdef OS_ANDROID
    int saved_mode = rho_file_get_fs_mode();
    rho_file_set_fs_mode(0);
#endif
    
    
    CFileTransaction oFT( RHODESAPP().getAppRootPath());
    if (oFT.start())
    {
        showError(oFT.getError(), oFT.getErrorText());
#ifdef OS_ANDROID
        rho_file_set_fs_mode(saved_mode);
#endif
        return;
    }
    LOG(INFO) + "START";
    
#if defined(OS_MACOSX) && !defined(RHODES_EMULATOR)
    // prepare main folder for update (replace sym-link by real folder/files)
    if ( rho_prepare_folder_for_upgrade( RHODESAPP().getAppRootPath().c_str() ) == 0) {
        // error
        showError(0, "Prepare bundle for upgrade on iOS failed." );
#ifdef OS_ANDROID
        rho_file_set_fs_mode(saved_mode);
#endif
        return;
    }
#endif    

    unsigned int nError = 0;
    String rho_filelist_path = "";
    String rho_filelist_apps_path = "";
    CFileList filelist;
    CFileList filelist_apps;
    
    bool is_partial_update = CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt").c_str()) || CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_remove_files.txt").c_str());

    
#ifdef OS_ANDROID
    String root = RHODESAPP().getRhoRootPath().c_str();
    LOG(TRACE) + "Android RhoRootPath = " + root.c_str();
    rho_filelist_path = CFilePath::join(root, "rho.dat");
    filelist.loadFromFile(rho_filelist_path, "apps/");
    rho_filelist_apps_path = CFilePath::join( root, "apps/rhofilelist.txt");
    filelist_apps.loadFromFile(rho_filelist_apps_path, "");
#else
    rho_filelist_path = CFilePath::join( RHODESAPP().getAppRootPath(), "rhofilelist.txt");
    filelist.loadFromFile(rho_filelist_path, "");
    rho_filelist_apps_path = rho_filelist_path;
    filelist_apps.loadFromFile(rho_filelist_apps_path, "");
#endif
    

    if (is_partial_update && mCheckFileList) {
        // check sync issue - compare rhofilelist's on device and from bundle zip
        // device file list alreadyloaded to filelist
        
        // load bundle list to newfillist
        CFileList newfilelist;
        newfilelist.loadFromFile(CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt").c_str(), "");
        
        CFileList add_list;
        add_list.loadFromFile(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt").c_str(), "");
    
        CFileList remove_list;
        remove_list.loadFromFile(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_remove_files.txt").c_str(), "");

        if (is_need_full_update(&filelist_apps, &newfilelist, &add_list, &remove_list)) {
            // stop and send responce
            sendRequestFullUpdate();
#ifdef OS_ANDROID
            rho_file_set_fs_mode(saved_mode);
#endif
            return;
        }
        
    }
    if (!is_partial_update) {
        // check full update main file
        String new_files_list_path = CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt");
        if (!CRhoFile::isFileExist(new_files_list_path.c_str())) {
            showError(nError, "Invalid new bundle content - not contain rhofilelist !" );
#ifdef OS_ANDROID
            rho_file_set_fs_mode(saved_mode);
#endif
            return;
        }
    }
    
    
    if (is_partial_update) {
        if (CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_remove_files.txt").c_str())) {
            //nError = removeFilesByList( CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt"), ::RHODESAPP().getAppRootPath(), true );
            nError = partialRemoveItemsByList( CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_remove_files.txt"), ::RHODESAPP().getAppRootPath(), &filelist, &filelist_apps, m_bundle_path);
            if ( nError != 0 )
            {
                showError(nError, "Remove original files from partial remove list failed." );
#ifdef OS_ANDROID
                rho_file_set_fs_mode(saved_mode);
#endif
                return;
            }
        }
    }
    else {
        // full update
        //Remove current files
#ifdef OS_ANDROID
        nError = removeFilesByList( rho_filelist_apps_path, ::RHODESAPP().getAppRootPath(), true );
#else
        nError = removeFilesByList( rho_filelist_apps_path, ::RHODESAPP().getAppRootPath(), true );
#endif
        if ( nError != 0 )
        {
            showError(nError, "Remove files from bundle failed." );
#ifdef OS_ANDROID
            rho_file_set_fs_mode(saved_mode);
#endif
            return;
        }
    }

    //Copy new bundle
    //nError = CRhoFile::moveFoldersContentToAnotherFolder( CFilePath::join(m_bundle_path, "RhoBundle/apps").c_str(), ::RHODESAPP().getAppRootPath().c_str() );
    
    if (is_partial_update) {
        if (CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt").c_str())) {
            // partial update
            nError = partialAddFilesByList( CFilePath::join(m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt").c_str(), 
				CFilePath::join(m_bundle_path, "RhoBundle/apps"), 
				::RHODESAPP().getAppRootPath().c_str() , &filelist, &filelist_apps);
            if ( nError != 0 )
            {
                showError(nError, "Copy files to bundle failed." );
#ifdef OS_ANDROID
                rho_file_set_fs_mode(saved_mode);
#endif
                return;
            }
        }
    }
    else {
        // full update
#ifdef OS_ANDROID
        nError = partialAddFilesByList( CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt").c_str(), CFilePath::join(m_bundle_path, "RhoBundle/apps"), ::RHODESAPP().getAppRootPath().c_str() , &filelist, &filelist_apps);
        if ( nError != 0 )
        {
            showError(nError, "Copy files to bundle failed." );
#ifdef OS_ANDROID
            rho_file_set_fs_mode(saved_mode);
#endif
            return;
        }
#else
        nError = moveFilesByList( CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt").c_str(), CFilePath::join(m_bundle_path, "RhoBundle/apps"), ::RHODESAPP().getAppRootPath().c_str() );
        if ( nError != 0 )
        {
            showError(nError, "Copy files to bundle failed." );
#ifdef OS_ANDROID
            rho_file_set_fs_mode(saved_mode);
#endif
            return;
        }
#endif
    }
    
    if (is_partial_update) {
        filelist.saveToFile();
#ifdef OS_ANDROID
        filelist_apps.saveToFile();
#endif
    }
    else {
#ifdef OS_ANDROID
        
            // no CFilePath::join(m_bundle_path, "RhoBundle/rho.dat") in current bundle - we should modify and copy CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt") !!!
            filelist.saveToFile();
        
        nError = CRhoFile::deleteFile( rho_filelist_apps_path.c_str() );
        if (nError != 0)
        {
            LOG(ERROR) + "Cannot remove file old rhofilelist.txt";
        }
        if (CRhoFile::copyFile(CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt").c_str(), rho_filelist_apps_path.c_str()))
        {
            int err = errno;
            LOG(ERROR) + "Cannot replace rhofilelist.txt, errno: " + LOGFMT("%d") + err;
            showError(nError, "Cannot replace rhofilelist.txt" );
#ifdef OS_ANDROID
            rho_file_set_fs_mode(saved_mode);
#endif
            return;
        }
            //rho_file_patch_stat_table(CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt"))
            //if (CRhoFile::copyFile(CFilePath::join(m_bundle_path, "RhoBundle/rho.dat").c_str(), CFilePath::join(RHODESAPP().getRhoRootPath().c_str(), "rho.dat").c_str()))
            //{
            //    int err = errno;
            //    LOG(ERROR) + "Cannot copy rho.dat, errno: " + LOGFMT("%d") + err;
            //}
#else
        // copy new rhofilelist.txt
        nError = CRhoFile::deleteFile( rho_filelist_path.c_str() );
        if (nError != 0)
        {
            LOG(ERROR) + "Cannot remove file old rhofilelist.txt";
        }

        
        if (CRhoFile::copyFile(CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt").c_str(), rho_filelist_path.c_str()))
        {
            int err = errno;
            LOG(ERROR) + "Cannot replace rhofilelist.txt, errno: " + LOGFMT("%d") + err;
            showError(nError, "Cannot replace rhofilelist.txt" );
#ifdef OS_ANDROID
            rho_file_set_fs_mode(saved_mode);
#endif
            return;
        }
#endif
    
    
    }

    LOG(INFO) + "STOP";
    oFT.commit();
    
    //Delete bundle folder
    CRhoFile::deleteFolder( m_bundle_path.c_str());

#ifdef OS_ANDROID
    rho_android_file_reload_stat_table();
#endif
#ifdef OS_ANDROID
    rho_file_set_fs_mode(saved_mode);
#endif

}

}
}

extern "C" 
{

void rho_sys_replace_current_bundleEx(const char* path, const char* finish_callback, bool do_not_restart_app, bool not_thread_mode, bool check_filelist, bool all_via_callback_only)
{
    
#ifdef OS_ANDROID
    rho_android_force_all_files();
#endif
    
    bool is_finished_flag = false;
    CReplaceBundleThread* replace_thread = new CReplaceBundleThread(path, finish_callback, do_not_restart_app, (not_thread_mode ? &is_finished_flag : 0), check_filelist, all_via_callback_only);
    if (not_thread_mode) {
        while (!is_finished_flag) {
            replace_thread->sleep(10);
        }
    }
}

void rho_sys_replace_current_bundle(const char* path, rho_param *p)
{
    const char* finish_callback = "";
    bool do_not_restart_app = false;
    bool not_thread_mode = false;
    
    if (p != NULL) {
        if (p->type == RHO_PARAM_HASH) {

            int size = p->v.hash->size;
            for (int i = 0; i < size; ++i) {
                char *name = p->v.hash->name[i];
                rho_param *value = p->v.hash->value[i];
                
                if (strcasecmp(name, "callback") == 0) {
                    if (value->type == RHO_PARAM_STRING) {
                        finish_callback = value->v.string;
                    }
                }
                if (strcasecmp(name, "do_not_restart_app") == 0) {
                    if (value->type == RHO_PARAM_STRING) {
                        if (strcasecmp(value->v.string, "true") == 0) {
                            do_not_restart_app = true;
                        }
                    }
                }
                if (strcasecmp(name, "not_thread_mode") == 0) {
                    if (value->type == RHO_PARAM_STRING) {
                        if (strcasecmp(value->v.string, "true") == 0) {
                            not_thread_mode = true;
                        }
                    }
                }
            }
        }
    }
    rho_sys_replace_current_bundleEx( path, finish_callback, do_not_restart_app, not_thread_mode , false, false );
}

int rho_sys_check_rollback_bundle(const char* szRhoPath)
{
    if (rho_log_system_is_ready()) {
        LOG(INFO) + "rho_sys_check_rollback_bundle()";
    }
    CFileTransaction oFT( CFilePath::join(szRhoPath, "apps"), false );
    return oFT.rollback() != 0 ? 0 : 1;
}

//TODO: remove  rho_sys_delete_folder
int rho_sys_delete_folder(const char* szRhoPath)
{
    return CRhoFile::deleteFolder(szRhoPath);
}

int rho_sysimpl_remove_bundle_files(const char* path, const char* fileListName)
{
    unsigned int nError = CReplaceBundleThread::removeFilesByList(CFilePath::join(path, fileListName), path, false);
    if ( nError != 0 )
    {
        CReplaceBundleThread::showError(nError, String("Remove of bundle files is failed: ") += path);
    }
    return nError;
}
    
    
    
    
    
    
    
    
    
    
    

} //extern "C"
