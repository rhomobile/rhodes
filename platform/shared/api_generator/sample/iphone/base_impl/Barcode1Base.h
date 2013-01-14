

#import "../api/IBarcode1.h"

@interface Barcode1Base : NSObject {
    NSMutableDictionary* mProperties;
}

- (id) init;

-(NSString*)getProperty:(NSString*)property_name;
-(void) setProperty:(NSString*)property_name value:(NSObject*)value;
-(NSDictionary*) getAllProperties;
-(NSDictionary*) getPropertiesByList:(NSArray*)listOfPropertyNames;

-(void) getProps:(id<IMethodResult>)methodResult;
-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult;
-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult;

-(void) dealloc;

@end