#include "../../../shared/generated/cpp/JavascriptVMBase.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

// WebViewContainer is used for wrapping UIWebView into object and calling JS functions from C++
@interface WebViewContainer : NSObject <UIApplicationDelegate>

@property (strong, nonatomic) UIWebView* webView;

- (NSString*)callJSFunction:(NSString*)function;

// WebViewContainer implementation
@implementation WebViewContainer

@synthesize webView = _webView;

- (id) init {
    self = [super init];
    if (self != nil) {
        [self performSelectorOnMainThread:@selector(perfromInit)
                               withObject:nil
                            waitUntilDone:YES];
    }
    return self;
}

- (void) perfromInit {
    _webView = [[UIWebView alloc] initWithFrame:CGRectZero];
}

- (NSString*)executeScript:(NSString*)function
{
    NSMutableArray *args = [[NSMutableArray arrayWithCapacity:3] autorelease];
    [args addObject:_webView];
    [args addObject:function];
    [args addObject:[[NSString alloc] autorelease]];

    [self performSelectorOnMainThread:@selector(executeScriptImpl:)
                           withObject:args
                        waitUntilDone:YES];

    return [args objectAtIndex:2];
}

- (void) executeScriptImpl:(NSMutableArray*)args
{
    UIWebView* view = [args objectAtIndex:0];
    NSString* js = [args objectAtIndex:1];
    [args replaceObjectAtIndex:2 withObject:[view stringByEvaluatingJavaScriptFromString:js]];
}

- (void) dealloc {
    [super dealloc];
}
@end

namespace rho {

using namespace apiGenerator;

class CJavascriptVMImpl: public CJavascriptVMBase
{
public:
    CJavascriptVMImpl(): CJavascriptVMBase()
    {
    }


};

class CJavascriptVMSingleton: public CJavascriptVMSingletonBase
{
public:
    CJavascriptVMSingleton() : self( NULL )
    {
        self = [[WebViewContainer alloc] init];
    }
protected:
    ~CJavascriptVMSingleton()
    {
        if (self)
        {
            [(id)self dealloc];
            self = NULL;
        }
    }

    virtual void executeScript( const rho::StringW& script, rho::apiGenerator::CMethodResult& oResult)
    {
        const wchar_t * c_str = script.c_str();
        NSString* script = [[NSString alloc] initWithBytes:charText length:wcslen(c_str)*sizeof(*c_str) encoding:NSUTF32LittleEndianStringEncoding];
        NSString* result = [[(id)self executeScript:script] autorelease];
        oResult.set([result UTF8String]);
    }

private:
    void * self;
};

class CJavascriptVMFactory: public CJavascriptVMFactoryBase
{
    ~CJavascriptVMFactory(){}
    virtual IJavascriptVMSingleton* createModuleSingleton();
};

extern "C" void Init_JavascriptVM_extension()
{
    CJavascriptVMFactory::setInstance( new CJavascriptVMFactory() );
    Init_JavascriptVM_API();
}

IJavascriptVMSingleton* CJavascriptVMFactory::createModuleSingleton()
{
    return new CJavascriptVMSingleton();
}

}