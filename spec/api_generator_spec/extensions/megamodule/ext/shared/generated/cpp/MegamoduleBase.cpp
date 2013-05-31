#include "MegamoduleBase.h"
#include "common/RhodesApp.h"


namespace rho {
namespace examples {

IMPLEMENT_LOGCLASS(CMegamoduleSingletonBase, "Megamodule");
IMPLEMENT_LOGCLASS(CMegamoduleBase, "Megamodule");

rho::common::CAutoPtr< CMegamoduleFactoryBase> CMegamoduleFactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 
const char IMegamodule::CONST1_STRING[] = "value1"; 
const char IMegamodule::CONST_STRING_1[] = "const1"; 
const char IMegamodule::CONST_STRING_2[] = "const2"; 

////////////////////////////////////////////////

CMegamoduleBase::CMegamoduleBase()
{

    m_mapPropAccessors["StringProperty"] = 0;
    m_mapPropAccessors["string_property"] = 0;
    m_mapPropAccessors["IntegerProperty"] = 0;
    m_mapPropAccessors["integer_property"] = 0;


}

void CMegamoduleBase::getProperty( const rho::String& propertyName, CMethodResult& oResult)
{
    CMethodAccessor< IMegamodule >* pAccessor = m_mapPropAccessors[propertyName];
    if ( pAccessor )
        pAccessor->callGetter(this, oResult);
    else
    {
        
        oResult.set(m_hashProps[propertyName]);
        
    }
}

void CMegamoduleBase::getProperties( const rho::Vector<rho::String>& arrayofNames, CMethodResult& oResult)
{
    rho::Hashtable<rho::String, rho::String> res;
    oResult.setCollectionMode(true);
    for ( int i = 0; i < (int)arrayofNames.size(); i++ )
    {
        getProperty(arrayofNames[i], oResult);

        if ( oResult.isError() )
            break;

        res[arrayofNames[i]] = oResult.toString();
    }

    oResult.setCollectionMode(false);
    if ( oResult.isError() )
        oResult.callCallback();
    else
        oResult.set(res);
}

void CMegamoduleBase::getAllProperties(CMethodResult& oResult)
{
    rho::Hashtable<rho::String, rho::String> res;
    oResult.setCollectionMode(true);
    
    // existing properties
    for ( rho::Hashtable<rho::String, rho::apiGenerator::CMethodAccessor< ISystemSingleton > *>::const_iterator it = m_mapPropAccessors.begin();  it != m_mapPropAccessors.end(); ++it )
    {
        getProperty(it->first, oResult);
        
        if ( oResult.isError() )
            break;
        
        res[it->first] = oResult.toString();
    }
    
    
    if (!oResult.isError())
    {
        // user defined properties
        for ( rho::Hashtable<rho::String, rho::String>::const_iterator it = m_hashProps.begin();  it != m_hashProps.end(); ++it )
        {
            res[it->first] = it->second;
        }
    }
    

    oResult.setCollectionMode(false);
    if ( oResult.isError() )
        oResult.callCallback();
    else
        oResult.set(res);
}

void CMegamoduleBase::setProperty( const rho::String& propertyName,  const rho::String& propertyValue, CMethodResult& oResult)
{
    CMethodAccessor< IMegamodule >* pAccessor = m_mapPropAccessors[propertyName];
    if (pAccessor && pAccessor->hasSetter())
        m_mapPropAccessors[propertyName]->callSetter(this, propertyValue, oResult);
    else
    {
        
        m_hashProps.put(propertyName, propertyValue);
        
    }
}

void CMegamoduleBase::setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult)
{
    for ( rho::Hashtable<rho::String, rho::String>::const_iterator it = propertyMap.begin();  it != propertyMap.end(); ++it )
    {
        setProperty( it->first, it->second, oResult );
        if ( oResult.isError() )
            break;
    }
}

void CMegamoduleBase::clearAllProperties(CMethodResult& oResult)
{

    m_hashProps.clear();

    // ToDo: set default values to existing properties 
}



void CMegamoduleBase::getStringProperty(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "StringProperty", oResult); 
}

void CMegamoduleBase::getIntegerProperty(rho::apiGenerator::CMethodResult& oResult)
{ 
    getProperty( "IntegerProperty", oResult); 
}

void CMegamoduleBase::setIntegerProperty( int IntegerProperty, rho::apiGenerator::CMethodResult& oResult)
{ 
    setProperty( "IntegerProperty", rho::common::convertToStringA(IntegerProperty), oResult );
}
CMegamoduleSingletonBase::CMegamoduleSingletonBase()
{
    RHODESAPP().getExtManager().registerExtension( "Megamodule", this );
}

CMegamoduleSingletonBase::~CMegamoduleSingletonBase()
{
    CMegamoduleFactoryBase::setInstance(0);
}


}
}
