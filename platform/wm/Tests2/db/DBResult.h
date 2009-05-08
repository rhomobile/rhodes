#pragma once

#include "common/RhoStd.h"
#include "common/AutoPointer.h"

namespace rho{
namespace db{

class CDBResult
{
public:
    CDBResult(void);
    ~CDBResult(void);

    virtual bool isEnd(){ return true; }
    void next(){}

    virtual String getStringByIdx(int nCol)
    {
        return "";
    }
};

#define DBResult(name, call)\
    rho::common::CAutoPtr<rho::db::CDBResult> p##name = call;\
    rho::db::CDBResult& name = *p##name;

}
}
