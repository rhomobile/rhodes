#pragma once

#include "logging/RhoLog.h"
#include "ruby/ext/rho/rhoruby.h"
#include "MethodResult.h"
#include "MethodResultConvertor.h"

extern "C" VALUE rjson_tokener_parse(const char *str, char** pszError );

namespace rho {
namespace apiGenerator {

using namespace rho::common;

template<>
class CRubyResultConvertor<CMethodResult> {
    CMethodResult& m_oResult;
    bool m_bForCallback;

    bool hasObjectClass()
    {
        return m_oResult.getObjectClass() != 0 || m_oResult.getObjectClassPath().length() > 0;
    }
    VALUE getObjectWithId(const rho::String& id)
    {
        VALUE res;
        static VALUE classValue = m_oResult.getObjectClass();
        if(rho_ruby_is_NIL(classValue))
        {
            classValue = rho_ruby_get_class_byname(m_oResult.getObjectClassPath().c_str());
        }
        if(!rho_ruby_is_NIL(classValue))
        {
            res = rho_ruby_create_object_with_id(classValue, id.c_str());
        } else
        {
            res = rho_ruby_get_NIL();
        }
        return res;
    }

    VALUE getObjectOrString(const rho::String& id)
    {
        VALUE valObj = 0;
        if(hasObjectClass())
        {
            valObj = getObjectWithId(id.c_str());
            if(rho_ruby_is_NIL(valObj))
                valObj = rho_ruby_create_string(id.c_str());
        } else
            valObj = rho_ruby_create_string(id.c_str());
        return valObj;
    }
public:
    CRubyResultConvertor(CMethodResult& result, bool forCallback) : m_oResult(result), m_bForCallback(forCallback) {}

    bool isBool() { return m_oResult.getType() == CMethodResult::eBool; }
    bool isInt() { return m_oResult.getType() == CMethodResult::eInt; }
    bool isDouble() { return m_oResult.getType() == CMethodResult::eDouble; }
    bool isString() { return m_oResult.getType() == CMethodResult::eString || m_oResult.getType() == CMethodResult::eStringW; }
    bool isJSON() { return m_oResult.getType() == CMethodResult::eJSON; }
    bool isArray() { return m_oResult.getType() == CMethodResult::eStringArray || m_oResult.getType() == CMethodResult::eArrayHash; }
    bool isHash() { return m_oResult.getType() == CMethodResult::eStringHash || m_oResult.getType() == CMethodResult::eStringHashVector; }
    bool isError() { return m_oResult.isError(); }
    bool isNil() { return m_oResult.getType() == CMethodResult::eNone; }

    VALUE getBool() { return rho_ruby_create_boolean(m_oResult.getBool() ? 1 : 0); }
    VALUE getInt() { return rho_ruby_create_integer(m_oResult.getInt()); }
    VALUE getDouble() { return rho_ruby_create_double(m_oResult.getDouble()); }
    VALUE getString()
    {
        VALUE res;
        rho::String str;

        RAWTRACEC("CRubyResultConvertor", "getString()");

        if(m_oResult.getType() == CMethodResult::eString)
            str = m_oResult.getString();
        else if(m_oResult.getType() == CMethodResult::eStringW)
            str = convertToStringA(m_oResult.getStringW());

        RAWTRACEC1("CRubyResultConvertor", "getString(): %s", str.c_str());

        if(hasObjectClass())
        {
            RAWTRACEC("CRubyResultConvertor", "getString(): create object by id");
            res = getObjectWithId(str);
        } else
        {
            RAWTRACEC("CRubyResultConvertor", "getString(): create string");
            res = rho_ruby_create_string(str.c_str());
        }

        return res;
    }
    VALUE getArray()
    {
        RAWTRACEC("CRubyResultConvertor", "getArray()");

        CHoldRubyValue valArray(rho_ruby_create_array());

        if(m_oResult.getType() == CMethodResult::eStringArray)
        {
            for(size_t i = 0; i < m_oResult.getStringArray().size(); ++i)
            {
                rho_ruby_add_to_array(valArray, getObjectOrString(m_oResult.getStringArray()[i]));
            }
        } else
        if(m_oResult.getType() == CMethodResult::eArrayHash)
        {
            static VALUE classValue = 0;
            if (m_oResult.getResultClassType() == CMethodResult::rctEntity)
            {
                classValue = rho_ruby_get_class_byname(m_oResult.getObjectClassPath().c_str());
            }
            
            for(size_t i = 0; i < m_oResult.getHashArray().size(); ++i)
            {
                CHoldRubyValue valItem(rho_ruby_createHash());

                for(rho::Hashtable<rho::String, rho::String>::const_iterator it = m_oResult.getHashArray()[i].begin(); it != m_oResult.getHashArray()[i].end(); ++it)
                {
                    addStrToHash(valItem, it->first.c_str(), it->second.c_str());
                }
                
                if (m_oResult.getResultClassType() == CMethodResult::rctEntity)
                {
                    CHoldRubyValue resItem( rho_ruby_callmethod_arg(classValue, "create_instance", valItem) );
                    
                    rho_ruby_add_to_array(valArray, resItem);
                } else {
                    rho_ruby_add_to_array(valArray, valItem);
                }
            }
        }

        return valArray;
    }
    VALUE getHash()
    {
        //RAWLOGC_INFO("CRubyResultConvertor", "getHash()");
        
        if (m_oResult.getResultClassType() != CMethodResult::rctEntity)
        {
            CHoldRubyValue valHash(rho_ruby_createHash());

            for(rho::Hashtable<rho::String, rho::String>::const_iterator it = m_oResult.getStringHash().begin(); it != m_oResult.getStringHash().end(); ++it)
            {
                //RAWLOGC_INFO2("CRubyResultConvertor", "getHash(): %s->%s", it->first.c_str(), it->second.c_str());
                addHashToHash(valHash, it->first.c_str(), getObjectOrString(it->second));
            }

            for(rho::Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> >::const_iterator it = m_oResult.getStringHashL2().begin(); it != m_oResult.getStringHashL2().end(); ++it)
            {
                CHoldRubyValue valHashL2(rho_ruby_createHash());

                //RAWLOGC_INFO1("CRubyResultConvertor", "getHash(): %s->L2 HASH", it->first.c_str());

                for(rho::Hashtable<rho::String, rho::String>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //RAWLOGC_INFO2("CRubyResultConvertor", "getHash(): L2: %s->%s", it2->first.c_str(), it2->second.c_str());
                    addStrToHash(valHashL2, it2->first.c_str(), it2->second.c_str());
                }

                addHashToHash(valHash, it->first.c_str(), valHashL2 );
            }
            
            for(rho::Hashtable<rho::String, rho::Vector<rho::String> >::const_iterator it = m_oResult.getStringHashVector().begin(); it != m_oResult.getStringHashVector().end(); ++it)
            {
                CHoldRubyValue valArray(rho_ruby_create_array());
                
                for(rho::Vector<rho::String>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    //RAWLOGC_INFO2("CRubyResultConvertor", "getHash(): L2: %s->%s", it2->first.c_str(), it2->second.c_str());
                    rho_ruby_add_to_array(valArray, getObjectOrString(*it2));
                }
                
                addHashToHash(valHash, it->first.c_str(), valArray );
            }
            return valHash;
        }  else {
            static VALUE classValue = rho_ruby_get_class_byname(m_oResult.getObjectClassPath().c_str());
            CHoldRubyValue valHash (rho_ruby_createHash());
            
            for(rho::Hashtable<rho::String, rho::String>::const_iterator it = m_oResult.getStringHash().begin(); it != m_oResult.getStringHash().end(); ++it)
            { 
                addStrToHash(valHash, it->first.c_str(), it->second.c_str());
            }
            
            CHoldRubyValue resItem( rho_ruby_callmethod_arg(classValue, "create_instance", valHash) );
            return resItem;
        }
    }

    VALUE getJSON()
    {
        char* szError = 0;
        VALUE valObj = rjson_tokener_parse(m_oResult.getJSON().c_str(), &szError);

        if ( !valObj )
        {
            LOG(ERROR) + "Incorrect json body.Error:" + (szError ? szError : "");
            if ( szError )
                free(szError);

            valObj = rho_ruby_get_NIL();
        }

        return valObj;
    }

    const char* getResultParamName()
    {
        switch(m_oResult.getType())
        {
        case CMethodResult::eArgError:
            return "argError";
        case CMethodResult::eError:
            return "runtimeError";
        default:
            return m_oResult.getParamName().c_str();
        }
    }
    VALUE getError()
    {
        RAWTRACEC("CRubyResultConvertor", "getErrorMessage()");

        if(m_bForCallback)
        {
            return rho_ruby_create_string(m_oResult.getErrorString().c_str());
        } else
        {
            if(m_oResult.getType() == CMethodResult::eArgError)
                rho_ruby_raise_argerror(m_oResult.getErrorString().c_str());
            else if(m_oResult.getType() == CMethodResult::eError)
                rho_ruby_raise_runtime(m_oResult.getErrorString().c_str());
        }
        return rho_ruby_get_NIL();
    }
};

namespace {

inline bool isNil(VALUE value) {
    return (rho_ruby_is_NIL(value) || value == 0);
}

template <typename ArrayVal>
bool valueTo(VALUE value, ArrayVal& dest) {
    return false;
}

template <>
bool valueTo<int>(VALUE value, int& dest) {
    return rho_ruby_to_int(value, &dest) != 0;
}

template <>
bool valueTo<double>(VALUE value, double& dest) {
    return rho_ruby_to_double(value, &dest) != 0;
}

template <>
bool valueTo<bool>(VALUE value, bool& dest) {
    int dst = 0;
    bool res = rho_ruby_to_bool(value, &dst) != 0;
    dest = dst != 0;
    return res;
}

template <>
bool valueTo<rho::String>(VALUE value, rho::String& dest)
{
    dest.clear();
    const char *cptr = 0;
    int len = 0;
    if (rho_ruby_to_str(value, &cptr, &len)){
        if (cptr!=0 && len != 0) {
            size_t len = static_cast<size_t>(len);
            dest.assign(cptr, len);
        }
    }
    return true;
}

}

template <typename ArrayVal>
bool rho_value_to_typed_array(VALUE value, rho::Vector<ArrayVal>& dest)
{
    if (isNil(value))
        return false;

    if (!rho_ruby_is_array(value))
        return false;

    bool result = true;
    int len = rho_ruby_array_get_size(value);

    dest.clear();
    dest.reserve(len);

    ArrayVal elem;
    for (int i=0; i<len; i++)
    {
        dest.push_back(elem);
        if (valueTo(rho_ruby_array_get(value, i), dest.back()))
        {
            result = false;
        }
    }
    return result;
}

template <typename KeyType,typename ValueType>
bool rho_value_to_typed_hash(VALUE value, rho::Hashtable<KeyType, ValueType>& dest)
{
    if (!isNil(value))
        return false;

    if (rho_ruby_is_hash(value))
        return false;

    bool result = false;

    VALUE keys = rho_ruby_hash_keys(value);
    VALUE akey;

    KeyType _key;
    ValueType _value;

    while ((akey = rho_ruby_each_key(keys))) {
        VALUE val = rho_ruby_hash_get(value, akey);
        if (!(valueTo(akey, _key) && valueTo(val, dest[_key]))) {
            result = false;
        }
    }
    return result;
}

template <typename InnnerArrayValue>
bool rho_value_to_typed_array_array( VALUE value, rho::Vector< rho::Vector<InnnerArrayValue> >& dest, rho::String& error)
{

    dest.clear();

    if (isNil(value))
        return false;

    if (!rho_ruby_is_array(value))
        return false;

    bool result = true;

    int len = rho_ruby_array_get_size(value);
    dest.reserve(len);

    rho::Vector<InnnerArrayValue> buff;

    for (int i=0; i<len; i++)
    {
        dest.push_back(buff);
        VALUE inner = rho_ruby_array_get(value, i);
        if (!rho_value_to_typed_array(inner, dest.back()))
        {
            result = false;
        }
    }

    return result;
}

template <typename InnnerHashKey, typename InnnerHashValue>
bool rho_value_to_typed_array_hash( VALUE value, rho::Vector< rho::Hashtable<InnnerHashKey, InnnerHashValue> >& dest, rho::String& error)
{

    dest.clear();

    if (isNil(value))
        return false;

    if (!rho_ruby_is_array(value))
        return false;

    bool result = true;

    int len = rho_ruby_array_get_size(value);
    dest.reserve(len);

    rho::Hashtable<InnnerHashKey, InnnerHashValue> buff;
    for (int i=0; i<len; i++)
    {
        dest.push_back(buff);
        VALUE inner = rho_ruby_array_get(value, i);
        if (!rho_value_to_typed_hash(inner, dest.back()))
        {
            result = false;
        }
    }

    return result;
}

template <typename HashKey, typename InnnerArrayValue>
bool rho_value_to_typed_hash_array( VALUE value, rho::Hashtable< HashKey, rho::Vector<InnnerArrayValue> >& dest, rho::String& error)
{
    if (isNil(value))
        return false;

    if (rho_ruby_is_hash(value))
        return false;

    bool result = true;

    VALUE keys = rho_ruby_hash_keys(value);
    VALUE akey;

    HashKey _key;
    rho::Vector<InnnerArrayValue> _value;
    while ((akey = rho_ruby_each_key(keys))) {
        VALUE val = rho_ruby_hash_get(value, akey);
        if (!(valueTo(akey, _key) && rho_value_to_typed_array(val, dest[_key]))) {
            result = false;
        }
    }

    return  result;
}

template <typename HashKey, typename InnnerHashKey, typename InnnerHashValue>
bool rho_value_to_typed_hash_hash( VALUE value, rho::Hashtable< HashKey, rho::Hashtable<InnnerHashKey, InnnerHashValue> >& dest, rho::String& error)
{
    if (isNil(value))
        return false;

    if (rho_ruby_is_hash(value))
        return false;

    bool result = true;

    VALUE keys = rho_ruby_hash_keys(value);
    VALUE akey;

    HashKey _key;
    rho::Hashtable<InnnerHashKey, InnnerHashValue> _value;
    while ((akey = rho_ruby_each_key(keys))) {
        VALUE val = rho_ruby_hash_get(value, akey);
        if (valueTo(akey, _key) && rho_value_to_typed_hash(val, _value)) {
            dest[_key] = _value;
        } else {
            result = false;
        }
    }

    return  result;
}



}}
