
#import <Foundation/Foundation.h>

#import "api_generator/iphone/IMethodResult.h"

@protocol IBarcode1 <NSObject>

-(void) getProps:(id<IMethodResult>)methodResult;
-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult;
-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult;
/*
-(void) setProps:(NSDictionary*)props;
-(void) setProperty:(NSString*)propertyName propertyValue:(NSObject*)propertyValue;
*/

// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

@end


@protocol IBarcode1Singleton <NSObject>
-(void) enumerate:(id<IMethodResult>)methodResult;
-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;
-(NSString*)getInitialDefaultID;
@end


@protocol IBarcode1Factory <NSObject>
-(id<IBarcode1Singleton>) getBarcode1Singleton;
-(id<IBarcode1>) getBarcode1ByID:(NSString*)ID;
@end



@interface Barcode1FactorySingleton : NSObject {
}
+(id<IBarcode1Factory>) getBarcode1FactoryInstance;
@end


