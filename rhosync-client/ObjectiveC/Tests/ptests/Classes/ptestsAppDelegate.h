//
//  ptestsAppDelegate.h
//  ptests
//
//  Created by Vlad on 8/25/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ptestsViewController;

@interface ptestsAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    ptestsViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet ptestsViewController *viewController;

@end

