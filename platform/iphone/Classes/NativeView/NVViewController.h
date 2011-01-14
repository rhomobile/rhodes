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

@class NVDelegate;

@interface NVViewController : RhoViewController {
	UIView *nvView; 
	NVDelegate* nvDelegate;
}

- (id)initWithRect:(CGRect)rect nvview:(UIView*)nvview delegate:(NVDelegate*)delegate;

- (void)loadView;

@end
