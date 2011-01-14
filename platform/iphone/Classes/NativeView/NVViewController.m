//
//  SignatureView.m
//  rhorunner
//
//  Created by Soldatenkov Dmitry on 27/07/10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import "NVViewController.h"

#import "AppManager.h"
#import "Rhodes.h"

#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NVViewController"


@implementation NVViewController


- (id)initWithRect:(CGRect)rect nvview:(UIView*)nvview delegate:(NVDelegate*)delegate {
	[self init];
	
	nvDelegate = delegate;
	self.view.frame = rect;

	self.view.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	self.view.autoresizesSubviews = YES;
	
	
	CGRect srect = self.view.frame;
	srect.origin.y = 0;
	
	nvView = nvview;
	nvView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	nvView.autoresizesSubviews = YES;
	nvView.frame = srect;
	
	[self.view addSubview:nvView];
	
	return self;

}


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    UIView* root = [[UIView alloc] init];
    root.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    root.autoresizesSubviews = YES;
    self.view = root;
    [root release];	
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	//[self.view 
}


/*
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
//	[signatureView setNeedsDisplay];
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
	[signatureView setNeedsDisplay];
}
 */

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

- (void)dealloc {
	[nvView removeFromSuperview];
    [super dealloc];
}



@end
