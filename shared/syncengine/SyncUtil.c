#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "SyncManagerI.h"
#include "Constants.h"
#include "SyncJSONReader.h"
#include "SyncUtil.h"

extern char* fetch_remote_data(char *url);
extern int push_remote_data(char* url, char* data, size_t data_size);
extern char *get_session(const char *url_string);
extern void delete_db_session(const char *url_string);
extern char *get_database();
extern char *get_client_id();
extern void lock_sync_mutex();
extern void unlock_sync_mutex();

#if defined(_WIN32_WCE)
extern void delete_winmo_session(const char *url_string);
extern char *get_winmo_session_size(const char *url_string);
#define stricmp _stricmp
#endif

static sqlite3_stmt *op_list_source_ids_statement = NULL;
static sqlite3_stmt *ob_count_statement = NULL;
static sqlite3_stmt *client_id_statement = NULL;
static sqlite3_stmt *client_id_insert_statement = NULL;
static sqlite3_stmt *client_session_statement = NULL;
static sqlite3_stmt *client_db_statement = NULL;
static sqlite3_stmt *sync_status_statement = NULL;
static sqlite3_stmt *session_db_statement = NULL;
static sqlite3_stmt *del_session_db_statement = NULL;
static sqlite3_stmt *del_all_client_info_statement = NULL;
static sqlite3_stmt *del_all_ob_val_statement = NULL;

void finalize_sync_util_statements() {
	if (op_list_source_ids_statement) {
		sqlite3_finalize(op_list_source_ids_statement);
		op_list_source_ids_statement = NULL;
	}
	if (ob_count_statement) {
		sqlite3_finalize(ob_count_statement);
		ob_count_statement = NULL;
	}
	if (client_id_statement) {
		sqlite3_finalize(client_id_statement);
		client_id_statement = NULL;
	}
	if (client_id_insert_statement) {
		sqlite3_finalize(client_id_insert_statement);
		client_id_insert_statement = NULL;
	}
	if (client_session_statement) {
		sqlite3_finalize(client_session_statement);
		client_session_statement = NULL;
	}
	if (client_db_statement) {
		sqlite3_finalize(client_db_statement);
		client_db_statement = NULL;
	}
	if (sync_status_statement) {
		sqlite3_finalize(sync_status_statement);
		sync_status_statement = NULL;
	}
	if (session_db_statement) {
		sqlite3_finalize(session_db_statement);
		session_db_statement = NULL;
	}
	if (del_session_db_statement) {
		sqlite3_finalize(del_session_db_statement);
		del_session_db_statement = NULL;
	}
	if (del_all_ob_val_statement) {
		sqlite3_finalize(del_all_ob_val_statement);
		del_all_ob_val_statement = NULL;
	}
	if (del_all_client_info_statement) {
		sqlite3_finalize(del_all_client_info_statement);
		del_all_client_info_statement = NULL;
	}
}

/**
 * This global buffer will store source url between requests
 * It can be used by other routines to understand what is the original source url. To avoid parsing etc...
 */
static char g_source_url[1024] = {0};

void save_source_url(const char* source_url)
{
	int size = 0; 
	
	memset( g_source_url, 0, sizeof(g_source_url));
	
	if ( source_url )
	{
		size = strlen(source_url);
		
		if ( (size + 1 < sizeof(g_source_url) ) )
			memcpy( (void*)g_source_url, (const void*)source_url, size);
	}
}

const char* load_source_url()
{
	return g_source_url;
}

/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
int fetch_remote_changes(sqlite3 *database, char *client_id, pSource src) {
	pSyncObject *list;
	char url_string[4096];
	int j,available;//,i,source_length;
	char *json_string;
	char *type = NULL;
	
	//pSource *source_list;
	//source_list = malloc(MAX_SOURCES*sizeof(pSource));
	
	//source_length = get_sources_from_database(source_list, database, MAX_SOURCES);
	available = 0;
	//printf("Iterating over %i sources...\n", source_length);
	
	/* iterate over each source id and do a fetch */
	//for(i = 0; i < source_length; i++) 
  {
		int success=0, size_deleted=0, size_inserted=0;
		double start=0,duration = 0;
		save_source_url(src->_source_url);
		sprintf(url_string, 
				"%s%s&client_id=%s", 
				src->_source_url, 
				SYNC_SOURCE_FORMAT, 
				client_id);
		start = time(NULL);
		json_string = fetch_remote_data(url_string);
		if(json_string && strlen(json_string) > 0) {
			int size = MAX_SYNC_OBJECTS;
			//printf("JSON data: %s\n", json_string);
			// Initialize parsing list and call json parser
			list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncObject));
			if (list) {
                struct json_object* json_to_free = 0;
				available = parse_json_list(list, json_string, size, &json_to_free);
				printf("Parsed %i records from sync source...\n", available);
				if(available > 0) {
					for(j = 0; j < available; j++) {
						list[j]->_database = database;
						type = list[j]->_db_operation;
						if (type) {
							if(strcmp(type, "insert") == 0) {
								/*printf("Inserting record %i - %s - %s: %s\n", 
									   list[j]->_primary_key, list[j]->_object, 
									   list[j]->_attrib, list[j]->_value);*/
								insert_into_database(list[j]);
								size_inserted++;
							} 
							else if (strcmp(type, "delete") == 0) {
								/*printf("Deleting record %i\n", list[j]->_primary_key);*/
								delete_from_database(list[j]);
								size_deleted++;
							} else {
								printf("Warning: received improper update_type: %s...\n", type);
							}
						}
					}
				}
				/* free the in-memory list after populating the database */
				free_ob_list(list, available);
				free(list);

                if ( json_to_free )
                    json_object_put(json_to_free);
			}
			success = 1;
			free(json_string);
		} else {
			success = 0;
		}
		duration = time(NULL) - start;
		update_source_sync_status((sqlite3 *)get_database(), 
								  src, size_inserted, size_deleted, duration, success);
	}
	//free_source_list(source_list, source_length);
    
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
  lock_sync_mutex();

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

  unlock_sync_mutex();	

	return count;
}

int get_object_count_from_database(sqlite3 *database) {

	int count = 0;
	int success = 0;
	
    lock_sync_mutex();	

	prepare_db_statement("SELECT count(*) from object_values",
						 database,
						 &ob_count_statement);
	success = sqlite3_step(ob_count_statement);
	if (success == SQLITE_ROW) {
		count = sqlite3_column_int(ob_count_statement, 0);
	}
	sqlite3_reset(ob_count_statement);

    unlock_sync_mutex();	
    
	return count;
}

/* setup client id from database, otherwise intialize from source */
char *set_client_id(sqlite3 *database, pSource source) {
	char *json_string;
	char url_string[4096];
	char *c_id = NULL;

	lock_sync_mutex();	

	prepare_db_statement("SELECT client_id from client_info limit 1",
						 database,
						 &client_id_statement);
	sqlite3_step(client_id_statement);
	c_id = str_assign((char *)sqlite3_column_text(client_id_statement, 0));
	unlock_sync_mutex();	

	if (c_id != NULL && strlen(c_id) > 0) {
		printf("Using client_id %s from database...\n", c_id);
	} else {
		sprintf(url_string, "%s/clientcreate%s", source->_source_url, SYNC_SOURCE_FORMAT);
		json_string = fetch_remote_data(url_string);
		if(json_string && strlen(json_string) > 0) {
			c_id = str_assign((char *)parse_client_id(json_string));
            free(json_string);
		}
    set_db_client_id(database,c_id);
	}
	lock_sync_mutex();	
	sqlite3_reset(client_id_statement);
	unlock_sync_mutex();	

	return c_id;
}

void set_db_client_id( sqlite3 *database, char *c_id ){
	lock_sync_mutex();	

	prepare_db_statement("INSERT INTO client_info (client_id) values (?)",
						 database,
						 &client_id_insert_statement);
	sqlite3_bind_text(client_id_insert_statement, 1, c_id, -1, SQLITE_TRANSIENT);
	sqlite3_step(client_id_insert_statement);
	printf("Intialized new client_id %s from source...\n", c_id);
	sqlite3_reset(client_id_insert_statement);

	unlock_sync_mutex();	
}

void update_source_sync_status(sqlite3 *database, pSource source, 
							   int num_inserted, int num_deleted, double sync_duration, int status) {
	time_t now_in_seconds;
	now_in_seconds = time(NULL);
	
	lock_sync_mutex();	

	prepare_db_statement("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, \
						 last_sync_duration=?,last_sync_success=? WHERE source_id=?",
						 database,
						 &sync_status_statement);
	sqlite3_bind_int(sync_status_statement, 1, now_in_seconds);
	sqlite3_bind_int(sync_status_statement, 2, num_inserted);
	sqlite3_bind_int(sync_status_statement, 3, num_deleted);
	sqlite3_bind_double(sync_status_statement, 4, sync_duration);
	sqlite3_bind_int(sync_status_statement, 5, status);
	sqlite3_bind_int(sync_status_statement, 6, source->_source_id);
	sqlite3_step(sync_status_statement); 
	sqlite3_reset(sync_status_statement);

  unlock_sync_mutex();	
}

/**
 * Retrieve cookie from database storage
 */
char *get_db_session(const char* source_url) {
	char *session = NULL;
	
	if ( source_url )
	{
    lock_sync_mutex();	

		prepare_db_statement("SELECT session FROM sources WHERE source_url=?",
						     (sqlite3 *)get_database(),
							 &session_db_statement);
		sqlite3_bind_text(session_db_statement, 1, source_url, -1, SQLITE_TRANSIENT);
		sqlite3_step(session_db_statement);
		
		session = str_assign((char *)sqlite3_column_text(session_db_statement, 0));
		sqlite3_reset(session_db_statement);

    unlock_sync_mutex();	

	}
	return session;
}

char *get_db_session_by_server(char* source_url) {
	char *session = NULL;
	
	if ( source_url )
	{
    char* szServer = parseServerFromUrl(source_url);
    lock_sync_mutex();	

	  prepare_db_statement("SELECT session,source_url from sources", 
						   (sqlite3 *)get_database(),
						   &op_list_source_ids_statement);
	  while(sqlite3_step(op_list_source_ids_statement) == SQLITE_ROW) {
		  char * sess = (char*)sqlite3_column_text(op_list_source_ids_statement, 0);
		  char *url = (char *)sqlite3_column_text(op_list_source_ids_statement, 1);

      char* szServer2 = parseServerFromUrl(url);
      if ( sess && strlen(sess) > 0 &&
           szServer && szServer2 && strcmp(szServer,szServer2) == 0 ){
        session = str_assign(sess);
        break;
      }
	  }
	  sqlite3_reset(op_list_source_ids_statement);

    unlock_sync_mutex();	
	}

	return session;
}

void delete_db_session( const char* source_url )
{
	if ( source_url )
	{
    lock_sync_mutex();	
		prepare_db_statement("UPDATE sources SET session=NULL where source_url=?",
						 (sqlite3 *)get_database(),
						 &del_session_db_statement);
		sqlite3_bind_text(del_session_db_statement, 1, source_url, -1, SQLITE_TRANSIENT);
		sqlite3_step(del_session_db_statement); 
		sqlite3_reset(del_session_db_statement);

    unlock_sync_mutex();	

#if defined(_WIN32_WCE)
		// Delete from winmo cookies
		delete_winmo_session(source_url);
#endif
	}
}
/**
 * Save cookie to the database storage
 */
int set_db_session(const char* source_url, const char *session) {

	int success = 0;
	
	if ( source_url && session )
	{
		lock_sync_mutex();	

		prepare_db_statement("UPDATE sources SET session=? WHERE source_url=?",
							 (sqlite3 *)get_database(),
							 &session_db_statement);
		
		sqlite3_bind_text(session_db_statement, 1, session, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(session_db_statement, 2, source_url, -1, SQLITE_TRANSIENT);
		success = sqlite3_step(session_db_statement);
		sqlite3_reset(session_db_statement);

		unlock_sync_mutex();
	}
	return success;
}

/**
 * Reset sync db
 * (deletes all object_values and client_info rows)
 */
void reset_sync_db() {
	lock_sync_mutex();	
	printf("Resetting sync db...\n");
	
	prepare_db_statement("delete from object_values",
						 (sqlite3 *)get_database(),
						 &del_all_ob_val_statement);
	prepare_db_statement("delete from client_info",
						 (sqlite3 *)get_database(),
						 &del_all_client_info_statement);
	printf("Deleting all objects from db...\n");
	sqlite3_step(del_all_ob_val_statement);
	sqlite3_reset(del_all_ob_val_statement);
	printf("Deleting client info from db...\n");
	sqlite3_step(del_all_client_info_statement);
	sqlite3_reset(del_all_client_info_statement);
	
	unlock_sync_mutex();
}

