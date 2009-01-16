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

#ifdef __SYMBIAN32__
#include <sys/types.h>
#endif //__SYMBIAN32__

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H

void RhoRubyStart();

VALUE getnil();	
VALUE createHash();
VALUE addTimeToHash(VALUE hash, const char* key, time_t val);	
VALUE addIntToHash(VALUE hash, const char* key, int val);	
VALUE addStrToHash(VALUE hash, const char* key, const char* val, int len);
VALUE addHashToHash(VALUE hash, const char* key, VALUE val);	

char* callFramework(VALUE hashReq);
char* callServeIndex(char* index_name);
char* callGetStartPage();
char* callGetRhobundleZipUrl();

void RhoRubyStop();

const char* RhoGetRootPath();


#if defined(__cplusplus)
}
#endif
		
	
#endif //_RHO_RVM_H
