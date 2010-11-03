#include "JSONIterator.h"

#include "json/json.h"
#ifndef RHO_NO_RUBY
#include "ruby/ext/rho/rhoruby.h"
#endif //RHO_NO_RUBY
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

/////////////////////////////////////////////////////////////////////
//CJSONArrayIterator

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

CJSONArrayIterator::CJSONArrayIterator(const CJSONEntry& oEntry, const char* strName)
{
    m_array = 0;
    m_nCurItem = 0;
    m_rootObject = 0;

    CJSONEntry oItem = oEntry.getEntry(strName);
    if ( !oItem.isEmpty() )
        m_array = json_object_get_array( oItem.getObject() );
}

CJSONArrayIterator::CJSONArrayIterator(const CJSONEntry& oEntry)
{
    m_array = 0;
    m_nCurItem = 0;
    m_rootObject = 0;

    m_array = json_object_get_array( oEntry.getObject() );
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

/////////////////////////////////////////////////////////////////////
//CJSONStructIterator

CJSONStructIterator::CJSONStructIterator(const char* szData)
{
    m_struct = 0;
    m_curEntry = 0;
    m_rootObject = json_tokener_parse(const_cast<char*>(szData));

	if ( !m_rootObject || is_error(m_rootObject) ) 
        m_rootObject = 0;
    else
    {
        m_struct = json_object_get_object((struct json_object *)m_rootObject);
        m_curEntry = m_struct->head;
    }
}

CJSONStructIterator::CJSONStructIterator(const CJSONEntry& oEntry, const char* strName)
{
    m_struct = 0;
    m_curEntry = 0;
    m_rootObject = 0;

    CJSONEntry oItem = oEntry.getEntry(strName);
    if ( !oItem.isEmpty() )
    {
        m_struct = json_object_get_object( oItem.getObject() );
        m_curEntry = m_struct->head;
    }
}

CJSONStructIterator::CJSONStructIterator(const CJSONEntry& oEntry)
{
    m_rootObject = 0;

    m_struct = json_object_get_object( oEntry.getObject() );
    m_curEntry = m_struct->head;
}

CJSONStructIterator::~CJSONStructIterator(void)
{
    if ( m_rootObject != 0 )
        json_object_put(m_rootObject);
}

boolean CJSONStructIterator::isEnd()
{
    return m_curEntry == 0;
}

void  CJSONStructIterator::next()
{
    m_curEntry = m_curEntry->next;
}

void CJSONStructIterator::reset()
{
    m_curEntry = m_struct->head;
}

String CJSONStructIterator::getCurKey()
{
    return isEnd() ? String() : String((char*)m_curEntry->k);
}

CJSONEntry CJSONStructIterator::getCurValue()
{
    return CJSONEntry( isEnd() ? 0 :
        ( struct json_object *) m_curEntry->v );
}

String CJSONStructIterator::getCurString()
{
    return getCurValue().getString();
}

/////////////////////////////////////////////////////////////////////
//CJSONEntry

CJSONEntry::CJSONEntry(struct json_object * obj) : m_rootObject(null)
{
    m_object = obj;
}

CJSONEntry::CJSONEntry(const CJSONEntry& oCopy) : m_rootObject(null)
{
    m_object = oCopy.m_object;
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

const char* CJSONEntry::getString()
{
    return json_object_get_string(m_object);
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

CJSONEntry CJSONEntry::getEntry(const char* name)const
{
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));

    return CJSONEntry(obj);
}

/*static*/ String CJSONEntry::quoteValue(const String& strValue)
{
    int pos = 0, start_offset = 0;
    unsigned char c;
    String strRes = "\"";
    const char* str = strValue.c_str();
    do 
    {
        c = str[pos];
        switch(c) {
        case '\0':
            break;
        case '\b':
        case '\n':
        case '\r':
        case '\t':
        case '"':
        case '\\':
        case '/':
            if(pos - start_offset > 0)
                strRes.append(str + start_offset, pos - start_offset);

            if(c == '\b') strRes.append( "\\b", 2 );
            else if(c == '\n') strRes.append( "\\n", 2);
            else if(c == '\r') strRes.append( "\\r", 2);
            else if(c == '\t') strRes.append( "\\t", 2);
            else if(c == '"') strRes.append( "\\\"", 2);
            else if(c == '\\') strRes.append( "\\\\", 2);
            else if(c == '/') strRes.append( "\\/", 2);

            start_offset = ++pos;
            break;
        default:
            if(c < ' ') 
            {
                if(pos - start_offset > 0)
                    strRes.append( str + start_offset, pos - start_offset);

                char buf[128];
                int nSize = snprintf(buf, 128, "\\u00%c%c", json_hex_chars[c >> 4], json_hex_chars[c & 0xf]);
                strRes.append(buf, nSize);

                start_offset = ++pos;
            }else 
                pos++;
        }
    } while(c);

    if ( strRes.length() == 1 )
        return "\"" + strValue + "\"";

    if ( pos - start_offset > 0 )
        strRes.append( str + start_offset, pos - start_offset);

    strRes.append("\"");
    return strRes;
}

}
}
#ifndef RHO_NO_RUBY
extern "C" VALUE rho_json_quote_value(VALUE v,VALUE str)
{
    rho::String strRes = rho::json::CJSONEntry::quoteValue(getStringFromValue(str));

    return rho_ruby_create_string(strRes.c_str());
}
#endif //RHO_NO_RUBY