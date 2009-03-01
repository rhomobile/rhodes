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

	void fire_notification(int source_id);
	void free_notifications();	
	void set_notification(int source_id, const char *url);
	void clear_notification(int source_id);

#if defined(__cplusplus)
}
#endif

#endif //__SYNCENGINE__NOTIFICATIONS___
