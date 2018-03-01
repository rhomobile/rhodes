/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#ifndef _RHO_RVM_H
#define _RHO_RVM_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "common/RhoDefs.h"

#ifdef __SYMBIAN32__
#include <sys/types.h>
#endif //__SYMBIAN32__
    
#if defined(OS_MACOSX)
#include <sys/types.h>
#endif
    
#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H
	
#if defined( WINDOWS_PLATFORM )
typedef unsigned __int64 uint64__;
#else
typedef unsigned long long uint64__;
#ifndef OS_SYMBIAN
typedef unsigned long long __int64;
#endif
#endif
	
void RhoRubyStart();
void RhoRubyInitApp();
//void RhoRubyThreadStart();
//void RhoRubyThreadStop();
void rho_ruby_call_config_conflicts();
void rho_ruby_activateApp();
void rho_ruby_deactivateApp();
void rho_ruby_uiCreated();
void rho_ruby_uiDestroyed();
void rho_ruby_loadserversources(const char* szData);
void rho_ruby_loadallsyncsources();
void rho_ruby_reset_db_on_sync_user_changed();
int  rho_ruby_is_started();
void rho_ruby_callmethod(const char* szMethodPath);
VALUE rho_ruby_callmethod_arg(VALUE classValue, const char *name, VALUE arg);
    
VALUE rho_ruby_createHash();
VALUE addTimeToHash(VALUE hash, const char* key, time_t val);	
VALUE addIntToHash(VALUE hash, const char* key, int val);	
VALUE addBoolToHash(VALUE hash, const char* key, int val);	
VALUE addStrToHash(VALUE hash, const char* key, const char* val);
VALUE addStrToHashLen(VALUE hash, const char* key, const char* val, int len);
VALUE addHashToHash(VALUE hash, const char* key, VALUE val);	
VALUE rho_ruby_hash_aref(VALUE hash, const char* key);
VALUE rho_ruby_hash_get(VALUE hash, VALUE key);
VALUE rho_ruby_array_get(VALUE ar, int nIndex);
int rho_ruby_array_get_size(VALUE ar);

char* getStringFromValue(VALUE val);
int getStringLenFromValue(VALUE val);

void rho_ruby_holdValue(VALUE val);
void rho_ruby_releaseValue(VALUE val);

VALUE rho_ruby_disable_gc();
void  rho_ruby_enable_gc(VALUE val);

VALUE callFramework(VALUE hashReq);
VALUE callServeIndex(char* index_name, VALUE hashReq);

void RhoRubyStop();

//const char* RhoGetRootPath();
VALUE rho_ruby_get_NIL();
int rho_ruby_is_NIL(VALUE val);
int rho_ruby_get_bool(VALUE val);
long rho_ruby_get_int(VALUE val);
double rho_ruby_get_double(VALUE val);
VALUE rho_ruby_create_array();
VALUE rho_ruby_create_string(const char* szVal);
VALUE rho_ruby_create_string_withlen(int len);
VALUE rho_ruby_create_string_withlen2(const char* szVal, int len);
VALUE rho_ruby_create_boolean(unsigned char b);
VALUE rho_ruby_create_integer(__int64 i);
VALUE rho_ruby_create_double(double d);
VALUE rho_ruby_create_time(long t);
void rho_ruby_add_to_array(VALUE ar, VALUE val);
void rho_ruby_add_to_hash(VALUE hash, VALUE key, VALUE item);


VALUE rho_ruby_get_time(VALUE rTime);
unsigned long rho_ruby_get_time_part(VALUE rDate, const char* szPart);

VALUE rho_ruby_create_byte_array(unsigned char* buf, int length);	
int rho_ruby_unpack_byte_array(VALUE array_value, unsigned char* buf, int max_length);
	
const char* rho_ruby_getRhoDBVersion();

typedef void rho_hash_eachstr_func(const char*, const char*, int, void*);
typedef void rho_hash_each_func(const char*, VALUE, void*);
void rho_ruby_enum_strhash(VALUE hash, rho_hash_eachstr_func *, void* data);
void rho_ruby_enum_strhash_json(VALUE hash, rho_hash_eachstr_func *, void* data);
void rho_ruby_enum_hash(VALUE hash, rho_hash_each_func *, void* data);

typedef void rho_ary_eachstr_func(const char*, int, void*);
typedef void rho_ary_each_func(VALUE, void*);
void rho_ruby_enum_strary(VALUE ary, rho_ary_eachstr_func *, void* data);
void rho_ruby_enum_strary_json(VALUE ary, rho_ary_eachstr_func *, void* data);
void rho_ruby_enum_ary(VALUE ary, rho_ary_each_func *, void* data);

struct CRhoRubyStringOrInt 
{
    const char* m_szStr;
    uint64__    m_nInt;
};

struct CRhoRubyStringOrInt rho_ruby_getstringorint(VALUE val);

const char* rho_ruby_getMessageText(const char* szName);
const char* rho_ruby_getErrorText(int nError);
const char* rho_ruby_internal_getErrorText(int nError);
const char* rho_ruby_internal_getMessageText(const char* szName);

VALUE rho_ruby_create_mutex();
void rho_ruby_destroy_mutex(VALUE val);
VALUE rho_ruby_main_thread();
VALUE rho_ruby_current_thread();
void rho_ruby_lock_mutex(VALUE val);
void rho_ruby_unlock_mutex(VALUE val);

void rho_ruby_raise_runtime(const char* szText);
void rho_ruby_raise_argerror(const char *fmt, ...);

VALUE rho_ruby_create_object_with_id( VALUE klass, const char* szID );
const char* rho_ruby_get_object_id( VALUE valObj );
void rho_ruby_clear_object_id( VALUE valObj );
int rho_ruby_is_string(VALUE val);
int rho_ruby_is_array(VALUE val);
int rho_ruby_is_hash(VALUE val);
int rho_ruby_is_proc(VALUE val);
int rho_ruby_is_method(VALUE val);
int rho_ruby_is_object_of_class(VALUE val, VALUE valClass);
VALUE rho_ruby_get_object_class(VALUE val);
VALUE rho_ruby_get_class_byname(const char* szName);
VALUE rho_ruby_getProcBinding(VALUE proc);
int rho_ruby_is_integer(VALUE val);
int rho_ruby_is_boolean(VALUE val);
int rho_ruby_is_double(VALUE val);

int rho_ruby_to_int(VALUE val, int* dest);
int rho_ruby_to_double(VALUE val, double* dest);
int rho_ruby_to_bool(VALUE val, int* dest);
int rho_ruby_to_str(VALUE val, const char** dest, int* len);
// convert primitive types to string, otherwise to json
int rho_ruby_to_str_ex(VALUE val, const char** dest, int* len);

VALUE rho_ruby_hash_keys(VALUE val);
VALUE rho_ruby_each_key(VALUE val);


VALUE rho_ruby_safe_require(const char *fname);

#define RHO_PARAM_UNKNWON 0
#define RHO_PARAM_STRING 1
#define RHO_PARAM_ARRAY 2
#define RHO_PARAM_HASH 3

struct rho_param_t;

typedef struct rho_array_t
{
    int size;
    struct rho_param_t **value;
} rho_array;

typedef struct rho_hash_t
{
    int size;
    char **name;
    struct rho_param_t **value;
} rho_hash;

typedef struct rho_param_t
{
    int type;
    union {
        char *string;
        rho_array *array;
        rho_hash *hash;
    } v;
} rho_param;

rho_param *rho_param_str(char *s);
rho_param *rho_param_array(int size);
rho_param *rho_param_hash(int size);

rho_param *rho_param_hash_get(rho_param *p, const char* name);

rho_param *rho_param_dup(rho_param *p);
void rho_param_free(rho_param *p);

rho_param *rho_param_fromvalue(VALUE v);

#if defined(__cplusplus)
}

struct CHoldRubyValue
{
    VALUE m_value;
    CHoldRubyValue(VALUE val) : m_value(val){ rho_ruby_holdValue(m_value); }
    ~CHoldRubyValue(){ rho_ruby_releaseValue(m_value); }

    VALUE getValue(){return m_value;}
    operator VALUE() {return m_value;}
};

#endif
		
#endif //_RHO_RVM_H
