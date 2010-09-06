//
//  LoginViewController.h
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WaitLoginController.h"

@class RhoSyncNotify;
@interface LoginViewController : UIViewController {
	IBOutlet UITextField* txtLogin;
	IBOutlet UITextField* txtPassword;
	IBOutlet WaitLoginController *waitPage;
}

@property (retain, nonatomic) IBOutlet UITextField* txtLogin;
@property (retain, nonatomic) IBOutlet UITextField* txtPassword;
@property (nonatomic, retain) IBOutlet WaitLoginController* waitPage;

- (IBAction)doLogin:(id)sender;
- (void)loginComplete:(RhoSyncNotify*) notify;

@end
