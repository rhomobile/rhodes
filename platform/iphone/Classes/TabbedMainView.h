//
//  TabbedMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"
#import "RhoViewController.h"

@interface RhoUITabBarController : UITabBarController
{
	int bkgColor;
}

@property (nonatomic,assign) int bkgColor;

@end

@interface RhoCustomTabBarItem : UITabBarItem  
{
    UIImage *customHighlightedImage;
    UIImage *customStdImage;
}

@property (nonatomic, retain) UIImage *customHighlightedImage;
@property (nonatomic, retain) UIImage *customStdImage;

@end



@interface TabbedMainView : RhoViewController<RhoMainView> {
    RhoUITabBarController *tabbar;
    NSArray *tabbarData;
    int tabindex;
}

@property (nonatomic,retain) RhoUITabBarController *tabbar;
@property (nonatomic,retain) NSArray *tabbarData;
@property (nonatomic,assign) int tabindex;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p tabs:(NSArray*)items;

- (UIWebView*)getWebView:(int)tab_index;

- (void)onViewWillActivate:(RhoViewController*)view;

@end
