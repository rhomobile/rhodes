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

#include "json/JSONIterator.h"
#include "common/StringConverter.h"

#include "json.h"

#ifndef RHO_NO_RUBY
#include "ruby/ext/rho/rhoruby.h"
#endif //RHO_NO_RUBY
#if defined(WINDOWS_PLATFORM)
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

#define is_error(ptr) ((unsigned long)ptr > (unsigned long)-4000L)

static const char *json_hex_chars = "0123456789abcdefABCDEF";

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

boolean CJSONArrayIterator::isEnd() const
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

int CJSONArrayIterator::getSize() const
{
    return m_array == 0 ? 0 : array_list_length(m_array);
}

/////////////////////////////////////////////////////////////////////
//CJSONArray


CJSONEntry CJSONArray::getItem(int nPos)
{
    return CJSONEntry( nPos >= getSize() ? 0 :
        ( struct json_object *) array_list_get_idx(m_array, nPos) );
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

boolean CJSONStructIterator::isEnd() const
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

String CJSONStructIterator::getCurKey() const
{
    return isEnd() ? String() : String((char*)m_curEntry->k);
}

CJSONEntry CJSONStructIterator::getCurValue() const
{
    return CJSONEntry( isEnd() ? 0 :
        ( struct json_object *) m_curEntry->v );
}

String CJSONStructIterator::getCurString() const
{
    return getCurValue().getStringObject();
}

/////////////////////////////////////////////////////////////////////
//CJSONEntry

CJSONEntry::CJSONEntry(struct json_object * obj) : m_rootObject(NULL)
{
    m_object = obj;
}

CJSONEntry::CJSONEntry(const CJSONEntry& oCopy) : m_rootObject(NULL)
{
    m_object = oCopy.m_object;
}

CJSONEntry::CJSONEntry(const char* szData) : m_rootObject(NULL)
{
    m_rootObject = json_tokener_parse(const_cast<char*>(szData));
    
    if ( !m_rootObject || is_error(m_rootObject) )
        m_rootObject = 0;
    
    m_object = m_rootObject;
}

CJSONEntry::~CJSONEntry()
{
    if ( m_rootObject != 0 )
        json_object_put(m_rootObject);
}

boolean CJSONEntry::isEmpty() const
{
    return m_object == 0;
}

boolean CJSONEntry::isString() const
{
    if ( isEmpty() )
        return false;

    return json_object_is_type(m_object, json_type_string) ? true : false;
}

boolean CJSONEntry::isNull() const
{
    if ( isEmpty() )
        return true;

    return json_object_is_type(m_object, json_type_null) ? true : false;
}

boolean CJSONEntry::isArray() const
{
    if ( isEmpty() )
        return false;

    return json_object_is_type(m_object, json_type_array) ? true : false;
}

boolean CJSONEntry::isObject() const
{
    if ( isEmpty() )
        return false;

    return json_object_is_type(m_object, json_type_object) ? true : false;
}
    
boolean CJSONEntry::isInteger() const
{
    if ( isEmpty() )
        return false;

    return json_object_is_type(m_object, json_type_int) ? true : false;
}

boolean CJSONEntry::isFloat() const
{
    if ( isEmpty() )
        return false;

    return json_object_is_type(m_object, json_type_double) ? true : false;
}

boolean CJSONEntry::isBoolean() const
{
    if ( isEmpty() )
        return false;
    
    return json_object_is_type(m_object, json_type_boolean) ? true : false;
}

boolean CJSONEntry::hasName(const String& name) const
{
    return json_object_object_get(m_object,const_cast<char*>(name.c_str())) != NULL;
}
    
String CJSONEntry::getStringObject( const char* name, const char* szDefValue )
{
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
    {
        if (json_object_is_type(obj,json_type_string)) {
            int len = json_object_get_string_len(obj);
            const char* str = json_object_get_string(obj);
            return String(str, len);
        }
        else {
            const char* str = json_object_get_string(obj);
            return String(str);
        }
    }
    else if (szDefValue != 0 )
    {
        return String(szDefValue);
    }
    
    return "";
}

    
String CJSONEntry::getStringObject()
{
    if (isString()) {
        int len = json_object_get_string_len(m_object);
        const char* str = json_object_get_string(m_object);
        return String(str, len);
    }
    else {
        const char* str = json_object_get_string(m_object);
        return String(str);
    }
}


const char* CJSONEntry::getString(const char* name)
{
    const char* szRes = 0;
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
        szRes = json_object_get_string(obj);

    return szRes;
}

const char* CJSONEntry::getString(const char* name, const char* szDefValue)
{
    const char* szRes = szDefValue;
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));
    if ( obj != 0 )
        szRes = json_object_get_string(obj);

    return szRes;
}



const char* CJSONEntry::getString() const
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
        nRes = (uint64)json_object_get_int64(obj);

    return nRes;
}

double CJSONEntry::getDouble(const char* name)
{
    double res = 0;
    struct json_object* obj = json_object_object_get(m_object, const_cast<char*>(name));
    if (obj != 0)
        res = json_object_get_double(obj);

    return res;
}

int CJSONEntry::getInt() const
{
    int nRes = 0;
    if ( m_object != 0 )
        nRes = static_cast<int>(json_object_get_int(m_object));

    return nRes;
}

boolean CJSONEntry::getBoolean() const
{
    boolean nRes = false;
    if ( m_object != 0 )
        nRes = json_object_get_boolean(m_object) != 0 ? true : false;
    
    return nRes;
}

uint64 CJSONEntry::getUInt64() const
{
    uint64 nRes = 0;
    if ( m_object != 0 )
        nRes = (uint64)json_object_get_int(m_object);

    return nRes;
}

double CJSONEntry::getDouble() const
{
    double res = 0;
    if (m_object != 0)
        res = json_object_get_double(m_object);

    return res;
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

CJSONEntry CJSONEntry::getEntry(const char* name) const
{
    struct json_object* obj = json_object_object_get(m_object,const_cast<char*>(name));

    return CJSONEntry(obj);
}

/*static*/ String CJSONEntry::quoteValueW(const StringW& strValue)
{
    return quoteValue( rho::common::convertToStringA(strValue) );
}

/*static*/ String CJSONEntry::quoteValue(const String& strValue)
{
    // string for symbols
    String sym;
    sym.reserve(16);
    
    String strRes;
    // \" + <1 char per symbol in an average case> + \"
    strRes.reserve(strValue.length()+2);
    strRes += "\"";
    
    size_t pos = 0, start_offset = 0, len = strValue.length();
    for(; pos < len; ++pos)
    {
        unsigned char c = strValue[pos];
        if (c < ' ') {
            switch (c) {
                case '\n':
                    sym = "\\n";
                    break;
                case '\r':
                    sym = "\\r";
                    break;
                case '\t':
                    sym = "\\t";
                    break;
                case '\b':
                    sym = "\\b";
                    break;
                case '\f':
                    sym = "\\f";
                    break;
                default:
                    sym = "\\u00";
                    sym += json_hex_chars[c >> 4];
                    sym += json_hex_chars[c & 0xf];
                    break;
            }
        } else if ( c >= 0x80 ) {
            char buf[5];
            sprintf( buf, "\\x%02x", (int)c );
            sym += buf;
        }
        else {
            switch (c) {
                case '"':
                    sym = "\\\"";
                    break;
                case '\\':
                    sym = "\\\\";
                    break;
            }
        }
        if (sym.length() > 0) {
            if(pos - start_offset > 0)
                strRes.append(strValue.c_str() + start_offset, pos - start_offset);
            
            start_offset = pos+1;
            strRes.append(sym);
            sym.clear();
        }
    }

    if ( pos - start_offset > 0 )
        strRes.append(strValue.c_str() + start_offset, pos - start_offset);
    
    strRes.append("\"");
    return strRes;
}

/*static*/ String CJSONEntry::toJSON(const Hashtable<String,String>& hash)
{
    String resHash = "{ ";
    unsigned i = 0;
    for( rho::Hashtable<rho::String, rho::String>::const_iterator it = hash.begin(); it != hash.end(); ++it, ++i)
    {
        if (i > 0)
            resHash += ",";

        resHash += "\"";
        resHash += it->first;
        resHash += "\": ";
        resHash += CJSONEntry::quoteValue(it->second);
    }

    resHash += "}";

    return resHash;
}

/*static*/ String CJSONEntry::toJSON(const Vector<String>& ar)
{
	String strRes = "[";
    for( int i = 0; i < (int)ar.size(); i++ )
    {
        if ( i > 0 )
            strRes += ",";

        strRes += CJSONEntry::quoteValue(ar[i]);
    }
    
	strRes += "]";

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
