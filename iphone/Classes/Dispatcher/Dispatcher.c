/*
 *  Dispatcher.c
 *  rhorunner
 *
 *  Created by vlad on 9/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <sys/types.h>

#include "rhoruby.h"

#include "defs.h"
#include "HttpContext.h"
#include "HttpMessage.h"
#include "Dispatcher.h"
#include "AppManagerI.h"


char *trim(char *str)
{
	char *ibuf = str, *obuf = str;

	if (str) {
		// Remove leading spaces
		for(ibuf = str; *ibuf && isspace(*ibuf); ++ibuf);
		if (str != ibuf)
			memmove(str, ibuf, ibuf - str);

		// Remove trailing spaces
		int i = strlen(str);
		while (--i >= 0) {
			if (!(isspace(obuf[i])||obuf[i]==':'))
				break;
		}
		obuf[++i] = 0;
	}
	return str;
}

bool _GetRoute(char* url, RouteRef route) {
	memset(route, 0, sizeof(route[0]));
	if (url[0]=='/') url++;
	
	route->application = url;	
	if ((route->controller = strchr(url,'/')) != NULL) {
		route->controller[0] = '\0';
		route->controller++;
		if ((route->action = strchr(route->controller,'/')) !=NULL) {
			route->action[0] = '\0';
			route->action++;
		}
	}
	
	return ((route->application != NULL) && (route->controller != NULL) && (route->action != NULL));
}

VALUE 
_CreateRequestHash(HttpContextRef context) {
	VALUE hash = createHash();
	
	const char* method = HTTPGetMethod(context->_request->_method);
	addStrToHash(hash, "request-method", method, strlen(method));
	
	const char* uri = context->_request->_uri;
	addStrToHash(hash, "request-uri", uri, strlen(uri));
	
	const char* query = context->_request->_query == NULL ? "" : context->_request->_query;
	addStrToHash(hash, "request-query", query, strlen(query));
	
	VALUE hash_headers = createHash();
	struct parsed_header* h = &context->_request->_cheaders.cl;
	for (int i = 0; i < sizeof(struct headers)/sizeof(struct parsed_header); i++) {
		if (h->_name) {
			char* name = trim(strdup(h->_name));
			if (h->_type == HDR_STRING) {
				addStrToHash(hash_headers,name,h->_v.v_vec.ptr,h->_v.v_vec.len);
			} else if (h->_type == HDR_INT) {
				addIntToHash(hash_headers, name, h->_v.v_big_int);
			} else if (h->_type == HDR_DATE) {
				addTimeToHash(hash_headers, name, h->_v.v_time);
			}
			free(name);
		}
		h++;
	}
	addHashToHash(hash,"request-headers",hash_headers);
	
	int buflen = CFDataGetLength(context->_rcvdBytes);
	if (buflen > 0) {
		addStrToHash(hash, "request_body", 
					 (char*)CFDataGetBytePtr(context->_rcvdBytes), buflen);
	}
	
	return hash;
}

int 
_CallRubyFramework(HttpContextRef context)
{
	DBG(("Calling ruby test\n"));
		
	char* body = callFramework(_CreateRequestHash(context));
	
	if (body) {
		DBG(("BODY:\n"));
		_dbg_print_data((UInt8*)body, strlen(body));
		DBG(("-- eof --\n"));
		
		DBG(("Sending reply\n"));
		return HTTPSendReply(context, body);
	}
	
	return 0;
}

int _ExecuteApp(HttpContextRef context, RouteRef route) {
	
	if (route->application && !strcmp(route->application,"AppManager")) {
		DBG(("Executing AppManager\n"));
		return ExecuteAppManager(context,route);
	} else if (route->application && !strcmp(route->application,"Test")) {
		DBG(("Executing Ruby Test\n"));
		return _CallRubyFramework(context);
	}
	return 0;
}

/*
 *
 */
int Dispatch(HttpContextRef context) {
	int ret = 0;
	Route route;
	char* url = strdup(context->_request->_uri);
	
	if( _GetRoute(url, &route) ) {
		ret = _ExecuteApp(context, &route);
	}
	
	free(url);
	return ret;
}