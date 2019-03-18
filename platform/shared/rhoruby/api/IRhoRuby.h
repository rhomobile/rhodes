#pragma once


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

    // call command in ruby thread
    virtual void executeInRubyThread(IRunnable* command) = 0;

    // call ruby server url (net request) and receive responce in callabck
    virtual void executeRubyServerURL(const char* url, const char* body, IRubyServerCallback* callback) = 0;

    // execute ruby code in current thread. parameters can be simple object (string, integer etc. - in this case one parameters will be passed to method.)
    // also parameters can be IArray - in this case list of parameters will be passed to method (parameters from array)
    // this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread for very simple opertions without modifying ruby objects
    virtual IObject* executeRubyObjectMethod(IObject* object, const char* method_name, IObject* parameters) = 0;

    // this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread
    // full class name has :: delimiter
    virtual IObject* makeRubyClassObject(const char* full_class_name) = 0;

    // can be execute from any thread - for construct parameters for execute ruby code
    // developer can make only mutable objects and Nil
    virtual IObject* makeBaseTypeObject(BASIC_TYPES type) = 0;


    // register callback for execute from Ruby side via - it is needed for direct call native code foem ruby code
    // Rho::Ruby.callNativeCallback(callback_id, param)
    virtual void addRubyNativeCallback(const char* callback_id, IRubyNativeCallback* callback) = 0;
    virtual void removeRubyNativeCallback(const char* callback_id) = 0;

    // util methods (used for parse responce from server etc.)
    virtual IObject* convertJSON_to_Objects(const char* json) = 0;
    virtual IString* convertObject_to_JSON(IObject* obj) = 0;

};

}
}
