//
//  SignatureDelegate.h
//  rhorunner
//
//  Created by Soldatenkov Dmitry on 27/07/10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "RhoDelegate.h"
#import "NVViewController.h"

@interface NVDelegate : RhoDelegate {
	NVViewController* nvViewController;
	UIView* parentView;
	UIView* prevView;
}


-(void)setNVViewControllerValue:(NVViewController*)svc;
-(void)setParentView:(UIView*)parent_view;
-(void)setPrevView:(UIView*)prev_view;
-(void)doClose;

@end