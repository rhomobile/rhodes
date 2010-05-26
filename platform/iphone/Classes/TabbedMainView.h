//
//  TabbedMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"

@interface TabbedMainView : UIViewController<RhoMainView> {
    UIView *parent;
    UITabBarController *tabbar;
    NSArray *tabbarData;
}

@property (nonatomic,retain) UITabBarController *tabbar;
@property (nonatomic,retain) NSArray *tabbarData;

//- (id)initWithParentView:(UIView*)v items:(NSArray*)items;
//- (id)initWithParentView:(UIView*)v frame:(CGRect)frame items:(NSArray*)items;
- (id)initWithMainView:(id<RhoMainView>)v tabs:(NSArray*)items;

@end
