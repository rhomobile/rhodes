/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
