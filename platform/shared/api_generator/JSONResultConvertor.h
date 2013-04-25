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
        return convertToStringA(m_oResult.getInt());
    }

    rho::String getDouble()
    {
        return convertToStringA(m_oResult.getDouble());
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
                    if (j > 0)
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
};


}}

