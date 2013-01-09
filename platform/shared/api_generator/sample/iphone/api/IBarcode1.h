
#import <Foundation/Foundation.h>

#import "../../../common/iphone/IMethodResult.h"

@protocol IBarcode1 <NSObject>

-(void) getProps:(id<IMethodResult>)methodResult;
-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult;
-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult;

// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

+(void) enumerate:(id<IMethodResult>)methodResult;

@end


@protocol IBarcode1Factory <NSObject>

-(id<IBarcode1>) getBarcode1ByID:(NSString*)ID;
-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;
-(NSString*)getInitialDefaultID;
-(Class) getBarcode1ImplementationClass;

@end

@interface Barcode1Singletone : NSObject {
}

+(id<IBarcode1Factory>) getBarcode1Factory;

@end