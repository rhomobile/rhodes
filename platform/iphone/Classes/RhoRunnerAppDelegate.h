//
//  BrowserAppDelegate.h
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ServerHost.h"
#import "PickImageDelegate.h"
#import "LogViewController.h"
#import "LogOptionsController.h"

@class WebViewController;

@interface RhoRunnerAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet WebViewController *webViewController;
	LogViewController* logViewController;
	LogOptionsController* logOptionsController;
    ServerHost * serverHost;
	PickImageDelegate* pickImageDelegate;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) WebViewController *webViewController;

@end

