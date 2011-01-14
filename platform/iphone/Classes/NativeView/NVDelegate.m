//
//  SignatureDelegate.m
//  rhorunner
//
//  Created by Soldatenkov Dmitry on 27/07/10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import "Rhodes.h"
#import "NVDelegate.h"
#import "AppManager.h"
#import "common/RhodesApp.h"

@implementation NVDelegate


-(void)setNVViewControllerValue:(NVViewController *)svc {
	nvViewController = svc;
}

-(void)setParentView:(UIView*)parent_view {
	parentView = parent_view;
}

-(void)setPrevView:(UIView*)prev_view {
	prevView = prev_view;
}


-(void)doClose {
    [nvViewController.view removeFromSuperview];
    [nvViewController release];
	[parentView addSubview:prevView];
	[parentView layoutSubviews];
	[prevView release];
}


@end

