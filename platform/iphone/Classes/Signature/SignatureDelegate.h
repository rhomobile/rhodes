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
#import "SignatureViewController.h"

@interface SignatureDelegate : RhoDelegate {
	SignatureViewController* signatureViewController;
	UIView* parentView;
	UIView* prevView;
	NSString* imageFormat;
}


-(void)doDone:(UIImage*)image;
-(void)doCancel;
-(void)setSignatureViewControllerValue:(SignatureViewController *)svc;
-(void)setParentView:(UIView*)parent_view;
-(void)setPrevView:(UIView*)prev_view;
-(void)setImageFormat:(NSString*)format;

@end