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
#include "SyncManagerI.h"

int stop_running = 0;
#if !defined(_WIN32_WCE)
pthread_cond_t sync_cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t sync_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_condattr_t sync_details;
#endif
sqlite3 *database;

//
int process_local_changes() {
  /* List holding operations */
  if (!stop_running) {
	  // Process local changes
	  int result = 0;
	  result += process_op_list("update");
	  result += process_op_list("create");
	  result += process_op_list("delete");
  	
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
  } else {
	  finalize_statements();
	  finalize_op_statements();
  	
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
	  
    if(process_local_changes()) {
      break;
    }

	}
	pthread_mutex_unlock(&sync_mutex);
	
    return NULL;
}
#endif

int process_op_list(char *type) {
  int available;

	pSyncOperation *op_list = NULL;
	op_list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncOperation));
	available = get_op_list_from_database(op_list, database, MAX_SINGLE_OP_SIZE, type);
	printf("Found %i available records for %s processing...\n", available, type);
	
	if(push_remote_changes(op_list, available) == SYNC_PUSH_CHANGES_OK) {
		printf("Successfully processed %i records for %s...\n", available, type);
		
		remove_op_list_from_database(op_list, database, type);
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
	printf("called lock_sync_mutex...\n");
	pthread_mutex_lock(&sync_mutex);
	printf("sync_mutex locked...\n");
}

/* exposed function to release lock on sync mutex */
void unlock_sync_mutex() {
	printf("called unlock_sync_mutex...\n");
	pthread_mutex_unlock(&sync_mutex);
	printf("sync_mutex unlocked...\n");
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
	database = db;
	
	// Initialize thread
    return_val = pthread_attr_init(&attr);
	pthread_condattr_init(&sync_details);
	pthread_cond_init(&sync_cond, &sync_details);
	pthread_condattr_destroy(&sync_details);
    assert(!return_val);
    return_val = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!return_val);
	
    int thread_error = pthread_create(&p_thread_id, &attr, &sync_engine_main_routine, NULL);
	
    return_val = pthread_attr_destroy(&attr);
    assert(!return_val);
    if (thread_error != 0) {
		//TODO: Report error
    }
	
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
