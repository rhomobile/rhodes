
#import "MegamoduleBase.h"

@implementation MegamoduleBase


- (id) init {
    self = [super init];

    mProperties = [NSMutableDictionary dictionary];

    return self;
}



-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[mProperties objectForKey:propertyName]];
}

-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue {
   [mProperties setObject:propertyValue forKey:propertyName];
}

-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult {
    [methodResult setResult:[mProperties dictionaryWithValuesForKeys:arrayofNames]];
}

-(void) getAllProperties:(id<IMethodResult>)methodResult {
    [methodResult setResult:[mProperties dictionaryWithValuesForKeys:[[mProperties keyEnumerator] allObjects]]];
}


-(void) setProperties:(NSDictionary*)propertyMap {
   [mProperties addEntriesFromDictionary:propertyMap];
}

-(void) clearAllProperties {
   [mProperties removeAllObjects];
}







-(void) getStringProperty:(id<IMethodResult>)methodResult; {

    [self getProperty:@"StringProperty" methodResult:methodResult];

}




-(void) dealloc {

    [mProperties release];

    [super dealloc];
}

@end