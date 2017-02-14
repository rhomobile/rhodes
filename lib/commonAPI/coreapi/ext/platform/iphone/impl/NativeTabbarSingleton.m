
#import "NativeTabbarSingleton.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeTabbar"


extern void rho_create_tabbar(NSArray* items, NSDictionary* properties, id<IMethodResult>callback);
extern BOOL nativetabbar_started();
extern void remove_native_tabbar();
extern void native_tabbar_set_tab_badge(int index,char *val);
extern int native_tabbar_get_current_tab();
extern void native_tabbar_switch_tab(int index);

@implementation NativeTabbarSingleton



-(void) create:(NSArray*)tabElements tabBarProperties:(NSDictionary*)tabBarProperties methodResult:(id<IMethodResult>)methodResult {
    if (![tabElements isKindOfClass:[NSArray class]]) {
        RAWLOG_ERROR("Tabbar.create ERROR - tabElements parameter should be Array !");
        return;
    }
    rho_create_tabbar(tabElements, tabBarProperties, methodResult);
}

-(void) currentTabIndex:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:native_tabbar_get_current_tab()]];
}

-(void) remove:(id<IMethodResult>)methodResult{
    remove_native_tabbar();
}

-(void) setTabBadge:(int)tabIndex badge:(NSString*)badge methodResult:(id<IMethodResult>)methodResult{
    native_tabbar_set_tab_badge(tabIndex, (char*)[badge UTF8String]);
}

-(void) switchTab:(int)tabIndex methodResult:(id<IMethodResult>)methodResult{
    native_tabbar_switch_tab(tabIndex);
}

-(void) isCreated:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:nativetabbar_started()]];
}

-(void) removeTab:(int)tabIndex methodResult:(id<IMethodResult>)methodResult{
    // unsupported
}



@end
