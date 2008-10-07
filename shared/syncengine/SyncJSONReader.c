/*
 *  SyncJSONReader.c
 *  RhoSyncClient
 *
 *  Parses the JSON response for the fetch:
 *  [{"object_value": {
 *      "updated_at": "2008-09-17T22:31:42Z", 
 *      "update_type": "query", 
 *      "id": 1, 
 *      "value": "24/7 Couriers 964608", 
 *      "source_id": 1, 
 *      "object": "24004283442000", 
 *      "created_at": "2008-09-17T22:31:42Z", 
 *      "attrib": "name"}
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

int parse_json_list(pSyncObject *list, char *input, int size) {
	/* Parsing vars */
	struct json_object *json;
	struct array_list *json_list;
	struct lh_entry *entry;
	char *key;
	struct json_object *val;	
	struct lh_entry *sub_entry;
	char *sub_key;
	struct json_object *sub_val;
	int add_to_list = 0;
	pSyncObject current_parse_object;
	
	json = json_tokener_parse(input);
	json_list = json_object_get_array((struct json_object *)json);
	int parsed_size = array_list_length(json_list);
	if (size < parsed_size) parsed_size = size;
	for (int i = 0; i < parsed_size; i++) {
		struct json_object *jsonSync = (struct json_object *) array_list_get_idx(json_list, i);		
		
		for (entry = json_object_get_object(jsonSync)->head; entry; entry = entry->next) {
			key = (char *) entry->k;
			val = (struct json_object *) entry->v;
			
			
			if (strcmp(key, "object_value") == 0) {
				printf("object_value: %s\n",
					   json_object_get_string(val));
				/* Initialize a new SyncObject for each object_value found */
				current_parse_object = (pSyncObject)SyncObjectCreate();
				sub_entry = json_object_get_object(val)->head;
				for (;;) {
					 
					if (sub_entry != NULL && sub_entry->k != NULL) {
						add_to_list = 1;
						sub_key = (char *) sub_entry->k;
						sub_val = (struct json_object *) sub_entry->v;
						if (strcmp(sub_key, "updated_at") == 0) {
							current_parse_object->_updated_at = json_object_get_string(sub_val);
						} else if (strcmp(sub_key, "update_type") == 0) {
							current_parse_object->_update_type = json_object_get_string(sub_val);
						} else if (strcmp(sub_key, "id") == 0) {
							current_parse_object->_primary_key = atoi(json_object_get_string(sub_val));
						} else if (strcmp(sub_key, "value") == 0) {
							current_parse_object->_value = json_object_get_string(sub_val);
						} else if (strcmp(sub_key, "source_id") == 0) {
							current_parse_object->_source_id = atoi(json_object_get_string(sub_val));
						} else if (strcmp(sub_key, "object") == 0) {
							current_parse_object->_object = json_object_get_string(sub_val);
						} else if (strcmp(sub_key, "created_at") == 0) {
							current_parse_object->_created_at = json_object_get_string(sub_val);
						} else if (strcmp(sub_key, "attrib") == 0) {
							current_parse_object->_attrib = json_object_get_string(sub_val);
						} 
					} else {
						/* reached end of entry */
						break;
					}
					
					sub_entry = sub_entry->next; 
				}
				if (add_to_list) {
					list[i] = current_parse_object;
					current_parse_object->_dirty = 1;
					current_parse_object->_hydrated = 0;
					add_to_list = 0;
				}
				
			}
		}
	}
	return parsed_size;
}