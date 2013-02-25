
#import <Foundation/Foundation.h>

#import "api_generator/iphone/IMethodResult.h"








@protocol IJavascriptVM <NSObject>





// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

@end


@protocol IJavascriptVMSingleton <NSObject>




/* Execute any script */
-(void) executeScript:(NSString*)script methodResult:(id<IMethodResult>)methodResult;


@end


@protocol IJavascriptVMFactory <NSObject>
-(id<IJavascriptVMSingleton>) getJavascriptVMSingleton;
-(id<IJavascriptVM>) getJavascriptVMByID:(NSString*)ID;
-(void) destroyObjectByID:(NSString*)ID;
@end


@interface JavascriptVMFactorySingleton : NSObject {
}
+(id<IJavascriptVMFactory>) getJavascriptVMFactoryInstance;
@end


