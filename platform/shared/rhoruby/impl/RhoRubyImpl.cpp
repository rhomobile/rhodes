
#include <cstddef>
#include "RhoRubyImpl.h"
#include "ObjectImpl.h"

#include "NilImpl.h"
#include "MutableStringImpl.h"
#include "MutableBooleanImpl.h"
#include "MutableIntegerImpl.h"
#include "MutableFloatImpl.h"
#include "MutableArrayImpl.h"
#include "MutableHashImpl.h"


#include "../../ruby/ext/rho/rhoruby.h"



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
        if (object == NULL) {
            return NULL;
        }
        CObject* obj_impl = (CObject*)object;
        
        int param_count = 0;
        IArray* params_array = NULL;
        
        // check for array
        if (parameters != NULL) {
            if ((parameters->getBasicType() == BASIC_TYPES::Array) || (parameters->getBasicType() == BASIC_TYPES::MutableArray)) {
                params_array = (IArray*)parameters;
                param_count = params_array->getItemsCount();
            }
        }
        
        VALUE result = rho_ruby_get_NIL();
        VALUE params[10];
        
        //convert Objects into params
        int i;
        if (param_count > 10) {
            param_count = 10;
        }
        for (i = 0; i < param_count; i++) {
            params[i] = convertObject_to_VALUE(params_array->getItem(i));
        }
        
        if (param_count == 0) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 0, NULL);
        }
        else if (param_count == 1) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 1, params[0]);
        }
        else if (param_count == 2) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 2, params[0], params[1]);
        }
        else if (param_count == 3) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 3, params[0], params[1], params[2]);
        }
        else if (param_count == 4) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 4, params[0], params[1], params[2], params[3]);
        }
        else if (param_count == 5) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 5, params[0], params[1], params[2], params[3], params[4]);
        }
        else if (param_count == 6) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 6, params[0], params[1], params[2], params[3], params[4], params[5]);
        }
        else if (param_count == 7) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 7, params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
        }
        else if (param_count == 8) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 8, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
        }
        else if (param_count == 9) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 9, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8]);
        }
        else if (param_count == 10) {
            result = rb_funcall(obj_impl->getValue(), rb_intern(method_name), 10, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9]);
        }

        IObject* obj_result = convertVALUE_to_Object(result);
        
        
        return obj_result;
    }

    // this method recommended execute from ruby thread (from IRunnable command), but can be executed from other thread
    IObject* RhoRubyImpl::makeRubyClassObject(const char* full_class_name) {
        VALUE rb_klass = rb_path_to_class(rho_ruby_create_string(full_class_name));
        CObject* rho_obj = new CObject();
        rho_obj->setValue(rb_klass);
        rho_obj->setBasicType(BASIC_TYPES::Class);
        rho_obj->setClassName("Class");
        return rho_obj;
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

    IObject* RhoRubyImpl::convertVALUE_to_Object(VALUE ruby_value) {
        int i, size;
        switch(rb_type(ruby_value)) {
            case T_STRING:
            {
                CMutableString* str = new CMutableString(false);
                str->setValue(ruby_value);
                str->setUTF8(RSTRING_PTR(ruby_value));
                return str;
            }
                break;
            case T_FLOAT:
            {
                CMutableFloat* o = new CMutableFloat(false);
                o->setValue(ruby_value);
                o->setDouble(RFLOAT_VALUE(ruby_value));
                return o;
            }
                break;
            case T_FIXNUM:
            case T_BIGNUM:
            {
                CMutableInteger* o = new CMutableInteger(false);
                o->setValue(ruby_value);
                o->setLong((long int)(NUM2LL(ruby_value)));
                return o;
            }
                break;
            case T_TRUE:
            {
                CMutableBoolean* o = new CMutableBoolean(false);
                o->setValue(ruby_value);
                o->setBool(true);
                return o;
            }
                break;
            case T_FALSE:
            {
                CMutableBoolean* o = new CMutableBoolean(false);
                o->setValue(ruby_value);
                o->setBool(false);
                return o;
            }
                break;
            case T_ARRAY:
            {
                CMutableArray* ar = new CMutableArray(false);
                size = (int)RARRAY_LEN(ruby_value);
                ar->reserve(size);
                for (i = 0; i < size; ++i) {
                    VALUE item = rb_ary_entry(ruby_value, i);
                    ar->addItem(convertVALUE_to_Object(item));
                }
                return ar;
            }
                break;
            case T_HASH:
            {
                VALUE keys = rb_funcall(ruby_value, rb_intern("keys"), 0, NULL);
                size = (int)RARRAY_LEN(keys);
                CMutableHash* hash = new CMutableHash(false);
                for (i = 0; i < size; ++i) {
                    VALUE key = rb_ary_entry(keys, i);
                    VALUE value = rb_hash_aref(ruby_value, key);
                    if (rb_type(key) != T_STRING) {
                        key = rb_funcall(key, rb_intern("to_s"), 0, NULL);
                    }
                    hash->addItem(RSTRING_PTR(key), convertVALUE_to_Object(value));
                }
                return hash;
            }
                break;
            default:
            {
                if (rho_ruby_is_NIL(ruby_value)) {
                    return new CNil();
                }
                CObject* str = new CObject();
                VALUE v_class = rb_funcall(ruby_value, rb_intern("class"), 0, NULL);
                VALUE v_class_name = rb_funcall(v_class, rb_intern("name"), 0, NULL);
                str->setClassName(RSTRING_PTR(v_class_name));
                str->setBasicType(BASIC_TYPES::Object);
                str->setValue(ruby_value);
                return str;
            }
        }
        return NULL;
    }
    
    VALUE RhoRubyImpl::convertObject_to_VALUE(IObject* obj) {
        return rho_ruby_get_NIL();
    }
    
    
}
}
