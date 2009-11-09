#pragma once

#include "common/RhoStd.h"

struct json_object;
struct array_list;

namespace rho {
namespace json {

class CJSONEntry
{
    struct json_object* m_rootObject;
    struct json_object* m_object;
public:
    CJSONEntry(struct json_object * obj);
    CJSONEntry(const char* szData);
    CJSONEntry() : m_rootObject(null){}
    ~CJSONEntry();

    boolean isEmpty();
	boolean hasName(String name);

    int getInt(const char* name);
    uint64 getUInt64(const char* name);
    const char* getString(const char* name);

    CJSONEntry getEntry(const char* name);

    struct json_object* getObject(){ return m_object; }
};

class CJSONArrayIterator
{
    struct json_object* m_rootObject;
    struct array_list*  m_array;
    int                 m_nCurItem;

public:
    CJSONArrayIterator(const char* szData);
	CJSONArrayIterator(CJSONEntry& oEntry, const char* strName);
    ~CJSONArrayIterator(void);

    boolean isEnd();
    void    next();
    void    reset(int nPos);
    int     getCurPos(){ return m_nCurItem; }

    CJSONEntry getCurItem();
};

}
}
