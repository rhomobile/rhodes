#import "RhoMainView.h"


@interface  RhoMainViewStubImpl : NSObject<RhoMainView> {}

- (UIView*)view;

- (id<RhoWebView,NSObject>)detachRhoWebView;

- (void)loadHTMLString:(NSString*)data;

- (void)back:(int)index;
- (void)forward:(int)index;
- (void)navigate:(NSString*)url tab:(int)index;
- (void)navigateRedirect:(NSString*)url tab:(int)index;
- (void)reload:(int)index;

- (void)executeJs:(NSString*)js tab:(int)index;

- (NSString*)currentLocation:(int)index;
- (NSString*)get_current_url:(int)index;

- (void)switchTab:(int)index;
- (int)activeTab;

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right;
- (void)removeNavBar;

- (id<RhoWebView,NSObject>)getRhoWebView:(int)tab_index;

-(void)openNativeView:(UIView*)nv_view tab_index:(int)tab_index;
-(void)closeNativeView:(int)tab_index;

-(void)setTabBarBadge:(NSString*)badge_text tab_index:(int)tab_index;

-(UIViewController*)getMainViewController;

@end
