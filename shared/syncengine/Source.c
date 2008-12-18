/*
 *  Source.c
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

#include "Source.h"

pSource SourceCreate(char *source_url, int source_id) {
	pSource source = malloc(sizeof(Source));
	source->_source_url = str_assign(source_url);
	source->_source_id = source_id;
	return source;
}

void free_source_list(pSource *list, int length) {
	int i;
	for(i = 0; i < length; i++) {
		SourceRelease(list[i]);
	}
	free(list);
}

void SourceRelease(pSource source) {
	if(source != NULL) {
		if(source->_source_url != NULL) {
			free(source->_source_url);
		}
		free(source);
	}
}