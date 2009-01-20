/*
 *  SyncObject.h
 *  RhoSyncClient
 *  Represents a synchronized object
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

#ifndef __SYNCOBJECT__
#define __SYNCOBJECT__

#if !defined(_WIN32_WCE)
#include <sqlite3.h>
#else
#include "sqlite3.h"
#endif

#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
	sqlite3*		_database;
	/* track the database operation to use */
	char*			_db_operation;
	/* object_values columns */
	int				_primary_key;
	char*			_attrib;
	int				_source_id;
	char*			_object;
	char*			_value;
	char*			_update_type;
} SyncObject;

typedef SyncObject* pSyncObject;
	
void finalize_sync_obj_statements();

/* Initialize empty sync object */
pSyncObject SyncObjectCreate();
/* Initialize sync object with values */
pSyncObject SyncObjectCreateWithValues(sqlite3* db, int primary_key, 
									   char *attrib, int source_id, char *object, 
									   char *value, char *update_type);
	
pSyncObject SyncObjectCopy(pSyncObject new_object);
int exists_in_database(pSyncObject ref);
int insert_into_database(pSyncObject ref);
int delete_from_database(pSyncObject ref);
void free_ob_list(pSyncObject *list, int available);
	
/* Free object */
void SyncObjectRelease(pSyncObject ref);

#if defined(__cplusplus)
}
#endif

#endif
