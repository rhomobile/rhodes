
#import "../api/IJavascriptVM.h"
#import "JavascriptVMFactoryBase.h"

static JavascriptVMFactoryBase* ourJavascriptVMFactory = nil;

@implementation JavascriptVMFactorySingleton

+(id<IJavascriptVMFactory>) getJavascriptVMFactoryInstance {
    if (ourJavascriptVMFactory == nil) {
        ourJavascriptVMFactory = [[JavascriptVMFactoryBase alloc] init];
    }
    return ourJavascriptVMFactory;
}

@end