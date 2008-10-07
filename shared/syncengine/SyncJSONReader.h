/*
 *  SyncJSONReader.h
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

#ifndef __SYNCJSONREADER__
#define __SYNCJSONREADER__

#include "json.h"
#include "SyncObject.h"
#import <stdio.h>
#import <string.h>

#if defined(__cplusplus)
extern "C" {
#endif
	
int parse_json_list(pSyncObject *list, char *input, int size);


#if defined(__cplusplus)
}
#endif

#endif