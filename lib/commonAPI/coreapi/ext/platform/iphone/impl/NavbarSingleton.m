
#import "NavbarSingleton.h"

extern void create_navbar(NSDictionary *p);
extern void remove_navbar();
extern BOOL navbar_started();


@implementation NavbarSingleton



-(void) create:(NSDictionary*)navBarProperties methodResult:(id<IMethodResult>)methodResult{
    if (![navBarProperties isKindOfClass:[NSDictionary class]]) {
        NSLog(@"Navbar.create ERROR - parameter should be HASH !");
        return;
    }
    create_navbar(navBarProperties);
}

-(void) remove:(id<IMethodResult>)methodResult{
    remove_navbar();
}

-(void) started:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithBool:navbar_started()]];
}




@end