
#import "IIntent.h"
#import "IntentSingletonBase.h"
#import "IAppMessageReceiver.h"


@interface IntentSingleton : IntentSingletonBase<IIntentSingleton,IAppMessageReceiver> {
    id<IMethodResult> mIntentListener;
}

- (id) init;

-(void) send:(NSDictionary*)params methodResult:(id<IMethodResult>)methodResult;
-(void) startListening:(id<IMethodResult>)methodResult;
-(void) stopListening:(id<IMethodResult>)methodResult;

-(void) onAppMessageReceived:(NSString *)message app:(NSString*)app;





@end