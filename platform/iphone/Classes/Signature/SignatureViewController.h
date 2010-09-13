//
//  SignatureView.h
//  rhorunner
//
//  Created by Soldatenkov Dmitry on 27/07/10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"
#import "RhoViewController.h"

@class SignatureDelegate;

@interface SignatureViewController : RhoViewController {
	UIView *signatureView; 
	UIToolbar *toolbar;
	SignatureDelegate* signatureDelegate;
}

- (id)initWithRect:(CGRect)rect delegate:(SignatureDelegate*)delegate;

- (void)loadView;

@end
