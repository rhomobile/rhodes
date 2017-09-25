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
#import "Signature/SignatureViewController.h"


static void updateViewRect(CGRect* pRect)
{
#ifdef __IPHONE_7_0
    if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) {
        char fsc[] = "0";
        const char* fs = get_app_build_config_item("iphone_use_new_ios7_status_bar_style");
        if (fs == NULL) {
            fs = fsc;
        }
        
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) {
            CGSize screenSize = [[UIScreen mainScreen] bounds].size;
            if (screenSize.height == 812.0f)
                //NSLog(@"iPhone X");
                fs = "1";
        }

        if ((fs[0] == '0')) {
            pRect->origin.y += 20;
            pRect->size.height -= 20;
        }
    }
    
#endif
}

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
    
    updateViewRect(&left_frame);
	
	leftView.frame = left_frame;
    //leftView.bounds = left_frame;
	rightView.frame = right_frame;
	
}


@end



@implementation SplittedMainView


@synthesize viewControllers;

- (void)dealloc {
	[viewControllers release];
    //[mCreationTimeMainView release];
    [super dealloc];
}

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p  bar_info:(NSDictionary*)bar_info {
    
	self = [self initWithNibName:nil bundle:nil];
    
    mCreationTimeMainView = v;

	RightViewController *rightView = NULL;
	LeftViewController *leftView = NULL;
	//create the master view
	leftView = [[LeftViewController alloc]
										initWithItems:bar_info
										parent:self];
	
	//create the details view
	rightView = [[RightViewController alloc]
									  initWithItems:bar_info
									  parent:self];

	//RhoUISplitView* rv = [[RhoUISplitView alloc] init];
	
	
	
	CGRect parent_frame = [[v view] frame];
	self.view.frame = parent_frame;
	
	
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
    
    self.view.backgroundColor = [UIColor grayColor];
	

	RhoUISplitView* rv = (RhoUISplitView*)self.view;
	rv.leftWidth = [leftView getPreferredWidth];

	
	[leftView release];
	[rightView release];
	
    CGRect rc = [v view].bounds;
    updateViewRect(&rc);
	[ leftView updateViewHeight: rc.size.height] ;
    [self switchTab: [leftView getFirstActiveItem]];
    
	//[self.view layoutSubviews];
	//[self.view setNeedsLayout];
	
    return self;
}


- (id<RhoMainView>)getCreationTimeMainView {
    return mCreationTimeMainView;
}



- (void)loadView {
	RhoUISplitView* rv = [[RhoUISplitView alloc] initWithFrame:CGRectMake(0, 0, 100, 100)];
    rv.hidden = NO;
    rv.userInteractionEnabled = YES;
    rv.autoresizesSubviews = YES;
    rv.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;

	self.view = rv;
	[rv release];
}

/*
- (void)viewDidLoad
{
	[super viewDidLoad];
}
*/






/*
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // MUST return YES to allow all orientations
    return YES;
}
 */


- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	[[self getRightViewController] didRotateFromInterfaceOrientation:fromInterfaceOrientation];
}



- (RightViewController*)getRightViewController {
	RightViewController* rvc = (RightViewController*)[self.viewControllers objectAtIndex:1];
	return rvc;
}


- (LeftViewController*)getLeftViewController {
	LeftViewController* lvc = (LeftViewController*)[self.viewControllers objectAtIndex:0];
	return lvc;
}


- (id<RhoWebView,NSObject>)getRhoWebView:(int)tab_index {
    RightViewController* rvc = [self getRightViewController];
    return [rvc getRhoWebView:tab_index];
}

- (id<RhoWebView,NSObject>)detachRhoWebView {
    RightViewController* rvc = [self getRightViewController];
    return [rvc detachRhoWebView];
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

-(void)openNativeView:(UIView*)nv_view tab_index:(int)tab_index {
	RightViewController* rvc = [self getRightViewController];
	[rvc openNativeView:nv_view tab_index:tab_index];
}

-(void)closeNativeView:(int)tab_index {
	RightViewController* rvc = [self getRightViewController];
	[rvc closeNativeView:tab_index];
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

- (NSString*)get_current_url:(int)index {
	RightViewController* rvc = [self getRightViewController];
	return [rvc get_current_url:index];
}


-(UIViewController*)getMainViewController {
    return self;
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

 
-(void)setTabBarBadge:(NSString*)badge_text tab_index:(int)tab_index {
    // nothing
}


@end
