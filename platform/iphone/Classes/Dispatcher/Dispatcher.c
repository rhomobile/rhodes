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

#include "ruby/ext/rho/rhoruby.h"

#include "defs.h"
#include "HttpContext.h"
#include "HttpMessage.h"
#include "Dispatcher.h"
#include "AppManagerI.h"
//#include "geolocation.h"
//#include "SyncEngine.h"
#include "sync/syncthread.h"
#include "common/RhodesApp.h"
#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Dispatcher"

extern void rho_map_location(char* query);

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
	RAWTRACE("Creating Req Hash");
	
	VALUE hash = rho_ruby_createHash();
    rho_ruby_holdValue(hash);

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
	
	VALUE hash_headers = rho_ruby_createHash();
    rho_ruby_holdValue(hash_headers);

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
    rho_ruby_releaseValue(hash_headers);

	int buflen = CFDataGetLength(context->_rcvdBytes);
	if (buflen > 0) {
		addStrToHash(hash, "request-body", 
					 (char*)CFDataGetBytePtr(context->_rcvdBytes), buflen);
	}

    rho_ruby_releaseValue(hash);
	return hash;
}

static int 
_CallApplication(HttpContextRef context, RouteRef route) {
	RAWLOG_INFO("Calling ruby framework");
		
	VALUE val = callFramework(_CreateRequestHash(context,route));
	char* res = getStringFromValue(val);
	if (res) {
		RAWTRACE("RESPONSE:");
		RAWTRACE_DATA((UInt8*)res, strlen(res));
		RAWTRACE( "RESPONSE -- eof --");
		
		RAWTRACE("Add response to the send buffer");
		CFDataAppendBytes(context->_sendBytes, (UInt8*)res, (CFIndex)strlen(res));
		
		releaseValue(val);
		return 1;
	}
	
	return 0;
}

extern char* GeoGetLocation();
extern int ExecuteAppManager(HttpContextRef context, RouteRef route);

int _ExecuteApp(HttpContextRef context, RouteRef route) {
	
	if (route->_application && !strcmp(route->_application,"AppManager")) {
		RAWLOG_INFO("Executing AppManager");
		return ExecuteAppManager(context,route);
	} else if (route->_application && !strcmp(route->_application,"system")) {
		if (context->_request->_method == METHOD_GET) {
			if (route->_model && !strcmp(route->_model,"geolocation")) {
				return HTTPSendReply(context,GeoGetLocation()); 	
			} else if (route->_model && !strcmp(route->_model,"syncdb")) {
				rho_sync_doSyncAllSources(TRUE);
				return HTTPSendReply(context,"OK"); 	
			} else if (route->_model && !strcmp(route->_model,"redirect_to")) {
				if (context->_request->_query && !strncmp("url=",context->_request->_query,4)) {
					char* location = context->_request->_query+4;
					int len = strlen(location);
					HTTPUrlDecode(location, len, location, len+1);
					return HTTPRedirect(context, location);
				} else {
					return HTTPRedirect(context, "/app/");
				}
			} else if (route->_model && !strcmp(route->_model,"map")) {
				char* query = "";
				if (context->_request->_query) {
					RAWLOG_INFO1("Map %s", context->_request->_query);
					query = context->_request->_query;
				}
				rho_map_location(query);
				return HTTPSendReply(context,"");
			}
		}
	} else if (route->_application && !strcmp(route->_application,"shared")) {
		return 0;
	} else {
		RAWLOG_INFO1( "Executing %s",route->_application);
		return _CallApplication(context, route);
	}
	return 0;
}

int ServeIndex(HttpContextRef context, char* index_name) {
	RAWLOG_INFO("Calling ruby framework to serve index");
	if ( CFDataGetLength(context->_rcvdBytes) == 0)
		rho_rhodesapp_keeplastvisitedurl(index_name);

	VALUE val = callServeIndex(index_name);
	char* res = getStringFromValue(val);
	if (res) {
		RAWTRACE("RESPONSE:");
		RAWTRACE_DATA((UInt8*)res, strlen(res));
		RAWTRACE("RESPONSE -- eof --");
		
		RAWTRACE("Add response to the send buffer");
		CFDataAppendBytes(context->_sendBytes, (UInt8*)res, (CFIndex)strlen(res));
		
		releaseValue(val);
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
		if ( CFDataGetLength(context->_rcvdBytes) == 0)
			rho_rhodesapp_keeplastvisitedurl(context->_request->_uri);
		
		ret = _ExecuteApp(context, &route);
	}
	
	free(url);
	return ret;
}