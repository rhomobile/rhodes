
#import <Foundation/Foundation.h>


// interface for set result/call callback from API method
// if you want call callback after method is finish - hold this id with retain and release it after use (can be used multiple time)
// All Ruby/JS types has equivalent in Objective C - you should make Objective C object corresponding type and set it
// NSString -> STRING
// NSDictionary -> HASH
// NSArray -> ARRAY
// NSNumber (created by numberWithInt/numberWithLongLong) -> INTEGER
// NSNumber (created by numberWithBool) -> TRUE/FALSE Bool
// NSNumber (created by numberWithDouble/numberWithFloat) -> FLOAT
// NSNULL -> nil/null
// If you want return comlicated type - just combine Objective C objects into any combination (hash of array of int for example etc.) - they will be converted to equivalent Ruby/JS structure
// If you want return error - make CMethodResultError and set it as result
// If you want return Class instance - make CRhoAPIClassInstance and use it as result or combine into any other types (array/hash)
@protocol IMethodResult <NSObject>

- (void) setResult:(NSObject*)value;

-(BOOL) hasCallback;

@end



// Simple IMethodResult implementation - use it for receive result from any API method
@interface CMethodResult_SimpleHolder : NSObject <IMethodResult> {
    NSObject* resultObject;
}

-(id) init;

+(CMethodResult_SimpleHolder*) makeEmptyHolder;

- (void) setResult:(NSObject*)value;
-(BOOL) hasCallback;

- (NSObject*) getResult;

@end



// Error result object - make it and set as result if you want send error instead of result
@interface CMethodResultError : NSObject {
    NSString* errorType;
    NSString* errorDescription;
}

-(id)init;

+ (CMethodResultError*) errorWithType:(NSString*)error_type description:(NSString*)description;

-(NSString*)getErrorType;
-(NSString*)getErrorDescription;

-(void)dealloc;

@end


// Object presenting of Ruby/JS Class instance - make it if you want return class instance
@interface CRhoAPIClassInstance : NSObject {
    NSString* mClassName;
    NSString* mInstanceID;
}

- (id) init:(NSString*)clasRubyFullName iID:(NSString*)iID;

+(CRhoAPIClassInstance*) rubyClassByName:(NSString*)className instanceID:(NSString*)instanceID;

-(NSString*)getClassName;

-(NSString*)getInstanceID;

@end
