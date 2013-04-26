
#import "IGenCallbackTests.h"
#import "GenCallbackTestsSingletonBase.h"

@interface GenCallbackTestsSingleton : GenCallbackTestsSingletonBase<IGenCallbackTestsSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) testString:(NSString*)value methodResult:(id<IMethodResult>)methodResult;
-(void) testBool:(BOOL)value methodResult:(id<IMethodResult>)methodResult;
-(void) testInt:(int)value methodResult:(id<IMethodResult>)methodResult;
-(void) testFloat:(float)value methodResult:(id<IMethodResult>)methodResult;
-(void) testArray:(NSArray*)value methodResult:(id<IMethodResult>)methodResult;
-(void) testHash:(NSDictionary*)value methodResult:(id<IMethodResult>)methodResult;
-(void) testNull:(id<IMethodResult>)methodResult;




@end