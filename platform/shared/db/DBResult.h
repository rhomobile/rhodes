#pragma once

#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhoMutexLock.h"
#include "sqlite3.h"

namespace rho{
namespace db{

class CDBResult
{
    sqlite3_stmt* m_dbStatement;
    common::CMutexLock m_lockDB;
    boolean m_bReportNonUnique;
    int     m_nErrorCode;
public:
    CDBResult(sqlite3_stmt* st,common::CMutex& mx) : m_lockDB(mx), m_dbStatement(st){m_bReportNonUnique=false;m_nErrorCode=0;}
    CDBResult(common::CMutex& mx) : m_lockDB(mx), m_dbStatement(null){m_bReportNonUnique=false;m_nErrorCode=0;}

    ~CDBResult(void);

    void setStatement(sqlite3_stmt* st);
    sqlite3_stmt* getStatement(){ return m_dbStatement; }
    boolean getReportNonUnique(){ return m_bReportNonUnique; }
    void setReportNonUnique(boolean bSet){ m_bReportNonUnique = bSet; }
    boolean isNonUnique(){ return m_nErrorCode==SQLITE_CONSTRAINT; }
    int     getErrorCode(){ return m_nErrorCode; }
    void    setErrorCode(int nError){ m_nErrorCode=nError; }

    virtual bool isEnd(){ return m_dbStatement == null; }
    void next()
    {
        if ( sqlite3_step(m_dbStatement) != SQLITE_ROW )
            setStatement(null);
    }

    virtual String getStringByIdx(int nCol)
    {
        char* res = (char *)sqlite3_column_text(m_dbStatement, nCol);
        return res ? res : String();
    }

    int getIntByIdx(int nCol)
    {
        return sqlite3_column_int(m_dbStatement, nCol);
    }

    uint64 getUInt64ByIdx(int nCol)
    {
        return sqlite3_column_int64(m_dbStatement, nCol);
    }
//private:
    CDBResult() : m_lockDB(*new common::CMutex() ){} //TEST ONLY

};

typedef rho::common::CAutoPtr<rho::db::CDBResult> DBResultPtr;
#define DBResult(name, call)\
    rho::db::DBResultPtr p##name = call;\
    rho::db::CDBResult& name = *p##name;

}
}
