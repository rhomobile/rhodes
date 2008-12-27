#include <stdio.h>
#include <stdlib.h>
#include "SyncManagerI.h"
#include "Constants.h"
#include "SyncJSONReader.h"
#include "SyncUtil.h"

extern char* fetch_remote_data(char *url);
extern int push_remote_data(char* url, char* data, size_t data_size);

static sqlite3_stmt *op_list_source_ids_statement = NULL;
static sqlite3_stmt *ob_count_statement = NULL;
static sqlite3_stmt *client_id_statement = NULL;

void finalize_sync_util_statements() {
	if (op_list_source_ids_statement) sqlite3_finalize(op_list_source_ids_statement);
	if (ob_count_statement) sqlite3_finalize(ob_count_statement);
	if (client_id_statement) sqlite3_finalize(client_id_statement);
}

/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
int fetch_remote_changes(sqlite3 *database, char *client_id) {
	pSyncObject *list;
	char url_string[4096];
	int max_size = 100;
	int i,j,available,source_length;
	char *json_string;
	char *type = "noop";
	
	pSource *source_list;
	source_list = malloc(max_size*sizeof(pSource));
	
	source_length = get_sources_from_database(source_list, database, max_size);
	available = 0;
	printf("Iterating over %i sources...\n", source_length);
	
	/* iterate over each source id and do a fetch */
	for(i = 0; i < source_length; i++) {
		sprintf(url_string, 
				"%s%s&client_id=%s", 
				source_list[i]->_source_url, 
				SYNC_SOURCE_FORMAT, 
				client_id);
		printf("url_string: %s\n", url_string);
		
		json_string = fetch_remote_data(url_string);
		if(json_string && strlen(json_string) > 0) {
			int size = MAX_SYNC_OBJECTS;
			// Initialize parsing list and call json parser
			list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncObject));
			if (list) {
				available = parse_json_list(list, json_string, size);
				printf("Parsed %i records from sync source...\n", available);
				if(available > 0) {
					//delete_from_database_by_source(database, source_list[i]->_source_id);
					for(j = 0; j < available; j++) {
						list[j]->_database = database;
						//insert_into_database(list[j]);
						type = list[j]->_db_operation;
						if (type) {
							if(strcmp(type, "insert") == 0) {
								printf("Inserting record %s\n...", list[j]->_object);
								insert_into_database(list[j]);
							} 
							else if (strcmp(type, "delete") == 0) {
								printf("Deleting record %s\n...", list[j]->_object);
								delete_from_database(list[j]);
							} else {
								printf("Warning: received improper update_type: %s...\n", type);
							}
						}
					}
				}
				/* free the in-memory list after populating the database */
				free_ob_list(list, available);
				free(list);
			}
			free(json_string);
		}
	}
	
	free_source_list(source_list, source_length);
	printf("fetch remote changes done\n");
	return available;
}

/*
 * Pushes changes from list to rhosync server
 */
int push_remote_changes(pSyncOperation *list, int size) {
	char *data;
	int i, success = 0;
	size_t data_size = 0;
	
	if (size == 0) return SYNC_PUSH_CHANGES_OK;
	
	for (i = 0; i < size; i++) {
		data_size += strlen(list[i]->_post_body);
		if (i != (size - 1)) {
			data_size += 1;
		}
	}
	
	if (!data_size) return SYNC_PUSH_CHANGES_OK; 
	
	data = malloc(data_size+1);
	*data = 0;
	
	for (i = 0; i < size; i++) {
		strcat(data,list[i]->_post_body);
		if (i != (size - 1)) {
			strcat(data,"&");
		}
	}
	
	success = push_remote_data(list[0]->_uri,data,data_size);
	free(data);
	
	return success == SYNC_PUSH_CHANGES_OK ? SYNC_PUSH_CHANGES_OK : SYNC_PUSH_CHANGES_ERROR;
}

int get_sources_from_database(pSource *list, sqlite3 *database, int max_size) {
	int count = 0;
	prepare_db_statement("SELECT source_id,source_url from sources", 
						 database, 
						 &op_list_source_ids_statement);
	while(sqlite3_step(op_list_source_ids_statement) == SQLITE_ROW && count < max_size) {
		int id = (int)sqlite3_column_int(op_list_source_ids_statement, 0);
		char *url = (char *)sqlite3_column_text(op_list_source_ids_statement, 1);
		list[count] = SourceCreate(url, id);
		count++;
	}
	sqlite3_reset(op_list_source_ids_statement);
	return count;
}

int get_object_count_from_database(sqlite3 *database) {
	int count = 0;
	int success = 0;
	prepare_db_statement("SELECT count(*) from object_values",
						 database,
						 &ob_count_statement);
	success = sqlite3_step(ob_count_statement);
	if (success == SQLITE_ROW) {
		count = sqlite3_column_int(ob_count_statement, 0);
	}
	sqlite3_reset(ob_count_statement);
	return count;
}

/* setup client id from database, otherwise intialize from source */
char *get_client_id(sqlite3 *database, pSource source) {
	char *json_string;
	char url_string[4096];
	char *c_id = NULL;
	prepare_db_statement("SELECT client_id from client_info limit 1",
						 database,
						 &client_id_statement);
	sqlite3_step(client_id_statement);
	c_id = str_assign((char *)sqlite3_column_text(client_id_statement, 0));
	if (c_id != NULL) {
		printf("Using client_id %s from database...\n", c_id);
	} else {
		sqlite3_reset(client_id_statement);
		sprintf(url_string, "%s/clientcreate%s", source->_source_url, SYNC_SOURCE_FORMAT);
		json_string = fetch_remote_data(url_string);
		if(json_string && strlen(json_string) > 0) {
			c_id = str_assign((char *)parse_client_id(json_string));
		}
		prepare_db_statement("INSERT INTO client_info (client_id) values (?)",
							 database,
							 &client_id_statement);
		sqlite3_bind_text(client_id_statement, 1, c_id, -1, SQLITE_TRANSIENT);
		sqlite3_step(client_id_statement);
		printf("Intialized new client_id %s from source...\n", c_id);
	}
	sqlite3_reset(client_id_statement);
	return c_id;
}