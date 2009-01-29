/*
 *  Dispatcher.c
 *  rhorunner
 *
 *  Created by vlad on 9/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <sys/types.h>
#include <string.h>

#include "rhoruby.h"

#include "defs.h"
#include "HttpContext.h"
#include "HttpMessage.h"
#include "Dispatcher.h"
#include "AppManagerI.h"
//#include "geolocation.h"
#include "SyncEngine.h"

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

static bool _isid(char* str) {
	if (str!=NULL) {
		int l = strlen(str);
		if ( (l>2) && (str[0]=='{') && (str[l-1]=='}') ) 
			return true;
	}
	return false;
}

static char* _tok(char* t) {
	if (!t) return NULL;
	char* s = strchr(t,'/');
	if ( s != NULL ) {
		s[0] = '\0'; s++;
	}
	return s;
}

static bool 
_GetRoute(char* url, RouteRef route) {
	char *actionorid,*next;
	
	memset(route, 0, sizeof(route[0]));
	if (url[0]=='/') url++;	
	
	route->_application = url;	
	if ((route->_model = _tok(url)) == NULL)
		return false;
	
	if ((actionorid = _tok(route->_model)) == NULL)
		return true;
	
	next = _tok(actionorid);
	
	if (_isid(actionorid)) {
		route->_id = actionorid;
		route->_action = next;
	} else {
		route->_id = next;
		route->_action = actionorid;
	}
	_tok(next);
	
	return true;
}

static VALUE 
_CreateRequestHash(HttpContextRef context, RouteRef route) {
	DBG(("Creating Req Hash\n"));
	
	VALUE hash = createHash();

	const char* applicationName = route->_application;
	addStrToHash(hash, "application", applicationName, strlen(applicationName));

	const char* modelName = route->_model;
	addStrToHash(hash, "model", modelName, strlen(modelName));

	if (route->_action!=NULL) {
		const char* actionName = route->_action;
		addStrToHash(hash, "action", actionName, strlen(actionName));
	}
	
	if (route->_id!=NULL) {
		const char* _id = route->_id;
		addStrToHash(hash, "id", _id, strlen(_id));
	}
	
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
	addHashToHash(hash,"headers",hash_headers);
	
	int buflen = CFDataGetLength(context->_rcvdBytes);
	if (buflen > 0) {
		addStrToHash(hash, "request-body", 
					 (char*)CFDataGetBytePtr(context->_rcvdBytes), buflen);
	}
	
	return hash;
}

static int 
_CallApplication(HttpContextRef context, RouteRef route) {
	DBG(("Calling ruby framework\n"));
		
	char* res = callFramework(_CreateRequestHash(context,route));
	
	if (res) {
		DBG(("RESPONSE:\n"));
		_dbg_print_data((UInt8*)res, strlen(res));
		DBG(("-- eof --\n"));
		
		DBG(("Add response to the send buffer"))
		CFDataAppendBytes(context->_sendBytes, (UInt8*)res, (CFIndex)strlen(res));
		
		return 1;
	}
	
	return 0;
}

int _ExecuteApp(HttpContextRef context, RouteRef route) {
	
	if (route->_application && !strcmp(route->_application,"AppManager")) {
		DBG(("Executing AppManager\n"));
		return ExecuteAppManager(context,route);
	} else if (route->_application && !strcmp(route->_application,"system")) {
		if (context->_request->_method == METHOD_GET) {
			if (route->_model && !strcmp(route->_model,"geolocation")) {
				return HTTPSendReply(context,GeoGetLocation()); 	
			} else if (route->_model && !strcmp(route->_model,"syncdb")) {
				wake_up_sync_engine();
				return HTTPSendReply(context,"OK"); 	
			}
		}
	} else if (route->_application && !strcmp(route->_application,"shared")) {
		return 0;
	} else {
		DBG(("Executing %s\n",route->_application));
		return _CallApplication(context, route);
	}
	return 0;
}

int ServeIndex(HttpContextRef context, char* index_name) {
	DBG(("Calling ruby framework to serve index\n"));
	
	char* res = callServeIndex(index_name);
	
	if (res) {
		DBG(("RESPONSE:\n"));
		_dbg_print_data((UInt8*)res, strlen(res));
		DBG(("-- eof --\n"));
		
		DBG(("Add response to the send buffer"))
		CFDataAppendBytes(context->_sendBytes, (UInt8*)res, (CFIndex)strlen(res));
		
		return 1;
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