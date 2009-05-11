#include "JSONIterator.h"

#include "json/json.h"

#if defined(OS_SYMBIAN)
long long int my_atoll(const char *instr)
{
  long long int retval = 0;
  int i;
  for (; *instr; instr++)
    retval = 10*retval + (*instr - '0');
  return retval;
}
#define rho_atoi64 rho_atoll
#else
#define rho_atoi64 _atoi64
#endif

namespace rho {
namespace json {

CJSONArrayIterator::CJSONArrayIterator(const String& strData)
{
    m_array = 0;
    m_nCurItem = 0;
    m_rootObject = json_tokener_parse(const_cast<char*>(strData.c_str()));

	if ( !m_rootObject || is_error(m_rootObject) ) 
        m_rootObject = 0;
    else
        m_array = json_object_get_array((struct json_object *)m_rootObject);
}

CJSONArrayIterator::~CJSONArrayIterator(void)
{
    if ( m_rootObject != 0 )
        json_object_put(m_rootObject);
}

boolean CJSONArrayIterator::isEnd()
{
    return !(m_array != 0 && m_nCurItem < array_list_length(m_array));
}

void  CJSONArrayIterator::next()
{
    m_nCurItem++;
}

CJSONEntry CJSONArrayIterator::getCurItem()
{
    return CJSONEntry( isEnd() ? 0 :
        ( struct json_object *) array_list_get_idx(m_array, m_nCurItem) );
}

CJSONEntry::CJSONEntry(struct json_object * obj)
{
    m_object = obj;
}

boolean CJSONEntry::isEmpty()
{
    return m_object == 0;
}

const char* CJSONEntry::getString(const char* name)
{
    const char* szRes = 0;
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
        szRes = json_object_get_string(obj);

    return szRes;
}

int CJSONEntry::getInt(const char* name)
{
    int nRes = 0;
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
        nRes = json_object_get_int(obj);

    return nRes;
}

uint64 CJSONEntry::getUInt64(const char* name)
{
    uint64 nRes = 0;
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
        nRes = rho_atoi64(json_object_get_string(obj));

    return nRes;
}

CJSONEntry CJSONEntry::getEntry(const char* name)
{
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));

    return CJSONEntry(obj);
}

}
}