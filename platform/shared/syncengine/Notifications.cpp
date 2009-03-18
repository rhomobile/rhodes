/*
 *  Notifications.cpp
 *  rhosynclib
 *
 *  Created by Vlad on 2/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __SYMBIAN32__

#if defined(_WIN32_WCE)
// Fixing compiler error C2039: 'wcsftime' : is not a member of '`global namespace''
size_t __cdecl wcsftime(wchar_t *, size_t, const wchar_t *, const struct tm*);
// strdup is implemented as part of ruby CE port
extern "C" char *strdup(const char * str);
extern "C" int _shttpd_snprintf(char *buf, size_t buflen, const char *fmt, ...);
#define snprintf _shttpd_snprintf
#endif

#include <string>
#include <map>

#include "Notifications.h"
#include "UniversalLock.h"
// TODO: temporary fix for compile error
#ifndef __APPLE__
#include "tcmalloc/rhomem.h"
#endif

static std::map<int,notification_t*> _notifications;
INIT_LOCK(notify);

extern "C" void perform_notification(char* callback, char* params);
extern "C" char* get_current_location();
extern "C" char* HTTPResolveUrl(char* url);

static void _clear_notification(int source_id);
	
static notification_t* get_notification(int source_id) {
	std::map<int,notification_t*>::iterator it = _notifications.find(source_id);
	if (it!=_notifications.end()) {
		return it->second;
	}
	return NULL;
}

void fire_notification(int source_id,int sucess) {
    LOCK(notify);

	try {
		notification_t* pn = get_notification(source_id);
		if (pn) {
			if (pn->url) {
				//compose message
				const char* status = sucess ? "ok" : "error";
				int msglen = 100+(pn->params?strlen(pn->params):0);
				char* message = (char*)malloc(msglen+1);
				snprintf(message,msglen,"status=%s&%s",status,(pn->params?pn->params:""));
				//execute notification
				perform_notification(pn->url,message);
				free(message);
			}
			//erase callback so we don't call more than once
			_clear_notification(source_id);
		}
	} catch(...) {
	}
	
    UNLOCK(notify);
}

void free_notifications() {
    LOCK(notify);

	try {
		std::map<int,notification_t*>::iterator it;
		for ( it=_notifications.begin() ; it != _notifications.end(); it++ ) {
			_clear_notification(it->first);
		}
		_notifications.clear();
	} catch(...) {
	}
	
    UNLOCK(notify);
}

void set_notification(int source_id, const char *url, char* params) {
    LOCK(notify);

	try {
		if (url) {
			notification_t* notification = (notification_t*) malloc(sizeof(notification_t));
			_clear_notification(source_id);
			notification->url = HTTPResolveUrl(strdup(url));
			notification->params = strdup(params);
			printf("Resolved [%s] in [%s]\n", url, notification->url);								 
            printf("Setting notification (%s) for source %d, params: %s\n", notification->url, source_id, params);
			_notifications[source_id] = notification;
		}
	} catch(...) {
	}
	
    UNLOCK(notify);
}

void free_notification_t(notification_t* pn) {
	if (pn) {
		if (pn->url) {
			free(pn->url);
		}
		if (pn->params) {
			free(pn->params);
		}
		free(pn);
	}
}

static void _clear_notification(int source_id) {
	try {
		free_notification_t(get_notification(source_id));		
		_notifications.erase(source_id);
	} catch(...) {
	}
}

void clear_notification(int source_id) {
    // Don't lock notify here, we hide the function instead
	LOCK(notify);
	_clear_notification(source_id); 
	UNLOCK(notify);
}

#else //__SYMBIAN32__

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdapis/stdio.h>

#include "Constants.h"
#include "Notifications.h"
#include "UniversalLock.h"

static notification_t** _notifications;

static notification_t** get_notifications() 
{
	static int isInitialized = 0;
	if ( !isInitialized )
	{
		isInitialized = 1;
		_notifications = (notification_t**)calloc(MAX_SOURCES,sizeof(notification_t*));
	}
	
	return _notifications;
}

INIT_LOCK(notify);

extern "C" void perform_notification(char* callback, char* params);
extern "C" char* get_current_location();
extern "C" char* HTTPResolveUrl(char* url);

static void _clear_notification(int source_id);
	
static notification_t* get_notification(int source_id) {
	
	if ( source_id < MAX_SOURCES && source_id >= 0 )
		return get_notifications()[source_id];
	else
		return NULL;
}

void fire_notification(int source_id,int sucess) {
    LOCK(notify);

	try {
		notification_t* pn = get_notification(source_id);
		if (pn) {
			if (pn->url) {
				//compose message
				const char* status = sucess ? "ok" : "error";
				int msglen = 100+(pn->params?strlen(pn->params):0);
				char* message = (char*)malloc(msglen+1);
				snprintf(message,msglen,"status=%s&%s",status,(pn->params?pn->params:""));
				//execute notification
				perform_notification(pn->url,message);
				free(message);
			}
			//erase callback so we don't call more than once
			_clear_notification(source_id);
		}
	} catch(...) {
	}
	
    UNLOCK(notify);
}

void free_notifications() {
    LOCK(notify);

	try {
		int i = 0;
		for ( ; i < MAX_SOURCES; i++ ) {
			_clear_notification(i);
 		}
		memset(get_notifications(), 0, MAX_SOURCES * sizeof(notification_t*));
	} catch(...) {
	}
	
    UNLOCK(notify);
}

void set_notification(int source_id, const char *url, char* params) {
    LOCK(notify);

	try {
		if (url) {
			notification_t* notification = (notification_t*) malloc(sizeof(notification_t));
			_clear_notification(source_id);
			notification->url = HTTPResolveUrl(strdup(url));
			notification->params = params ? strdup(params) : NULL;
			printf("Resolved [%s] in [%s]\n", url, notification->url);								 
			printf("Setting notification (%s) for source %d\n", notification->url, source_id);			
			get_notifications()[source_id] = notification;
		}
	} catch(...) {
	}
	
    UNLOCK(notify);
}

void free_notification_t(notification_t* pn) {
	if (pn) {
		if (pn->url) {
			free(pn->url);
		}
		if (pn->params) {
			free(pn->params);
		}
		free(pn);
	}
}

static void _clear_notification(int source_id) {
	try {
		notification_t * pnt = get_notification(source_id);
		if ( pnt )
			free_notification_t(pnt);		
		get_notifications()[source_id]=0;
	} catch(...) {
	}
}

void clear_notification(int source_id) {
    // Don't lock notify here, we hide the function instead
	LOCK(notify);
	_clear_notification(source_id); 
	UNLOCK(notify);
}

#endif