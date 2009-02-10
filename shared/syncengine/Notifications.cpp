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

void fire_notification(int source_id) {
	char* url = _notifications.find(source_id)->second;
	if (url != NULL) {
		//execute notification
		printf("Executing notification (%s) for source %d", url, source_id);

		//erase callback
		free(url);
		_notifications.erase(source_id);
	}
}

void free_notifications() {
	std::map<int,char*>::iterator it;
	for ( it=_notifications.begin() ; it != _notifications.end(); it++ ) {
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
		_notifications[source_id] = strdup(url);
	}
}

void clear_notification(int source_id) {
	char* tmp_url = _notifications.find(source_id)->second;
	if (tmp_url) {
		free(tmp_url);
		_notifications.erase(source_id);
	}
}
