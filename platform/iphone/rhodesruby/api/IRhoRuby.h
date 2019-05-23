#import <Foundation/Foundation.h>
#import "IRhoRubyObject.h"
#import "IRhoRubyRunnable.h"
#import "IRhoRubyNativeCallback.h"


@protocol IRhoRubyNativeCallback <NSObject>

-(void) onRubyNativeWithParam:(id<IRhoRubyObject>)param;

@end


@protocol IRubyLocalServerRequestCallback <NSObject>

-(void) onLocalServerResponce:(NSString*)responce;

@end




@protocol IRhoRuby <NSObject>

// get local Ruby server URL
-(NSString*) getRubyServerURL;

// call command in ruby thread
-(void) executeInRubyThread:(id<IRhoRubyRunnable>)command;

// call ruby server url (get request) and receive responce (body if exist or nil if error etc.)
-(void) executeGetRequestToRubyServer:(NSString*)url callback:(id<IRubyLocalServerRequestCallback>)callback;


// execute ruby code in current thread. parameters can be simple object (string, integer etc. - in this case one parameters will be passed to method.)
// also parameters can be IArray - in this case list of parameters will be passed to method (parameters from array)
// this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread for very simple opertions without modifying ruby objects
-(id<IRhoRubyObject>) executeRubyObjectMethod:(id<IRhoRubyObject>)rho_object method_name:(NSString*)method_name parameters:(id<IRhoRubyObject>)paramaters;

// this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread
// full class name has :: delimiter
-(id<IRhoRubyObject>) makeRubyClassObject:(NSString*)full_class_name;

// can be execute from any thread - for construct parameters for execute ruby code
// developer can make only mutable objects and Nil
-(id<IRhoRubyObject>) makeBaseTypeObject:(RHO_RUBY_BASIC_TYPES)basic_type;


// register callback for execute from Ruby side via - it is needed for direct call native code foem ruby code
// Rho::Ruby.callNativeCallback(callback_id, param)
-(void) addRubyNativeCallback:(id<IRhoRubyNativeCallback>)native_callback callback_id:(NSString*)callback_id;
-(void) removeRubyNativeCallbackWithID:(NSString*)callback_id;


-(id<IRhoRubyObject>) convertJSON_to_Objects:(NSString*)json;
-(NSString*) convertObject_to_JSON:(id<IRhoRubyObject>)obj;

-(void) loadRubyFile:(NSString*)ruby_file_path;
-(void) loadModel:(NSString*)ruby_file_path;
-(id<IRhoRubyObject>) executeRubyMethod:(NSString*)full_class_name  method_name:(NSString*)method_name parameters:(id<IRhoRubyObject>)paramaters;

//return JSON
-(NSString*) executeRubyMethodWithJSON:(NSString*)full_class_name  method_name:(NSString*)method_name paramaters_in_json:(NSString*)paramaters_in_json;


@end






