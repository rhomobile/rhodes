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
	
void RhoRubyStart(const char* szAppPath);

char* RhoProcessRequest( char* szRequest);

void RhoRubyStop();

#if defined(__cplusplus)
}
#endif
		
	
#endif //_RHO_RVM_H