#include "SQLite3Base.h"
#include "common/RhodesApp.h"


namespace rho {
namespace database {

IMPLEMENT_LOGCLASS(CSQLite3SingletonBase, "SQLite3");
IMPLEMENT_LOGCLASS(CSQLite3Base, "SQLite3");

rho::common::CAutoPtr< CSQLite3FactoryBase> CSQLite3FactoryBase::m_pInstance;



///////////////////////////////////////
//string constants definiton 

////////////////////////////////////////////////

CSQLite3Base::CSQLite3Base()
{

}
CSQLite3SingletonBase::CSQLite3SingletonBase()
{
    RHODESAPP().getExtManager().registerExtension( "SQLite3", this );
}

CSQLite3SingletonBase::~CSQLite3SingletonBase()
{
    CSQLite3FactoryBase::setInstance(0);
}


}
}
