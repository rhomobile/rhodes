/*
 *  SyncJSONReader.h
 *  RhoSyncClient
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
#include <stdio.h>
#include <string.h>

#if defined(__cplusplus)
extern "C" {
#endif
	
int parse_json_list(pSyncObject *list, char *input, int size, struct json_object** json_to_free);

char *parse_client_id(char *input);


#if defined(__cplusplus)
}
#endif

#endif