//
//  LoginViewController.h
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "WaitLoginController.h"
#import "RootViewController.h"

@class RhoSyncNotify;
@interface LoginViewController : UIViewController {
	IBOutlet UITextField* txtLogin;
	IBOutlet UITextField* txtPassword;
	IBOutlet WaitLoginController *waitPage;
	IBOutlet RootViewController *indexPage;
	
}

@property (retain, nonatomic) IBOutlet UITextField* txtLogin;
@property (retain, nonatomic) IBOutlet UITextField* txtPassword;
@property (nonatomic, retain) IBOutlet WaitLoginController* waitPage;
@property (nonatomic, retain) IBOutlet RootViewController* indexPage;

- (IBAction)doLogin:(id)sender;
- (void)loginComplete:(RhoSyncNotify*) notify;

@end
