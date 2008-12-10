/*
 *  Source.h
 *  rhodes
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

#ifndef __SOURCE__
#define __SOURCE__

#include <stdlib.h>
#include "Utils.h"
#include "SyncObject.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
	int				_source_id;		/* id corresponding to source */
	char*			_source_url;	/* url for the source */
} Source;
	
typedef Source* pSource;
	
pSource SourceCreate(char *source_url, int source_id);
	
void free_source_list(pSource *list, int length);
	
void SourceRelease(pSource source);

#if defined(__cplusplus)
}
#endif

#endif