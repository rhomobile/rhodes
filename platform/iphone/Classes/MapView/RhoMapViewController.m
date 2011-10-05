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

#ifdef __IPHONE_3_0

#import "MapAnnotation.h"
#import "RhoMapViewController.h"
#import "Rhodes.h"
#import "RhoMainView.h"

#include "logging/RhoLog.h"
#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "MapView"

static RhoMapViewController *mc = nil;

@interface RhoCreateRhoMapTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoCreateRhoMapTask
+ (void)run:(NSValue*)value {
    if (mc) {
        [mc close];
        mc = nil;
    }
    RhoMapViewController* map = [[RhoMapViewController alloc] init];
    [map setParams:[value pointerValue]];
    UIWindow *window = [[Rhodes sharedInstance] rootWindow];
	map.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	map.view.autoresizesSubviews = YES;
	
	UIView* v = [[[Rhodes sharedInstance] mainView] view];
	map.savedMainView = v;
	[map.savedMainView retain];
    [map.savedMainView removeFromSuperview];
	[window addSubview:map.view];
    mc = map;
}
@end

@interface RhoCloseRhoMapTask : NSObject
+ (void)run;
@end

@implementation RhoCloseRhoMapTask
+ (void)run {
    if (mc) {
        [mc close];
        mc = nil;
    }
}
@end


@implementation RhoMapViewController

@synthesize savedMainView;

+ (void)createMap:(rho_param *)params {
    id runnable = [RhoCreateRhoMapTask class];
    id arg = [NSValue valueWithPointer:params];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

+ (void)closeMap {
    id runnable = [RhoCloseRhoMapTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
         // setup
    
    }
    return self;
}

- (void)close {
    [self dismissModalViewControllerAnimated:YES]; 
    
	UIWindow *window = [[Rhodes sharedInstance] rootWindow];


	[window addSubview:self.savedMainView];
	[self.view removeFromSuperview];

	[self.savedMainView release];
	self.savedMainView = nil;
}

- (void)setParams:(rho_param*)p {

    
    
    rho_param_free(p);
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    //Initialize the toolbar
    toolbar = [[UIToolbar alloc] init];
    toolbar.barStyle = UIBarStyleBlack;
    UIBarButtonItem *closeButton = [[UIBarButtonItem alloc]
                               initWithTitle:@"Close" style:UIBarButtonItemStyleBordered 
                               target:self action:@selector(close_clicked:)];
    [toolbar setItems:[NSArray arrayWithObjects:closeButton,nil]];

	
	toolbar.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | 
	UIViewAutoresizingFlexibleWidth;
	toolbar.autoresizesSubviews = YES;
    [toolbar sizeToFit];
	
    CGFloat toolbarHeight = [toolbar frame].size.height;
	// hack for do not reduce height of toolbar in Landscape mode
	if (toolbarHeight < 44) {
		toolbarHeight = 44;
	}

	CGRect rootViewBounds = [[[Rhodes sharedInstance] mainView] view].frame;//bounds;
	
	self.view.frame = rootViewBounds;
	
    CGFloat rootViewHeight = rootViewBounds.size.height;
	//CGFloat rootViewHeight = CGRectGetHeight(rootViewBounds);
    CGFloat rootViewWidth = CGRectGetWidth(rootViewBounds);
    CGRect rectArea = CGRectMake(0, rootViewHeight - toolbarHeight, rootViewWidth, toolbarHeight);
    toolbar.frame = rectArea;
    
	
	[self.view addSubview:toolbar];
    [closeButton release];
    
    CGRect rectMapArea = CGRectMake(0, 0, rootViewWidth, rootViewHeight - toolbarHeight);
    //mapView =[[MKMapView alloc] initWithFrame:rectMapArea];
	mapView = [[UIView alloc] initWithFrame:rectMapArea];
    mapView.frame = rectMapArea;
    //mapView.delegate=self;
    mapView.autoresizesSubviews = YES;
    mapView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	mapView.backgroundColor = [UIColor groupTableViewBackgroundColor];
	
	
    [self.view insertSubview:mapView atIndex:0];
	//[[self.view superview] layoutSubviews];
	
}

- (void) close_clicked:(id)sender {
    [self close];
	//mc = nil;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}



+ (BOOL)isStarted {
    return mc != nil;
}

+ (double)centerLatitude {
	return 0.0;
}

+ (double)centerLongitude {
	return 0.0;
}


@end

#endif
