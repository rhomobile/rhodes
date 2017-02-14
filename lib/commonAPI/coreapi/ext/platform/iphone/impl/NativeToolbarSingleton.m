
#import "NativeToolbarSingleton.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NativeToolbar"

extern void rho_create_toolbar(NSArray* items, NSDictionary* properties);
extern BOOL nativetoolbar_started();
extern void remove_native_toolbar();

@implementation NativeToolbarSingleton



-(void) create:(NSArray*)toolbarElements toolBarProperties:(NSDictionary*)toolBarProperties methodResult:(id<IMethodResult>)methodResult{
    if (![toolbarElements isKindOfClass:[NSArray class]]) {
        RAWLOG_ERROR("Toolbar.create ERROR - elements parameter should be Array !");
        return;
    }
    rho_create_toolbar(toolbarElements, toolBarProperties);
}

-(void) remove:(id<IMethodResult>)methodResult{
    remove_native_toolbar();
}

-(void) isCreated:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:nativetoolbar_started()]];
}




@end
