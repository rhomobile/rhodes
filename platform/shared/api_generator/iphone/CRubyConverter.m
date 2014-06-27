
#import "CRubyConverter.h"
#import "CMethodResult.h"
#import "ruby/ext/rho/rhoruby.h"
#import "ruby/include/ruby.h"



@implementation CRubyConverter

+ (VALUE) convertToRuby:(NSObject*)objectiveC_value {
    VALUE v = rho_ruby_get_NIL();
    
    if ([objectiveC_value isKindOfClass:[NSString class]]) {
        // string
        NSString* objString = (NSString*)objectiveC_value;
        v = rho_ruby_create_string([objString UTF8String]);
    }
    else if ([objectiveC_value isKindOfClass:[NSNumber class]]) {
        // int, bool or float
        NSNumber* objNumber = (NSNumber*)objectiveC_value;
        if ([CMethodResult isBoolInsideNumber:objNumber]) {
            v = rho_ruby_create_boolean([objNumber boolValue]);
        }
        else if ([CMethodResult isFloatInsideNumber:objNumber]) {
            v = rho_ruby_create_double([objNumber doubleValue]);
        }
        else {
            v = rho_ruby_create_integer([objNumber intValue]);
        }
    }
    else if ([objectiveC_value isKindOfClass:[NSArray class]]) {
        // array
        NSArray* objArray = (NSArray*)objectiveC_value;
        int count = [objArray count];
        int i;
        v = rho_ruby_create_array();
        for (i = 0; i < count; i++) {
            NSObject* obj = [objArray objectAtIndex:i];
            VALUE objValue = [CRubyConverter convertToRuby:obj];
            rho_ruby_add_to_array(v, objValue);
        }
    }
    else if ([objectiveC_value isKindOfClass:[CRhoAPIClassInstance class]]) {
        // rubyModule
        CRhoAPIClassInstance* rubyModule = (CRhoAPIClassInstance*)objectiveC_value;
        NSString* rubyPath = [[rubyModule getClassName] stringByReplacingOccurrencesOfString:@"." withString:@"::"];
        VALUE klass = rb_path_to_class(rho_ruby_create_string([rubyPath UTF8String]));
        if (!rho_ruby_is_NIL(klass)) {
            v = rho_ruby_create_object_with_id(klass, [[rubyModule getInstanceID] UTF8String]);
        }
        else {
            rb_raise(rho_ruby_get_NIL(), "can not found Ruby Class (%s)", [rubyPath UTF8String]);
        }
    }
    else if ([objectiveC_value isKindOfClass:[NSDictionary class]]) {
        // dictionary
        NSDictionary* objDictionary = (NSDictionary*)objectiveC_value;
        v = rho_ruby_createHash();
        NSEnumerator* enumerator = [objDictionary keyEnumerator];
        NSObject* obj = nil;
        while ((obj = [enumerator nextObject]) != nil) {
            NSObject* objKey = obj;
            NSObject* objValue = [objDictionary objectForKey:objKey];
            VALUE vKey = [CRubyConverter convertToRuby:objKey];
            VALUE vItem = [CRubyConverter convertToRuby:objValue];
            rho_ruby_add_to_hash(v, vKey, vItem);
        }
    }
    else if ([objectiveC_value isKindOfClass:[CMethodResultError class]]) {
        CMethodResultError* errorObject = (CMethodResultError*)objectiveC_value;
        rb_raise(rho_ruby_get_NIL(), "error with code [%d] description [%s]", [errorObject getErrorCode], [[errorObject getErrorDescription] UTF8String]);
        
    }
    // ToDo: Support CMethodResultError
    
    return v;
}


+ (NSObject*) convertFromRuby:(VALUE)ruby_value {
    int i, size;
    switch(rb_type(ruby_value)) {
        case T_FLOAT:
        {
            return [NSNumber numberWithDouble:RFLOAT_VALUE(ruby_value)];
        }
            break;
        case T_FIXNUM:
        case T_BIGNUM:
        {
            return [NSNumber numberWithInt:(int)(NUM2LL(ruby_value))];
        }
            break;
        case T_TRUE:
        {
            return [NSNumber numberWithBool:YES];
        }
            break;
        case T_FALSE:
        {
            return [NSNumber numberWithBool:NO];
        }
            break;
        case T_ARRAY:
        {
            size = RARRAY_LEN(ruby_value);
            NSMutableArray* ar = [NSMutableArray arrayWithCapacity:size];
            for (i = 0; i < size; ++i) {
                VALUE item = rb_ary_entry(ruby_value, i);
                [ar addObject:[CRubyConverter convertFromRuby:item]];
            }
            return ar;
        }
            break;
        case T_HASH:
        {
            VALUE keys = rb_funcall(ruby_value, rb_intern("keys"), 0, NULL);
            size = RARRAY_LEN(keys);
            NSMutableDictionary* dic = [NSMutableDictionary dictionaryWithCapacity:size];
            for (i = 0; i < size; ++i) {
                VALUE key = rb_ary_entry(keys, i);
                VALUE value = rb_hash_aref(ruby_value, key);
                [dic setObject:[CRubyConverter convertFromRuby:value] forKey:[CRubyConverter convertFromRuby:key]];
            }
            return dic;
        }
            break;
        default:
        {
            VALUE value = ruby_value;
            if (rho_ruby_is_NIL(value)) {
                return [NSNull null];
            }
            if (rb_type(ruby_value) != T_STRING)
                value = rb_funcall(value, rb_intern("to_s"), 0, NULL);
            return [NSString stringWithUTF8String:RSTRING_PTR(value)];
        }
    }
    return nil;
}


@end
