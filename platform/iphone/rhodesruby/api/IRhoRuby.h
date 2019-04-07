#import <Foundation/Foundation.h>
#import "IRhoRubyObject.h"
#import "IRhoRubyRunnable.h"
#import "IRhoRubyNativeCallback.h"


@protocol IRhoRuby <NSObject>

// call command in ruby thread
-(void) executeInRubyThread:(id<IRhoRubyRunnable>)command;

// call ruby server url (net request) and receive responce in callabck
//virtual void executeRubyServerURL(const char* url, const char* body, IRubyServerCallback* callback) = 0;

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

@end









/*#pragma once


#include "IObject.h"
#include "IArray.h"
#include "IString.h"
#include "IRunnable.h"

namespace rho {
namespace ruby {


class IRubyServerResponce {

public:
    virtual ~IRubyServerResponce() {}

    virtual IString* getResponceBody() = 0;

};




class IRubyServerCallback {

public:
    virtual ~IRubyServerCallback() {}

    virtual void onRubyServerResponce(IRubyServerResponce* responce) = 0;

};


class IRubyNativeCallback {
public:
    virtual ~IRubyNativeCallback() {}

    virtual void onRubyNative(IObject* param) = 0;

};


class IRhoRuby {

public:
    virtual ~IRhoRuby() {}

 






    // util methods (used for parse responce from server etc.)
    virtual IObject* convertJSON_to_Objects(const char* json) = 0;
    virtual IString* convertObject_to_JSON(IObject* obj) = 0;

};

}
}
*/
