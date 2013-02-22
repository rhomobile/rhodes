
#import <Foundation/Foundation.h>

#import "api_generator/iphone/IMethodResult.h"








@protocol ISimpleOnlyStaticModule <NSObject>





// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

@end


@protocol ISimpleOnlyStaticModuleSingleton <NSObject>



-(void) calcSumm:(int)x y:(int)y methodResult:(id<IMethodResult>)methodResult;
-(void) joinStrings:(NSArray*)strings methodResult:(id<IMethodResult>)methodResult;
-(void) getPlatform:(id<IMethodResult>)methodResult;
-(void) showAlertFromUIThread;


@end


@protocol ISimpleOnlyStaticModuleFactory <NSObject>
-(id<ISimpleOnlyStaticModuleSingleton>) getSimpleOnlyStaticModuleSingleton;
-(id<ISimpleOnlyStaticModule>) getSimpleOnlyStaticModuleByID:(NSString*)ID;
-(void) destroyObjectByID:(NSString*)ID;
@end


@interface SimpleOnlyStaticModuleFactorySingleton : NSObject {
}
+(id<ISimpleOnlyStaticModuleFactory>) getSimpleOnlyStaticModuleFactoryInstance;
@end


