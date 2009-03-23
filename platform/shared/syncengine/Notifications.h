/*
 *  Notifications.h
 *  rhosynclib
 *
 *  Created by Vlad on 2/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __SYNCENGINE__NOTIFICATIONS___
#define __SYNCENGINE__NOTIFICATIONS___

#if defined(__cplusplus)
extern "C" {
#endif

	typedef struct __notification_t {
		char* url;
		char* params;
	} notification_t;
	void free_notification_t(notification_t* pn);
	
	void fire_notification(int source_id,int sucess);
	void free_notifications();	
	void set_notification(int source_id, const char *url, char* params);
	void clear_notification(int source_id);

#if defined(__cplusplus)
}
#endif

#endif //__SYNCENGINE__NOTIFICATIONS___
