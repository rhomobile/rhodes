
#import "IMegamodule.h"
#import "MegamoduleBase.h"

@interface Megamodule : MegamoduleBase<IMegamodule> {
    int intProp;
}

-(id)init;

-(void) getIntegerProperty:(id<IMethodResult>)methodResult;
-(void) setIntegerProperty:(int)value;
-(void) typesTest:(NSString*)paramStr paramBool:(BOOL)paramBool paramInt:(int)paramInt paramFloat:(float)paramFloat paramArray:(NSArray*)paramArray paramHash:(NSDictionary*)paramHash methodResult:(id<IMethodResult>)methodResult;
-(void) produceArray:(id<IMethodResult>)methodResult;
-(void) produceHash:(id<IMethodResult>)methodResult;
-(void) produceComplicatedResult:(id<IMethodResult>)methodResult;
-(void) showAlertFromUIThread:(NSString*)message;
-(void) setPeriodicallyCallback:(int)periodInMilliseconds methodResult:(id<IMethodResult>)methodResult;
-(void) isPeriodicallyCallbackSetted:(id<IMethodResult>)methodResult;
-(void) stopPeriodicallyCallback;
-(void) complicatedTypesTest1:(NSArray*)paramArray methodResult:(id<IMethodResult>)methodResult;



@end
