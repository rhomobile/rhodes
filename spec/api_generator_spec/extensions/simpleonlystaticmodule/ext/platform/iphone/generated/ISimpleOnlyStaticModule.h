
#import <Foundation/Foundation.h>

#import "api_generator/iphone/IMethodResult.h"








@protocol ISimpleOnlyStaticModule <NSObject>





// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

@end


@protocol ISimpleOnlyStaticModuleSingleton <NSObject>




/* calc summ of two integer */
-(void) calcSumm:(int)x y:(int)y methodResult:(id<IMethodResult>)methodResult;

/* join strings from input array and return one string */
-(void) joinStrings:(NSArray*)strings methodResult:(id<IMethodResult>)methodResult;

/* return string with current platform : "iphone", "android" etc. Method should call syncronously */
-(void) getPlatform:(id<IMethodResult>)methodResult;

/* example method for not return, not callback - just make some functionality */
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


