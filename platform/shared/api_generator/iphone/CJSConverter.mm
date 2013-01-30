
#import "CJSConverter.h"
#import "api_generator/js_helpers.h"
#import "CMethodResult.h"

@implementation CJSConverter

+ (NSString*) convertToJS:(NSObject*)objectiveC_value {
    
/*
    rho::String strRes = "{}";
    if ( m_ResType == eStringArray )
    {
        strRes = "[";
        for( int i = 0; i < (int)m_arStrRes.size(); i++ )
        {
            if ( i > 0 )
                strRes += ",";
            
            strRes += CJSONEntry::quoteValueW(m_arStrRes[i]);
        }
        
        strRes += "]";
    }else if ( m_ResType == eStringHash )
    {
        strRes = "{";
        
        for ( rho::Hashtable<rho::StringW, rho::StringW>::iterator it = m_hashStrRes.begin(); it != m_hashStrRes.end(); ++it)
        {
            if ( it != m_hashStrRes.begin() )
                strRes += ",";
            
            strRes += CJSONEntry::quoteValueW(it->first) + ":" + CJSONEntry::quoteValueW(it->second);
        }
        
        strRes += "}";
    }else if ( m_ResType == eString)
    {
        strRes = "{'_RhoValue':" + convertToStringA(m_strRes) + "}";
    }else if ( m_ResType == eArgError )
    {
        strRes = "{'_RhoArgError':" + CJSONEntry::quoteValueW(m_strError) + "}";
    }else if ( m_ResType == eError)
    {
        strRes = "{'_RhoRuntimeError':" + CJSONEntry::quoteValueW(m_strError) + "}";
    }
    
    return strRes;
*/
    
 return nil;
}

+ (NSObject*) convertFromJS:(void*)json_entry {
    rho::json::CJSONEntry* jsonEntry = (rho::json::CJSONEntry*)json_entry;
    
    
    //TODO:
    return nil;
}

+ (NSObject*) getObjectiveCNULL {
    return [CMethodResult getObjectiveCNULL];
}

@end
