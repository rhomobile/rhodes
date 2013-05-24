
#import "MegamoduleBase.h"



static NSDictionary* ourPropertyAliases= nil;



@implementation MegamoduleBase


- (id) init {
    self = [super init];

    mProperties = [NSMutableDictionary dictionary];
    
    [self setProperty:@"StringProperty" propertyValue:@""];
    [self setProperty:@"IntegerProperty" propertyValue:@"12345"];

    return self;
}



+(NSString*) applyAliasesToPropertyName:(NSString*)prop_name {

    if (ourPropertyAliases == nil) {
        ourPropertyAliases = [NSDictionary dictionaryWithObjectsAndKeys:
                              @"IntegerProperty", @"integer_property",
                              @"StringProperty", @"string_property",
                              nil];
    }
    NSObject* resNameObj = [ourPropertyAliases objectForKey:prop_name];
    if (resNameObj != nil) {
        return (NSString*)resNameObj;
    }
    return prop_name;

}

+(NSDictionary*) applyAliasesToDictionary:(NSDictionary*)dict {

    NSArray* keys = [dict allKeys];
    int i;
    NSMutableDictionary* resDict = [NSMutableDictionary dictionaryWithCapacity:[keys count]];
    for (i = 0; i < [keys count]; i++) {
        NSString* key = (NSString*)[keys objectAtIndex:i];
        NSObject* value = [dict objectForKey:key];
        [resDict setObject:value forKey:[MegamoduleBase applyAliasesToPropertyName:key]];
    }
    return resDict;

}




-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[mProperties objectForKey:[MegamoduleBase applyAliasesToPropertyName:propertyName]]];
}

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue {
   [mProperties setObject:propertyValue forKey:[MegamoduleBase applyAliasesToPropertyName:propertyName]];
}

-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult {
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:[arrayofNames count]];
    NSArray* keys = arrayofNames;
    CMethodResult_SimpleHolder* resultHolder = [CMethodResult_SimpleHolder makeEmptyHolder];
    int i;
    for (i = 0; i < [keys count]; i++) {
        NSString* key = (NSString*)[keys objectAtIndex:i];
        [resultHolder setResult:nil];
        [self getProperty:key methodResult:resultHolder];
        if ([resultHolder getResult] != nil) {
            NSString* value = (NSString*)[resultHolder getResult];
            [dict setObject:value forKey:key];
        }
    }
    [methodResult setResult:dict];
}

-(void) getAllProperties:(id<IMethodResult>)methodResult {
    [self getProperties:[mProperties allKeys] methodResult:methodResult];
}


-(void) setProperties:(NSDictionary*)propertyMap {
    NSArray* keys = [propertyMap allKeys];
    int i;
    for (i = 0; i < [keys count]; i++) {
        NSString* key = (NSString*)[keys objectAtIndex:i];
        NSString* value = (NSString*)[propertyMap objectForKey:key];
        [self setProperty:key propertyValue:value];
    }
}

-(void) clearAllProperties {
   [mProperties removeAllObjects];
}










-(void) getStringProperty:(id<IMethodResult>)methodResult {

    CMethodResult_SimpleHolder* resultHolder = [CMethodResult_SimpleHolder makeEmptyHolder];
    [self getProperty:@"StringProperty" methodResult:resultHolder];
    if ([resultHolder getResult] != nil) {
        NSString* strResult = (NSString*)[resultHolder getResult];
        NSString* typedResult = strResult;
        [methodResult setResult:typedResult];
    }
    else {
        [methodResult setResult:nil];
    }

}

-(void) getIntegerProperty:(id<IMethodResult>)methodResult {

    CMethodResult_SimpleHolder* resultHolder = [CMethodResult_SimpleHolder makeEmptyHolder];
    [self getProperty:@"IntegerProperty" methodResult:resultHolder];
    if ([resultHolder getResult] != nil) {
        NSString* strResult = (NSString*)[resultHolder getResult];
        NSNumber* typedResult = [NSNumber numberWithInt:[strResult intValue]];
        [methodResult setResult:typedResult];
    }
    else {
        [methodResult setResult:nil];
    }

}

-(void) setIntegerProperty:(int)IntegerProperty {

    NSString* strValue = [NSString stringWithFormat:@"%@", [NSNumber numberWithInt:IntegerProperty]];
    [self setProperty:@"IntegerProperty" propertyValue:strValue];

}




-(void) dealloc {

    [mProperties release];

    [super dealloc];
}

@end