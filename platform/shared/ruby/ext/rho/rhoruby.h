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
#include "common/rhoparams.h"

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
void rho_ruby_start_threadidle();
void rho_ruby_stop_threadidle();

VALUE createHash();
VALUE addTimeToHash(VALUE hash, const char* key, time_t val);	
VALUE addIntToHash(VALUE hash, const char* key, int val);	
VALUE addStrToHash(VALUE hash, const char* key, const char* val);
VALUE addStrToHashLen(VALUE hash, const char* key, const char* val, int len);
VALUE addHashToHash(VALUE hash, const char* key, VALUE val);	

rho_param *valueToRhoParam(VALUE v);

char* getStringFromValue(VALUE val);
int getStringLenFromValue(VALUE val);

void rho_ruby_holdValue(VALUE val);
void  rho_ruby_releaseValue(VALUE val);

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

typedef void rho_eachstr_func(const char*, const char*, void*);
void rho_ruby_enum_strhash(VALUE hash, rho_eachstr_func *, void* data);
void rho_ruby_set_const(const char* szName, const char* szVal);

struct CRhoRubyStringOrInt 
{
    const char* m_szStr;
    uint64__    m_nInt;
};

struct CRhoRubyStringOrInt rho_ruby_getstringorint(VALUE val);

const char* rho_ruby_getMessageText(const char* szName);
const char* rho_ruby_getErrorText(int nError);
const char* rho_ruby_internal_getErrorText(int nError);

#if defined(__cplusplus)
}
#endif
		
	
#endif //_RHO_RVM_H
