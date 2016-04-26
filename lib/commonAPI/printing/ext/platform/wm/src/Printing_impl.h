#include "../../../shared/generated/cpp/PrinterBase.h"

namespace rho 
{

class CPrinterImpl: public CPrinterBase
{
public:
    rho::String m_ID;

public:
    void setID(const rho::String& ID);
 
    virtual void getID(rho::apiGenerator::CMethodResult& oResult);
 
    virtual void getDeviceName(rho::apiGenerator::CMethodResult& oResult);
    
    virtual void getIsConnected(rho::apiGenerator::CMethodResult& oResult);
     
    virtual void connect(rho::apiGenerator::CMethodResult& oResult);
 
    virtual void connectWithOptions( const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);

    virtual void disconnect(rho::apiGenerator::CMethodResult& oResult);

    virtual void printFile( const rho::String& path,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);

    virtual void printImageFromFile( const rho::String& path,  int x,  int y,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);

    virtual void printRawString( const rho::String& command,  const rho::Hashtable<rho::String, rho::String>& options, rho::apiGenerator::CMethodResult& oResult);

    virtual void enumerateSupportedControlLanguages(rho::apiGenerator::CMethodResult& oResult);

    virtual void requestState( const rho::Vector<rho::String>& listOfParameters, rho::apiGenerator::CMethodResult& oResult);
};

}