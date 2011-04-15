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
    NSString* on_change_tab_callback;
}

@property (nonatomic,retain) RhoUITabBarController *tabbar;
@property (nonatomic,retain) NSArray *tabbarData;
@property (nonatomic,assign) int tabindex;
@property (nonatomic,retain) NSString *on_change_tab_callback;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p bar_info:(NSDictionary*)bar_info;

- (UIWebView*)getWebView:(int)tab_index;

- (void)onViewWillActivate:(RhoViewController*)view;

-(void)callCallback:(int)new_index;

@end
