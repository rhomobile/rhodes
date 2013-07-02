#include "generated/cpp/RhoFileBase.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
#include "logging/RhoLog.h"

namespace rho {

using namespace apiGenerator;
using namespace common;

class CFileSingletonImpl: public CRhoFileSingletonBase
{
public:

    CFileSingletonImpl(): CRhoFileSingletonBase(){}
    
    virtual void copy( const rho::String& from,  const rho::String& to, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)CRhoFile::copyFile(from.c_str(),to.c_str()) );
    }
    
    virtual void rename( const rho::String& from,  const rho::String& to, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)CRhoFile::renameFile(from.c_str(), to.c_str() ) );
    }
    
    virtual void makeDir( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)CRhoFile::createFolder( path.c_str() ) );
    }
    
    virtual void exists( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( CRhoFile::isFileExist( path.c_str() ) );
    }
    
    virtual void getFileSize( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)CRhoFile::getFileSize( path.c_str() ) );
    }
    
    virtual void isDir( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( CRhoFile::isDirectory( path.c_str() ) );
    }
    
    virtual void isFile( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( CRhoFile::isFile( path.c_str() ) );
    }
    
    virtual void deleteFile( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)CRhoFile::deleteFile( path.c_str() ) );
    }
    
    virtual void deleteDir( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)CRhoFile::deleteEmptyFolder( path.c_str() ) );
    }
    
    virtual void makeDirs( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        CRhoFile::recursiveCreateDir( path.c_str(), "" );
    }
    
    virtual void deleteRecursive( const rho::String& path,  bool leaveRoot, rho::apiGenerator::CMethodResult& oResult) {
        int result = (int)CRhoFile::deleteFolder( path.c_str() );
        if (leaveRoot && result == 0 )
        {
            result = (int)CRhoFile::createFolder( path.c_str() );
        }
        oResult.set( result );
    }
    
    virtual void listDir( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        Vector<String> entries;
        if (CRhoFile::listFolderEntries(path,entries)) {
            oResult.set(entries);
        }
    }
    
    virtual void basename( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        CFilePath oPath(path);
        oResult.set( oPath.getBaseName() );
    }
    
    virtual void dirname( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        CFilePath oPath(path);
        oResult.set( oPath.getFolderName() );
        
    }
    
    virtual void join( const rho::String& p1,  const rho::String& p2, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( CFilePath::join(p1,p2) );
    }
    
    virtual void read( const rho::String& path, rho::apiGenerator::CMethodResult& oResult) {
        String data;
        CRhoFile::loadTextFile( path.c_str(), data );
        oResult.set(data);
    }
};
    
class CRhoFileImpl : public CRhoFileBase
{
    CRhoFile m_oFile;
public:
    virtual ~CRhoFileImpl() {}
    
    virtual void open( const rho::String& path,  int mode, rho::apiGenerator::CMethodResult& oResult) {
        m_oFile.open(path.c_str(), (CRhoFile::EOpenModes)mode);
    }
    
    virtual void close(rho::apiGenerator::CMethodResult& oResult) {
        m_oFile.close();
    }
    
    virtual void isOpened(rho::apiGenerator::CMethodResult& oResult) {
        oResult.set(m_oFile.isOpened());
    }
    
    virtual void read( int size, rho::apiGenerator::CMethodResult& oResult) {
        char* buf = 0;
        if ( size > 0 ) {
            buf = new char[size + 1];
        }
        if (buf)
        {
            int actualsize = m_oFile.readData((void*)buf,0,size);
            if ( actualsize> 0 )
            {
                buf[actualsize] = 0;
                
                String data(buf);
                oResult.set(data);
            }else
                oResult.set("");

            delete[] buf;
        }
        else {
            oResult.set("");
        }
    }
    
    virtual void readAll(rho::apiGenerator::CMethodResult& oResult) {
        String data;
        m_oFile.readString(data);
        oResult.set(data);
        
    }
    
    virtual void write( const rho::String& val, rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)m_oFile.write((void*)val.c_str(),val.size()) );
    }
    
    virtual void flush(rho::apiGenerator::CMethodResult& oResult) {
        m_oFile.flush();
    }
    
    virtual void seek( int pos, rho::apiGenerator::CMethodResult& oResult) {
        m_oFile.setPosTo(pos);
    }
    
    virtual void size(rho::apiGenerator::CMethodResult& oResult) {
        oResult.set( (int)m_oFile.size() );
    }
    
};

////////////////////////////////////////////////////////////////////////

class CRhoFileFactory: public CRhoFileFactoryBase
{
public:
    CRhoFileFactory(){}

    IRhoFileSingleton* createModuleSingleton()
    { 
        return new CFileSingletonImpl();
    }
    
    virtual IRhoFile* createModuleByID(const rho::String& strID){ return new CRhoFileImpl(); };

};

}

extern "C" void Init_RhoFile()
{
    rho::CRhoFileFactory::setInstance( new rho::CRhoFileFactory() );
    rho::Init_RhoFile_API();
}
