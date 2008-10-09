/*
 *  HttpMessage.h
 *  Browser
 *
 *  Created by adam blum on 9/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __HTTPMESSAGE__
#define __HTTPMESSAGE__

#include <CoreFoundation/CoreFoundation.h>


#if defined(__cplusplus)
extern "C" {
#endif

int HttpSnprintf(char *buf, size_t buflen, const char *fmt, ...);    
const char* HTTPGetMethod(int m);
    
int HTTPParseRequest(HttpContextRef context);
int HTTPProcessMessage(HttpContextRef context);
int HTTPRedirect(HttpContextRef context, char* location);
int HTTPSendReply(HttpContextRef context, char* body);
    
                      
#if defined(__cplusplus)
                      }
#endif
                      
#endif /*__HTTPMESSAGE__*/