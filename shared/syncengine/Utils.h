/*
 *  Utils.h
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

#ifndef __UTILS__
#define __UTILS__

#if !defined(_WIN32_WCE)
#include <sqlite3.h>
#else
#include "sqlite3.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif
	
/*
 * TODO: This should be included in stdlib.h?
 */
char* itoa (int n);
	
/* Allocate a string based on size of data */
char* str_assign(char* data);
char* str_assign_ex(char* data,int len);
char* parseServerFromUrl(char* url);

unsigned int DJBHash(char* str, unsigned int len);

void prepare_db_statement(char *sql, sqlite3 *db, sqlite3_stmt **statement);

#if defined(__cplusplus)
}
#endif

#endif