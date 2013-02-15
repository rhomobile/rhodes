
#import "../api/IJavascriptVM.h"
#import "../base_impl/JavascriptVMSingletonBase.h"

@interface JavascriptVMSingleton : JavascriptVMSingletonBase<IJavascriptVMSingleton> {
}



-(void) executeScript:(NSString*)script methodResult:(id<IMethodResult>)methodResult;




@end