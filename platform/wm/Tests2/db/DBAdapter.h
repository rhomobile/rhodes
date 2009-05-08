#pragma once

#include "DBResult.h"

namespace rho{
namespace db{

class CDBAdapter
{
public:
    CDBAdapter(void){}
    ~CDBAdapter(void){}

    template<typename T>
    void bind(T p)
    {
        //TODO: bind
    }

    template<typename T1, typename T2>
    CDBResult* executeSQL( const char* szSt, T1 p1, T2 p2 )
    {
        bind(p1);
        bind(p2);

        return executeStatement();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    CDBResult* executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
    {
        bind(p1);
        bind(p2);
        bind(p3);
        bind(p4);
        bind(p5);
        bind(p6);

        return executeStatement();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    CDBResult* executeSQL( const char* szSt, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
    {
        bind(p1);
        bind(p2);
        bind(p3);
        bind(p4);
        bind(p5);
        bind(p6);
        bind(p7);

        return executeStatement();
    }

    template<typename T1>
    CDBResult* executeSQL( const char* szSt, T1 p1)
    {
        bind(p1);
        return executeStatement();
    }

    CDBResult* executeSQL( const char* szSt)
    {
        return executeStatement();
    }

//private:
    virtual CDBResult* executeStatement()
    {
        return new CDBResult();
    }

};

}
}

