
#import "INavbar.h"
#import "NavbarSingletonBase.h"

@interface NavbarSingleton : NavbarSingletonBase<INavbarSingleton> {
}



-(void) create:(NSDictionary*)navBarProperties;
-(void) remove;
-(void) started:(id<IMethodResult>)methodResult;




@end