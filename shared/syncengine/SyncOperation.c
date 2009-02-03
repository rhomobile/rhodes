/*
 *  SyncOperation.c
 *  RhoSyncClient
 *  Manages connections to rhosync service
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

#include "SyncOperation.h"
#include "Constants.h"
#include "SyncEngine.h"

extern void save_source_url(const char* source_url);
extern char *get_client_id();

static char* attr_format = "attrvals[][%s]=%s";

static sqlite3_stmt *op_list_select_statement = NULL;
static sqlite3_stmt *op_list_delete_statment = NULL;

void finalize_sync_op_statements() {
	if (op_list_select_statement) {
		sqlite3_finalize(op_list_select_statement);
		op_list_select_statement = NULL;
	}
	if (op_list_delete_statment) {
		sqlite3_finalize(op_list_delete_statment);
		op_list_delete_statment = NULL;
	}
}

/*
 * Creates an instance of SyncManager and populates the operation information
 */
pSyncOperation SyncOperationCreate(pSyncObject new_sync_object, char *source, char *operation) {
	pSyncOperation sync = malloc(sizeof(SyncOperation));
	sync->_sync_object = SyncObjectCopy(new_sync_object);
	sync->_operation = str_assign(operation);
	sync->_source_id = sync->_sync_object->_source_id;
	sync->_uri = malloc(SYNC_URI_SIZE);
	sync->_post_body = malloc(POST_BODY_SIZE);
	set_sync_uri(sync, source);
	set_sync_post_body(sync);

	save_source_url(source);
	return sync;
}

/*
 * Generate the sync uri based on values in struct
 */
void set_sync_uri(pSyncOperation sync, char *source) {
	static char temp[SYNC_URI_SIZE];
	
	/* construct the uri */
	sprintf(temp, 
			"%s/%s%s&client_id=%s", 
			source, 
			sync->_operation, 
			SYNC_SOURCE_FORMAT,
			get_client_id());
	strcpy((void *)sync->_uri, temp);
}

/*
 * Construct the body of the request by filtering 
 * the attr_filter string. The body format should
 * look like the following:
 * create: attrvals[][attrib]=<name|industry>&attrvals[][object]=<locallygeneratedid>&attrvals[][value]=<some value>
 * update: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>&attrvals[][value]=<some new value>
 * delete: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>
 */
void set_sync_post_body(pSyncOperation op) {
	char buffer[POST_BODY_SIZE] = "";
	char target[POST_BODY_SIZE] = "";
 	strcpy(buffer, attr_format);
	sprintf(buffer, attr_format, 
			"attrib", (char *)op->_sync_object->_attrib);
	strcpy(target, buffer);
	
	/* check if object exists */
	if (op->_sync_object->_object) {
		strcat(target, "&");
		sprintf(buffer, attr_format,
				"object", (char *)op->_sync_object->_object);
		strcat(target, buffer);
	} 
	
	/* check if value exists */
	if (op->_sync_object->_value) {
		strcat(target, "&");
		sprintf(buffer, attr_format,
				"value", (char *)op->_sync_object->_value);
		strcat(target, buffer);
	}
	
	printf("Formatted post string: %s\n", target);
	strcpy((char *)op->_post_body, target);
}

/* Retrieves the current list of objects for remote processing */
int get_op_list_from_database(pSyncOperation *list, sqlite3* database, int max_count, pSource source, char *type) {
	pSyncOperation current_op;
	int i,count = 0;

  lock_sync_mutex();	

	prepare_db_statement("SELECT attrib, source_id, object, value, update_type \
						 FROM object_values where source_id=? and update_type =?",
						 database,
						 &op_list_select_statement);
	sqlite3_bind_int(op_list_select_statement, 1, source->_source_id);
	sqlite3_bind_text(op_list_select_statement, 2, type, -1, SQLITE_TRANSIENT);
	/* Step through each row and create the sync operation value */
	while(sqlite3_step(op_list_select_statement) == SQLITE_ROW) {
		char *tmp_operation,*tmp_attrib,*tmp_object,*tmp_value;
		int tmp_source_id;
		pSyncObject tmp_sync_object;

		if (count >= max_count) { break; }
		if (strcmp(type, "create") == 0){
			tmp_operation = UPDATE_TYPE_CREATE;
		} else if (strcmp(type, "update") == 0) {
			tmp_operation = UPDATE_TYPE_UPDATE;
		} else if (strcmp(type, "delete") == 0) {
			tmp_operation = UPDATE_TYPE_DELETE;
		}
		tmp_attrib = (char *)sqlite3_column_text(op_list_select_statement, 0);
		tmp_source_id = (int)sqlite3_column_int(op_list_select_statement, 1);
		tmp_object = (char *)sqlite3_column_text(op_list_select_statement, 2);
		tmp_value = (char *)sqlite3_column_text(op_list_select_statement, 3);
		tmp_sync_object = (pSyncObject)SyncObjectCreateWithValues(NULL, -1, tmp_attrib, 
																  tmp_source_id, tmp_object, 
																  tmp_value, type);
		
		current_op = (pSyncOperation)SyncOperationCreate(tmp_sync_object, source->_source_url, tmp_operation);
		list[count] = current_op;
		count++;
	} 
	for (i = 0; i < count; i++) {
		printf("Adding sync operation (attrib, source_id, object, value, update_type, uri): %s, %i, %s, %s, %s, %s\n", 
			   list[i]->_sync_object->_attrib, 
			   list[i]->_sync_object->_source_id, 
			   list[i]->_sync_object->_object, 
			   list[i]->_sync_object->_value,
			   list[i]->_sync_object->_update_type,
			   list[i]->_uri);
	}
	sqlite3_reset(op_list_select_statement);

  unlock_sync_mutex();	

	return count;
}

/* remove the operations from the database after processing */
void remove_op_list_from_database(pSyncOperation *list, sqlite3 *database, char *type) {
  lock_sync_mutex();	

	prepare_db_statement("DELETE FROM object_values WHERE update_type=? and source_id=?",
						 database,
						 &op_list_delete_statment);
	sqlite3_bind_text(op_list_delete_statment, 1, type, -1, SQLITE_TRANSIENT);
	sqlite3_bind_int(op_list_delete_statment, 2, list[0]->_source_id);
	sqlite3_step(op_list_delete_statment);
	sqlite3_reset(op_list_delete_statment);

  unlock_sync_mutex();	
}

void free_op_list(pSyncOperation *list, int available) {
	int j;
	/* Free up our op_list */
	for(j = 0; j < available; j++) {
		SyncOperationRelease(list[j]);
	}
}

/* 
 * Releases the current SyncManager instance
 */
void SyncOperationRelease(pSyncOperation sync) {
	if (sync != NULL) {
		if (sync->_sync_object != NULL) {
			SyncObjectRelease(sync->_sync_object);
		}
		free(sync->_post_body);
		free(sync->_uri);
		free(sync->_operation);
		free(sync);
	}
}
