#pragma once

#include "logging/RhoLog.h"
#include "json/JSONIterator.h"

extern "C" VALUE rjson_tokener_parse(const char *str, char** pszError );
using namespace rho::json;

namespace rho {
namespace apiGenerator {

using namespace rho::common;

template <typename RESULT> struct CRubyResultConvertor;
template <typename RESULT> struct CJSONResultConvertor;

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
    bool isHash() { return m_oResult.getType() == CMethodResult::eStringHash; }
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
            for(size_t i = 0; i < m_oResult.getHashArray().size(); ++i)
            {
                CHoldRubyValue valItem(rho_ruby_createHash());

                for(rho::Hashtable<rho::String, rho::String>::iterator it = m_oResult.getHashArray()[i].begin(); it != m_oResult.getHashArray()[i].end(); ++it)
                {
                    addStrToHash(valItem, it->first.c_str(), it->second.c_str());
                }

                rho_ruby_add_to_array(valArray, valItem);
            }
        }

        return valArray;
    }
    VALUE getHash()
    {
        //RAWLOGC_INFO("CRubyResultConvertor", "getHash()");

        CHoldRubyValue valHash(rho_ruby_createHash());

        for(rho::Hashtable<rho::String, rho::String>::iterator it = m_oResult.getStringHash().begin(); it != m_oResult.getStringHash().end(); ++it)
        {
            //RAWLOGC_INFO2("CRubyResultConvertor", "getHash(): %s->%s", it->first.c_str(), it->second.c_str());
            addHashToHash(valHash, it->first.c_str(), getObjectOrString(it->second));
        }

        for(rho::Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> >::iterator it = m_oResult.getStringHashL2().begin(); it != m_oResult.getStringHashL2().end(); ++it)
        {
            CHoldRubyValue valHashL2(rho_ruby_createHash());

            //RAWLOGC_INFO1("CRubyResultConvertor", "getHash(): %s->L2 HASH", it->first.c_str());

            for(rho::Hashtable<rho::String, rho::String>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                //RAWLOGC_INFO2("CRubyResultConvertor", "getHash(): L2: %s->%s", it2->first.c_str(), it2->second.c_str());
                addStrToHash(valHashL2, it2->first.c_str(), it2->second.c_str());
            }

            addHashToHash(valHash, it->first.c_str(), valHashL2 );
        }

        return valHash;
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
    VALUE getErrorMessage()
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

template<>
class CJSONResultConvertor<CMethodResult>
{
    CMethodResult& m_oResult;

    bool hasObjectClass()
    {
        return m_oResult.getObjectClassPath().length() > 0;
    }
    rho::String getObjectOrString(const rho::String& str)
    {
        rho::String res;
        if(hasObjectClass())
        {
            res = "{ \"__rhoID\": \"";
            res += str;
            res += "\", \"__rhoClass\": ";
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
    bool isHash() { return m_oResult.getType() == CMethodResult::eStringHash; }
    bool isError() { return m_oResult.isError(); }
    bool isNil() { return m_oResult.getType() == CMethodResult::eNone; }

    rho::String getBool() { return m_oResult.getBool() ? "true" : "false"; }
    rho::String getInt()
    {
        char buf[32];
        sprintf("%d", buf, static_cast<int>(m_oResult.getInt()));
        return buf;
    }

    rho::String getDouble()
    {
        char buf[32];
        sprintf("%f", buf, m_oResult.getDouble());
        return buf;
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
        rho::String resArray = "[";

        if(m_oResult.getType() == CMethodResult::eStringArray)
        {
            for(size_t i = 0; i < m_oResult.getStringArray().size(); ++i)
            {
                if(i > 0)
                    resArray += ",";
                resArray += getObjectOrString(m_oResult.getStringArray()[i]);
            }
        } else
        if(m_oResult.getType() == CMethodResult::eArrayHash)
        {
            for(size_t i = 0; i < m_oResult.getHashArray().size(); ++i)
            {
                if (i > 0)
                    resArray += ",";

                resArray += "{";
                int j = 0;
                for(rho::Hashtable<rho::String, rho::String>::iterator it = m_oResult.getHashArray()[i].begin(); it != m_oResult.getHashArray()[i].end(); ++it, ++j)
                {
                    if (j > 1)
                        resArray += ",";

                    resArray += CJSONEntry::quoteValue(it->first);
                    resArray += ":";
                    resArray += CJSONEntry::quoteValue(it->second);
                }

                resArray += "}";
            }
        }

        resArray += "]";

        return resArray;
    }
    rho::String getHash()
    {
        rho::String resHash = "{ ";
        unsigned i = 0;

        for(rho::Hashtable<rho::String, rho::String>::iterator it = m_oResult.getStringHash().begin(); it != m_oResult.getStringHash().end(); ++it, ++i)
        {
            if (i > 0)
                resHash += ",";

            resHash += CJSONEntry::quoteValue(it->first);
            resHash += ": ";
            resHash += getObjectOrString(it->second);
        }

        for(rho::Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> >::iterator it = m_oResult.getStringHashL2().begin(); it != m_oResult.getStringHashL2().end(); ++it, ++i)
        {
            if (i > 0)
                resHash += ",";

            resHash += CJSONEntry::quoteValue(it->first);
            resHash += ":{";
            for(rho::Hashtable<rho::String, rho::String>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                if (it2 != it->second.begin())
                    resHash += ",";

                resHash += CJSONEntry::quoteValue(it2->first);
                resHash += ":";
                resHash += CJSONEntry::quoteValue(it2->second);
            }
            resHash += "}";
        }

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
};

class CMethodResultConvertor
{
public:
    template <typename RESULT>
    VALUE toRuby(RESULT& result, bool forCallback)
    {
        CRubyResultConvertor<RESULT> convertor(result, forCallback);
        rho::common::CAutoPtr<CHoldRubyValue> pRes;
        if (convertor.isNil())
            pRes = new CHoldRubyValue(rho_ruby_get_NIL());
        else if (convertor.isHash())
            pRes = new CHoldRubyValue(convertor.getHash());
        else if (convertor.isArray())
            pRes = new CHoldRubyValue(convertor.getArray());
        else if (convertor.isBool())
            pRes = new CHoldRubyValue(convertor.getBool());
        else if (convertor.isInt())
            pRes = new CHoldRubyValue(convertor.getInt());
        else if (convertor.isDouble())
            pRes = new CHoldRubyValue(convertor.getDouble());
        else if (convertor.isString())
            pRes = new CHoldRubyValue(convertor.getString());
        else if (convertor.isJSON())
            pRes = new CHoldRubyValue(convertor.getJSON());
        else if (convertor.isError()) 
        {
            VALUE message = convertor.getErrorMessage();
            if(rho_ruby_is_NIL(message))
            {
                return rho_ruby_get_NIL();
            }
            pRes = new CHoldRubyValue(message);
        } else
        {
            LOG(ERROR) + "Unknown reulst type.";
            pRes = new CHoldRubyValue(rho_ruby_get_NIL());
        }

        if(forCallback && !rho_ruby_is_hash(pRes->getValue()) )
        {
            CHoldRubyValue resHash(rho_ruby_createHash());
            if(static_cast<CHoldRubyValue*>(pRes) != 0)
            {
                //RAWLOGC_INFO1("CMethodResultConvertor","Create '%s' in result hash", convertor.getResultParamName());
                addHashToHash(resHash, convertor.getResultParamName(), *pRes);
            }
            return resHash;
        } else
        {
            return pRes->getValue();
        }

    }

    template <typename RESULT>
    rho::String toJSON(RESULT& result)
    {
        CJSONResultConvertor<RESULT> convertor(result);
        rho::String strRes = "\"result\":";
        if (convertor.isNil())
            strRes += "null";
        else if (convertor.isArray())
        {
            strRes += convertor.getArray();
        }else if (convertor.isHash())
        {
            strRes += convertor.getHash();
        } else if (convertor.isString())
        {
            strRes += convertor.getString();
        }else if (convertor.isJSON())
        {
            strRes += convertor.getJSON();
        } else if (convertor.isBool())
        {
            strRes += convertor.getBool();
        } else if (convertor.isInt())
        {
            strRes += convertor.getInt();
        } else  if (convertor.isDouble())
        {
            strRes += convertor.getDouble();
        } else  if (convertor.isError())
        {
            strRes = "\"error\": ";
            strRes += convertor.getError();
        }else
           strRes += "null";

        return strRes;
    }
};

}
}
