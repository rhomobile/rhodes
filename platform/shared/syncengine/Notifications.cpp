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
#endif

#include <string>
#include <map>

#include "Notifications.h"
#include "UniversalLock.h"
// TODO: temporary fix for compile error
#ifndef __APPLE__
#include "tcmalloc/rhomem.h"
#endif

static std::map<int,char*> _notifications;
INIT_LOCK(notify);

extern "C" void perform_webview_refresh();
extern "C" char* get_current_location();
extern "C" char* HTTPResolveUrl(char* url);

//#if defined(_WIN32_WCE)
static char* get_url(int source_id) {
	std::map<int,char*>::iterator it = _notifications.find(source_id);
	if (it!=_notifications.end()) {
		return it->second;
	}
	return NULL;
}
//#else
//#define get_url(source_id) _notifications.find(source_id)->second
//#endif

void fire_notification(int source_id) {
    LOCK(notify);

	try {
		char* url = get_url(source_id);
 
		if (url != NULL) {
			//execute notification
			char* current_url = get_current_location();
			if (current_url && (strcmp(current_url,url)==0)) {
				printf("Executing notification (%s) for source %d\n", url, source_id);
				perform_webview_refresh();
			}
			//erase callback so we don't call more than once
			clear_notification(source_id);
		}
	} catch(...) {
	}
    UNLOCK(notify);
}

void free_notifications() {
    LOCK(notify);

	try {
		std::map<int,char*>::iterator it;
		for ( it=_notifications.begin() ; it != _notifications.end(); it++ ) {
			printf("Freeing notification (%s) for source %d\n", (char*)((*it).second), it->first);
			free((char*)((*it).second));
		}
		_notifications.clear();
	} catch(...) {
	}
    UNLOCK(notify);
}

void set_notification(int source_id, const char *url) {
    LOCK(notify);

	try {
		if (url) {
			char* tmp_url = get_url(source_id);
			if (tmp_url) free(tmp_url);
			tmp_url = HTTPResolveUrl(strdup(url));
			printf("Resolved [%s] in [%s]\n", url, tmp_url);								 
			printf("Setting notification (%s) for source %d\n", tmp_url, source_id);
			_notifications[source_id] = tmp_url;
		}
	} catch(...) {
	}
    UNLOCK(notify);
}

void clear_notification(int source_id) {
    LOCK(notify);

	try {
		char* tmp_url = get_url(source_id);
		if (tmp_url) {
			free(tmp_url);
			_notifications.erase(source_id);
		}
	} catch(...) {
	}
    UNLOCK(notify);
}

#else //__SYMBIAN32__

extern "C"{
void fire_notification(int source_id) {
}

void clear_notification(int source_id) {
}

void set_notification(int source_id, const char *url) {
}

void free_notifications() {
}
}

#endif