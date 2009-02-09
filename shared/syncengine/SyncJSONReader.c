/*
 *  SyncJSONReader.c
 *  RhoSyncClient
 *
 *  Parses the JSON response from the fetch:
 *  [{"object_value": {
 *      "update_type": "query", 
 *      "id": 1, 
 *      "value": "24/7 Couriers 964608", 
 *      "source_id": 1, 
 *      "object": "24004283442000", 
 *      "attrib": "name",
 *      "db_operation": "insert"}
 *  }]
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

#include "SyncJSONReader.h"
#include "Utils.h"

int parse_json_list(pSyncObject *list, char *input, int size, struct json_object** json_to_free) {
	/* Parsing vars */
	struct json_object *json;
	struct array_list *json_list;
	struct lh_entry *entry;
	char *key;
	struct json_object *val;	
	struct lh_entry *sub_entry;
	char *sub_key;
	struct json_object *sub_val;
	int i, parsed_size, add_to_list = 0;
	pSyncObject current_parse_object;
	
	json = json_tokener_parse(input);
	if ( !json ) return 0;
    if ( json_to_free )
        *json_to_free = json;

	json_list = json_object_get_array((struct json_object *)json);
	parsed_size = array_list_length(json_list);
	if (size < parsed_size) parsed_size = size;
	for (i = 0; i < parsed_size; i++) {
		struct json_object *jsonSync = (struct json_object *) array_list_get_idx(json_list, i);		
		
		for (entry = json_object_get_object(jsonSync)->head; entry; entry = entry->next) {
			key = (char *) entry->k;
			val = (struct json_object *) entry->v;
			if (strcmp(key, "object_value") == 0) {
				/* Initialize a new SyncObject for each object_value found */
				current_parse_object = (pSyncObject)SyncObjectCreate();
				
				for (sub_entry = json_object_get_object(val)->head;
					 sub_entry != NULL && sub_entry->k != NULL;
					 sub_entry = sub_entry->next) {
					add_to_list = 1;
					sub_key = (char *) sub_entry->k;
					sub_val = (struct json_object *) sub_entry->v;
					if (strcmp(sub_key, "update_type") == 0) {
						current_parse_object->_update_type = json_object_get_string(sub_val);
					} else if (strcmp(sub_key, "id") == 0) {
						current_parse_object->_primary_key = atoi(json_object_get_string(sub_val));
					} else if (strcmp(sub_key, "value") == 0) {
						current_parse_object->_value = json_object_get_string(sub_val);
					} else if (strcmp(sub_key, "source_id") == 0) {
						current_parse_object->_source_id = atoi(json_object_get_string(sub_val));
					} else if (strcmp(sub_key, "object") == 0) {
						current_parse_object->_object = json_object_get_string(sub_val);
					} else if (strcmp(sub_key, "attrib") == 0) {
						current_parse_object->_attrib = json_object_get_string(sub_val);
					} else if (strcmp(sub_key, "db_operation") == 0) {
						current_parse_object->_db_operation = json_object_get_string(sub_val);
					}
				}
				if (add_to_list) {
					list[i] = current_parse_object;
					add_to_list = 0;
				}
				
			}
		}
	}

	return parsed_size;
}

char *parse_client_id(char *input) {
	struct json_object *json;
	struct lh_entry *entry;
	char *key;
	struct json_object *val;	
	struct lh_entry *sub_entry;
	char *sub_key;
	struct json_object *sub_val;
	char *c_id;
	
	json = json_tokener_parse(input);
	for (entry = json_object_get_object(json)->head; entry != NULL; entry = entry->next) {
		if(entry) {
			key = (char *) entry->k;
			val = (struct json_object *) entry->v;
			if (strcmp(key, "client") == 0) {
				for (sub_entry = json_object_get_object(val)->head; 
					 sub_entry != NULL && sub_entry->k != NULL; 
					 sub_entry = sub_entry->next) {
					
					sub_key = (char *) sub_entry->k;
					sub_val = (struct json_object *) sub_entry->v;
					if (strcmp(sub_key, "client_id") == 0) {
						c_id = str_assign(json_object_get_string(sub_val));
					}
				}
			}
		}
	}
    if ( json )
        json_object_put(json);

	return c_id;
}
