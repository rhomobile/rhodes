/*
 *  SyncEngine.h
 *  RhoSyncClient
 *  Main thread for handling asynchronous sync operations
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#if !defined(_WIN32_WCE)
#include <sys/time.h>
#endif

#include "SyncEngine.h"
#include "SyncUtil.h"
#include "SyncManagerI.h"
#include "Constants.h"

int stop_running = 0;
//int delay_sync = 0;
int db_reset_delay = 0;
#if !defined(_WIN32_WCE)
pthread_cond_t sync_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sync_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sync_mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_condattr_t sync_details;
#endif
static sqlite3 *database;
char *client_id = NULL;

extern void delete_db_session(const char *source_url);
extern void reset_sync_db();
extern void trigger_sync_db_reset();
// WM function
extern void triggerSyncDbReset();

#ifdef __SYMBIAN32__
extern int g_cur_source;
#endif

int process_local_changes() {
  if (!stop_running) {
	  // Process local changes
	  int i,result,source_length = 0;
	  pSource *source_list;
	  source_list = calloc(MAX_SOURCES,sizeof(pSource));

	  source_length = get_sources_from_database(source_list, database, MAX_SOURCES);
//#if 0	  
	  for(i = 0; i < source_length&& !stop_running; i++) {
		  if(client_id == NULL) {
			  client_id = set_client_id(database, source_list[i]);
		  }
		  result = 0;
      if ( stop_running )
        break;
		  result += process_op_list(source_list[i], "update");
      if ( stop_running )
        break;
		  result += process_op_list(source_list[i], "create");
      if ( stop_running )
        break;
		  result += process_op_list(source_list[i], "delete");
	  }
  	
	  if (result > 0) 
	  {
		  printf("Remote update failed, not continuing with sync...\n");
	  } 
	  else 
	  {
		  /* fetch new list from sync source */
	  
#ifdef __SYMBIAN32__
	  /**
	   * [AA] In case of out of memory problems we need to restart sync thread for each source 
	   */
		  if ( !stop_running && g_cur_source < source_length )
		  {
			  int available_remote = fetch_remote_changes(database, client_id, source_list[g_cur_source]);
			  if(available_remote > 0) {
				  printf("Successfully processed %i records...\n", available_remote);
			  }
			  
			  g_cur_source++;
			  stop_running = 1; //stop sync thread
		  }

		  if ( g_cur_source >= source_length )
			  g_cur_source = 0;

#else
		  for(i = 0; i < source_length && !stop_running; i++)
		  {
				int available_remote = fetch_remote_changes(database, client_id, source_list[i]);
				if(available_remote > 0) {
					printf("Successfully processed %i records...\n", available_remote);
				}
		  }
#endif	  
	  }
//#endif 
	  free_source_list(source_list, source_length);
  } 
  
  if (stop_running) {
	  printf("process_local_changes: cleanup\n");
  
	  clear_client_id();
	  shutdown_database();
  }
  return 0;
}

/*
 * The main routine for the sync engine thread.
 * This routine periodically polls the database for operations
 * marked "create", "update", or "delete". If an operation is
 * found, sync operation is created and executed against the 
 * rhosync service.
 */
#if !defined(_WIN32_WCE)
void* sync_engine_main_routine(void* data) {
	printf("Starting sync engine main routine...\n");
	//delay_sync = get_object_count_from_database(database);
	pthread_mutex_lock(&sync_mutex2);
	while(!stop_running) {
		struct timespec   ts;
		struct timeval    tp;
		gettimeofday(&tp, NULL);
		/* Convert from timeval to timespec */
		ts.tv_sec  = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;

#ifdef __SYMBIAN32__		
		if ( g_cur_source != 0 )
		{
			//delay_sync = 0;
			ts.tv_sec += 1;
		}
		else
		{
			ts.tv_sec += WAIT_TIME_SECONDS;
		}
#else
		ts.tv_sec += WAIT_TIME_SECONDS;
#endif
		
		printf("Sync engine blocked for %d seconds...\n",WAIT_TIME_SECONDS);
		pthread_cond_timedwait(&sync_cond, &sync_mutex2, &ts);
		printf("Sync engine continues w/ current operations...\n");
	
		if(/*!delay_sync &&*/ !db_reset_delay) {
			if(process_local_changes()) {
				break;
			}
		} else if (db_reset_delay) {
			/* reset db for next iteration */
			reset_sync_db();
			clear_client_id();
			db_reset_delay = 0;
			//delay_sync = 0;
		} else {
			//delay_sync = 0;
		}

	}
	pthread_mutex_unlock(&sync_mutex2);
	
#ifdef __SYMBIAN32__
	
	pthread_cond_destroy(&sync_cond);
	pthread_mutex_destroy(&sync_mutex2);
	pthread_mutex_destroy(&sync_mutex);

	stop_running = 0;
#endif	
	
	return NULL;
}
#endif

int process_op_list(pSource source, char *type) {
	int available;
	int success;

	pSyncOperation *op_list = NULL;
	op_list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncOperation));
	available = get_op_list_from_database(op_list, database, MAX_SINGLE_OP_SIZE, source, type);
	if (available > 0) {
		printf("Found %i available records for %s processing on source %i...\n", available, type, source->_source_id);
	}
	
	success = push_remote_changes(op_list, available);
	if(success == SYNC_PUSH_CHANGES_OK) {
		if(available > 0) {
			remove_op_list_from_database(op_list, database, type);
		}
	} else {
		printf("There was an error processing records, not removing from database yet...\n");
		free_op_list(op_list, available);
		return 1;
	}
	
	free_op_list(op_list, available);
	return 0;
}

sqlite3 *get_database() {
	return database;
}

char *get_client_id() {
	return client_id;
}

#if !defined(_WIN32_WCE)
/* exposed function to acquire lock on sync mutex */
void lock_sync_mutex() {
	//printf("lock_sync_mutex\n");
	pthread_mutex_lock(&sync_mutex);
}

/* exposed function to release lock on sync mutex */
void unlock_sync_mutex() {
	//printf("unlock_sync_mutex\n");
	pthread_mutex_unlock(&sync_mutex);
}

void wake_up_sync_engine() {
	//pthread_mutex_lock(&sync_mutex);
	printf("Waking up sync engine...\n");
	//delay_sync = 0;
	pthread_cond_broadcast(&sync_cond);
	//pthread_mutex_unlock(&sync_mutex);
}

/*
 * Main entry point to the sync engine
 */
void start_sync_engine(sqlite3 *db) {	
    pthread_attr_t  attr;
    pthread_t       p_thread_id;
    int             return_val;
    int thread_error;
	database = db;
	
#ifndef __SYMBIAN32__	
	// Initialize thread
    return_val = pthread_attr_init(&attr);
	pthread_condattr_init(&sync_details);
	pthread_cond_init(&sync_cond, &sync_details);
	pthread_condattr_destroy(&sync_details);
    assert(!return_val);
    return_val = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!return_val);
	  
    thread_error = pthread_create(&p_thread_id, &attr, &sync_engine_main_routine, NULL);
	
    return_val = pthread_attr_destroy(&attr);
    assert(!return_val);
    if (thread_error != 0) {
		  //TODO: Report error
    }
#else
    //[AA] posix thread is not required under the Symbian because we are using native Symbian threads
	pthread_condattr_init(&sync_details);
	pthread_cond_init(&sync_cond, &sync_details);
	pthread_condattr_destroy(&sync_details);
	
	pthread_mutex_init(&sync_mutex2, NULL);
	pthread_mutex_init(&sync_mutex, NULL);
	
    
#endif	  
}

void stop_sync_engine() {
	printf("Shutting down sync engine routine...\n");
	//pthread_mutex_lock(&sync_mutex);
	stop_running = 1;
	pthread_cond_broadcast(&sync_cond);
	//pthread_mutex_unlock(&sync_mutex);
}

void shutdown_database() {
	finalize_sync_obj_statements();
	finalize_sync_util_statements();
	finalize_sync_op_statements();
  	
#ifndef __SYMBIAN32__	
	sqlite3_close(database);
#endif	
	printf("Sync engine is shutdown...\n");
}

#else
void start_sync_engine(sqlite3 *db) {	
	database = db;
}
void shutdown_database() {
	finalize_sync_obj_statements();
	finalize_sync_util_statements();
	finalize_sync_op_statements();

	printf("Sync engine is shutdown...\n");
}
#endif //!defined(_WIN32_WCE)

/**
 * Called from ruby to trigger
 * a database reset
 */
void trigger_sync_db_reset() {
#if defined(__APPLE__) || defined(__SYMBIAN32__)  
	db_reset_delay = 1;
	wake_up_sync_engine();
#else
	triggerSyncDbReset();
#endif
}

void clear_client_id() {
	if ( client_id ) free( client_id );
	client_id = NULL;
}

#ifndef __APPLE__
/**
 * login to rhosync server (default implementation)
 * If succeeded stores session into the database
 * 
 * @param login
 * @param password
 * @return 1 - succeeded, 0 - failed
 */
int login(const char* login, const char* password) {
	int retval = 0;
	int i,source_length;
	pSource *source_list;
	void* pDomainData = 0;

	if (login && password) {
		source_list = malloc(MAX_SOURCES*sizeof(pSource));
		source_length = get_sources_from_database(source_list, database, MAX_SOURCES);
		
		/* iterate over each source id and get session */
		//lock_sync_mutex();
		for(i = 0; i < source_length; i++) {
			char login_url[1024] = {0};
			char* session = 0;
			char* headers = 0;
			char data[100];

		  session = get_db_session_by_server(source_list[i]->_source_url);
		  if ( session == 0 ){
				  sprintf(login_url, "%s/client_login", source_list[i]->_source_url);
				
				  //fetch session from server
				  sprintf(data,"login=%s&password=%s&remember_me=1",login, password);
				
				  save_source_url( source_list[i]->_source_url );
				  retval = makeLoginRequest( login_url, data );
#if defined(_WIN32_WCE)
				// just using db as a placeholder for winmo since 
				// we can't delete the session
				set_db_session( source_list[i]->_source_url, "exists" );
#endif
		  }else{
			set_db_session( source_list[i]->_source_url, session );
			free(session);
		  }
		}
		
		//unlock_sync_mutex();
		free_source_list(source_list, source_length);
	}
	else {
		printf("Unable to login: 'login' or 'password' parameter is not specified.\n");
	}
	return retval;
}

/**
 * check if user is logged in to rhosync server 
 *
 * @return 1 - session exists, 0 - session is null or empty
 */
int logged_in() {
	char *session;
	int i,source_length,retval = 0;
	pSource *source_list;
	
	source_list = malloc(MAX_SOURCES*sizeof(pSource));
	source_length = get_sources_from_database(source_list, database, MAX_SOURCES);
		
	/* iterate over each source id and delete session */
	//lock_sync_mutex();
	for(i = 0; i < source_length; i++) {
		session = get_db_session(source_list[i]->_source_url);
		if (session && strlen(session) > 0) {
			retval = 1;
			free(session);
		}
	}
	free_source_list(source_list, source_length);
	
	//unlock_sync_mutex();
	return retval;
}

/**
 * logout from rhosync server
 */
void logout() {
	int i,source_length;
	pSource *source_list;
	source_list = malloc(MAX_SOURCES*sizeof(pSource));
	source_length = get_sources_from_database(source_list, database, MAX_SOURCES);
		
	/* iterate over each source id and delete session */
	//lock_sync_mutex();
	for(i = 0; i < source_length; i++) {
		delete_db_session(source_list[i]->_source_url);
	}
	free_source_list(source_list, source_length);
	//unlock_sync_mutex();
}

#endif //__APPLE__
