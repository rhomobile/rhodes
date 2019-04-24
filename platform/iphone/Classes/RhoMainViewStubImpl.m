#import "RhoMainViewStubImpl.h"


@implementation RhoMainViewStubImpl


- (UIView*)view {
    return nil;
}

- (id<RhoWebView,NSObject>)detachRhoWebView {
    return nil;
}

- (void)loadHTMLString:(NSString*)data {

}

- (void)back:(int)index {

}

- (void)forward:(int)index {

}

- (void)navigate:(NSString*)url tab:(int)index {

}

- (void)navigateRedirect:(NSString*)url tab:(int)index {

}

- (void)reload:(int)index {

}

- (void)executeJs:(NSString*)js tab:(int)index {

}

- (NSString*)currentLocation:(int)index {
    return nil;
}

- (NSString*)get_current_url:(int)index {
    return nil;
}

- (void)switchTab:(int)index {
    return;
}

- (int)activeTab {
    return 0;
}

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right{

}

- (void)removeNavBar {

}

- (id<RhoWebView,NSObject>)getRhoWebView:(int)tab_index {
    return nil;
}

-(void)openNativeView:(UIView*)nv_view tab_index:(int)tab_index {

}

-(void)closeNativeView:(int)tab_index {

}

-(void)setTabBarBadge:(NSString*)badge_text tab_index:(int)tab_index {

}

-(UIViewController*)getMainViewController {
    return nil;
}


@end
