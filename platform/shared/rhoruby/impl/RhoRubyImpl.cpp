
#include <cstddef>



#include "ruby.h"




#include "RhoRubyImpl.h"
#include "ObjectImpl.h"
#include "../api/RhoRubySingletone.h"


#include "NilImpl.h"
#include "MutableStringImpl.h"
#include "MutableBooleanImpl.h"
#include "MutableIntegerImpl.h"
#include "MutableFloatImpl.h"
#include "MutableArrayImpl.h"
#include "MutableHashImpl.h"

#include "../../ruby/ext/rho/rhoruby.h"

#include "common/RhodesApp.h"


#include "../../../../lib/commonAPI/coreapi/ext/shared/generated/cpp/NetworkBase.h"


#define RHOLIB_LOCAL_SERVER_URL "/system/rholib_callback"


static VALUE rb_api_mParent;
static VALUE rb_api_mRubyNative;


static void _free_class_object(void *p)
{
    ruby_xfree(p);
}

static VALUE _allocate_class_object(VALUE klass)
{
    VALUE valObj = 0;
    char ** ppString = NULL;
    void* pData = ALLOC(void*);
    memset( pData, 0, sizeof(pData) );
    
    valObj = Data_Wrap_Struct(klass, 0, _free_class_object, pData);
    
    Data_Get_Struct(valObj, char *, ppString);
    *ppString = (char*)xmalloc(20);
    sprintf(*ppString, "%X", valObj);
    
    return valObj;
}

extern "C" VALUE c_rb_Rho_Ruby_callNativeCallback(int argc, VALUE *argv) {
    rho::ruby::RhoRubyImpl* rr = (rho::ruby::RhoRubyImpl*)(rho::ruby::RhoRubySingletone::getRhoRuby());
    return rr->rb_Rho_Ruby_callNativeCallback(argc, argv);
}

static void rholib_local_server_callback_func(void *arg, rho::String const &strQuery) {
    rho::ruby::RhoRubyImpl* rr = (rho::ruby::RhoRubyImpl*)(rho::ruby::RhoRubySingletone::getRhoRuby());
    rr->rholib_local_server_callback();
    rho_http_sendresponse(arg, "");
}








typedef VALUE (*ruby_method_func_type)(...);

namespace rho {
namespace ruby {

    
    class CRhoLocalServerRequestTask
    {
        rho::String m_strUrl;
        IRubyLocalServerRequestCallback* m_Callback;
    public:
        CRhoLocalServerRequestTask(const rho::String& strUrl, IRubyLocalServerRequestCallback* callback)
        : m_strUrl(strUrl), m_Callback(callback)
        {}
        
        void run(common::CRhoThread &)
        {
            rho::net::CNetRequestWrapper wrap_req = rho::net::CNetRequestWrapper( rho_get_RhoClassFactory()->createNetRequestImpl(), NULL );
            NetResponse resp = wrap_req.pullData( m_strUrl.c_str(), NULL );
            IMutableString* str_resp = NULL;
            if (resp.isOK()) {
                const char* body = resp.getCharData();
                if (body != NULL) {
                    str_resp = (IMutableString*)rho::ruby::RhoRubySingletone::getRhoRuby()->makeBaseTypeObject(BASIC_TYPES::MutableString);
                    str_resp->setUTF8(body);
                }
            }
            if (m_Callback != NULL) {
                m_Callback->onLocalServerResponce(str_resp);
            }
        }
    };

    


    RhoRubyImpl::~RhoRubyImpl() {

    }

    RhoRubyImpl::RhoRubyImpl() {
        // register Rho::Ruby.callNativeCallback(callback_id, param)

        rb_api_mParent = rb_define_module("Rho");
        
        rb_api_mRubyNative = rb_define_class_under(rb_api_mParent, "Ruby", rb_cObject);
        
        rb_define_alloc_func(rb_api_mRubyNative, _allocate_class_object);
        
        rb_define_singleton_method(rb_api_mRubyNative, "callNativeCallback", (ruby_method_func_type)c_rb_Rho_Ruby_callNativeCallback, -1);
     
        
        rho::common::CRhodesApp::getInstance()->registerLocalServerUrl(RHOLIB_LOCAL_SERVER_URL, rholib_local_server_callback_func);
        
        
    }
    
    void RhoRubyImpl::rholib_local_server_callback() {
        
        int i;
        std::vector<IRunnable*> copy_of_commands_list;

        {
            common::CMutexLock lock(m_mxSyncMutex);
            for (i = 0; i<mRubyCommands.size(); i++) {
                copy_of_commands_list.push_back(mRubyCommands[i]);
            }
            mRubyCommands.clear();
        }
        // call all copied commands

        for (i = 0; i < copy_of_commands_list.size(); i++) {
            if (copy_of_commands_list[i] != NULL) {
                copy_of_commands_list[i]->run();
            }
        }
        
    }

    // get local Ruby server URL
    IString* RhoRubyImpl::getRubyServerURL() {
        IMutableString* ms = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
        ms->setUTF8(RHODESAPP().getRubyHomeURL().c_str());
        return ms;
    }

    // call command in ruby thread
    void RhoRubyImpl::executeInRubyThread(IRunnable* command) {
        {
            common::CMutexLock lock(m_mxSyncMutex);
            mRubyCommands.push_back(command);
        }
        //rho::common::CRhodesApp::getInstance()->callCallbackWithData(RHOLIB_LOCAL_SERVER_URL, "", "", false);
        rho::common::CRhodesApp::getInstance()->runCallbackInThread(RHOLIB_LOCAL_SERVER_URL, "");
    }

    void RhoRubyImpl::executeGetRequestToRubyServer(const char* url, IRubyLocalServerRequestCallback* callback) {
        rho::String s_url = RHODESAPP().canonicalizeRhoUrl(url);
        rho::common::rho_rhodesapp_call_in_thread(new CRhoLocalServerRequestTask(s_url, callback ) );
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

        
        VALUE result = rho_ruby_get_NIL();
        VALUE params[10];
        int i;

        // check for array
        if (parameters != NULL) {
            if ((parameters->getBasicType() == BASIC_TYPES::Array) || (parameters->getBasicType() == BASIC_TYPES::MutableArray)) {
                params_array = (IArray*)parameters;
                param_count = params_array->getItemsCount();
                if (param_count > 10) {
                    param_count = 10;
                }
                for (i = 0; i < param_count; i++) {
                    params[i] = convertObject_to_VALUE(params_array->getItem(i));
                }
            }
            else {
                param_count = 1;
                params[0] = convertObject_to_VALUE(parameters);
            }
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
        switch(type) {
            case BASIC_TYPES::Array:
            case BASIC_TYPES::MutableArray:
            {
                return new CMutableArray(true);
            }
                break;
            case BASIC_TYPES::Boolean:
            case BASIC_TYPES::MutableBoolean:
            {
                return new CMutableBoolean(true);
            }
                break;
            case BASIC_TYPES::Float:
            case BASIC_TYPES::MutableFloat:
            {
                return new CMutableFloat(true);
            }
                break;
            case BASIC_TYPES::Hash:
            case BASIC_TYPES::MutableHash:
            {
                return new CMutableHash(true);
            }
                break;
            case BASIC_TYPES::Integer:
            case BASIC_TYPES::MutableInteger:
            {
                return new CMutableInteger(true);
            }
                break;
            case BASIC_TYPES::String:
            case BASIC_TYPES::MutableString:
            {
                return new CMutableString(true);
            }
                break;
            case BASIC_TYPES::RubyNil:
            {
                return new CNil();
            }
                break;
            default:
            {
                return NULL;
            }
        }
        return NULL;
    }


    // register callback for execute from Ruby side via - it is needed for direct call native code foem ruby code
    // Rho::Ruby.callNativeCallback(callback_id, param)
    void RhoRubyImpl::addRubyNativeCallback(const char* callback_id, IRubyNativeCallback* callback) {
        mCallbacks[callback_id] = callback;
    }

    void RhoRubyImpl::removeRubyNativeCallback(const char* callback_id) {
        mCallbacks.erase(callback_id);
    }



    IObject* RhoRubyImpl::convertJSON_to_Object(json::CJSONEntry* jsonEntry) {
        if (jsonEntry == NULL) {
            return NULL;
        }
        if (jsonEntry->isString()) {
            rho::String str = jsonEntry->getStringObject();
            IMutableString* ms = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
            ms->setUTF8(str.c_str());
            return ms;
        }
        if (jsonEntry->isInteger()) {
            IMutableInteger* ms = (IMutableInteger*)makeBaseTypeObject(BASIC_TYPES::MutableInteger);
            ms->setLong(jsonEntry->getInt());
            return ms;
        }
        if (jsonEntry->isFloat()) {
            IMutableFloat* ms = (IMutableFloat*)makeBaseTypeObject(BASIC_TYPES::MutableFloat);
            ms->setDouble(jsonEntry->getDouble());
            return ms;
        }
        if (jsonEntry->isBoolean()) {
            IMutableBoolean* ms = (IMutableBoolean*)makeBaseTypeObject(BASIC_TYPES::MutableBoolean);
            ms->setBool(jsonEntry->getBoolean());
            return ms;
        }
        if (jsonEntry->isArray()) {
            rho::json::CJSONArray js_array(*jsonEntry);
            int i;
            int count = js_array.getSize();
            IMutableArray* ms = (IMutableArray*)makeBaseTypeObject(BASIC_TYPES::MutableArray);
            for (i =0 ; i < count; i++) {
                rho::json::CJSONEntry js_entry = js_array[i];
                IObject* item = convertJSON_to_Object(&js_entry);
                if (item != NULL) {
                    ms->addItem(item);
                    item->release();
                }
                else {
                    RAWLOG_ERROR1("ERROR during convert JSON data 2 ! data = %s", js_entry.getString());
                }
            }
            return ms;
        }
        if (jsonEntry->isObject()) {
            rho::json::CJSONStructIterator js_iterator(*jsonEntry);
            IMutableHash* ms = (IMutableHash*)makeBaseTypeObject(BASIC_TYPES::MutableHash);
            while (!js_iterator.isEnd()) {
                rho::String key = js_iterator.getCurKey();
                rho::json::CJSONEntry js_entry = js_iterator.getCurValue();
                IObject* item = convertJSON_to_Object(&js_entry);
                if (item != NULL) {
                    ms->addItem(key.c_str(), item);
                    item->release();
                }
                else {
                    RAWLOG_ERROR1("ERROR during convert JSON data 2 ! data = %s", js_entry.getString());
                }
                js_iterator.next();
            }
            return ms;
        }
        return NULL;
    }

    // util methods (used for parse responce from server etc.)
    IObject* RhoRubyImpl::convertJSON_to_Objects(const char* json) {
        if (json == NULL) {
            return NULL;
        }
        if (strlen(json) <= 0) {
            return NULL;
        }
        json::CJSONEntry oEntry(json);
        return convertJSON_to_Object(&oEntry);
    }

    IString* RhoRubyImpl::convertObject_to_JSON(IObject* obj) {
        if (obj == NULL) {
            return NULL;
        }
        BASIC_TYPES o_type = obj->getBasicType();
        switch(o_type) {
            case BASIC_TYPES::Array:
            case BASIC_TYPES::MutableArray:
            {
                IArray* ar = (IArray*)obj;
                int count = ar->getItemsCount();
                int i;
                rho::String str;
                str = "[";
                for (i = 0; i < count; i++) {
                    IObject* r_obj = ar->getItem(i);
                    IString* r_str = convertObject_to_JSON(r_obj);
                    str.append(r_str->getUTF8());
                    if (i < (count-1)) {
                        str.append(",");
                    }
                    r_str->release();
                }
                str.append("]");
                IMutableString* o_str = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
                o_str->setUTF8(str.c_str());
                return o_str;
            }
                break;
            case BASIC_TYPES::Boolean:
            case BASIC_TYPES::MutableBoolean:
            {
                IBoolean* bo = (IBoolean*)obj;
                IMutableString* o_str = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
                if (bo->getBool()) {
                    o_str->setUTF8("true");
                }
                else {
                    o_str->setUTF8("false");
                }
                return o_str;
            }
                break;
            case BASIC_TYPES::Float:
            case BASIC_TYPES::MutableFloat:
            {
                IFloat* flo = (IFloat*)obj;
                char buf[100];
                sprintf(buf, "%lf", flo->getDouble());
                IMutableString* o_str = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
                o_str->setUTF8(buf);
                return o_str;
            }
                break;
            case BASIC_TYPES::Hash:
            case BASIC_TYPES::MutableHash:
            {
                IHash* ha = (IHash*)obj;
                IArray* keys = ha->getKeys();
                int i;
                int count = keys->getItemsCount();
                rho::String str;
                str = "{";
                for (i = 0; i < count; i++) {
                    IString* r_key = (IString*)keys->getItem(i);
                    IObject* r_value = ha->getItem(r_key->getUTF8());
                    IString* r_str = convertObject_to_JSON(r_value);
                    str.append(rho::json::CJSONEntry::quoteValue(r_key->getUTF8()));
                    str.append(":");
                    str.append(r_str->getUTF8());
                    if (i < (count-1)) {
                        str.append(",");
                    }
                    r_str->release();
                }
                str.append("}");
                IMutableString* o_str = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
                o_str->setUTF8(str.c_str());
                return o_str;
            }
                break;
            case BASIC_TYPES::Integer:
            case BASIC_TYPES::MutableInteger:
            {
                IInteger* flo = (IInteger*)obj;
                char buf[100];
                sprintf(buf, "%ld", flo->getLong());
                IMutableString* o_str = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
                o_str->setUTF8(buf);
                return o_str;
            }
                break;
            case BASIC_TYPES::String:
            case BASIC_TYPES::MutableString:
            {
                IString* str = (IString*)obj;
                IMutableString* o_str = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
                o_str->setUTF8(rho::json::CJSONEntry::quoteValue(str->getUTF8()).c_str());
                return o_str;
            }
                break;
            case BASIC_TYPES::RubyNil:
            {
                IMutableString* o_str = (IMutableString*)makeBaseTypeObject(BASIC_TYPES::MutableString);
                o_str->setUTF8("null");
                return o_str;
            }
                break;
            default:
            {
                return NULL;
            }
        }
        return NULL;
    }
    
    void RhoRubyImpl::loadRubyFile(const char* ruby_file_path) {
        rb_require(ruby_file_path);
    }
    
    void RhoRubyImpl::loadModel(const char* ruby_file_path) {
        VALUE rb_Kernel_klass = rb_path_to_class(rho_ruby_create_string("Kernel"));
        rb_funcall(rb_Kernel_klass, rb_intern("require_model"), 1, rho_ruby_create_string(ruby_file_path));
    }
    
    IObject* RhoRubyImpl::executeRubyMethod(const char* full_class_name, const char* method_name, IObject* parameters) {
        IObject* obj = makeRubyClassObject(full_class_name);
        IObject* result = executeRubyObjectMethod(obj, method_name, parameters);
        obj->release();
        return result;
    }
    
    IString* RhoRubyImpl::executeRubyMethodWithJSON(const char* full_class_name, const char* method_name, const char* parameters_in_json) {
        IObject* result = executeRubyMethod(full_class_name, method_name, convertJSON_to_Objects(parameters_in_json));
        IString* str = convertObject_to_JSON(result);
        result->release();
        return str;
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
                    IObject* o_item = convertVALUE_to_Object(item);
                    ar->addItem(o_item);
                    o_item->release();
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
                    IObject* o_item = convertVALUE_to_Object(value);
                    hash->addItem(RSTRING_PTR(key), o_item);
                    o_item->release();
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
        if (obj == NULL) {
            return rho_ruby_get_NIL();
        }
        VALUE v = rho_ruby_get_NIL();
        BASIC_TYPES o_type = obj->getBasicType();
        switch(o_type) {
        case BASIC_TYPES::Array:
        case BASIC_TYPES::MutableArray:
            {
                IArray* ar = (IArray*)obj;
                int count = ar->getItemsCount();
                int i;
                v = rho_ruby_create_array();
                for (i = 0; i < count; i++) {
                    IObject* r_obj = ar->getItem(i);
                    VALUE objValue = convertObject_to_VALUE(r_obj);
                    rho_ruby_add_to_array(v, objValue);
                }
                return v;
            }
            break;
        case BASIC_TYPES::Boolean:
        case BASIC_TYPES::MutableBoolean:
            {
                IBoolean* bo = (IBoolean*)obj;
                v = rho_ruby_create_boolean(bo->getBool());
                return v;
            }
            break;
        case BASIC_TYPES::Float:
        case BASIC_TYPES::MutableFloat:
            {
                IFloat* flo = (IFloat*)obj;
                v = rho_ruby_create_double(flo->getDouble());
                return v;
            }
            break;
        case BASIC_TYPES::Hash:
        case BASIC_TYPES::MutableHash:
            {
                v = rho_ruby_createHash();
                IHash* ha = (IHash*)obj;
                IArray* keys = ha->getKeys();
                int i;
                int count = keys->getItemsCount();
                for (i = 0 ; i < count; i++) {
                    IString* r_key = (IString*)keys->getItem(i);
                    IObject* r_value = ha->getItem(r_key->getUTF8());
                    VALUE vKey = convertObject_to_VALUE(r_key);
                    VALUE vItem = convertObject_to_VALUE(r_value);
                    rho_ruby_add_to_hash(v, vKey, vItem);
                }
                keys->release();
                return v;
            }
            break;
        case BASIC_TYPES::Integer:
        case BASIC_TYPES::MutableInteger:
            {
                IInteger* in = (IInteger*)obj;
                v = rho_ruby_create_integer(in->getLong());
                return v;
            }
            break;
        case BASIC_TYPES::String:
        case BASIC_TYPES::MutableString:
            {
                IString* str = (IString*)obj;
                v = rho_ruby_create_string(str->getUTF8());
                return v;
            }
            break;
        case BASIC_TYPES::RubyNil:
            {
                return rho_ruby_get_NIL();
            }
            break;
        default:
            {
                return rho_ruby_get_NIL();
            }
        }
        return rho_ruby_get_NIL();
    }
    
    
    VALUE RhoRubyImpl::rb_Rho_Ruby_callNativeCallback(int argc, VALUE *argv) {
        if (argc != 2) {
            return rho_ruby_get_NIL();
        }
        VALUE v_callback_id = argv[0];
        VALUE v_param = argv[1];
        
        if (rb_type(v_callback_id) != T_STRING) {
            return rho_ruby_get_NIL();
        }
        const char* callback_id = RSTRING_PTR(v_callback_id);
        
        IRubyNativeCallback* callback = mCallbacks[callback_id];
        if (callback == NULL) {
            return rho_ruby_get_NIL();
        }
        callback->onRubyNative(convertVALUE_to_Object(v_param));
        return rho_ruby_get_NIL();
    }

    
    
    
}
}
