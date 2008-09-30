/*
 *  Dispatcher.h
 *  rhorunner
 *
 *  Created by vlad on 9/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

typedef struct __Route {
	char* application;
	char* controller;
	char* action;
} Route, * RouteRef;

int Dispatch(HttpContextRef context); 