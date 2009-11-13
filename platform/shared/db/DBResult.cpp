#include "DBResult.h"

namespace rho{
namespace db{

CDBResult::~CDBResult(void)
{
    setStatement(null);
}

void CDBResult::setStatement(sqlite3_stmt* st)
{
    if ( m_dbStatement != null )
        sqlite3_reset(m_dbStatement);

    m_dbStatement = st;
}

}
}
