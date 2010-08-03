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
    UITabBarController *tabbar;
    NSArray *tabbarData;
}

@property (nonatomic,retain) UITabBarController *tabbar;
@property (nonatomic,retain) NSArray *tabbarData;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p tabs:(NSArray*)items;
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;

@end
