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


@interface RhoUISplitView : UIView {
	int leftWidth;
}
@property (nonatomic,assign) int leftWidth;

- (void)layoutSubviews;

@end


@interface SplittedMainView : RhoViewController<RhoMainView> {
	NSArray* viewControllers;

}

@property (nonatomic,retain) NSArray *viewControllers;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p tabs:(NSArray*)items;






//- (UIView*)view;

- (UIWebView*)detachWebView;

- (void)loadHTMLString:(NSString*)data;

- (void)back:(int)index;
- (void)forward:(int)index;
- (void)navigate:(NSString*)url tab:(int)index;
- (void)navigateRedirect:(NSString*)url tab:(int)index;
- (void)reload:(int)index;

- (void)executeJs:(NSString*)js tab:(int)index;

- (NSString*)currentLocation:(int)index;

- (void)switchTab:(int)index;
- (int)activeTab;

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right;
- (void)removeNavBar;

- (UIWebView*)getWebView:(int)tab_index;




@end
