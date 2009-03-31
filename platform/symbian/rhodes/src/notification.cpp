/*
 * notification.cpp
 *
 *  Created on: Mar 18, 2009
 *      Author: Anton
 */
#include <e32std.h>
#include <e32base.h>
#include <eikenv.h>
#include "HttpClient.h"
#include "HttpConstants.h"
#include <aknglobalnote.h>

#include "tcmalloc/rhomem.h"

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdapis/stdio.h>

#include <unistd.h>

#include <assert.h>

extern "C" {

/*void showMessageBox(const char* str)
{
	if ( str )
	{
		CAknGlobalNote* iGlobalNote = CAknGlobalNote::NewL(); //Initialize the note
		CleanupStack::PushL(iGlobalNote);
	
		TPtrC8 ptr(reinterpret_cast<const TUint8*>(str));
		HBufC* buffer = HBufC::NewL(ptr.Length());
		buffer->Des().Copy(ptr);
		
		iGlobalNote->ShowNoteL(EAknGlobalInformationNote, buffer->Des());
		CleanupStack::PopAndDestroy(); // globalNote
		
		delete buffer;
	}
}*/
	
void* notification_callback(void *parm);

typedef struct {
	char* callback;
	char* params;
} notification_params_t;

void perform_notification(char* callback, char* params) {
	pthread_attr_t attr;
	pthread_t p_thread_id;
	notification_params_t* thread_params;
	int return_val;
	int thread_error;

	return_val = pthread_attr_init(&attr);
	assert(!return_val);

	thread_params = (notification_params_t*) malloc(sizeof(notification_params_t));
	thread_params->callback = strdup(callback);
	thread_params->params = strdup(params);

	return_val = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	assert(!return_val);

	thread_error = pthread_create(&p_thread_id, &attr, &notification_callback,
			(void*) thread_params);

	return_val = pthread_attr_destroy(&attr);
}

void* notification_callback(void *parm) {
	notification_params_t* thread_params = (notification_params_t*) parm;
	
	if (thread_params) {
		// Create and install the active scheduler
	
		CActiveScheduler* activeScheduler = new (ELeave) CActiveScheduler;
		CleanupStack::PushL(activeScheduler);
		CActiveScheduler::Install(activeScheduler);

		CHttpClient* gHttpClient = CHttpClient::NewL();

		gHttpClient->InvokeHttpMethodL(CHttpConstants::EPost,
				(const TUint8*) thread_params->callback, strlen(
						thread_params->callback),
				(const TUint8*) thread_params->params, strlen(
						thread_params->params), NULL);

		delete gHttpClient;

		CleanupStack::PopAndDestroy(activeScheduler);
	}

	//clear parameters
	if (thread_params) {
		if (thread_params->callback)
			free( thread_params->callback );
		if (thread_params->params)
			free( thread_params->params );

		free(thread_params);
	}

	pthread_exit(NULL);
	return NULL;
}

//TODO: This is stubbed until we implement!
int has_network_impl() {
	return 1;
}

}//extern "C"
