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

int stop_running = 0;
int delay_sync = 0;
#if !defined(_WIN32_WCE)
pthread_cond_t sync_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sync_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_condattr_t sync_details;
#endif
sqlite3 *database;

int process_local_changes() {
  if (!stop_running) {
	  // Process local changes
	  int i,result,source_length;
	  int max_size = 100;
	  pSource *source_list;
	  source_list = malloc(max_size*sizeof(pSource));
	  source_length = get_sources_from_database(source_list, database, max_size);
	  
	  for(i = 0; i < source_length; i++) {
		  result = 0;
		  printf("Processing local changes for source %i...\n", source_list[i]->_source_id);
		  setup_client_id(database, source_list[i]);
		  result += process_op_list(source_list[i], "update");
		  result += process_op_list(source_list[i], "create");
		  result += process_op_list(source_list[i], "delete");
	  }
  	
	  if (result > 0) {
		  printf("Remote update failed, not continuing with sync...\n");
	  } else {
		  /* fetch new list from sync source */
		  int available_remote = fetch_remote_changes(database);
		  if(available_remote > 0) {
			  printf("Successfully processed %i records...\n", available_remote);
  			
		  }
		  /* update the in-memory list */
		  /*populate_list(database);*/
	  }
	  free_source_list(source_list, source_length);
  } else {
	  finalize_statements();
	  finalize_op_statements();
	  finalize_src_statements();
  	
	  // Close the database.
	  if (sqlite3_close(database) != SQLITE_OK) {
		  printf("Error: failed to close database with message '%s'.\n", sqlite3_errmsg(database));
	  }
	  printf("Sync engine is shutdown...\n");
	  return 1;
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
	delay_sync = get_object_count_from_database(database);
	pthread_mutex_lock(&sync_mutex);
	while(!stop_running) {
		struct timespec   ts;
		struct timeval    tp;
		gettimeofday(&tp, NULL);
		/* Convert from timeval to timespec */
		ts.tv_sec  = tp.tv_sec;
		ts.tv_nsec = tp.tv_usec * 1000;
		ts.tv_sec += WAIT_TIME_SECONDS;
		
		printf("Sync engine blocked for %d seconds...\n",WAIT_TIME_SECONDS);
		pthread_cond_timedwait(&sync_cond, &sync_mutex, &ts);
		printf("Sync engine continues w/ current operations...\n");
	
		if(!delay_sync) {
			if(process_local_changes()) {
				break;
			}
		} else {
			delay_sync = 0;
		}

	}
	pthread_mutex_unlock(&sync_mutex);
	
    return NULL;
}
#endif

int process_op_list(pSource source, char *type) {
	int available;
	int success;

	pSyncOperation *op_list = NULL;
	op_list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncOperation));
	available = get_op_list_from_database(op_list, database, MAX_SINGLE_OP_SIZE, source, type);
	printf("Found %i available records for %s processing...\n", available, type);
	
	success = push_remote_changes(op_list, available);
	if(success == SYNC_PUSH_CHANGES_OK) {
		printf("Successfully processed %i records for %s...\n", available, type);
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

#if !defined(_WIN32_WCE)
/* exposed function to acquire lock on sync mutex */
void lock_sync_mutex() {
	pthread_mutex_lock(&sync_mutex);
}

/* exposed function to release lock on sync mutex */
void unlock_sync_mutex() {
	pthread_mutex_unlock(&sync_mutex);
}

void wake_up_sync_engine() {
	pthread_mutex_lock(&sync_mutex);
	printf("Waking up sync engine...\n");
	pthread_cond_broadcast(&sync_cond);
	pthread_mutex_unlock(&sync_mutex);
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
    
#endif	  
}

void stop_sync_engine() {
	printf("Shutting down sync engine routine...\n");
	pthread_mutex_lock(&sync_mutex);
	stop_running = 1;
	pthread_cond_broadcast(&sync_cond);
	pthread_mutex_unlock(&sync_mutex);
}
#else
void start_sync_engine(sqlite3 *db) {	
	database = db;
}
#endif //!defined(_WIN32_WCE)
