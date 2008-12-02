/*
 *  Dispatcher.h
 *  rhorunner
 *
 *  Created by vlad on 9/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

typedef struct __Route {
	char* _application;
	char* _model;
	char* _id;
	char* _action;
} Route, * RouteRef;

int ServeIndex(HttpContextRef context, char* index_name);
int Dispatch(HttpContextRef context); 