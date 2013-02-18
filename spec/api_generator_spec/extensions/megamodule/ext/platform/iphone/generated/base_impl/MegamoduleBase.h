

#import "IMegamodule.h"

@interface MegamoduleBase : NSObject {

    NSMutableDictionary* mProperties;

}

- (id) init;


-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult;
-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult;
-(void) getAllProperties:(id<IMethodResult>)methodResult;
-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue;
-(void) setProperties:(NSDictionary*)propertyMap;
-(void) clearAllProperties;


-(void) getStringProperty:(id<IMethodResult>)methodResult;


-(void) dealloc;

@end