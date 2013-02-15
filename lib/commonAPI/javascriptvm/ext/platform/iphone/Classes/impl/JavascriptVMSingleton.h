
#import "../api/IJavascriptVM.h"
#import "../base_impl/JavascriptVMSingletonBase.h"
#import "api_generator/iphone/CMethodResult.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface JavascriptVMSingleton : JavascriptVMSingletonBase<IJavascriptVMSingleton> {
}

@property (strong, nonatomic) UIWebView* webView;

-(void) executeScript:(NSString*)script methodResult:(CMethodResult*)methodResult;

@end