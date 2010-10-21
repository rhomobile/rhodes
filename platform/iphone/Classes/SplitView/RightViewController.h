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
#import "SplittedMainView.h"

@interface RightViewController : UIViewController<RhoMainView> {
	SplittedMainView* splittedView;
    NSArray *itemsData;
    int tabindex;	
	
}

@property (nonatomic,retain) NSArray *itemsData;
@property (nonatomic,assign) int tabindex;

- (id)initWithItems:(NSArray*)items parent:(SplittedMainView*)parent;




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
