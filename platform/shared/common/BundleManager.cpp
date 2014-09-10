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


// REMOVE IT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    
//#define OS_ANDROID    



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
#endif


#if !defined(OS_WINDOWS_DESKTOP) && !defined(OS_WINCE) && !defined(OS_MACOSX)
void rho_sys_impl_exit_with_errormessage(const char* szTitle, const char* szMsg)
{
}
#endif

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
    

    
class CFileList 
{
public:
    CFileList();
    virtual ~CFileList();
    
    bool removeFile(const String& path); 
    void removeFolder(const String& path); 
    void addFile(const String& full_line); 
    void addFolder(const String& full_line); 
    
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
    
    
    bool CFileList::removeFile(const String& path) {
        String f = mPrefix + path;
        LOG(TRACE) + "CFileList::removeFile( " + f + ") :";
        bool founded = false;
        Vector<String>::iterator it;
        String to_find = f + "\t";
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
            if (strLine.length() > 0) {
                mLines.push_back(strLine);
                LOG(TRACE) + "                " + strLine;
            }
        }
    }
    
    
    
    
    

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
    
    static unsigned int partialAddFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDstFolder, CFileList* filelist);
    static unsigned int partialRemoveItemsByList( const String& strListPath, const String& strSrcFolder, CFileList* filelist);
    
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
    LOG(TRACE) + "FileTransaction is rolling back: " + m_strFolder;

    String strFolder = m_strFolder;
    m_strFolder = "";

    if ( strFolder.length() == 0 ) {
        LOG(TRACE) + "FileTransaction rolling back: empty transaction" + m_strFolder;
        return 0;
    }

    CRhoFile::deleteFolder( CFilePath(strFolder).changeBaseName("RhoBundle").c_str() );
    CRhoFile::deleteFolder( (strFolder + "_temp_journal").c_str() );

    if ( !CRhoFile::isFileExist((strFolder + "_journal").c_str()) )
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

    LOG(TRACE) + "FileTransaction has rolled back";

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

    
unsigned int CReplaceBundleThread::partialAddFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDstFolder, CFileList* filelist) {
	LOG(TRACE) + "Partial Moving files by list: " + strSrcFolder + " -> " + strDstFolder;
    
	unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);
    const char nl[] = "\n";
    CTokenizer oTokenizer( strList, nl );
	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();
        
        if (strLine.length() > 0) {
            if ((strLine[0] == 0xD) || (strLine[0] == 0xA)) {
                strLine = strLine.substr(1, strLine.length()-1);
            }
            if ((strLine[strLine.length()-1] == 0xD) || (strLine[strLine.length()-1] == 0xA)) {
                strLine = strLine.substr(0, strLine.length()-1);
            }
        }
        
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
            }
        }else if ( strType.compare("file") == 0)
        {
            LOG(TRACE) + "Renaming file: " + strSrcPath + " -> " + strDstPath;

            bool must_be_exist = filelist->removeFile(strPath);

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
            }
        }
    }
    
    return nError;
}
    
unsigned int CReplaceBundleThread::partialRemoveItemsByList( const String& strListPath, const String& strSrcFolder, CFileList* filelist) {
	LOG(TRACE) + "Partial Removing items by list: " + strSrcFolder + ", list: " + strListPath;
    
    unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);
    
    
    const char nl[] = "\n";
    CTokenizer oTokenizer( strList, nl );

	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();

        if (strLine.length() > 0) {
            if ((strLine[0] == 0xD) || (strLine[0] == 0xA)) {
                strLine = strLine.substr(1, strLine.length()-1);
            }
            if ((strLine[strLine.length()-1] == 0xD) || (strLine[strLine.length()-1] == 0xA)) {
                strLine = strLine.substr(0, strLine.length()-1);
            }
        }
        
        CTokenizer oLineTok( strLine, "\t" );
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strPath = oLineTok.nextToken();

        if (strPath.length() <= 0) {
            continue;
        }
        
        bool is_dir = CRhoFile::isDirectory(CFilePath::join( strSrcFolder,strPath).c_str());
        
        
        if ( is_dir)
        {
            LOG(TRACE) + "Deleting item: " + CFilePath::join( strSrcFolder,strPath);
            
            if (CRhoFile::isFileExist(CFilePath::join( strSrcFolder,strPath).c_str()) ) {
                
                nError = CRhoFile::deleteFolder( CFilePath::join( strSrcFolder,strPath).c_str() );
#ifdef OS_ANDROID
                nError = 0;
#endif                
                
                if (nError != 0)
                {
                    LOG(ERROR) + "Cannot remove folder: " + CFilePath::join( strSrcFolder,strPath);
                    break;
                }
                else {
                    filelist->removeFolder(strPath);
                }
            }
            else {
                LOG(ERROR) + "Cannot remove folder (not found): " + CFilePath::join( strSrcFolder,strPath);
                break;
            }
        }
        else
        {
            LOG(TRACE) + "Deleting file: " + CFilePath::join( strSrcFolder,strPath);
            
            if (CRhoFile::isFileExist(CFilePath::join( strSrcFolder,strPath).c_str()) ) {

                nError = CRhoFile::deleteFile( CFilePath::join( strSrcFolder,strPath).c_str() );
#ifdef OS_ANDROID
                nError = 0;
#endif                
                if (nError != 0)
                {
                    LOG(ERROR) + "Cannot remove file: " + CFilePath::join( strSrcFolder,strPath);
                    break;
                }
                else {
                    filelist->removeFile(strPath);
                }
            }
            else {
                LOG(ERROR) + "Cannot remove file (not found): " + CFilePath::join( strSrcFolder,strPath);
                nError = 1;
                break;
            }
        }
    }
    
    return nError;
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
        }
    }
    else {
        rho_sys_impl_exit_with_errormessage("Bundle update", strMsg.c_str());
    }
}

void CReplaceBundleThread::run()
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

    
void CReplaceBundleThread::doReplaceBundle()
{
    CFileTransaction oFT( RHODESAPP().getAppRootPath());
    if (oFT.start())
    {
        showError(oFT.getError(), oFT.getErrorText());
        return;
    }
    LOG(INFO) + "START";
    
#if defined(OS_MACOSX) && !defined(RHODES_EMULATOR)
    // prepare main folder for update (replace sym-link by real folder/files)
    if ( rho_prepare_folder_for_upgrade( RHODESAPP().getAppRootPath().c_str() ) == 0) {
        // error
        showError(0, "Prepare bundle for upgrade on iOS failed." );
        return;
    }
#endif    

    unsigned int nError = 0;
    CFileList filelist;
    
    bool is_partial_update = CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt").c_str()) || CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_remove_files.txt").c_str());
    

    if (is_partial_update) {
#ifdef OS_ANDROID
        filelist.loadFromFile(CFilePath::join(RHODESAPP().getRhoRootPath().c_str(), "rho.dat"), "apps/");
#else
        filelist.loadFromFile(CFilePath::join( RHODESAPP().getAppRootPath(), "rhofilelist.txt"), "");
#endif        
        if (CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_remove_files.txt").c_str())) {
            //nError = removeFilesByList( CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt"), ::RHODESAPP().getAppRootPath(), true );
            nError = partialRemoveItemsByList( CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_remove_files.txt"), ::RHODESAPP().getAppRootPath(), &filelist);
            if ( nError != 0 )
            {
                showError(nError, "Remove original files from partial remove list failed." );
                return;
            }
        }
    }
    else {
        // full update
        //Remove current files
        nError = removeFilesByList( CFilePath::join( RHODESAPP().getAppRootPath(), "rhofilelist.txt"), ::RHODESAPP().getAppRootPath(), true );
        if ( nError != 0 )
        {
            showError(nError, "Remove files from bundle failed." );
            return;
        }
    }

    //Copy new bundle
    //nError = CRhoFile::moveFoldersContentToAnotherFolder( CFilePath::join(m_bundle_path, "RhoBundle/apps").c_str(), ::RHODESAPP().getAppRootPath().c_str() );
    
    if (is_partial_update) {
        if (CRhoFile::isFileExist(CFilePath::join( m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt").c_str())) {
            // partial update
            nError = partialAddFilesByList( CFilePath::join(m_bundle_path, "RhoBundle/apps/upgrade_package_add_files.txt").c_str(), CFilePath::join(m_bundle_path, "RhoBundle/apps"), ::RHODESAPP().getAppRootPath().c_str() , &filelist);
            if ( nError != 0 )
            {
                showError(nError, "Copy files to bundle failed." );
                return;
            }
        }
    }
    else {
        // full update
        nError = moveFilesByList( CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt").c_str(), CFilePath::join(m_bundle_path, "RhoBundle/apps"), ::RHODESAPP().getAppRootPath().c_str() );
        if ( nError != 0 )
        {
            showError(nError, "Copy files to bundle failed." );
            return;
        }
    }
    
    
    LOG(INFO) + "STOP";
    oFT.commit();

    if (is_partial_update) {
        filelist.saveToFile();
#ifdef OS_ANDROID
        rho_android_file_reload_stat_table();
#endif
    }
    
#ifdef OS_ANDROID
    if (!is_partial_update) {
        //rho_file_patch_stat_table(CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt"))
        if (CRhoFile::copyFile(CFilePath::join(m_bundle_path, "RhoBundle/rho.dat").c_str(), CFilePath::join(RHODESAPP().getRhoRootPath().c_str(), "rho.dat").c_str()))
        {
            int err = errno;
            LOG(ERROR) + "Cannot copy rho.dat, errno: " + LOGFMT("%d") + err;
        }
    }
#endif


    //Delete bundle folder
    CRhoFile::deleteFolder( m_bundle_path.c_str());
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
    CReplaceBundleThread* replace_thread = new CReplaceBundleThread(path, finish_callback, do_not_restart_app, (not_thread_mode ? &is_finished_flag : 0), false, false);
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
    LOG(INFO) + "rho_sys_check_rollback_bundle()";
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
