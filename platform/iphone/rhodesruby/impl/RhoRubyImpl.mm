
#import "RhoRubyImpl.h"
#import "../api/RhoRubySingletone.h"
#import "RhoRubyFabrique.h"

//#include "../../../shared/rhoruby/api/RhoRuby.h"

#import "RhoRubyNilImpl.h"
#import "RhoRubyObjectImpl.h"
#import "RhoRubyMutableArrayImpl.h"
#import "RhoRubyMutableBooleanImpl.h"
#import "RhoRubyMutableFloatImpl.h"
#import "RhoRubyMutableHashImpl.h"
#import "RhoRubyMutableIntegerImpl.h"
#import "RhoRubyMutableStringImpl.h"
#import "RhoRubyNilImpl.h"


@implementation RhoRubyFabrique

+(id<IRhoRuby>) createRhoRuby {
    return [[RhoRubyImpl alloc] init];
}

@end



class CRhoRubyNativeCallbackHolder : public  rho::ruby::IRubyNativeCallback {
public:
    
    CRhoRubyNativeCallbackHolder(id<IRhoRubyNativeCallback> callback) {
        mCallback = [callback retain];
    }
    
    virtual ~CRhoRubyNativeCallbackHolder() {}
    
    virtual void onRubyNative(rho::ruby::IObject* param) {
        RhoRubyImpl* rr = (RhoRubyImpl*)[RhoRubySingletone getRhoRuby];
        id<IRhoRubyObject> r_object = [rr makeRhoRubyObjectFromCpp:param];
        [mCallback onRubyNativeWithParam:r_object];
    }
    
    
private:
    id<IRhoRubyNativeCallback> mCallback;
    
};


class CRhoRubyRunnableHolder : public  rho::ruby::IRunnable {
public:
    
    CRhoRubyRunnableHolder(id<IRhoRubyRunnable> command) {
        mCommand = [command retain];
    }
    
    virtual ~CRhoRubyRunnableHolder() {}
    
    virtual void run() {
        [mCommand rhoRubyRunnableRun];
        [mCommand release];
        delete this;
    }

private:
    id<IRhoRubyRunnable> mCommand;
    
};


class CRhoRubyLocalServerRequestCallbackHolder : public  rho::ruby::IRubyLocalServerRequestCallback {
public:
    
    CRhoRubyLocalServerRequestCallbackHolder(id<IRubyLocalServerRequestCallback> callback) {
        mCallback = [callback retain];
    }
    
    virtual ~CRhoRubyLocalServerRequestCallbackHolder() {}
    
    virtual void onLocalServerResponce(rho::ruby::IString* responce) {
        NSString* resp = nil;
        if (responce != NULL) {
            resp = [NSString stringWithUTF8String:responce->getUTF8()];
        }
        [mCallback onLocalServerResponce:resp];
        [mCallback release];
        //delete this;
    }
    
    
private:
    id<IRubyLocalServerRequestCallback> mCallback;
    
};



@implementation RhoRubyImpl

// call command in ruby thread
-(void) executeInRubyThread:(id<IRhoRubyRunnable>)command {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->executeInRubyThread(new CRhoRubyRunnableHolder(command));
}

-(void) executeGetRequestToRubyServer:(NSString*)url callback:(id<IRubyLocalServerRequestCallback>)callback {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->executeGetRequestToRubyServer([url UTF8String], new CRhoRubyLocalServerRequestCallbackHolder(callback));
}

// execute ruby code in current thread. parameters can be simple object (string, integer etc. - in this case one parameters will be passed to method.)
// also parameters can be IArray - in this case list of parameters will be passed to method (parameters from array)
// this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread for very simple opertions without modifying ruby objects
-(id<IRhoRubyObject>) executeRubyObjectMethod:(id<IRhoRubyObject>)rho_object method_name:(NSString*)method_name parameters:(id<IRhoRubyObject>)paramaters {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    RhoRubyObjectImpl* rr_obj = (RhoRubyObjectImpl*)rho_object;
    rho::ruby::IObject* cpp_param = NULL;
    if (paramaters != nil) {
        RhoRubyObjectImpl* rr_param = (RhoRubyObjectImpl*)paramaters;
        cpp_param = [rr_param getCppObject];
    }
    rho::ruby::IObject* cpp_object = rr->executeRubyObjectMethod([rr_obj getCppObject], [method_name UTF8String], cpp_param);
    id<IRhoRubyObject> r_object = [self makeRhoRubyObjectFromCpp:cpp_object];
    cpp_object->release();
    return r_object;
}

// this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread
// full class name has :: delimiter
-(id<IRhoRubyObject>) makeRubyClassObject:(NSString*)full_class_name {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IObject* cpp_object = rr->makeRubyClassObject([full_class_name UTF8String]);
    id<IRhoRubyObject> r_object = [self makeRhoRubyObjectFromCpp:cpp_object];
    cpp_object->release();
    return r_object;
}

// can be execute from any thread - for construct parameters for execute ruby code
// developer can make only mutable objects and Nil
-(id<IRhoRubyObject>) makeBaseTypeObject:(RHO_RUBY_BASIC_TYPES)basic_type {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IObject* cpp_object = rr->makeBaseTypeObject([self convertBasicTypeToCpp:basic_type]);
    id<IRhoRubyObject> r_object = [self makeRhoRubyObjectFromCpp:cpp_object];
    cpp_object->release();
    return r_object;
}


// register callback for execute from Ruby side via - it is needed for direct call native code foem ruby code
// Rho::Ruby.callNativeCallback(callback_id, param)
-(void) addRubyNativeCallback:(id<IRhoRubyNativeCallback>)native_callback callback_id:(NSString*)callback_id {
    CRhoRubyNativeCallbackHolder* cc = new CRhoRubyNativeCallbackHolder(native_callback);
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->addRubyNativeCallback([callback_id UTF8String], cc);
    
}

-(void) removeRubyNativeCallbackWithID:(NSString*)callback_id {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->removeRubyNativeCallback([callback_id UTF8String]);
}


-(id<IRhoRubyObject>) convertJSON_to_Objects:(NSString*)json {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IObject* cpp_obj = rr->convertJSON_to_Objects([json UTF8String]);
    id<IRhoRubyObject> r_obj = [self makeRhoRubyObjectFromCpp:cpp_obj];
    cpp_obj->release();
    return r_obj;
}

-(NSString*) convertObject_to_JSON:(id<IRhoRubyObject>)obj {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IObject* cpp_obj = NULL;
    if (obj != nil) {
        RhoRubyObjectImpl* rr_obj = (RhoRubyObjectImpl*)obj;
        cpp_obj = [rr_obj getCppObject];
    }
    rho::ruby::IString* res = rr->convertObject_to_JSON(cpp_obj);
    return [NSString stringWithUTF8String:res->getUTF8()];
}



-(void) loadRubyFile:(NSString*)ruby_file_path {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->loadRubyFile([ruby_file_path UTF8String]);
}

-(void) loadModel:(NSString*)ruby_file_path {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rr->loadModel([ruby_file_path UTF8String]);
}

-(id<IRhoRubyObject>) executeRubyMethod:(NSString*)full_class_name  method_name:(NSString*)method_name parameters:(id<IRhoRubyObject>)paramaters {
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IObject* cpp_param = NULL;
    if (paramaters != nil) {
        RhoRubyObjectImpl* rr_param = (RhoRubyObjectImpl*)paramaters;
        cpp_param = [rr_param getCppObject];
    }
    rho::ruby::IObject* cpp_object = rr->executeRubyMethod([full_class_name UTF8String], [method_name UTF8String], cpp_param);
    id<IRhoRubyObject> r_object = [self makeRhoRubyObjectFromCpp:cpp_object];
    cpp_object->release();
    return r_object;
}

-(NSString*) executeRubyMethodWithJSON:(NSString*)full_class_name  method_name:(NSString*)method_name paramaters_in_json:(NSString*)paramaters_in_json {
    /*rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    rho::ruby::IObject* cpp_param = NULL;
    if (paramaters_in_json != nil) {
        cpp_param = rr->convertJSON_to_Objects([paramaters_in_json UTF8String]);
    }
    rho::ruby::IObject* cpp_object = rr->executeRubyMethod([full_class_name UTF8String], [method_name UTF8String], cpp_param);
    rho::ruby::IString* res_str= NULL;
    if (cpp_object != NULL) {
        res_str = rr->convertObject_to_JSON(cpp_object);
    }
    NSString* res_ns_str = nil;
    if (res_str != NULL) {
        res_ns_str = [NSString stringWithUTF8String:res_str->getUTF8()];
    }
    if (cpp_param != NULL) {
        cpp_param->release();
    }
    if (cpp_object != NULL) {
        cpp_object->release();
    }
    if (res_str != NULL) {
        res_str->release();
    }
    return res_ns_str;
     */
    rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
    const char* params_in_j = NULL;
    if (paramaters_in_json != nil) {
        params_in_j =[paramaters_in_json UTF8String];
    }
    rho::ruby::IString* res_str = rr->executeRubyMethodWithJSON([full_class_name UTF8String], [method_name UTF8String], params_in_j);
    
    NSString* str_ns_result = nil;
    if (res_str != NULL) {
        const char* res_str_cstr = res_str->getUTF8();
        if (res_str_cstr != NULL) {
            str_ns_result = [NSString stringWithUTF8String:res_str_cstr];
        }
        res_str->release();
    }
    return str_ns_result;
}


-(id<IRhoRubyObject>) makeRhoRubyObjectFromCpp:(rho::ruby::IObject*)cpp_object {
    if (cpp_object == NULL) {
        return nil;
    }
    rho::ruby::BASIC_TYPES cpp_type = cpp_object->getBasicType();
    switch (cpp_type) {
        case rho::ruby::BASIC_TYPES::RubyNil :
            return [[RhoRubyNilImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::Array :
        case rho::ruby::BASIC_TYPES::MutableArray :
            return [[RhoRubyMutableArrayImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::Boolean :
        case rho::ruby::BASIC_TYPES::MutableBoolean :
            return [[RhoRubyMutableBooleanImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::Class :
            return [[RhoRubyObjectImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::Float :
        case rho::ruby::BASIC_TYPES::MutableFloat :
            return [[RhoRubyMutableFloatImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::Hash :
        case rho::ruby::BASIC_TYPES::MutableHash :
            return [[RhoRubyMutableHashImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::Integer :
        case rho::ruby::BASIC_TYPES::MutableInteger :
            return [[RhoRubyMutableIntegerImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::String :
        case rho::ruby::BASIC_TYPES::MutableString :
            return [[RhoRubyMutableStringImpl alloc] initWithCppObject:cpp_object];
            break;
        case rho::ruby::BASIC_TYPES::None :
            return nil;
            break;
        case rho::ruby::BASIC_TYPES::Object :
            return [[RhoRubyObjectImpl alloc] initWithCppObject:cpp_object];
            break;
            
        default:
            break;
    }
    return nil;
}

-(RHO_RUBY_BASIC_TYPES) convertBasicTypeFromCpp:(rho::ruby::BASIC_TYPES)cppBasicType {
    switch (cppBasicType) {
        case rho::ruby::BASIC_TYPES::RubyNil :
            return kRhoRubyNil;
            break;
        case rho::ruby::BASIC_TYPES::Array :
            return kRhoRubyArray;
            break;
        case rho::ruby::BASIC_TYPES::Boolean :
            return kRhoRubyBoolean;
            break;
        case rho::ruby::BASIC_TYPES::Class :
            return kRhoRubyClass;
            break;
        case rho::ruby::BASIC_TYPES::Float :
            return kRhoRubyFloat;
            break;
        case rho::ruby::BASIC_TYPES::Hash :
            return kRhoRubyHash;
            break;
        case rho::ruby::BASIC_TYPES::Integer :
            return kRhoRubyInteger;
            break;
        case rho::ruby::BASIC_TYPES::MutableArray :
            return kRhoRubyMutableArray;
            break;
        case rho::ruby::BASIC_TYPES::MutableBoolean :
            return kRhoRubyMutableBoolean;
            break;
        case rho::ruby::BASIC_TYPES::MutableFloat :
            return kRhoRubyMutableFloat;
            break;
        case rho::ruby::BASIC_TYPES::MutableHash :
            return kRhoRubyMutableHash;
            break;
        case rho::ruby::BASIC_TYPES::MutableInteger :
            return kRhoRubyMutableInteger;
            break;
        case rho::ruby::BASIC_TYPES::MutableString :
            return kRhoRubyMutableString;
            break;
        case rho::ruby::BASIC_TYPES::None :
            return kRhoRubyNone;
            break;
        case rho::ruby::BASIC_TYPES::Object :
            return kRhoRubyObject;
            break;
        case rho::ruby::BASIC_TYPES::String :
            return kRhoRubyString;
            break;
            
        default:
            break;
    }
    
    return kRhoRubyNone;
}

-(rho::ruby::BASIC_TYPES) convertBasicTypeToCpp:(RHO_RUBY_BASIC_TYPES)basicType {
    switch (basicType) {
        case kRhoRubyNil :
            return rho::ruby::BASIC_TYPES::RubyNil;
            break;
        case kRhoRubyArray :
            return rho::ruby::BASIC_TYPES::Array;
            break;
        case kRhoRubyBoolean :
            return rho::ruby::BASIC_TYPES::Boolean;
            break;
        case kRhoRubyClass :
            return rho::ruby::BASIC_TYPES::Class;
            break;
        case kRhoRubyFloat :
            return rho::ruby::BASIC_TYPES::Float;
            break;
        case kRhoRubyHash :
            return rho::ruby::BASIC_TYPES::Hash;
            break;
        case kRhoRubyInteger :
            return rho::ruby::BASIC_TYPES::Integer;
            break;
        case kRhoRubyMutableArray :
            return rho::ruby::BASIC_TYPES::MutableArray;
            break;
        case kRhoRubyMutableBoolean :
            return rho::ruby::BASIC_TYPES::MutableBoolean;
            break;
        case kRhoRubyMutableFloat :
            return rho::ruby::BASIC_TYPES::MutableFloat;
            break;
        case kRhoRubyMutableHash :
            return rho::ruby::BASIC_TYPES::MutableHash;
            break;
        case kRhoRubyMutableInteger :
            return rho::ruby::BASIC_TYPES::MutableInteger;
            break;
        case kRhoRubyMutableString :
            return rho::ruby::BASIC_TYPES::MutableString;
            break;
        case kRhoRubyNone :
            return rho::ruby::BASIC_TYPES::None;
            break;
        case kRhoRubyObject :
            return rho::ruby::BASIC_TYPES::Object;
            break;
        case kRhoRubyString :
            return rho::ruby::BASIC_TYPES::String;
            break;
            
        default:
            break;
    }
    
    return rho::ruby::BASIC_TYPES::None;
}


@end



