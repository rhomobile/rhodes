/*
 *  SyncObjectWrapper.m
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

#import "SyncObjectWrapper.h"
#include "Utils.h"

@implementation SyncObjectWrapper

- (id)init {
	if(self = [super init]) {
		_wrappedObject = SyncObjectCreate();
	}
	return self;
}

#pragma mark -

- (pSyncObject)wrappedObject {
	return _wrappedObject;
}

- (void)setWrappedObject:(pSyncObject)newWrappedObject {
	if (_wrappedObject) { SyncObjectRelease(_wrappedObject); }
	_wrappedObject = newWrappedObject;
}

- (void)setValue:(NSString *)value {
	_wrappedObject->_value = str_assign((char *)[value UTF8String]);
}

- (void)wrappedUpdate {
	add_update_type_to_database(_wrappedObject);
}

- (void)wrappedAdd {
	add_create_type_to_database(_wrappedObject);
}

- (void)dealloc {
	SyncObjectRelease(_wrappedObject);
	[super dealloc];
}

@end
