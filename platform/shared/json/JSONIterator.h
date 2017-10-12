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
    CJSONEntry() : m_rootObject(NULL){}
    ~CJSONEntry();

    boolean isEmpty() const;
	boolean hasName(const String& name) const;
    boolean isString() const;
    boolean isArray() const;
    boolean isObject() const;
    boolean isNull() const;
    boolean isInteger() const;
    boolean isFloat() const;
    boolean isBoolean() const;

    int getInt(const char* name);
    uint64 getUInt64(const char* name);
    double getDouble(const char* name);
    int getInt() const;
    uint64 getUInt64() const;
    double getDouble() const;
    boolean getBoolean() const;

    const char* getString(const char* name);
    const char* getString(const char* name, const char* szDefValue );
    const char* getString() const;
 
    String getStringObject( const char* name, const char* szDefValue = 0 );
    String getStringObject();
    
    CJSONEntry getEntry(const char* name) const;

    struct json_object* getObject() const { return m_object; }

    static String quoteValue(const String& strValue);
    static String quoteValueW(const StringW& strValue);

    static String toJSON(const Hashtable<String,String>& hash);
    static String toJSON(const Vector<String>& ar);
};

class CJSONArrayIterator
{
protected:
    struct json_object* m_rootObject;
    struct array_list*  m_array;
    int                 m_nCurItem;

public:
    CJSONArrayIterator(const char* szData);
	CJSONArrayIterator(const CJSONEntry& oEntry, const char* strName);
    CJSONArrayIterator(const CJSONEntry& oEntry);
    ~CJSONArrayIterator(void);

    boolean isEnd() const;
    void    next();
    void    reset(int nPos);
    int     getCurPos() const { return m_nCurItem; }

    CJSONEntry getCurItem();

    int getSize() const;
};

class CJSONArray : public CJSONArrayIterator
{
public:
    CJSONArray(const char* szData):CJSONArrayIterator(szData){}
    CJSONArray(const CJSONEntry& oEntry, const char* strName):CJSONArrayIterator(oEntry, strName){}
    CJSONArray(const CJSONEntry& oEntry):CJSONArrayIterator(oEntry){}

    CJSONEntry getItem(int nPos);
    CJSONEntry operator[](int nPos){return getItem(nPos);}
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

    boolean isEnd() const;
    void    next();
    void    reset();

    String     getCurKey() const;
    CJSONEntry getCurValue() const;
    String     getCurString() const;
    
};

}
}
