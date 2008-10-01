/*
 *  Dispatcher.c
 *  rhorunner
 *
 *  Created by vlad on 9/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "defs.h"
#include "HttpContext.h"
#include "Dispatcher.h"
#include "AppManagerI.h"

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

int _ExecuteApp(HttpContextRef context, RouteRef route) {
	
	if (route->application && !strcmp(route->application,"AppManager")) {
		DBG(("Executing AppManager\n"));
		return ExecuteAppManager(context,route);
	}	return 0;
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