#pragma once

#include "logging/RhoLog.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoStd.h"
#include "ruby/ext/rho/rhoruby.h"

namespace rho {
namespace apiGenerator {

using namespace rho::common;

template <typename RESULT> struct CRubyResultConvertor;
template <typename RESULT> struct CJSONResultConvertor;

class CMethodResultConvertor
{
public:
    template <typename RESULT>
    VALUE toRuby(RESULT& result, bool forCallback = false)
    {
        CRubyResultConvertor<RESULT> convertor(result, forCallback);
        CAutoPtr<CHoldRubyValue> pRes;
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
            VALUE message = convertor.getError();
            if(rho_ruby_is_NIL(message))
            {
                return rho_ruby_get_NIL();
            }
            pRes = new CHoldRubyValue(message);
        } else
        {
            LOG(ERROR) + "Unknown result type.";
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
    rho::String toJSON(RESULT& result/*, bool forCallback = false*/)
    {
        String strRes;
        CJSONResultConvertor<RESULT> convertor(result);
        if (convertor.isError())
        {
            strRes = "\"error\": ";
            strRes += convertor.getError();
        } else
        {
            strRes = "\"result\":";
            if (convertor.isNil())
            {
                strRes += "null";
            }else if (convertor.isArray())
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
            } else
            {
               strRes += "null";
            }

            //Double check for error during conversion
            if (convertor.isError())
            {
                strRes = "\"error\": " + convertor.getError();
            }
        }

        //if(forCallback)
        //{
        //    strRes = String("{ \"jsonrpc\": \"2.0\", ") + strRes + "}";
        //}

        return strRes;
    }
};

template <typename RESULT>
class CRubyCallbackResult : public rho::ICallbackObject
{
    RESULT m_oResult;
public:
    CRubyCallbackResult(const RESULT& oResult) : m_oResult(oResult){}
    virtual ~CRubyCallbackResult() { }

    virtual unsigned long getObjectValue()
    {
        return CMethodResultConvertor().toRuby(m_oResult, true);
    }

};

}
}
