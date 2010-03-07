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

@interface Rhodes : NSObject <UIApplicationDelegate>
{
    UIApplication *application;
    UIWindow *window;
    id<RhoMainView> mainView;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) id<RhoMainView> mainView;

+ (Rhodes*)sharedInstance;

+ (UIApplication*)application;

+ (void)performOnUiThread:(SEL)selector target:(id)target;
+ (void)performOnUiThread:(SEL)selector target:(id)target wait:(BOOL)wait;
+ (void)performOnUiThread:(SEL)selector target:(id)target withObject:(id)object;
+ (void)performOnUiThread:(SEL)selector target:(id)target withObject:(id)object wait:(BOOL)wait;

@end
