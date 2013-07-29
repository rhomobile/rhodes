
#import "INativeTabbar.h"
#import "NativeTabbarSingletonBase.h"

@interface NativeTabbarSingleton : NativeTabbarSingletonBase<INativeTabbarSingleton> {
}



-(void) create:(NSArray*)tabElements tabBarProperties:(NSDictionary*)tabBarProperties methodResult:(id<IMethodResult>)methodResult;
-(void) currentTabIndex:(id<IMethodResult>)methodResult;
-(void) remove:(id<IMethodResult>)methodResult;
-(void) setTabBadge:(int)tabIndex badge:(NSString*)badge methodResult:(id<IMethodResult>)methodResult;
-(void) switchTab:(int)tabIndex methodResult:(id<IMethodResult>)methodResult;
-(void) isCreated:(id<IMethodResult>)methodResult;




@end