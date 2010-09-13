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
}

@property (nonatomic,retain) UIWebView *webView;
@property (nonatomic,retain) UIToolbar *toolbar;
@property (nonatomic,retain) UINavigationBar *navbar;
@property (nonatomic,retain) NSString *nativeViewType;
@property (nonatomic,retain) UIView *nativeViewView;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p;
- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p toolbar:(NSArray*)items;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame;

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;
- (void)loadView;

@end
