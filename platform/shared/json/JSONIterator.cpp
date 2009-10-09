#include "JSONIterator.h"

#include "json/json.h"

#if defined(OS_WINCE) || defined (OS_WINDOWS)
#define rho_atoi64 _atoi64
#elif defined(OS_SYMBIAN)
long long int rho_atoi64(const char *instr)
{
	long long int retval = 0;
	int i;
	for (; *instr; instr++)
		retval = 10*retval + (*instr - '0');
	return retval;
}
#else
#define rho_atoi64 atoll
#endif

namespace rho {
namespace json {

CJSONArrayIterator::CJSONArrayIterator(const char* szData)
{
    m_array = 0;
    m_nCurItem = 0;
    m_rootObject = json_tokener_parse(const_cast<char*>(szData));

	if ( !m_rootObject || is_error(m_rootObject) ) 
        m_rootObject = 0;
    else
        m_array = json_object_get_array((struct json_object *)m_rootObject);
}

CJSONArrayIterator::CJSONArrayIterator(CJSONEntry& oEntry, const char* strName)
{
    m_array = 0;
    m_nCurItem = 0;
    m_rootObject = 0;

    CJSONEntry oItem = oEntry.getEntry(strName);
    if ( !oItem.isEmpty() )
        m_array = json_object_get_array( oItem.getObject() );
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

void CJSONArrayIterator::reset(int nPos)
{
    m_nCurItem = nPos;
}

CJSONEntry CJSONArrayIterator::getCurItem()
{
    return CJSONEntry( isEnd() ? 0 :
        ( struct json_object *) array_list_get_idx(m_array, m_nCurItem) );
}

CJSONEntry::CJSONEntry(struct json_object * obj) : m_rootObject(null)
{
    m_object = obj;
}

CJSONEntry::CJSONEntry(const char* szData) : m_rootObject(null)
{
    m_rootObject = json_tokener_parse(const_cast<char*>(szData));
    m_object = m_rootObject;
}

CJSONEntry::~CJSONEntry()
{
    if ( m_rootObject != 0 )
        json_object_put(m_rootObject);
}

boolean CJSONEntry::isEmpty()
{
    return m_object == 0;
}

boolean CJSONEntry::hasName(String name)
{
	return json_object_object_get(m_object,const_cast<char*>(name.c_str())) != null;
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
        nRes = static_cast<int>(json_object_get_int(obj));

    return nRes;
}

uint64 CJSONEntry::getUInt64(const char* name)
{
    uint64 nRes = 0;
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
        nRes = (uint64)json_object_get_int(obj);

    return nRes;
}

/*
uint64 CJSONEntry::getUInt64(const char* name)
{
    uint64 nRes = 0;
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
        nRes = rho_atoi64(json_object_get_string(obj));

    return nRes;
}*/

CJSONEntry CJSONEntry::getEntry(const char* name)
{
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));

    return CJSONEntry(obj);
}

}
}