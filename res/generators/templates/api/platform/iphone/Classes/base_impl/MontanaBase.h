

#import "../api/I<%= $cur_module.name %>.h"

@interface <%= $cur_module.name %>Base : NSObject {
    NSMutableDictionary* mProperties;
}

- (id) init;

-(NSString*)getProperty:(NSString*)property_name;
-(void) setProperty:(NSString*)property_name value:(NSObject*)value;
-(NSDictionary*) getAllProperties;
-(NSDictionary*) getPropertiesByList:(NSArray*)listOfPropertyNames;
-(void) setPropertiesByDictionary:(NSDictionary*)propertyMap;
-(void) clearAllProps;

-(void) dealloc;

@end