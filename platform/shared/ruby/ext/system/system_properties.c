/*
 *  system_properties.c
 *  rhorubylib
 *
 *  Created by Vlad on 12/2/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "ruby.h"
#include "system_properties.h"

static char* tolowerstr(char* str) {
	if (str) {
		char* s = str;
		while(*s) {
			*s = TOLOWER(*s); 
			 s++;
		}
	}
	return str;
}

char* get_property(char* property) {
	if (property) {
		char* pn = tolowerstr(property);
		if (strcmp("platform",pn) == 0) {
#if defined(__APPLE__) && (defined(__MACH__) || defined(__DARWIN__))
			return "APPLE";
#elif defined(_WIN32)
			return "WINDOWS";
#elif defined(__SYMBIAN32__)
			return "SYMBIAN";
#elif defined(OS_ANDROID)
            return "ANDROID";
#else
			return "UNKNOWN";
#endif			
		}
	}
	return NULL;
}
