
#import "INativeToolbar.h"
#import "NativeToolbarSingletonBase.h"

@interface NativeToolbarSingleton : NativeToolbarSingletonBase<INativeToolbarSingleton> {
}



-(void) create:(NSArray*)toolbarElements toolBarProperties:(NSDictionary*)toolBarProperties methodResult:(id<IMethodResult>)methodResult;
-(void) remove:(id<IMethodResult>)methodResult;
-(void) isCreated:(id<IMethodResult>)methodResult;




@end