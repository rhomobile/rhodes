//
//  SimpleMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 07.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

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
