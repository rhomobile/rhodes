#include "StringficationHelper.h"
#include "json/JSONIterator.h"

namespace rho
{
    namespace apiGenerator
    {
        using namespace rho::json;
        using namespace rho::common;
        
        void StringifyVector::push_back(const rho::String& value, bool escape)
        {
            m_bufferLen += 1;
            if (escape)
            {
                m_vector.push_back(CJSONEntry::quoteValue(value));
                m_bufferLen += m_vector.back().length();
            } else {
                m_vector.push_back(value);
                m_bufferLen += value.length();
            }
        }
        
        void StringifyVector::push_back(const IStringSerializable& value)
        {
            rho::String buffer;
            value.toString(buffer);
            push_back(buffer, false);
        }
        
        void StringifyVector::toString(rho::String &buffer) const
        {
            buffer.clear();
            buffer.reserve(m_bufferLen + 2);
            buffer += "[";
            for(rho::Vector<rho::String>::const_iterator it = m_vector.begin(); it != m_vector.end(); it++)
            {
                if(it != m_vector.begin())
                {
                    buffer += ",";
                }
                buffer += *it;
            }
            buffer += "]";
        }
        
        void StringifyHash::set(const rho::String& key, const rho::String& value, bool escape)
        {
            m_bufferLen += 3;
            rho::String bk(CJSONEntry::quoteValue(key));
            if (escape)
            {
                rho::String bv(CJSONEntry::quoteValue(value));
                m_hash[bk] = bv;
                m_bufferLen += bk.length() + bv.length();
            } else {
                m_hash[bk] = value;
                m_bufferLen += bk.length() + value.length();
            }
        }
        
        void StringifyHash::set(const rho::String& key, const IStringSerializable& value)
        {
            rho::String buffer;
            value.toString(buffer);
            set(key, buffer, false);
        }

        void StringifyHash::toString(rho::String &buffer) const
        {
            buffer.clear();
            buffer.reserve(m_bufferLen + 2);
            buffer += "{";
            for(rho::Hashtable<rho::String, rho::String>::const_iterator it = m_hash.begin(); it != m_hash.end(); it++)
            {
                if(it != m_hash.begin())
                {
                    buffer += ",";
                }
                buffer += it->first;
                buffer += ":";
                buffer += it->second;
            }
            buffer += "}";
        }

    }
}
