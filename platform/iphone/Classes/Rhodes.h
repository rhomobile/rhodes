/*
 *  Rhodes.h
 *  rhorunner
 *
 *  Created by Dmitry Moskalchuk on 07.03.10.
 *  Copyright 2010 Rhomobile Inc. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import "AppManager.h"
#import "RhoMainView.h"

@protocol RhoRunnable

- (void)run;

@end

@interface Rhodes : NSObject <UIApplicationDelegate, UIWebViewDelegate, UITabBarControllerDelegate>
{
    AppManager *appManager;
    UIApplication *application;
    UIWindow *window;
@public
    id<RhoMainView,NSObject> mainView;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) id<RhoMainView,NSObject> mainView;

+ (Rhodes*)sharedInstance;

+ (UIApplication*)application;

+ (void)performOnUiThread:(id<RhoRunnable>)runnable wait:(BOOL)wait;

- (UIWindow*)rootWindow;

@end
