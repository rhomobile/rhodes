

#import "../api/IBarcode1.h"

@interface Barcode1Base : NSObject {
    NSMutableDictionary* mProperties;
}

@property (assign)NSString* ID;

- (id) init:(NSString*)_ID;


-(NSString*)getProperty:(NSString*)property_name;
-(void) setProperty:(NSString*)property_name value:(NSObject*)value;
-(NSDictionary*) getAllProperties;
-(NSDictionary*) getListOfProperties:(NSArray*)listOfPropertyNames;

-(void) dealloc;

@end