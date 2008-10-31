#include <stdio.h>
#include <stdlib.h>
#include "SyncManagerI.h"
#include "Constants.h"
#include "SyncJSONReader.h"
#include "SyncUtil.h"

extern char* fetch_remote_data(char *url);
extern int push_remote_data(char* url, char* data, size_t data_size);

static sqlite3_stmt *op_list_source_ids_statement = NULL;

void finalize_src_statements() {
	if (op_list_source_ids_statement) sqlite3_finalize(op_list_source_ids_statement);
}

pSource SourceCreate(char *source_url, int source_id) {
	pSource source = malloc(sizeof(Source));
	source->_source_url = str_assign(source_url);
	source->_source_id = source_id;
	return source;
}

/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
int fetch_remote_changes(sqlite3 *database) {
	pSyncObject *list;
	char url_string[4096];
	int max_size = 100;
	int i,j,available,source_length;
	char *json_string;
	
	pSource *source_list;
	source_list = malloc(max_size*sizeof(pSource));
	
	source_length = get_source_urls_from_database(source_list, database, max_size);
	available = 0;
	printf("Iterating over %i sources...\n", source_length);
	
	/* iterate over each source id and do a fetch */
	for(i = 0; i < source_length; i++) {
		sprintf(url_string, "%s%s", source_list[i]->_source_url, SYNC_SOURCE_FORMAT);
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
					delete_from_database_by_source(database, source_list[i]->_source_id);
					for(j = 0; j < available; j++) {
						list[j]->_database = database;
						insert_into_database(list[j]);
						dehydrate(list[j]);
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
	int i, success;
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

int get_source_urls_from_database(pSource *list, sqlite3 *database, int max_size) {
	int count = 0;
	if (op_list_source_ids_statement == NULL) {
		const char *sql = "SELECT source_id,source_url from sources";
		if (sqlite3_prepare_v2(database, sql, -1, &op_list_source_ids_statement, NULL) != SQLITE_OK) {
			printf("Error: failed to prepare statement with message '%s'.", sqlite3_errmsg(database));
		}
		while(sqlite3_step(op_list_source_ids_statement) == SQLITE_ROW && count < max_size) {
			int id = (int)sqlite3_column_int(op_list_source_ids_statement, 0);
			char *url = (char *)sqlite3_column_text(op_list_source_ids_statement, 1);
			list[count] = SourceCreate(url, id);
			count++;
		}
		sqlite3_reset(op_list_source_ids_statement);
		sqlite3_finalize(op_list_source_ids_statement);
		op_list_source_ids_statement = NULL;
	}
	return count;
}

void free_source_list(pSource *list, int length) {
	int i;
	for(i = 0; i < length; i++) {
		SourceRelease(list[i]);
	}
	free(list);
}

void SourceRelease(pSource source) {
	if(source) {
		free(source->_source_url);
		free(source);
	}
}
