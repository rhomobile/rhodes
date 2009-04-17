#if defined(WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "SyncManagerI.h"
#include "Constants.h"
#include "SyncJSONReader.h"
#include "SyncUtil.h"
#include "SyncBlob.h"
#include "logging/RhoPlainLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SyncUtil"

extern char* fetch_remote_data(char *url);
extern int push_remote_data(char* url, char* data, size_t data_size, char* contentType);
extern void delete_db_session(const char *url_string);
extern char *get_database();
extern char *get_client_id();
extern void lock_sync_mutex();
extern void unlock_sync_mutex();

#if defined(_WIN32_WCE) || defined(WIN32)
extern void delete_winmo_session(const char *url_string);
extern char *get_winmo_session_size(const char *url_string);
#define stricmp _stricmp
#endif

static sqlite3_stmt *op_list_source_ids_statement = NULL;
static sqlite3_stmt *source_params_statement = NULL;
static sqlite3_stmt *ob_count_statement = NULL;
static sqlite3_stmt *client_id_statement = NULL;
static sqlite3_stmt *client_id_insert_statement = NULL;
static sqlite3_stmt *client_db_statement = NULL;
static sqlite3_stmt *sync_status_statement = NULL;
static sqlite3_stmt *session_db_statement = NULL;
static sqlite3_stmt *del_session_db_statement = NULL;
static sqlite3_stmt *del_all_client_info_statement = NULL;
static sqlite3_stmt *del_all_ob_val_statement = NULL;
static sqlite3_stmt *session_url_source_statement=NULL;
static sqlite3_stmt *set_session_db_statement=NULL;
static sqlite3_stmt *update_token_db_statement=NULL;
static sqlite3_stmt *update_all_tokens_db_statement=NULL;

void finalize_sync_util_statements() {
	if (op_list_source_ids_statement) {
		sqlite3_finalize(op_list_source_ids_statement);
		op_list_source_ids_statement = NULL;
	}
	if (source_params_statement) {
		sqlite3_finalize(source_params_statement);
		source_params_statement = NULL;
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
	if (session_url_source_statement) {
		sqlite3_finalize(session_url_source_statement);
		session_url_source_statement = NULL;
	}
	if (set_session_db_statement) {
		sqlite3_finalize(set_session_db_statement);
		set_session_db_statement = NULL;
	}
	if (update_token_db_statement) {
		sqlite3_finalize(update_token_db_statement);
		update_token_db_statement = NULL;
	}
	if (update_all_tokens_db_statement) {
		sqlite3_finalize(update_all_tokens_db_statement);
		update_all_tokens_db_statement = NULL;
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

static void processToken( sqlite_uint64 token, pSource src );
/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
int fetch_remote_changes(sqlite3 *database, char *client_id, pSource src, char *params, int* bStopSync) {
	pSyncObject *list;
	char url_string[4096];
	int j, nTotal = 0;//,i,source_length;
	char *json_string;
	char *type = NULL;
	int success=0, size_deleted=0, size_inserted=0, repeat=1;
	double start=0, duration = 0;
    SyncHeader header;
    int nTry = 0;

    header._count = -1;
    header._token = 0;

    if ( !src->_source_url || strlen(src->_source_url) == 0 )
        return nTotal;

	save_source_url(src->_source_url);
	start = time(NULL);

    list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncObject));
    if ( list )
    {
        do {
	        if (params && strlen(params) > 0) {
				// Don't repeat if we're calling ask method
				repeat=0;
		        sprintf(url_string, 
				        "%s%s%s&client_id=%s&question=%s&p_size=%d", 
				        src->_source_url,
				        SYNC_ASK_ACTION,
				        SYNC_SOURCE_FORMAT, 
				        client_id,
				        params, SYNC_PAGE_SIZE );
	        } else {
		        sprintf(url_string, 
				        "%s%s&client_id=%s&p_size=%d", 
				        src->_source_url,
				        SYNC_SOURCE_FORMAT, 
				        client_id, SYNC_PAGE_SIZE);
	        }
            if ( header._token != 0 ){
                char szToken[30];
#if defined(_WIN32_WCE) || defined(WIN32)
                sprintf(szToken, "&ack_token=%I64u",  header._token );
#else
				sprintf(szToken, "&ack_token=%llu",  header._token );
#endif
                strcat(url_string,szToken);
            }
            
            if ( src->_token == 0 )
                processToken( 1, src );

            header._count = -1;
            header._token = 0;

	        json_string = fetch_remote_data(url_string);
	        if(json_string && strlen(json_string) > 0) {
		        struct json_object* json_to_free = 0;

		        int available = parse_json_list(list, json_string, MAX_SYNC_OBJECTS, &json_to_free, &header);
		        RAWLOG_INFO1("Parsed %i records from sync source...", available);

                processToken( header._token, src );

                nTotal += available;
		        if(available > 0) {

                    char *zErr;
                    int nRes = sqlite3_exec(database, "BEGIN;",0,0,&zErr);
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
						        RAWLOG_INFO1("Warning: received improper update_type: %s...", type);
					        }
				        }
			        }
                    sqlite3_exec(database, "END;",0,0,&zErr);
		        }
		        /* free the in-memory list after populating the database */
		        free_ob_list(list, available);

		        if ( json_to_free )
			        json_object_put(json_to_free);

	            success = 1;
	            free(json_string);

	        } else {
                nTry++;
	        }
        }while( header._count != 0 && nTry < MAX_SYNC_TRY_COUNT && repeat);

        free(list);
    }


	duration = time(NULL) - start;
	update_source_sync_status((sqlite3 *)get_database(), 
							  src, size_inserted, size_deleted, duration, success);
    
    *bStopSync = header._count == -1;
	return nTotal;
}

static void processToken( sqlite_uint64 token, pSource src )
{
    if ( token > 1 && src->_token == token ){
		//Delete non-confirmed records

        delete_from_database_bytoken(src->_source_id, src->_token);
	}else// if ( token != 0 )
    {
        src->_token = token;

		lock_sync_mutex();	
		prepare_db_statement("UPDATE sources SET token=? where source_id=?",
						 (sqlite3 *)get_database(),
						 &update_token_db_statement);
		sqlite3_bind_int64(update_token_db_statement, 1, token);
        sqlite3_bind_int(update_token_db_statement, 2, src->_source_id);
		sqlite3_step(update_token_db_statement); 
		finish_db_statement(&update_token_db_statement);
		unlock_sync_mutex();	
	}
}

/*
 * Pushes changes from list to rhosync server
 */
int push_remote_changes(pSyncOperation *list, int size) {
	char *data;
	int i, retval = 0;
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
	
	retval = push_remote_data(list[0]->_uri,data,data_size,"application/x-www-form-urlencoded");
	free(data);
	
    return retval;// ? SYNC_PUSH_CHANGES_OK : SYNC_PUSH_CHANGES_ERROR;
}

int get_sources_from_database(pSource *list, sqlite3 *database, int max_size) {
	int count = 0;
	lock_sync_mutex();
	prepare_db_statement("SELECT source_id,source_url,token from sources order by source_id", 
						 database, 
						 &op_list_source_ids_statement);
	while(sqlite3_step(op_list_source_ids_statement) == SQLITE_ROW && count < max_size) {
		int id = (int)sqlite3_column_int(op_list_source_ids_statement, 0);
		char *url = (char *)sqlite3_column_text(op_list_source_ids_statement, 1);
        sqlite_uint64 token = sqlite3_column_int64(op_list_source_ids_statement, 2);
		list[count] = SourceCreate(url, id, token);
		count++;
	}
	finish_db_statement(&op_list_source_ids_statement);
	unlock_sync_mutex();	

	return count;
}

char *get_params_for_source(pSource source, sqlite3 *database) {
	char *params;
	char *ask_type = "ask";
	
	lock_sync_mutex();	
	prepare_db_statement("SELECT value,update_type,source_id from object_values \
						 where source_id=? and update_type=?",
						 database,
						 &source_params_statement);
	sqlite3_bind_int(source_params_statement, 1, source->_source_id);
	sqlite3_bind_text(source_params_statement, 2, ask_type, -1, SQLITE_TRANSIENT);
	sqlite3_step(source_params_statement);
	params = str_assign((char *)sqlite3_column_text(source_params_statement, 0));
	RAWLOG_INFO1("params for source: %s",params);
	finish_db_statement(&source_params_statement);
	unlock_sync_mutex();
	
	remove_op_list_from_database(source, database, ask_type);
	return params;
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
	finish_db_statement(&ob_count_statement);
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
		RAWLOG_INFO1("Using client_id %s from database...", c_id);
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
	finish_db_statement(&client_id_statement);
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
	RAWLOG_INFO1("Intialized new client_id %s from source...", c_id);
	finish_db_statement(&client_id_insert_statement);

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
	finish_db_statement(&sync_status_statement);
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
		finish_db_statement(&session_db_statement);

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
						   &session_url_source_statement);
	  while(sqlite3_step(session_url_source_statement) == SQLITE_ROW) {
		  char * sess = (char*)sqlite3_column_text(session_url_source_statement, 0);
		  char *url = (char *)sqlite3_column_text(session_url_source_statement, 1);

      char* szServer2 = parseServerFromUrl(url);
      if ( sess && strlen(sess) > 0 &&
           szServer && szServer2 && strcmp(szServer,szServer2) == 0 ){
        session = str_assign(sess);
        break;
      }
	  }
	  finish_db_statement(&session_url_source_statement);

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
		finish_db_statement(&del_session_db_statement);
		unlock_sync_mutex();	

#if defined(_WIN32_WCE) || defined(WIN32)
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
							 &set_session_db_statement);
		
		sqlite3_bind_text(set_session_db_statement, 1, session, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(set_session_db_statement, 2, source_url, -1, SQLITE_TRANSIENT);
		success = sqlite3_step(set_session_db_statement);
		finish_db_statement(&set_session_db_statement);
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
	RAWLOG_INFO("Resetting sync db...");
	
	prepare_db_statement("delete from object_values",
						 (sqlite3 *)get_database(),
						 &del_all_ob_val_statement);
	prepare_db_statement("delete from client_info",
						 (sqlite3 *)get_database(),
						 &del_all_client_info_statement);
	prepare_db_statement("UPDATE sources SET token=?",
					    (sqlite3 *)get_database(),
					    &update_all_tokens_db_statement);

	RAWLOG_INFO("Deleting all objects from db...");
	sqlite3_step(del_all_ob_val_statement);
	finish_db_statement(&del_all_ob_val_statement);
	RAWLOG_INFO("Deleting client info from db...");
	sqlite3_step(del_all_client_info_statement);
	finish_db_statement(&del_all_client_info_statement);

	RAWLOG_INFO("Clear tokens in source table...");
	sqlite3_bind_int64(update_all_tokens_db_statement, 1, 0);
	sqlite3_step(update_all_tokens_db_statement); 
	finish_db_statement(&update_all_tokens_db_statement);

	unlock_sync_mutex();
}