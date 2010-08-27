#include "DBResult.h"
#include "DBAdapter.h"

namespace rho{
namespace db{

CDBResult::CDBResult(sqlite3_stmt* st,CDBAdapter* pDB) : m_pDB(pDB), m_dbStatement(st)
{
    m_bReportNonUnique=false;
    //m_nErrorCode=0;

    if ( m_pDB )
        m_pDB->Lock();
}

CDBResult::CDBResult() : m_pDB(null), m_dbStatement(null)
{
    m_bReportNonUnique=false;
    //m_nErrorCode=0;
}

CDBResult::~CDBResult(void)
{
    setStatement(null);

    if ( m_pDB )
        m_pDB->Unlock();
}

void CDBResult::setStatement(sqlite3_stmt* st)
{
    if ( m_dbStatement != null )
        sqlite3_reset(m_dbStatement);

    m_dbStatement = st;
}

}
}
