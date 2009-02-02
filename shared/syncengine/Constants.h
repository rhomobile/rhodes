/*
 *  Constants.h
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

#ifndef __CONSTANTS__
#define __CONSTANTS__

#if defined(__cplusplus)
extern "C" {
#endif

/* Define sync source */
#define SYNC_SOURCE_FORMAT	"?format=json"

/* Sync operation types */
#define UPDATE_TYPE_UPDATE "updateobjects"
#define UPDATE_TYPE_CREATE "createobjects"
#define UPDATE_TYPE_DELETE "deleteobjects"

#define MAX_SYNC_OBJECTS 2000//100000
#define MAX_SOURCES 100

/* Sync Object db codes */
#define SYNC_OBJECT_SUCCESS 0
#define SYNC_OBJECT_DUPLICATE 1
#define SYNC_OBJECT_ERROR 2
	
/* Sync Engine */
#define MAX_SINGLE_OP_SIZE 200
#define SYNC_FETCH_CHANGES_OK 0
#define SYNC_FETCH_CHANGES_ERROR 1
#define SYNC_PUSH_CHANGES_OK 0
#define SYNC_PUSH_CHANGES_ERROR 1

#if defined(__cplusplus)
}
#endif

#endif
