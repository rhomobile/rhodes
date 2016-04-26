#include "../../../shared/generated/cpp/PrinterZebraBase.h"

namespace rho {

using namespace apiGenerator;

class CPrinterZebraImpl : public CPrinterZebraBase
{
public:
    rho::String m_ID;

public:
    void setID(const rho::String& ID) { m_ID = ID; }
    virtual void connect(rho::apiGenerator::CMethodResult& oResult);
    virtual void connectWithOptions( const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);
    virtual void disconnect(rho::apiGenerator::CMethodResult& oResult);
    virtual void getDeviceName(rho::apiGenerator::CMethodResult& oResult);
    virtual void getIsConnected(rho::apiGenerator::CMethodResult& oResult);
    virtual void requestState( const rho::Vector<rho::String>& listOfParameters, rho::apiGenerator::CMethodResult& oResult);
    virtual void printFile( const rho::String& fileURI,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);
    virtual void printRawString( const rho::String& command,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);
    virtual void retrieveFileNames(rho::apiGenerator::CMethodResult& oResult);
    virtual void retrieveFileNamesWithExtensions( const rho::Vector<rho::String>& extensions, rho::apiGenerator::CMethodResult& oResult);
    virtual void sendFileContents( const rho::String& filePath, rho::apiGenerator::CMethodResult& oResult);
    virtual void printStoredFormatWithHash( const rho::String& formatPathOnPrinter,  const rho::Hashtable<rho::String, rho::String>& vars, rho::apiGenerator::CMethodResult& oResult);
    virtual void printStoredFormatWithArray( const rho::String& formatPathOnPrinter,  const rho::Vector<rho::String>& vars, rho::apiGenerator::CMethodResult& oResult);
    virtual void printImageFromFile( const rho::String& path,  int x,  int y,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);
    virtual void storeImage( const rho::String& printerDriveAndFileName,  const rho::String& imageFilePathOnDevice,  int width,  int height, rho::apiGenerator::CMethodResult& oResult);
    virtual void enumerateSupportedControlLanguages(rho::apiGenerator::CMethodResult& oResult);
};

}