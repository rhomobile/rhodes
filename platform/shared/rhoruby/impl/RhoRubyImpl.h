#pragma once


#include "../api/IRhoRuby.h"
#include "../../ruby/include/ruby.h"
#include "common/RhoMutexLock.h"
#include "json/JSONIterator.h"

#include <string>
#include <map>
#include <vector>

namespace rho {
namespace ruby {


class RhoRubyImpl : public IRhoRuby {

public:
    virtual ~RhoRubyImpl();
    RhoRubyImpl(bool _lateInit = false);

    virtual void init() override;

    // get local Ruby server URL
    virtual IString* getRubyServerURL();
    
    // call command in ruby thread
    virtual void executeInRubyThread(IRunnable* command);

    // call ruby server url (get request) and receive responce (body if exist or NULL if error etc.)
    virtual void executeGetRequestToRubyServer(const char* url, IRubyLocalServerRequestCallback* callback);

    // execute ruby code in current thread. parameters can be simple object (string, integer etc. - in this case one parameters will be passed to method.)
    // also parameters can be IArray - in this case list of parameters will be passed to method (parameters from array)
    // this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread for very simple opertions without modifying ruby objects
    virtual IObject* executeRubyObjectMethod(IObject* object, const char* method_name, IObject* parameters);

    // this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread
    virtual IObject* makeRubyClassObject(const char* full_class_name);

    // can be execute from any thread - for construct parameters for execute ruby code
    virtual IObject* makeBaseTypeObject(BASIC_TYPES type);


    // register callback for execute from Ruby side via - it is needed for direct call native code foem ruby code
    // Rho::Ruby.callNativeCallback(callback_id, param)
    virtual void addRubyNativeCallback(const char* callback_id, IRubyNativeCallback* callback);
    virtual void removeRubyNativeCallback(const char* callback_id);

    // util methods (used for parse responce from server etc.)
    virtual IObject* convertJSON_to_Objects(const char* json);
    virtual IString* convertObject_to_JSON(IObject* obj);
    
    virtual void loadRubyFile(const char* ruby_file_path);
    virtual void loadModel(const char* ruby_file_path);
    virtual IObject* executeRubyMethod(const char* full_class_name, const char* method_name, IObject* parameters);
    virtual IString* executeRubyMethodWithJSON(const char* full_class_name, const char* method_name, const char* parameters_in_json);
    

    IObject* convertVALUE_to_Object(VALUE value);
    VALUE convertObject_to_VALUE(IObject* obj);
    
    VALUE rb_Rho_Ruby_callNativeCallback(int argc, VALUE *argv);
    void rholib_local_server_callback();

    void initRhoRuby_fromRubyThread();

private:
    IObject* convertJSON_to_Object(json::CJSONEntry* json_entry);
    
    //hashtable for native callback_id => callback pointer
    std::map<std::string, IRubyNativeCallback*> mCallbacks;
    
    
    std::vector<IRunnable*> mRubyCommands;
    common::CMutex m_mxSyncMutex;
    bool lateInit = false;

};

}
}

// function called from Rhodes code - after ruby and local server was created and initialized
extern "C" void InitRhoRuby_fromRubyThread();
