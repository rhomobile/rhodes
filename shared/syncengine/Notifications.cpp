/*
 *  Notifications.cpp
 *  rhosynclib
 *
 *  Created by Vlad on 2/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <map>
#include "Notifications.h"

static std::map<int,char*> _notifications;

extern "C" void perform_webview_refresh();
extern "C" char* get_current_location();
extern "C" char* HTTPResolveUrl(char* url);

void fire_notification(int source_id) {
	char* url = _notifications.find(source_id)->second;
	if (url != NULL) {
		//execute notification
		char* current_url = get_current_location();
		if (strcmp(current_url,url)==0) {
			printf("Executing notification (%s) for source %d\n", url, source_id);
			perform_webview_refresh();
		}
		//erase callback
		//free(url);
		//_notifications.erase(source_id);
	}
}

void free_notifications() {
	std::map<int,char*>::iterator it;
	for ( it=_notifications.begin() ; it != _notifications.end(); it++ ) {
		printf("Freeing notification (%s) for source %d\n", (char*)((*it).second), it->first);
		free((char*)((*it).second));
	}
	_notifications.clear();
}

void set_notification(int source_id, const char *url) {
	if (url) {
		char* tmp_url = _notifications.find(source_id)->second;
		if (tmp_url) {
			free(tmp_url);
		}
		tmp_url = HTTPResolveUrl(strdup(url));
		printf("Resolved [%s] in [%s]\n", url, tmp_url);								 
		printf("Setting notification (%s) for source %d\n", tmp_url, source_id);
		_notifications[source_id] = tmp_url;
	}
}

void clear_notification(int source_id) {
	char* tmp_url = _notifications.find(source_id)->second;
	if (tmp_url) {
		free(tmp_url);
		_notifications.erase(source_id);
	}
}
