//
//  TabBarDelegate.h
//  rhorunner
//
//  Created by lars on 8/14/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RhoDelegate.h"
#import "NativeBar.h"

@interface TabBarDelegate : RhoDelegate <UITabBarControllerDelegate> 
{
@private
	UITabBarController* tabBarController;
	NativeBar* tabBar;
	UIWindow *mainWindow;
}

@property(nonatomic, assign) NativeBar* tabBar;
@property(nonatomic, retain) UITabBarController* tabBarController;
@property (nonatomic, retain) UIWindow *mainWindow;

- (void)createTabBar:(UIWindow*)window;
- (void)loadTabBarItemFirstPage:(NSString*)location itemIndex:(int)index;
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController;
@end
