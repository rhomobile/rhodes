
#import "IWebView.h"
#import "WebViewFactoryBase.h"

static WebViewFactoryBase* ourWebViewFactory = nil;

@implementation WebViewFactorySingleton

+(id<IWebViewFactory>) getWebViewFactoryInstance {
    if (ourWebViewFactory == nil) {
        ourWebViewFactory = [[WebViewFactoryBase alloc] init];
    }
    return ourWebViewFactory;
}

@end