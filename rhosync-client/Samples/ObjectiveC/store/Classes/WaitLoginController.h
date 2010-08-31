//
//  WaitLoginController.h
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RootViewController.h"


@interface WaitLoginController : UIViewController {
	IBOutlet UILabel* lblMessage;
	IBOutlet UIActivityIndicatorView *indicator;
	IBOutlet RootViewController *homePage;
}

@property (nonatomic, retain) IBOutlet UILabel *lblMessage;
@property (nonatomic, retain) IBOutlet UIActivityIndicatorView *indicator;
@property (nonatomic, retain) IBOutlet RootViewController* homePage;

- (void)loginComplete:(NSString*) errorMessage;

@end
