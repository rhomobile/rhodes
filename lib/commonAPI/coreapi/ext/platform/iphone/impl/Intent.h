
#import "IIntent.h"
#import "IntentBase.h"

@interface Intent : IntentBase<IIntent> {
}

-(void) cleanStartParameters:(id<IMethodResult>)methodResult;


@end