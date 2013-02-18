
#import <Foundation/Foundation.h>

#import "api_generator/iphone/IMethodResult.h"




#define CONST1_STRING @"value1"
#define CONST2_INTEGER 123
#define CONST_STRING_1 @"const1"
#define CONST_STRING_2 @"const2"




@protocol IMegamodule <NSObject>

-(void) getStringProperty:(id<IMethodResult>)methodResult;
-(void) getIntegerProperty:(id<IMethodResult>)methodResult;
-(void) setIntegerProperty:(int)value;
-(void) typesTest:(NSString*)paramStr paramBool:(BOOL)paramBool paramInt:(int)paramInt paramFloat:(float)paramFloat paramArray:(NSArray*)paramArray paramHash:(NSDictionary*)paramHash methodResult:(id<IMethodResult>)methodResult;
-(void) produceArray:(id<IMethodResult>)methodResult;
-(void) showAlertFromUIThread:(NSString*)message;
-(void) setPeriodicallyCallback:(int)periodInMilliseconds methodResult:(id<IMethodResult>)methodResult;
-(void) isPeriodicallyCallbackSetted:(id<IMethodResult>)methodResult;
-(void) stopPeriodicallyCallback;
-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult;
-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult;
-(void) getAllProperties:(id<IMethodResult>)methodResult;
-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue;
-(void) setProperties:(NSDictionary*)propertyMap;
-(void) clearAllProperties;




// NOTE: if you want to hold methodResult(for periodically call callback for example) you should release it buself when you stop using it !

@end


@protocol IMegamoduleSingleton <NSObject>


-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;
-(NSString*)getInitialDefaultID;
    

-(void) getObjectsCount:(id<IMethodResult>)methodResult;
-(void) getObjectByIndex:(int)index methodResult:(id<IMethodResult>)methodResult;
-(void) enumerate:(id<IMethodResult>)methodResult;


@end


@protocol IMegamoduleFactory <NSObject>
-(id<IMegamoduleSingleton>) getMegamoduleSingleton;
-(id<IMegamodule>) getMegamoduleByID:(NSString*)ID;
-(void) destroyObjectByID:(NSString*)ID;
@end


@interface MegamoduleFactorySingleton : NSObject {
}
+(id<IMegamoduleFactory>) getMegamoduleFactoryInstance;
@end


