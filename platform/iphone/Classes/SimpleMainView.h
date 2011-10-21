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

#import <UIKit/UIKit.h>
#import "RhoMainView.h"
#import "RhoViewController.h"
#include "RhoNativeViewManagerOC.h"



@interface SimpleMainView : RhoViewController <RhoMainView, UIWebViewDelegate> {

    UIWebView *webView;
    UIToolbar *toolbar;
    UINavigationBar *navbar;
    CGRect rootFrame;
	id<NativeViewOC,NSObject> nativeView;
	NSString* nativeViewType;
	UIView* nativeViewView;
	BOOL urlBasedNativeView;
	RhoViewController* mTabBarCallback;
	NSString* url_after_set_background;
	BOOL is_url_after_set_background_redirect;
	BOOL isBackgroundSetted;
}

@property (nonatomic,retain) UIWebView *webView;
@property (nonatomic,retain) UIToolbar *toolbar;
@property (nonatomic,retain) UINavigationBar *navbar;
@property (nonatomic,retain) NSString *nativeViewType;
@property (nonatomic,retain) UIView *nativeViewView;
@property (nonatomic,assign) RhoViewController* mTabBarCallback;
@property (assign) BOOL urlBasedNativeView;
@property (assign) BOOL isBackgroundSetted;
@property (assign) BOOL is_url_after_set_background_redirect;
@property (nonatomic,retain) NSString* url_after_set_background;


- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p;
- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p bar_info:(NSDictionary*)bar_info;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame webview:(UIWebView*)webview;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame web_bkg_color:(UIColor*)web_bkg_color;

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;
- (void)loadView;

- (void)removeToolbar;
- (void)addToolbar:(NSDictionary*)bar_info;

-(void)setWebBackgroundColor:(int)bkg_color;

-(void)restoreWebView;

- (UIWebView*)getWebView:(int)tab_index;

+ (void) disableHiddenOnStart;

@end
