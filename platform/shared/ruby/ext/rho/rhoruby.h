/*
 *  rhoruby.h
 *  rhorubylib
 *
 *  Created by evgeny vovchenko on 10/1/08.
 *  Copyright 2008 RhoMobile. All rights reserved.
 *
 */
#ifndef _RHO_RVM_H
#define _RHO_RVM_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "common/RhoDefs.h"

#ifdef __SYMBIAN32__
#include <sys/types.h>
#endif //__SYMBIAN32__

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H
	
#if defined( OS_WINDOWS ) || defined( OS_WINCE )
typedef unsigned __int64 uint64__;
#else
typedef unsigned long long uint64__;
typedef unsigned long long __int64;	
#endif
	
void RhoRubyStart();
void RhoRubyInitApp();
//void RhoRubyThreadStart();
//void RhoRubyThreadStop();
void rho_ruby_activateApp();
void rho_ruby_loadserversources(const char* szData);
void rho_ruby_start_threadidle();
void rho_ruby_stop_threadidle();

VALUE rho_ruby_createHash();
VALUE addTimeToHash(VALUE hash, const char* key, time_t val);	
VALUE addIntToHash(VALUE hash, const char* key, int val);	
VALUE addStrToHash(VALUE hash, const char* key, const char* val);
VALUE addStrToHashLen(VALUE hash, const char* key, const char* val, int len);
VALUE addHashToHash(VALUE hash, const char* key, VALUE val);	

char* getStringFromValue(VALUE val);
int getStringLenFromValue(VALUE val);

void rho_ruby_holdValue(VALUE val);
void rho_ruby_releaseValue(VALUE val);

VALUE rho_ruby_disable_gc();
void  rho_ruby_enable_gc(VALUE val);

VALUE callFramework(VALUE hashReq);
VALUE callServeIndex(char* index_name);

void RhoRubyStop();

//const char* RhoGetRootPath();
VALUE rho_ruby_get_NIL();
VALUE rho_ruby_create_array();
VALUE rho_ruby_create_string(const char* szVal);
VALUE rho_ruby_create_boolean(unsigned char b);
VALUE rho_ruby_create_integer(__int64 i);
VALUE rho_ruby_create_double(double d);
void rho_ruby_add_to_array(VALUE ar, VALUE val);

char* RhoRuby_getRhoDBVersion();

typedef void rho_hash_eachstr_func(const char*, const char*, void*);
void rho_ruby_enum_strhash(VALUE hash, rho_hash_eachstr_func *, void* data);

typedef void rho_ary_eachstr_func(const char*, void*);
void rho_ruby_enum_strary(VALUE ary, rho_ary_eachstr_func *, void* data);

struct CRhoRubyStringOrInt 
{
    const char* m_szStr;
    uint64__    m_nInt;
};

struct CRhoRubyStringOrInt rho_ruby_getstringorint(VALUE val);

const char* rho_ruby_getMessageText(const char* szName);
const char* rho_ruby_getErrorText(int nError);
const char* rho_ruby_internal_getErrorText(int nError);

VALUE rho_ruby_create_mutex();
void rho_ruby_destroy_mutex(VALUE val);
VALUE rho_ruby_main_thread();
VALUE rho_ruby_current_thread();
void rho_ruby_lock_mutex(VALUE val);
void rho_ruby_unlock_mutex(VALUE val);

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
