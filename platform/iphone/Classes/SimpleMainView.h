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
#import "RhoWebView.h"
#import "RhoMainView.h"
#import "RhoViewController.h"
#include "RhoNativeViewManagerOC.h"



@interface SimpleMainView : RhoViewController <RhoMainView, RhoWebViewDelegate> {

    id<RhoWebView,NSObject> rhoWebView;
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
    int thisTabIndex;
}

@property (nonatomic,retain) id<RhoWebView,NSObject> rhoWebView;
@property (nonatomic,retain) UIToolbar *toolbar;
@property (nonatomic,retain) UINavigationBar *navbar;
@property (nonatomic,retain) NSString *nativeViewType;
@property (nonatomic,retain) UIView *nativeViewView;
@property (nonatomic,assign) RhoViewController* mTabBarCallback;
@property (assign) BOOL urlBasedNativeView;
@property (assign) BOOL isBackgroundSetted;
@property (assign) BOOL is_url_after_set_background_redirect;
@property (nonatomic,retain) NSString* url_after_set_background;
@property (assign) int thisTabIndex;


- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p;
- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p bar_info:(NSDictionary*)bar_info;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame rhowebview:(id<RhoWebView,NSObject>)rhowebview;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame web_bkg_color:(UIColor*)web_bkg_color;

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;
- (void)loadView;

- (void)removeToolbar;
- (void)addToolbar:(NSDictionary*)bar_info;

-(void)setWebBackgroundColor:(int)bkg_color;
	
-(void)restoreWebView;

- (id<RhoWebView,NSObject>)detachRhoWebView;
- (id<RhoWebView,NSObject>)getRhoWebView:(int)tab_index;

+ (void) disableHiddenOnStart;
+ (void) enableHiddenOnStart;

@end
