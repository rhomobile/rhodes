
#include <cstddef>
#include "RhoRubyImpl.h"

namespace rho {
namespace ruby {



    RhoRubyImpl::~RhoRubyImpl() {

    }

    RhoRubyImpl::RhoRubyImpl() {

    }

    // call command in ruby thread
    void RhoRubyImpl::executeInRubyThread(IRunnable* command) {

    }

    // call ruby server url (net request) and receive responce in callabck
    void RhoRubyImpl::executeRubyServerURL(const char* url, IRubyServerCallback* callback) {

    }

    // execute ruby code in current thread. parameters can be simple object (string, integer etc. - in this case one parameters will be passed to method.)
    // also parameters can be IArray - in this case list of parameters will be passed to method (parameters from array)
    // this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread for very simple opertions without modifying ruby objects
    IObject* RhoRubyImpl::executeRubyObjectMethod(IObject* object, const char* method_name, IObject* parameters) {
        return NULL;
    }

    // this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread
    IObject* RhoRubyImpl::getRubyClassObject(const char* full_class_name) {
        return NULL;
    }

    // can be execute from any thread - for construct parameters for execute ruby code
    IObject* RhoRubyImpl::makeBaseTypeObject(BASIC_TYPES type) {
        return NULL;
    }


    // register callback for execute from Ruby side via - it is needed for direct call native code foem ruby code
    // Rho::Ruby.callNativeCallback(callback_id, param)
    void RhoRubyImpl::addRubyNativeCallback(const char* callback_id, IRubyNativeCallback* callback) {

    }

    void RhoRubyImpl::removeRubyNativeCallback(const char* callback_id) {

    }

    // util methods (used for parse responce from server etc.)
    IObject* RhoRubyImpl::convertJSON_to_Objects(const char* json) {
        return NULL;
    }

    IString* RhoRubyImpl::convertObject_to_JSON(IObject* obj) {
        return NULL;
    }

}
}
