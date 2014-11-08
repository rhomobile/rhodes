#pragma once

#include "logging/RhoLog.h"
#include "json/JSONIterator.h"
#include "MethodResult.h"
#include "MethodResultConvertor.h"


namespace rho {
namespace apiGenerator {

using namespace rho::json;
using namespace rho::common;

template<>
class CJSONResultConvertor<CMethodResult>
{
    CMethodResult& m_oResult;

    bool hasObjectClass()
    {
        return m_oResult.getObjectClassPath().length() > 0;
    }
    rho::String getObjectOrString(const rho::String& str, const bool complex_object = false)
    {
        rho::String res;
        if(hasObjectClass())
        {
            res = "{ \"__rhoID\": ";
            if (!complex_object)
            {
                res += "\"" + str + "\" ";
            }
            else
            {
                res += str;
            }
            res += ", \"__rhoClass\": ";
            res += CJSONEntry::quoteValue(m_oResult.getObjectClassPath());
            res += "}";
        } else
        {
            res += CJSONEntry::quoteValue(str);
        }
        return res;
    }

public:
    CJSONResultConvertor(CMethodResult& result) : m_oResult(result) {}

    bool isBool() { return m_oResult.getType() == CMethodResult::eBool; }
    bool isInt() { return m_oResult.getType() == CMethodResult::eInt; }
    bool isDouble() { return m_oResult.getType() == CMethodResult::eDouble; }
    bool isJSON() { return m_oResult.getType() == CMethodResult::eJSON; }
    bool isString() { return m_oResult.getType() == CMethodResult::eString || m_oResult.getType() == CMethodResult::eStringW; }
    bool isArray() { return m_oResult.getType() == CMethodResult::eStringArray || m_oResult.getType() == CMethodResult::eArrayHash; }
    bool isHash() { return m_oResult.getType() == CMethodResult::eStringHash || m_oResult.getType() == CMethodResult::eStringHashVector; }
    bool isError() { return m_oResult.isError(); }
    bool isNil() { return m_oResult.getType() == CMethodResult::eNone; }

    rho::String getBool() { return m_oResult.getBool() ? "true" : "false"; }
    rho::String getInt()
    {
        return convertToStringA(m_oResult.getInt());
    }

    // we should not rely on default floating poing formatting
    rho::String getDouble()
    {
        char buf[100];
        sprintf( buf, "%.18lg", m_oResult.getDouble() );
        return String(buf);
    }
    rho::String getString()
    {
        rho::String str;

        if(m_oResult.getType() == CMethodResult::eString)
            str = m_oResult.getString();
        else if(m_oResult.getType() == CMethodResult::eStringW)
            str = convertToStringA(m_oResult.getStringW());

        return getObjectOrString(str);
    }
    rho::String getArray()
    {
        rho::String resArray;
        // preallocate a bit of memory
        resArray.reserve(128);
        unsigned idx = 0;
        
        resArray = "[";
        if(m_oResult.getType() == CMethodResult::eStringArray)
        {
            for(size_t i = 0; i < m_oResult.getStringArray().size(); ++i)
            {
                if (idx++!=0)
                {
                    resArray += ",";
                }
                resArray += getObjectOrString(m_oResult.getStringArray()[i]);
            }
        } else
        if(m_oResult.getType() == CMethodResult::eArrayHash)
        {
            rho::String buffer;
            
            for(size_t i = 0; i < m_oResult.getHashArray().size(); ++i)
            {
                if (idx++!=0)
                {
                    resArray += ",";
                }
                toString(m_oResult.getHashArray()[i],buffer);
                if (hasObjectClass())
                {
                    buffer = getObjectOrString(buffer, true);
                }
                resArray += buffer;
            }
        }
        resArray += "]";

        return resArray;
    }
    
    rho::String getHash()
    {
        rho::String resHash;
        // preallocate a bit of memory
        resHash.reserve(128);
        rho::String buffer;
        unsigned idx = 0;
        
        // return class with object
        if (m_oResult.getResultClassType() == CMethodResult::rctEntity)
        {
            toString(m_oResult.getStringHash(), resHash);
            return getObjectOrString(resHash, true);
        }
        
        resHash = "{";
        for(rho::Hashtable<rho::String, rho::String>::const_iterator it = m_oResult.getStringHash().begin(); it != m_oResult.getStringHash().end(); ++it)
        {
            if (idx++!=0)
            {
                resHash += ",";
            }

            resHash += CJSONEntry::quoteValue(it->first);
            resHash += ":";
            resHash += getObjectOrString(it->second);
        }

        for(rho::Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> >::const_iterator it = m_oResult.getStringHashL2().begin(); it != m_oResult.getStringHashL2().end(); ++it)
        {
            if (idx++!=0)
            {
                resHash += ",";
            }

            resHash += CJSONEntry::quoteValue(it->first);
            toString(it->second, buffer);
            resHash += ":";
            resHash += buffer;
        }
        
        for(rho::Hashtable<rho::String, rho::Vector<rho::String> >::const_iterator it = m_oResult.getStringHashVector().begin(); it != m_oResult.getStringHashVector().end(); ++it)
        {
            if (idx++!=0)
            {
                resHash += ",";
            }
            
            resHash += CJSONEntry::quoteValue(it->first);
            toString(it->second, buffer);
            resHash += ":";
            resHash += buffer;
        }

        resHash += "}";
        return resHash;
    }

    rho::String& getJSON()
    {
        return m_oResult.getJSON();
    }

    rho::String getError()
    {
        rho::String resHash = "{\"code\":";
        if(m_oResult.getType() == CMethodResult::eArgError)
            resHash += "-32602,\"message\":";
        else if(m_oResult.getType() == CMethodResult::eError)
            resHash += "-32603,\"message\":";

        resHash += CJSONEntry::quoteValue(m_oResult.getErrorString());
        resHash += "}";
        return resHash;
    }
    
protected:
    inline size_t getQuotedValueSizeEstimate(const rho::String& buffer )
    {
        return buffer.length() + 2;
    }
    
    void toString(const rho::Hashtable<rho::String, rho::String>& value, rho::String& buffer )
    {
        // estimate buffer size
        size_t size = 2;
        for(rho::Hashtable<rho::String, rho::String>::const_iterator it = value.begin(); it != value.end(); ++it)
        {
            size+= 2 + getQuotedValueSizeEstimate(it->first) + getQuotedValueSizeEstimate(it->second);
        };
        buffer.clear();
        buffer.reserve(size);
        
        // fill buffer
        buffer = "{";
        for(rho::Hashtable<rho::String, rho::String>::const_iterator it = value.begin(); it != value.end(); ++it)
        {
            if (it != value.begin())
                buffer += ",";
            
            buffer += CJSONEntry::quoteValue(it->first);
            buffer += ":";
            buffer += CJSONEntry::quoteValue(it->second);
        }
        buffer += "}";
    }
    
    void toString(const rho::Vector<rho::String>& value, rho::String& buffer )
    {
        // estimate buffer size
        size_t size = 2;
        for(rho::Vector<rho::String>::const_iterator it = value.begin(); it != value.end(); ++it)
        {
            size += 1 + getQuotedValueSizeEstimate(*it);
        };
        buffer.clear();
        buffer.reserve(size);
        
        // fill buffer
        buffer = "[";
        for(rho::Vector<rho::String>::const_iterator it = value.begin(); it != value.end(); ++it)
        {
            if (it != value.begin())
                buffer += ",";
            
            buffer += CJSONEntry::quoteValue(*it);
        }
        buffer += "]";
    }
};

namespace {
    inline bool isNil(const CJSONEntry& value) {
        return value.isEmpty();
    }

    template <typename VAL>
    bool valueTo(const CJSONEntry& value, VAL& dest) {
        return false;
    }

    template <>
    bool valueTo<int>(const CJSONEntry& value, int& dest) {
        dest = 0;
        if (isNil(value))
            return false;
        if (value.isInteger() || value.isFloat() || value.isBoolean()) {
            dest = value.getInt();
            return true;
        }
        return false;
    }

    template <>
    bool valueTo<double>(const CJSONEntry& value, double& dest) {
        dest = 0;
        if (isNil(value))
            return false;
        if (value.isFloat() || value.isInteger() || value.isBoolean() ) {
            dest = value.getDouble();
            return true;
        }
        return false;
    }

    template <>
    bool valueTo<bool>(const CJSONEntry& value, bool& dest) {
        dest = 0;
        if (isNil(value))
            return false;
        if (value.isBoolean() || value.isInteger() || value.isString()) {
            dest = value.getBoolean();
            return true;
        }
        return false;
    }

    template <>
    bool valueTo<rho::String>(const CJSONEntry& value, rho::String& dest)
    {
        dest.clear();
        if (isNil(value))
            return false;
        dest = value.getString();
        return true;
    }
}

template <typename ArrayVal>
bool rho_value_to_typed_array(const CJSONEntry& value, rho::Vector<ArrayVal>& dest)
{
    dest.clear();

    if (isNil(value))
        return false;

    if (value.isArray())
    {
        CJSONArrayIterator iter(value);

        dest.reserve(iter.getSize());

        ArrayVal elem;

        for (; !iter.isEnd(); iter.next())
        {
            if (valueTo(iter.getCurItem(), elem))
            {
                dest.push_back(elem);
            }
        }
        return true;
    }
    return false;
}

template <typename KeyType,typename ValueType>
bool rho_value_to_typed_hash(const CJSONEntry& value, rho::Hashtable<KeyType, ValueType>& dest)
{
    dest.clear();
    

    if (isNil(value))
        return false;

    if (value.isObject())
    {
        CJSONStructIterator iter(value);

        KeyType _key;
        ValueType _value;

        bool result = true;

        for (; !iter.isEnd(); iter.next())
        {
            _key = iter.getCurKey();
            if (valueTo(iter.getCurValue(), _value)) {
                dest[_key] = _value;
            } else {
                result = false;
                break;
            }
        }

        return  result;
    }

    return  false;
}

template <typename InnnerArrayValue>
bool rho_value_to_typed_array_array(const CJSONEntry& value, rho::Vector< rho::Vector<InnnerArrayValue> >& dest, rho::String& result)
{
    dest.clear();

    if (isNil(value))
        return false;

    if (value.isArray())
    {
        CJSONArrayIterator iter(value);

        dest.reserve(iter.getSize());

        rho::Vector<InnnerArrayValue> elem;

        for (; !iter.isEnd(); iter.next())
        {

            if (rho_value_to_typed_array(iter.getCurItem(), elem))
            {
                dest.push_back(elem);
            }
        }
        return true;
    }
    return false;
}

template <typename InnnerHashKey, typename InnnerHashValue>
bool rho_value_to_typed_array_hash(const CJSONEntry& value, rho::Vector< rho::Hashtable<InnnerHashKey, InnnerHashValue> >& dest, rho::String& result)
{

    dest.clear();

    if (isNil(value))
        return false;

    if (value.isArray())
    {
        CJSONArrayIterator iter(value);

        dest.reserve(iter.getSize());

        rho::Hashtable<InnnerHashKey, InnnerHashValue> buff;

        bool result = true;

        for (; !iter.isEnd(); iter.next())
        {
            dest.push_back(buff);
            if (!rho_value_to_typed_hash(iter.getCurItem(), dest.back()))
            {
                result = false;
                break;
            }
        }

        return result;
    }
    return false;

}

template <typename HashKey, typename InnnerArrayValue>
bool rho_value_to_typed_hash_array(const CJSONEntry& value, rho::Hashtable< HashKey, rho::Vector<InnnerArrayValue> >& dest, rho::String& result)
{
    dest.clear();

    if (isNil(value))
        return false;

    if (value.isObject())
    {
        CJSONStructIterator iter(value);

        HashKey _key;
        rho::Vector<InnnerArrayValue> _value;

        bool result = true;

        for (; !iter.isEnd(); iter.next())
        {
            _key = iter.getCurKey();
            if (rho_value_to_typed_array(iter.getCurValue(), _value)) {
                dest[_key] = _value;
            } else {
                result = false;
                break;
            }
        }
        return  result;
    }
    
    return  false;
    
}

template <typename HashKey, typename InnnerHashKey, typename InnnerHashValue>
bool rho_value_to_typed_hash_hash(const CJSONEntry& value, rho::Hashtable< HashKey, rho::Hashtable<InnnerHashKey, InnnerHashValue> >& dest, rho::String& result)
{
    dest.clear();

    if (isNil(value))
        return false;

    if (value.isObject())
    {
        CJSONStructIterator iter(value);

        HashKey _key;
        rho::Hashtable<InnnerHashKey, InnnerHashValue> _value;
        
        bool result = true;
        
        for (; !iter.isEnd(); iter.next())
        {
            _key = iter.getCurKey();
            if (rho_value_to_typed_hash(iter.getCurValue(), _value)) {
                dest[_key] = _value;
            } else {
                result = false;
                break;
            }
        }
        
        return  result;
    }
    
    return  false;
}



}}

