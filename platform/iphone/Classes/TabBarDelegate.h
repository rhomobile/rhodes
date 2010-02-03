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
#import "BarItem.h"

@interface TabBarDelegate : RhoDelegate <UITabBarControllerDelegate> 
{
@private
	UITabBarController* tabBarController;
	NativeBar* tabBar;
	UIWindow* mainWindow;
	NSMutableArray* barItems;
	int activeTab;
}

@property(nonatomic, assign) NativeBar* tabBar;
@property(nonatomic, retain) UITabBarController* tabBarController;
@property(nonatomic, retain) UIWindow* mainWindow;
@property(nonatomic, retain) NSMutableArray* barItems;
@property(assign) int activeTab;

- (id)init;
- (void)createTabBar:(UIWindow*)window;
- (void)deleteTabBar;
- (void)loadTabBarItemFirstPage:(BarItem*)item;
- (void)loadTabBarItemLocation:(BarItem*)item url:(NSString*)url;
- (void)refresh:(BarItem*)item;
- (void)executeJs:(BarItem*)item js:(JSString*)js;
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController;
- (void)switchTab:(int)index;
@end
