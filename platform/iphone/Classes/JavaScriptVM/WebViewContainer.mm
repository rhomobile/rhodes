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

#import "WebViewContainer.h"
#include "WebViewCInterface.h"
#include "logging/RhoLog.h"

@implementation WebViewContainer

WebViewCInterface::WebViewCInterface( void )
: self( NULL )
{   }

WebViewCInterface::~WebViewCInterface( void )
{
    [(id)self dealloc];
}

bool WebViewCInterface::init( void )
{
    self = [[WebViewContainer alloc] init];
    
    return (self);
}

VALUE WebViewCInterface::performAction( const char* actionName, const char* params) {
    assert(self);
    
    NSString* s = [(id)self executeJS:[NSString stringWithUTF8String:actionName] parameter:[NSString stringWithUTF8String:params]];
    
    return rho_ruby_create_string([s UTF8String]);
}

@synthesize javascriptBridge = _bridge;

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
    UIWebView* webView = [[UIWebView alloc] initWithFrame:CGRectZero];
    
    [WebViewJavascriptBridge enableLogging];
    
    _bridge = [WebViewJavascriptBridge bridgeForWebView:webView handler:^(id data, WVJBResponseCallback responseCallback) {
        RAWLOG_INFO("GOT MESSAGE");
        NSLog(@"ObjC received message from JS: %@", data);
        responseCallback(@"Response for message from ObjC");
    }];
    
    [_bridge registerHandler:@"testObjcCallback" handler:^(id data, WVJBResponseCallback responseCallback) {
        RAWLOG_INFO("GOT testObjcCallback");
        NSLog(@"testObjcCallback called: %@", data);
        responseCallback(@"Response from testObjcCallback");
    }];
    
    [_bridge send:@"A string sent from ObjC before Webview has loaded." responseCallback:^(id responseData) {
        RAWLOG_INFO("GOT string Webview");
        NSLog(@"objc got response! %@", responseData);
    }];
    
    [_bridge callHandler:@"testJavascriptHandler" data:[NSDictionary dictionaryWithObject:@"before ready" forKey:@"foo"]];
    
    [self loadExamplePage:webView];
    
    [_bridge send:@"A string sent from ObjC after Webview has loaded."];
}

- (void)loadExamplePage:(UIWebView*)webView {
    NSString* htmlPath = [[NSBundle mainBundle] pathForResource:@"ExampleApp" ofType:@"html"];
    NSString* appHtml = [NSString stringWithContentsOfFile:htmlPath encoding:NSUTF8StringEncoding error:nil];
    [webView loadHTMLString:appHtml baseURL:nil];
}

- (void) dealloc {
    [super dealloc];
}

- (NSString*)executeJS:(NSString*)function parameter:(NSString*)parameter
{
    NSString* s =@"<#string#>";
    [_bridge callHandler:function ];
	return s;
}

- (void)sendMessage:(id)sender {
    [_bridge send:@"A string sent from ObjC to JS" responseCallback:^(id response) {
        NSLog(@"sendMessage got response: %@", response);
    }];
}

- (void)callHandler:(id)sender {
    NSDictionary* data = [NSDictionary dictionaryWithObject:@"Hi there, JS!" forKey:@"greetingFromObjC"];
    [_bridge callHandler:@"testJavascriptHandler" data:data responseCallback:^(id response) {
        NSLog(@"testJavascriptHandler responded: %@", response);
    }];
}

@end
