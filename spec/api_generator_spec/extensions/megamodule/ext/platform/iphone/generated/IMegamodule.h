
#import <Foundation/Foundation.h>

#import "api_generator/iphone/IMethodResult.h"

/* This is documentation const 1 */
#define CONST1_STRING @"value1"

/* This is documentation const 2 */
#define CONST2_INTEGER 123

/* value doc 1 */
#define CONST_STRING_1 @"const1"

/* value doc 2 */
#define CONST_STRING_2 @"const2"


// hash keys used in properties and parameters

#define HK_ITEM_ARRAY @"itemArray"

#define HK_ITEM_STR @"itemStr"

#define HK_PARAMA2 @"parama2"

#define HK_PARAME1 @"parame1"

#define HK_STR_PARAM @"strParam"


@protocol IMegamodule <NSObject>


/* getter for "StringProperty" property */
-(void) getStringProperty:(id<IMethodResult>)methodResult;

/* getter for "IntegerProperty" property */
-(void) getIntegerProperty:(id<IMethodResult>)methodResult;

/* setter for "IntegerProperty" property */
-(void) setIntegerProperty:(int)IntegerProperty;

/* test types: return true if (paramStr == "ABC") AND (paramBool == true) AND (paramInt == 555123) AND (paramFloat == 3.14) AND (paramArray == [1,2,3]) AND (paramHash == {:p1 => "abc", :p2 => "qwe"}). */
-(void) typesTest:(NSString*)paramStr paramBool:(BOOL)paramBool paramInt:(int)paramInt paramFloat:(float)paramFloat paramArray:(NSArray*)paramArray paramHash:(NSDictionary*)paramHash methodResult:(id<IMethodResult>)methodResult;

/* example of producing Array for return - must return [1,2,3,4,5,6,7,8,9,10] */
-(void) produceArray:(id<IMethodResult>)methodResult;

/* example of producing Hash for return */
-(void) produceHash:(id<IMethodResult>)methodResult;

/* example of producing complicated type(array of hash of dif types) for return. should return ; [{ :parame1 => "value1", :parama2 = > 55 }, { :paramu3 => [1,2,3] } ] */
-(void) produceComplicatedResult:(id<IMethodResult>)methodResult;

/* show Alert from UI thread */
-(void) showAlertFromUIThread:(NSString*)message;

/* start periodically callback */
-(void) setPeriodicallyCallback:(int)periodInMilliseconds methodResult:(id<IMethodResult>)methodResult;

/* check for callback is setted */
-(void) isPeriodicallyCallbackSetted:(id<IMethodResult>)methodResult;

/* stop callback */
-(void) stopPeriodicallyCallback;

/* test complicated params: return true if params == [ { :intItem => 35, :strItem => 'ddd', hashItem => { :itemStr => 'eee', :itemArray => [true, false, true] } }, { :intItem => 12, :strItem => 'bla-bla-bla' } ] */
-(void) complicatedTypesTest1:(NSArray*)paramArray methodResult:(id<IMethodResult>)methodResult;

/* This is documentation */
-(void) getProperty:(NSString*)propertyName methodResult:(id<IMethodResult>)methodResult;

/* This is documentation */
-(void) getProperties:(NSArray*)arrayofNames methodResult:(id<IMethodResult>)methodResult;

/* This is documentation */
-(void) getAllProperties:(id<IMethodResult>)methodResult;

/* This is documentation */
-(void) setProperty:(NSString*)propertyName propertyValue:(NSString*)propertyValue;

/* This is documentation */
-(void) setProperties:(NSDictionary*)propertyMap;

/* This is documentation */
-(void) clearAllProperties;


// NOTE: if you want to hold methodResult(for example periodically call callbacks) you should release it manually when you stop using it!
@end


@protocol IMegamoduleSingleton <NSObject>


-(NSString*) getDefaultID;
-(void) setDefaultID:(NSString*)defaultID;
-(NSString*)getInitialDefaultID;
    


-(void) getObjectsCount:(id<IMethodResult>)methodResult;

-(void) getObjectByIndex:(int)index methodResult:(id<IMethodResult>)methodResult;

/* This is documentation */
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


