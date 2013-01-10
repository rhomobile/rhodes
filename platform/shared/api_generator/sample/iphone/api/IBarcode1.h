
#import <Foundation/Foundation.h>

#import "../../../common/iphone/IMethodResult.h"

@protocol IBarcode1 <NSObject>

-(void) getProps:(id<IMethodResult>)methodResult;
-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult;
-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult;

// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

@end


@protocol IBarcode1Singleton <NSObject>
-(void) enumerate:(id<IMethodResult>)methodResult;
-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;
-(NSString*)getInitialDefaultID;
-(Class) getBarcode1ImplementationClass;

@end

@interface Barcode1Factory : NSObject {
}

+(id<IBarcode1Singleton>) createBarcode1Singleton;
+(id<IBarcode1>) createBarcode1ByID:(NSString*)ID;

@end