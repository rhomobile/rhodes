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

#pragma once

#include "common/RhoStd.h"

struct json_object;
struct array_list;
struct lh_entry;
struct lh_table;

namespace rho {
namespace json {

class CJSONEntry
{
    struct json_object* m_rootObject;
    struct json_object* m_object;
public:
    CJSONEntry(struct json_object * obj);
    CJSONEntry(const char* szData);
    CJSONEntry(const CJSONEntry& oCopy);
    CJSONEntry() : m_rootObject(null){}
    ~CJSONEntry();

    boolean isEmpty();
	boolean hasName(String name);

    int getInt(const char* name);
    uint64 getUInt64(const char* name);
    double getDouble(const char* name);
    const char* getString(const char* name);
    const char* getString();

    CJSONEntry getEntry(const char* name)const;

    struct json_object* getObject()const{ return m_object; }

    static String quoteValue(const String& strValue);
};

class CJSONArrayIterator
{
    struct json_object* m_rootObject;
    struct array_list*  m_array;
    int                 m_nCurItem;

public:
    CJSONArrayIterator(const char* szData);
	CJSONArrayIterator(const CJSONEntry& oEntry, const char* strName);
    CJSONArrayIterator(const CJSONEntry& oEntry);
    ~CJSONArrayIterator(void);

    boolean isEnd();
    void    next();
    void    reset(int nPos);
    int     getCurPos(){ return m_nCurItem; }

    CJSONEntry getCurItem();
};

class CJSONStructIterator
{
    struct json_object* m_rootObject;
    struct lh_table*    m_struct;
    struct lh_entry*    m_curEntry;
public:
    CJSONStructIterator(const char* szData);
	CJSONStructIterator(const CJSONEntry& oEntry, const char* strName);
    CJSONStructIterator(const CJSONEntry& oEntry);
    ~CJSONStructIterator(void);

    boolean isEnd();
    void    next();
    void    reset();

    String     getCurKey();
    CJSONEntry getCurValue();
    String     getCurString();

};

}
}
