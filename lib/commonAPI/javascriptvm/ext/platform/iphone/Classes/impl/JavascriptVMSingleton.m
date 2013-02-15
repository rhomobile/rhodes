
#import "JavascriptVMSingleton.h"

@implementation JavascriptVMSingleton

@synthesize webView = _webView;

- (id) init
{
    self = [super init];
    if (self != nil) {
        [self performSelectorOnMainThread:@selector(perfromInit)
                               withObject:nil
                            waitUntilDone:YES];
    }
    return self;
}

- (void) perfromInit
{
    _webView = [[UIWebView alloc] initWithFrame:CGRectZero];
}

-(void) executeScript:(NSString*)script methodResult:(CMethodResult*)methodResult
{
    NSMutableArray *args = [[NSMutableArray arrayWithCapacity:3] autorelease];
    [args addObject:_webView];
    [args addObject:script];
    [args addObject:[[NSString alloc] autorelease]];
    
    [self performSelectorOnMainThread:@selector(executeScriptImpl:)
                           withObject:args
                        waitUntilDone:YES];
    
    [methodResult setResult:[args objectAtIndex:2]];
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