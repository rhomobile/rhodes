/*------------------------------------------------------------------------
 * (The MIT License)
 *
 * Copyright (c) 2008-2011 Rhomobile, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://rhomobile.com
 *------------------------------------------------------------------------*/

#import "CIOSJavaScriptVM.h"
#import "logging/RhoLog.h"
#import "WebViewContainer.h"

@implementation WebViewContainer

// CIOSJavaScriptVM implementation
CIOSJavaScriptVM::CIOSJavaScriptVM( void )
: self( NULL )
{
    self = [[WebViewContainer alloc] init];
}

CIOSJavaScriptVM::~CIOSJavaScriptVM( void )
{
    [(id)self dealloc];
}

rho::String CIOSJavaScriptVM::callJSFunction(const rho::String& function)
{
    NSString* s = [[(id)self callJSFunction:[NSString stringWithUTF8String:function.c_str()]] autorelease];
    return rho::String([s UTF8String]);
}

// WebViewContainer implementation
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

- (NSString*)callJSFunction:(NSString*)function
{
    NSMutableArray *args = [[NSMutableArray arrayWithCapacity:3] autorelease];
    [args addObject:_webView];
    [args addObject:function];
    [args addObject:[[NSString alloc] autorelease]];
    
    [self performSelectorOnMainThread:@selector(callJSFunctionImpl:)
                           withObject:args
                        waitUntilDone:YES];
    
    return [args objectAtIndex:2];
}

- (void) callJSFunctionImpl:(NSMutableArray*)args
{
    UIWebView* view = [args objectAtIndex:0];
    NSString* js = [args objectAtIndex:1];
    [args replaceObjectAtIndex:2 withObject:[view stringByEvaluatingJavaScriptFromString:js]];
}

- (void) dealloc {
    [super dealloc];
}
@end
