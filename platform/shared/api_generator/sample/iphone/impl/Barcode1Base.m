
#import "Barcode1Base.h"

@implementation Barcode1Base

@synthesize ID;

- (id) init:(NSString*)_ID {
    self = [super init];
    self.ID = _ID;
    mProperties = [NSMutableDictionary dictionary];
    [self setProperty:@"ID" value:self.ID];
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

-(NSDictionary*) getListOfProperties:(NSArray*)listOfPropertyNames {
    return [mProperties dictionaryWithValuesForKeys:listOfPropertyNames];
}

-(void) dealloc {
    [mProperties release];
    [super dealloc];
}

@end