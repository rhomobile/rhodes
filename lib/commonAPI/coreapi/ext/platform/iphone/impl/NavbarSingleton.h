
#import "INavbar.h"
#import "NavbarSingletonBase.h"

@interface NavbarSingleton : NavbarSingletonBase<INavbarSingleton> {
}



-(void) create:(NSDictionary*)navBarProperties methodResult:(id<IMethodResult>)methodResult;
-(void) remove:(id<IMethodResult>)methodResult;
-(void) started:(id<IMethodResult>)methodResult;




@end