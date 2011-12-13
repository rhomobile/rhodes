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

using namespace rho;
using namespace rho::common;
extern "C" void rho_sys_app_exit();
extern "C" void rho_sys_impl_show_errormessage(const char* szTitle, const char* szMsg);

#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
void rho_sys_impl_show_errormessage(const char* szTitle, const char* szMsg)
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
public:
    CFileTransaction(const String& strFolder);
    ~CFileTransaction();
    unsigned int start();
    void commit();
    unsigned int rollback();

    boolean isError(){ return m_nError != 0; }
    unsigned int getError(){ return m_nError; }
    String getErrorText(){ return m_strError; }

};

class CReplaceBundleThread : public rho::common::CRhoThread
{
    DEFINE_LOGCLASS;

    String m_bundle_path;

    unsigned int removeFilesByList( const String& strListPath, const String& strSrcFolder );
    unsigned int moveFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDstFolder );
    void doReplaceBundle();
public:

    CReplaceBundleThread(const char* root_path) 
    {
        m_bundle_path = root_path;
        start(rho::common::IRhoRunnable::epNormal);
    }
    
    virtual ~CReplaceBundleThread() {}
    
    virtual void run();

    static void showError(int nError, const String& strError );
};
IMPLEMENT_LOGCLASS(CReplaceBundleThread,"RhoBundle");

CFileTransaction::CFileTransaction(const String& strFolder) : m_strFolder(strFolder)
{
}

unsigned int CFileTransaction::start()
{
    CRhoFile::createFolder((m_strFolder + "_temp_journal").c_str());

    m_nError = CRhoFile::copyFoldersContentToAnotherFolder( CFilePath::join(m_strFolder, "").c_str(), (m_strFolder + "_temp_journal").c_str() );
    if (m_nError)
    {
        m_strError = "Unable to copy folder: " + m_strFolder + " to : " + m_strFolder + "_temp_journal";
        return m_nError;
    }

    m_nError = CRhoFile::renameFile( (m_strFolder + "_temp_journal").c_str(), (m_strFolder + "_journal").c_str());
    if (m_nError)
    {
        m_strError = "Unable to rename folder: " + m_strFolder + "_temp_journal to : " + m_strFolder + "_journal";
        return m_nError;
    }

    return m_nError;
}

CFileTransaction::~CFileTransaction()
{
    rollback();
}

void CFileTransaction::commit()
{
    String strFolder = m_strFolder;
    
    m_nError = CRhoFile::renameFile( (strFolder + "_journal").c_str(), (strFolder + "_temp_journal").c_str() );
    if (m_nError)
    {
        m_strError = "Unable to rename folder: " + strFolder + "_journal to : " + strFolder + "_temp_journal";
        return;
    }
    m_strFolder = "";

    m_nError = CRhoFile::deleteFolder( (strFolder + "_temp_journal").c_str() );
    if (m_nError)
    {
        m_strError = "Unable to delete folder: " + strFolder + "_temp_journal";
        return;
    }

}

unsigned int CFileTransaction::rollback()
{
    String strFolder = m_strFolder;
    m_strFolder = "";

    if ( strFolder.length() == 0 )
        return 0;                                 

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

    return m_nError;
}

unsigned int CReplaceBundleThread::removeFilesByList( const String& strListPath, const String& strSrcFolder )
{
    unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);

    CTokenizer oTokenizer( strList, "\n" );
	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();

        CTokenizer oLineTok( strLine, "\t" );
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strPath = oLineTok.nextToken();
        if ( !oLineTok.hasMoreTokens() )
            continue;
        String strType = oLineTok.nextToken();

        if ( strType.compare("file") == 0)
        {
            nError = CRhoFile::deleteFile( CFilePath::join( strSrcFolder,strPath).c_str() );
            if (nError != 0)
            {
                LOG(ERROR) + "Cannot remove file: " + CFilePath::join( strSrcFolder,strPath);
                break;
            }
        }
    }

    return nError;
}

unsigned int CReplaceBundleThread::moveFilesByList( const String& strListPath, const String& strSrcFolder, const String& strDestFolder )
{
    unsigned int nError = 0;
    String strList;
    CRhoFile::loadTextFile(strListPath.c_str(), strList);

    CTokenizer oTokenizer( strList, "\n" );
	while (oTokenizer.hasMoreTokens()) 
    {
		String strLine = oTokenizer.nextToken();

        CTokenizer oLineTok( strLine, "\t" );
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
            CRhoFile::createFolder(strDstPath.c_str());
        }else if ( strType.compare("file") == 0)
        {
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

void CReplaceBundleThread::showError(int nError, const String& strError )
{
    LOG(ERROR) + "Error happen when replace bundle: " + strError + "; Code: " + LOGFMT("%d") + nError;

    String strMsg = "Error happen when replace bundle: " + strError;

    rho_sys_impl_show_errormessage("Bundle update", strMsg.c_str());
}

void CReplaceBundleThread::run()
{
    //Stop application
    rho_rhodesapp_callAppActiveCallback(0);
    rho_rhodesapp_callUiDestroyedCallback();
    RHODESAPP().stopApp();

    doReplaceBundle();

    rho_platform_restart_application();
    rho_sys_app_exit();
}

void CReplaceBundleThread::doReplaceBundle()
{
    CFileTransaction oFT( RHODESAPP().getAppRootPath());
    if (oFT.start())
    {
        showError(oFT.getError(), oFT.getErrorText());
        return;
    }

    //Remove current files
    unsigned int nError = removeFilesByList( CFilePath::join( RHODESAPP().getAppRootPath(), "rhofilelist.txt"), ::RHODESAPP().getAppRootPath() );
    if ( nError != 0 )
    {
        showError(nError, "Remove files from bundle failed." );
        return;
    }

    LOG(INFO) + "START";
    //Copy new bundle
    //nError = CRhoFile::moveFoldersContentToAnotherFolder( CFilePath::join(m_bundle_path, "RhoBundle/apps").c_str(), ::RHODESAPP().getAppRootPath().c_str() );
    nError = moveFilesByList( CFilePath::join(m_bundle_path, "RhoBundle/apps/rhofilelist.txt").c_str(), CFilePath::join(m_bundle_path, "RhoBundle/apps"), ::RHODESAPP().getAppRootPath().c_str() );
    if ( nError != 0 )
    {
        showError(nError, "Copy files to bundle failed." );
        return;
    }
    LOG(INFO) + "STOP";
    oFT.commit();

    //Delete bundle folder
    CRhoFile::deleteFolder( m_bundle_path.c_str());
}

}
}

extern "C" 
{

void rho_sys_replace_current_bundle(const char* path)
{
    new CReplaceBundleThread(path);    
}

int rho_sys_check_rollback_bundle(const char* szRhoPath)
{
    CFileTransaction oFT( CFilePath::join(szRhoPath, "apps") );
    return oFT.rollback() != 0 ? 0 : 1;
}

int rho_sys_delete_folder(const char* szRhoPath)
{
    return CRhoFile::deleteFolder(szRhoPath);
}

} //extern "C"









