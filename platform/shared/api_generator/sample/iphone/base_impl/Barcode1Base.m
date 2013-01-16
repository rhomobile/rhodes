
#import "Barcode1Base.h"

@implementation Barcode1Base


- (id) init {
    self = [super init];
    mProperties = [NSMutableDictionary dictionary];
    return self;
}

-(NSString*)getProperty:(NSString*)property_name {
    return [mProperties objectForKey:property_name];
}

-(void) setProperty:(NSString*)property_name value:(NSObject*)value {
    [mProperties setObject:value forKey:property_name];
}

-(NSDictionary*) getAllProperties {
    return [mProperties dictionaryWithValuesForKeys:[[mProperties keyEnumerator] allObjects]];
}

-(NSDictionary*) getPropertiesByList:(NSArray*)listOfPropertyNames {
    return [mProperties dictionaryWithValuesForKeys:listOfPropertyNames];
}


-(void) getProps:(id<IMethodResult>)methodResult {
    [methodResult setResult:[self getAllProperties]];
}

-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[self getProperty:param1]];
}

-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[self getPropertiesByList:param1]];
}



-(void) dealloc {
    [mProperties release];
    [super dealloc];
}

@end