#include <stdio.h>
#include <stdlib.h>
#include "SyncEngine.h"
#include "SyncJSONReader.h"

extern char* fetch_remote_data(char *url);
extern int push_remote_data(char* url, char* data, size_t data_size);

int fetch_remote_changes(pSyncObject *list) {
	char *json_string, url_string[4096];
	strcpy(url_string, SYNC_SOURCE);
	strcat(url_string, SYNC_SOURCE_FORMAT);

  json_string = fetch_remote_data(url_string);
  if (json_string) {
		int size = MAX_SYNC_OBJECTS;
		// Initialize parsing list and call json parser
		int available = parse_json_list(list, json_string, size);
		printf("Parsed %i records from sync source...\n", available);
    free(json_string);
  }

  return 0;
}

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
