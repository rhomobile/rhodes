
#import "IJavascriptVM.h"
#import "JavascriptVMSingletonBase.h"
#import "api_generator/iphone/CMethodResult.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface JavascriptVMSingleton : JavascriptVMSingletonBase<IJavascriptVMSingleton> {
}

@property (strong, nonatomic, retain) UIWebView* webView;

/* Execute javascript code synchronously and wait for result */
-(void) executeScript:(NSString*)script methodResult:(id<IMethodResult>)methodResult;

/* Execute javascript code without blocking. No result returned. */
-(void) executeScriptAsync:(NSString*)script;

@end