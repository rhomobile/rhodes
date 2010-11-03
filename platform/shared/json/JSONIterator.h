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
