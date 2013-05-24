

#import "IMegamodule.h"

@interface MegamoduleBase : NSObject {

    NSMutableDictionary* mProperties;

}

- (id) init;



+(NSString*) applyAliasesToPropertyName:(NSString*)prop_name;
+(NSDictionary*) applyAliasesToDictionary:(NSDictionary*)dict;
-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult;
-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult;
-(void) getAllProperties:(id<IMethodResult>)methodResult;
-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue;
-(void) setProperties:(NSDictionary*)propertyMap;
-(void) clearAllProperties;


-(void) getStringProperty:(id<IMethodResult>)methodResult;
-(void) getIntegerProperty:(id<IMethodResult>)methodResult;
-(void) setIntegerProperty:(int)IntegerProperty;


-(void) dealloc;

@end