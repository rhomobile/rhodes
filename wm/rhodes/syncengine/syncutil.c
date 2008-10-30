#include <stdio.h>
#include <stdlib.h>
#include "SyncEngine.h"
#include "SyncJSONReader.h"

extern char* fetch_remote_data(char *url);
extern int push_remote_data(char* url, char* data, size_t data_size);

/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
int fetch_remote_changes(sqlite3 *database) {
  pSyncObject *list;
  char url_string[4096];
  int max_size = 100;
  int i,j,available,source_length,*source_list;
  char *json_string;

  source_list = malloc(max_size*sizeof(int));
  source_length = get_source_ids_from_database(source_list, database, max_size);
  available = 0;
  printf("Iterating over %i sources...\n", source_length);

  /* iterate over each source id and do a fetch */
  for(i = 0; i < source_length; i++) {
    sprintf(url_string, "%s%d%s", SYNC_SOURCE, source_list[i], SYNC_SOURCE_FORMAT);
    printf("url_string: %s\n", url_string);

    json_string = fetch_remote_data(url_string);
    if(json_string) {
      int size = MAX_SYNC_OBJECTS;
      // Initialize parsing list and call json parser
      list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncObject));
      if (list) {
        available = parse_json_list(list, json_string, size);
        printf("Parsed %i records from sync source...\n", available);
        if(available > 0) {
          delete_from_database_by_source(database, source_list[i]);
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

  free(source_list);
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

  return success ? SYNC_PUSH_CHANGES_OK : SYNC_PUSH_CHANGES_ERROR;
}
