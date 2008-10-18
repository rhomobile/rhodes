//
//  BrowserAppDelegate.h
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ServerHost.h"
#import "SyncEngine.h"

@class WebViewController;

@interface RhoRunnerAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet WebViewController *webViewController;
    ServerHost * serverHost;
	sqlite3 *database;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) WebViewController *webViewController;

@end

