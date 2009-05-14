#include "DBResult.h"

namespace rho{
namespace db{

CDBResult::CDBResult(void)
{
}

CDBResult::~CDBResult(void)
{
    //check ref counter
    //TODO: unlock db
}

}
}