/*
 * webview.c
 *
 *  Created on: Mar 12, 2009
 *      Author: Anton
 */

#include "rhodesAppView.h"

#include "rhodes.pan"
#include "rhodes.hrh"
#include <eikenv.h>

#include "tcmalloc/rhomem.h"
#include <stdio.h>
#include <string.h>

void SendWindowEvent(TInt aEvent, const char* data)
{
	// Create a window server event
	RWsSession wsSession;
	TWsEvent event;

	if ( wsSession.Connect() == KErrNone )
	{
		// Set event data. eventType.data = KData;
		event.SetType(EEventUser + aEvent); // set event type
		event.SetTimeNow(); // set the event time
		event.SetHandle(wsSession.WsHandle()); // set window server handle
		
		if ( data )
			((char**)event.EventData())[0] = strdup(data);
		// Send the created event
		wsSession.SendEventToAllWindowGroups(event);
	}
}

extern "C" {

void webview_navigate(char* url){
	SendWindowEvent(ECmdAppNavigate2Url, url);
}

}
