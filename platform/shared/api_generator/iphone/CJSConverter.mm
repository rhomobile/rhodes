
#import "CJSConverter.h"
#import "api_generator/js_helpers.h"
#import "CMethodResult.h"

@implementation CJSConverter




+ (NSString*) convertToJS:(NSObject*)objectiveC_value level:(int)level {

    NSString* strRes = @"";
    if (level == 0)
        strRes = [NSString stringWithUTF8String:"{"];
    
    if ([objectiveC_value isKindOfClass:[NSString class]]) {
        // string
        NSString* objString = (NSString*)objectiveC_value;
        
        if (level == 0)
            strRes = [strRes stringByAppendingString:@"'_RhoValue':'"];
        strRes = [strRes stringByAppendingString:objString];
        strRes = [strRes stringByAppendingString:@"'"];
    }
    else if ([objectiveC_value isKindOfClass:[NSNumber class]]) {
        // int, bool or float
        NSNumber* objNumber = (NSNumber*)objectiveC_value;
        NSString* str = @"";
        if ([CMethodResult isBoolInsideNumber:objNumber]) {
            if([objNumber boolValue]) {
                str = @"true";
            }
            else {
                str = @"false";
            }
        }
        else if ([CMethodResult isFloatInsideNumber:objNumber]) {
            str = [NSString stringWithFormat:@"%@", objNumber];
        }
        else {
            str = [NSString stringWithFormat:@"%@", objNumber];
        }
        if (level == 0)
            strRes = [strRes stringByAppendingString:@"'_RhoValue':"];
        strRes = [strRes stringByAppendingString:str];
    }
    else if ([objectiveC_value isKindOfClass:[NSArray class]]) {
        // array
        strRes = [strRes stringByAppendingString:@"["];
        NSArray* objArray = (NSArray*)objectiveC_value;
        int count = [objArray count];
        int i;
        for (i = 0; i < count; i++) {
            if (i > 0) {
                strRes = [strRes stringByAppendingString:@","];
            }
            NSObject* obj = [objArray objectAtIndex:i];
            strRes = [strRes stringByAppendingString:[CJSConverter convertToJS:obj level:(level+1)]];
        }
        strRes = [strRes stringByAppendingString:@"]"];
    }
    else if ([objectiveC_value isKindOfClass:[CRhoAPIClassInstance class]]) {
        // rubyModule
        CRhoAPIClassInstance* rubyModule = (CRhoAPIClassInstance*)objectiveC_value;
        NSString* rubyPath = [rubyModule getClassName];
        if (level == 0)
            strRes = [strRes stringByAppendingString:@"'_RhoValue':'"];
        strRes = [strRes stringByAppendingString:rubyPath];
        strRes = [strRes stringByAppendingString:@"'"];
    }
    else if ([objectiveC_value isKindOfClass:[NSDictionary class]]) {
        // dictionary
        strRes = [strRes stringByAppendingString:@"["];
        NSDictionary* objDictionary = (NSDictionary*)objectiveC_value;
        NSEnumerator* enumerator = [objDictionary keyEnumerator];
        NSObject* obj = nil;
        BOOL is_first = YES;
        while ((obj = [enumerator nextObject]) != nil) {
            NSString* objKey = (NSString*)obj;
            NSObject* objValue = [objDictionary objectForKey:objKey];
            if (!is_first) {
               strRes = [strRes stringByAppendingString:@","]; 
            }
            strRes = [strRes stringByAppendingString:@"'"];
            strRes = [strRes stringByAppendingString:objKey];
            strRes = [strRes stringByAppendingString:@"':"];
            
            strRes = [strRes stringByAppendingString:[CJSConverter convertToJS:objValue level:(level+1)]];
            
            is_first = NO;
        }
    }
    if (level == 0)
        strRes = [strRes stringByAppendingString:@"}"];
    return strRes;
    
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

+ (NSObject*) convertFromJSentry:(rho::json::CJSONEntry*)json_entry rho_api_param:(RHO_API_PARAM*)rho_api_param {
    rho::json::CJSONEntry* jsonEntry = (rho::json::CJSONEntry*)json_entry;
    

    if (jsonEntry->isString()) {
        return [NSString stringWithUTF8String:jsonEntry->getString()];
    }
    if (jsonEntry->isInteger()) {
        return [NSNumber numberWithInt:jsonEntry->getInt()];
    }
    if (jsonEntry->isFloat()) {
        return [NSNumber numberWithDouble:jsonEntry->getDouble()];
    }
    if (jsonEntry->isBoolean()) {
        return [NSNumber numberWithBool:(jsonEntry->getBoolean() == true)];
    }
    if (jsonEntry->isArray()) {
        rho::json::CJSONArray js_array(*jsonEntry);
        int i;
        int count = js_array.getSize();
        NSMutableArray* ns_array = [NSMutableArray arrayWithCapacity:count];
        for (i =0 ; i < count; i++) {
            rho::json::CJSONEntry js_entry = js_array[i];
            [ns_array addObject:[CJSConverter convertFromJSentry:&js_entry rho_api_param:NULL]];
        }
        return ns_array;
    }
    if (jsonEntry->isObject()) {
        rho::json::CJSONStructIterator js_iterator(*jsonEntry);
        
        NSMutableDictionary* ns_hash = [NSMutableDictionary dictionaryWithCapacity:10];
        
        while (!js_iterator.isEnd()) {
            rho::String key = js_iterator.getCurKey();
            rho::json::CJSONEntry js_entry = js_iterator.getCurValue();
            
            [ns_hash setObject:[CJSConverter convertFromJSentry:&js_entry rho_api_param:NULL] forKey:[NSString stringWithUTF8String:(key.c_str())]];
            
            js_iterator.next();
        }
        
        return ns_hash;
    }
    return nil;
}


+ (NSObject*) convertFromJS:(void*)json_entry rho_api_param:(RHO_API_PARAM*)rho_api_param {
    rho::json::CJSONEntry* jsonEntry = (rho::json::CJSONEntry*)json_entry;
    
    
    //TODO:
    @try {
        return [CJSConverter convertFromJSentry:jsonEntry rho_api_param:rho_api_param];
    }
    @catch (NSException *exception) {
        
    }
    @finally {
        
    }
    
    
    
    
    return nil;
}

+ (NSObject*) getObjectiveCNULL {
    return [CMethodResult getObjectiveCNULL];
}

@end
