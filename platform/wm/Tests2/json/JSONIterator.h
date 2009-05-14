#pragma once

#include "common/RhoStd.h"

struct json_object;
struct array_list;

namespace rho {
namespace json {

class CJSONEntry
{
    struct json_object* m_object;
public:
    CJSONEntry(struct json_object * obj);
    CJSONEntry(){}

    boolean isEmpty();
    int getInt(const char* name);
    uint64 getUInt64(const char* name);
    const char* getString(const char* name);

    CJSONEntry getEntry(const char* name);
};

class CJSONArrayIterator
{
    struct json_object* m_rootObject;
    struct array_list*  m_array;
    int                 m_nCurItem;

public:
    CJSONArrayIterator(const String& strData);
    ~CJSONArrayIterator(void);

    boolean isEnd();
    void    next();

    CJSONEntry getCurItem();
};

}
}