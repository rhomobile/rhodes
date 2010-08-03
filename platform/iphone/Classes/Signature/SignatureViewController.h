//
//  SignatureView.h
//  rhorunner
//
//  Created by Soldatenkov Dmitry on 27/07/10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"

@class SignatureDelegate;

@interface SignatureViewController : UIViewController {
	UIView *signatureView; 
	UIToolbar *toolbar;
	SignatureDelegate* signatureDelegate;
}

- (id)initWithRect:(CGRect)rect delegate:(SignatureDelegate*)delegate;

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;
- (void)loadView;

@end
