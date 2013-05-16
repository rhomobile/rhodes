
#import "INativeTabbar.h"
#import "NativeTabbarSingletonBase.h"

@interface NativeTabbarSingleton : NativeTabbarSingletonBase<INativeTabbarSingleton> {
}



-(void) create:(NSArray*)tabElements tabBarProperties:(NSDictionary*)tabBarProperties methodResult:(id<IMethodResult>)methodResult;
-(void) currentTabIndex:(id<IMethodResult>)methodResult;
-(void) remove;
-(void) setTabBadge:(int)tabIndex badge:(NSString*)badge;
-(void) switchTab:(int)tabIndex;
-(void) isCreated:(id<IMethodResult>)methodResult;




@end