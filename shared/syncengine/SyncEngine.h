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

#ifndef __SYNCENGINE__
#define __SYNCENGINE__

#import <sqlite3.h>
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "SyncObject.h"
#include "Constants.h"
#include "SyncOperation.h"

#if defined(__cplusplus)
extern "C" {
#endif

int stop_running;
pthread_cond_t sync_cond;
pthread_mutex_t sync_mutex;


void* sync_engine_main_routine(void* data);

/* Process operations by type */
int process_op_list(char *type);
	
void wake_up_sync_engine();
	
/* Main entry point to the sync engine */
void start_sync_engine(sqlite3 *db);
void stop_sync_engine();

#if defined(__cplusplus)
}
#endif

#endif