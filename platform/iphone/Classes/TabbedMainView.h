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

@interface TabbedMainView : RhoViewController<RhoMainView> {
    UITabBarController *tabbar;
    NSArray *tabbarData;
    int tabindex;
}

@property (nonatomic,retain) UITabBarController *tabbar;
@property (nonatomic,retain) NSArray *tabbarData;
@property (nonatomic,assign) int tabindex;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p tabs:(NSArray*)items;

- (UIWebView*)getWebView:(int)tab_index;

@end
