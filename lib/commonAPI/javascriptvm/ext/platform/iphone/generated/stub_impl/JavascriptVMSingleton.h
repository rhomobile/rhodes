
#import "IJavascriptVM.h"
#import "JavascriptVMSingletonBase.h"

@interface JavascriptVMSingleton : JavascriptVMSingletonBase<IJavascriptVMSingleton> {
}



-(void) executeScript:(NSString*)script methodResult:(id<IMethodResult>)methodResult;




@end