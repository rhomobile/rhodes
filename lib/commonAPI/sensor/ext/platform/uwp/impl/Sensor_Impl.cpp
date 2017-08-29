//
//  SensorImpl.cpp
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "../../../shared/generated/cpp/SensorBase.h"
#include "logging/RhoLog.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CSensorSingletonImpl: public CSensorSingletonBase
    {
    public:
        
        CSensorSingletonImpl(): CSensorSingletonBase(){}
        
        //methods
        // makeSensorByType Return the new sensor object by type. 
        virtual void makeSensorByType( const rho::String& type, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("makeSensorByType","Sensor");
            
        } 
        // enumerate This is documentation 
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("enumerate","Sensor");
            
        } 

    };
    
    class CSensorImpl : public CSensorBase
    {
    public:
        virtual ~CSensorImpl() {}

        //methods

        virtual void getMinimumGap(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setMinimumGap( int minimumGap, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getType(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getStatus(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void start(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void readData(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void stop(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult) {

        } 

        virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) {

        } 

    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CSensorFactory: public CSensorFactoryBase    {
    public:
        CSensorFactory(){}
        
        ISensorSingleton* createModuleSingleton()
        { 
            return new CSensorSingletonImpl();
        }
        
        virtual ISensor* createModuleByID(const rho::String& strID){ return new CSensorImpl(); };
        
    };
    
}

extern "C" void Init_Sensor_extension()
{
    rho::CSensorFactory::setInstance( new rho::CSensorFactory() );
    rho::Init_Sensor_API();
    
}