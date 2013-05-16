
#import "INativeToolbar.h"
#import "NativeToolbarSingletonBase.h"

@interface NativeToolbarSingleton : NativeToolbarSingletonBase<INativeToolbarSingleton> {
}



-(void) create:(NSArray*)toolbarElements toolBarProperties:(NSDictionary*)toolBarProperties;
-(void) remove;
-(void) isCreated:(id<IMethodResult>)methodResult;




@end