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

#ifndef VALUE
typedef unsigned long VALUE;
#endif

#if defined(__cplusplus)
extern "C" {
#endif
	
void RhoRubyStart(const char* szAppPath);

VALUE createHash();
VALUE addTimeToHash(VALUE hash, const char* key, time_t val);	
VALUE addIntToHash(VALUE hash, const char* key, int val);	
VALUE addStrToHash(VALUE hash, const char* key, const char* val, int len);
VALUE addHashToHash(VALUE hash, const char* key, VALUE val);	

char* callFramework(VALUE hashReq);

char* RhoProcessRequest( char* szRequest);

void RhoRubyStop();

#if defined(__cplusplus)
}
#endif
		
	
#endif //_RHO_RVM_H