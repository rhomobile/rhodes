/*
 *  Rhodes.h
 *  rhorunner
 *
 *  Created by Dmitry Moskalchuk on 07.03.10.
 *  Copyright 2010 Rhomobile Inc. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import "RhoMainView.h"

@protocol RhoRunnable

- (void)run;

@end

@interface Rhodes : NSObject <UIApplicationDelegate, UIWebViewDelegate>
{
    UIApplication *application;
    UIWindow *window;
@public
    id<RhoMainView> mainView;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) id<RhoMainView> mainView;

+ (Rhodes*)sharedInstance;

+ (UIApplication*)application;

+ (void)performOnUiThread:(id<RhoRunnable>)runnable wait:(BOOL)wait;

- (UIWindow*)rootWindow;

@end
