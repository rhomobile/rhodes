    //
//  TabbedMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SplittedMainView.h"
#import "SimpleMainView.h"
#import "Rhodes.h"
#import "AppManager.h"

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"


#import "LeftViewController.h"
#import "RightViewController.h"
#import "SplitViewDelegate.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SplittedMainView"


#import "TabbedMainView.h"
#import "SignatureViewController.h"



@implementation RhoUISplitView 

@synthesize leftWidth;

- (void)layoutSubviews {
	//[super layoutSubviews];
	
	CGRect my_frame = self.bounds;
	UIView* leftView = [[self subviews] objectAtIndex:0];
	UIView* rightView = [[self subviews] objectAtIndex:1];
	
	CGSize leftSize;
	leftSize.width = self.leftWidth;
	leftSize.height = my_frame.size.height;
	
	leftSize = [leftView sizeThatFits:leftSize];
	CGRect left_frame = leftView.bounds;
	CGRect right_frame = rightView.bounds;
	
	left_frame.origin.x = 0;//my_frame.origin.x;
	left_frame.origin.y = 0;//my_frame.origin.y;
	left_frame.size.width = leftSize.width;
	left_frame.size.height = my_frame.size.height; 
	
	right_frame.origin.x = left_frame.origin.x + left_frame.size.width + 2;
	right_frame.origin.y = 0;//my_frame.origin.y;
	right_frame.size.width = my_frame.size.width - left_frame.size.width - 2;
	right_frame.size.height = my_frame.size.height;
	
	leftView.frame = left_frame;
	rightView.frame = right_frame;
	
}


@end



@implementation SplittedMainView


@synthesize viewControllers;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p tabs:(NSArray *)items {
    
	self = [self init];

	RightViewController *rightView = NULL;
	LeftViewController *leftView = NULL;
	//create the master view
	leftView = [[LeftViewController alloc]
										initWithItems:items
										parent:self];
	
	//create the details view
	rightView = [[RightViewController alloc]
									  initWithItems:items
									  parent:self];

	RhoUISplitView* rv = [[RhoUISplitView alloc] init];
	
	CGRect parent_frame = [[v view] frame];
	rv.frame = parent_frame;
	
	self.view = rv;
    self.view.hidden = NO;
    self.view.userInteractionEnabled = YES;
    self.view.autoresizesSubviews = YES;
    self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	
	self.viewControllers = [NSArray arrayWithObjects:leftView, rightView, nil];
	[self.view addSubview:leftView.view];
	[self.view addSubview:rightView.view];
	
	
	//create and set the split view delegate
	//SplitViewDelegate* splitViewDelegate = [[SplitViewDelegate alloc] init];
	//splitViewDelegate.rightController = rightView;
	//self.delegate = splitViewDelegate;
	
	// undocumented feature
	//TODO: make legal way for not hide Master view in portrait mode 
	//if ( [self respondsToSelector:@selector(setHidesMasterViewInPortrait:)] ) {
	//	[self setHidesMasterViewInPortrait:NO];
	//}
	

	
	rv.leftWidth = [leftView getPreferredWidth];

	
	[leftView release];
	[rightView release];
	
	
	//[self.view layoutSubviews];
	//[self.view setNeedsLayout];
	
    return self;
}




- (void)loadView {
	[super loadView];
}

- (void)viewDidLoad
{
	[super viewDidLoad];
}







/*
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // MUST return YES to allow all orientations
    return YES;
}
 */


- (RightViewController*)getRightViewController {
	RightViewController* rvc = (RightViewController*)[self.viewControllers objectAtIndex:1];
	return rvc;
}


- (LeftViewController*)getLeftViewController {
	LeftViewController* lvc = (LeftViewController*)[self.viewControllers objectAtIndex:0];
	return lvc;
}


- (UIWebView*)detachWebView {
	RightViewController* rvc = [self getRightViewController];
	return [rvc detachWebView];
}

- (void)loadHTMLString:(NSString*)data {
	RightViewController* rvc = [self getRightViewController];
	[rvc loadHTMLString:data];
}

- (void)back:(int)index {
	RightViewController* rvc = [self getRightViewController];
	[rvc back:index];
}

- (void)forward:(int)index {
	RightViewController* rvc = [self getRightViewController];
	[rvc forward:index];
}

- (void)navigate:(NSString*)url tab:(int)index {
	RightViewController* rvc = [self getRightViewController];
	[rvc navigate:url tab:index];
}

- (void)navigateRedirect:(NSString*)url tab:(int)index {
	RightViewController* rvc = [self getRightViewController];
	[rvc navigateRedirect:url tab:index];
}

- (void)reload:(int)index {
	RightViewController* rvc = [self getRightViewController];
	[rvc reload:index];
}

- (void)executeJs:(NSString*)js tab:(int)index {
	RightViewController* rvc = [self getRightViewController];
	[rvc executeJs:js tab:index];
}

- (NSString*)currentLocation:(int)index {
	RightViewController* rvc = [self getRightViewController];
	return [rvc currentLocation:index];
}




- (void)switchTab:(int)index {
	// right
	RightViewController* rvc = [self getRightViewController];
	[rvc switchTab:index];
	// left
	LeftViewController* lvc = [self getLeftViewController];
	[lvc setSelection:index];
}

- (int)activeTab {
	RightViewController* rvc = [self getRightViewController];
	return [rvc activeTab];
}

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
	RightViewController* rvc = [self getRightViewController];
	[rvc addNavBar:title left:left right:right];
}

- (void)removeNavBar {
	RightViewController* rvc = [self getRightViewController];
	[rvc removeNavBar];
}

- (UIWebView*)getWebView:(int)tab_index {
	RightViewController* rvc = [self getRightViewController];
	return [rvc getWebView:tab_index];
}
 
/*

- (UIWebView*)detachWebView {
	return NULL;
}

- (void)loadHTMLString:(NSString*)data {
}

- (void)back:(int)index {
}

- (void)forward:(int)index {
}

- (void)navigate:(NSString*)url tab:(int)index {
}

- (void)navigateRedirect:(NSString*)url tab:(int)index {
}

- (void)reload:(int)index {
}

- (void)executeJs:(NSString*)js tab:(int)index {
}

- (NSString*)currentLocation:(int)index {
	return NULL;
}

- (void)switchTab:(int)index {
}

- (int)activeTab {
	return 0;
}

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
}

- (void)removeNavBar {
}

- (UIWebView*)getWebView:(int)tab_index {
	return NULL;
}

*/

@end
