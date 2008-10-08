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

#import <stdlib.h>
#import <sqlite3.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
	sqlite3*		_database;
	/* object_values columns */
	int				_primary_key;
	char*			_attrib;
	int				_source_id;
	char*			_object;
	char*			_value;
	char*			_created_at;
	char*			_updated_at;
	char*			_update_type;
	
	/* state variables */
	int				_hydrated;
	int				_dirty;
} SyncObject;

typedef SyncObject* pSyncObject;
	
/* Finalize (delete) all of the SQLite compiled queries. */
void finalize_statements();
/* Initialize empty sync object */
pSyncObject SyncObjectCreate();
/* Initialize sync object with values */
pSyncObject SyncObjectCreateWithValues(sqlite3* db, int primary_key, 
									   char *attrib, int source_id, char *object, 
									   char *value, char *update_type, int hydrated, int dirty) ;
/* Copy an existing object */
pSyncObject SyncObjectCopy(pSyncObject new_object);
	
/* Cleanup (delete) all of the placeholder objects. */
void cleanup_placeholders(sqlite3* db);
/* Check if object exists in database already */
int exists_in_database(pSyncObject ref);
/* Fetch the current list of query objects from the database */
int fetch_objects_from_database(sqlite3 *database, pSyncObject *db_list);
/* Initialize the row with a placeholder object (to be used by dehydrate later) */
int insert_into_database(pSyncObject ref);
/* Remove the object list from the database */
int delete_all_from_database(sqlite3 *db);

/* Process update types */
void add_delete_type_to_database(pSyncObject ref);
void add_update_type_to_database(pSyncObject ref);
void add_create_type_to_database(pSyncObject ref);
void add_type_to_database(sqlite3_stmt *statement, pSyncObject ref, char *type);

/* Brings the rest of the object data into memory. If already in memory, no action is taken (harmless no-op). */
pSyncObject hydrate(pSyncObject ref);
/* Flushes all but the primary key and title out to the database. */
void dehydrate(pSyncObject ref);

void free_ob_list(pSyncObject *list, int available);
	
/* Free object */
void SyncObjectRelease(pSyncObject ref);

#if defined(__cplusplus)
}
#endif

#endif
