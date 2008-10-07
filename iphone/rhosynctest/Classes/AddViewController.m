/*
 *  AddViewController.m
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

#import "AddViewController.h"

@implementation AddViewController

@synthesize newObjectValue, newObjectIndustry;

- (void)viewDidLoad {
    // Override the DetailViewController viewDidLoad with different navigation bar items and title
    self.title = @"New Object";
    self.navigationItem.leftBarButtonItem = [[[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel 
																						   target:self action:@selector(cancel:)] autorelease];
    self.navigationItem.rightBarButtonItem = [[[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemSave 
																							target:self action:@selector(save:)] autorelease];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (IBAction)cancel:(id)sender {
    [self.navigationController dismissModalViewControllerAnimated:YES];
}

- (IBAction)save:(id)sender {
	rhosynctestappdelegate *appDelegate = (rhosynctestappdelegate *)[[UIApplication sharedApplication] delegate];
	newObjectValue = [[SyncObjectWrapper alloc] init];
	newObjectIndustry = [[SyncObjectWrapper alloc] init];
	newObjectValue.wrappedObject->_value = str_assign((char *)[newValue.text UTF8String]);
	newObjectValue.wrappedObject->_attrib = str_assign("name");
	newObjectValue.wrappedObject->_database = [appDelegate getDatabase];
	newObjectValue.wrappedObject->_source_id = 1;
	newObjectIndustry.wrappedObject->_value = str_assign((char *)[newIndustry.text UTF8String]);
	newObjectIndustry.wrappedObject->_attrib = str_assign("industry");
	newObjectIndustry.wrappedObject->_database = [appDelegate getDatabase];
	newObjectIndustry.wrappedObject->_source_id = 1;
    [newObjectValue wrappedAdd];
	[newObjectIndustry wrappedAdd];
	[newObjectValue dealloc];
	[newObjectIndustry dealloc];
    [self.navigationController dismissModalViewControllerAnimated:YES];
}

- (void)dealloc {
    [super dealloc];
}

@end
