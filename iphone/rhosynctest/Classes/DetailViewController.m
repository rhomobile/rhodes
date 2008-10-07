/*
 *  DetailViewController.m
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
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#import "DetailViewController.h"
#import "EditingViewController.h"
#import "RootViewController.h"

@interface DetailViewController()

@property (nonatomic, retain) NSIndexPath *selectedIndexPath;

@end

@implementation DetailViewController

@synthesize tableCellObject, selectedIndexPath;

- (id)initWithSyncObject:(SyncObjectWrapper *)anObject {
	if (self = [super init]) {
		tableCellObject = anObject;
		[self initWithStyle:UITableViewStyleGrouped];
		return self;
	}
	return nil;
}


- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 7;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 1;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
	static NSString *MyIdentifier = @"MyIdentifier";
	
	UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:MyIdentifier];
	if (cell == nil) {
		cell = [[[UITableViewCell alloc] initWithFrame:CGRectMake(0,0,0,0) reuseIdentifier:MyIdentifier] autorelease];
	}
	// Configure the cell
	switch (indexPath.section) {
		case 0: cell.text = [NSString stringWithFormat:@"%d", tableCellObject.wrappedObject->_primary_key]; break;
		case 1: cell.text = [[NSString alloc] initWithUTF8String:tableCellObject.wrappedObject->_attrib]; break;
		case 2: cell.text = [NSString stringWithFormat:@"%d", tableCellObject.wrappedObject->_source_id]; break;
		case 3: cell.text = [[NSString alloc] initWithUTF8String:tableCellObject.wrappedObject->_object]; break;
		case 4: cell.text = [[NSString alloc] initWithUTF8String:tableCellObject.wrappedObject->_value]; break;
		case 5: cell.text = [[NSString alloc] initWithUTF8String:tableCellObject.wrappedObject->_created_at]; break;
		case 6: cell.text = [[NSString alloc] initWithUTF8String:tableCellObject.wrappedObject->_updated_at]; break;
		case 7: cell.text = [[NSString alloc] initWithUTF8String:tableCellObject.wrappedObject->_update_type]; break;
		default: break;			
	}
	return cell;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
    
    NSString *title = nil;
    switch (section) {
		case 0: return NSLocalizedString(@"ID", @"ID section title");
        case 1: return NSLocalizedString(@"Attribute", @"Attribute section title");
		case 2: return NSLocalizedString(@"Source ID", @"Source ID section title");
        case 3: return NSLocalizedString(@"Object", @"Object section title");
		case 4: return NSLocalizedString(@"Value", @"Value section title");
        case 5: return NSLocalizedString(@"Created At", @"Created At section title");
		case 6: return NSLocalizedString(@"Updated At", @"Created At section title");
		case 7: return NSLocalizedString(@"Update Type", @"Update Type section title");
        default: break;
    }
    return title;
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animated {
    [super setEditing:editing animated:animated];
    [self.navigationItem setHidesBackButton:editing animated:animated];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    EditingViewController *controller = [RootViewController editingViewController];
    
    controller.editedObject = tableCellObject;
    switch (indexPath.section) {
		case 4: {
			controller.textValue = [[NSString alloc] initWithUTF8String:tableCellObject.wrappedObject->_value];
			controller.editedFieldKey = @"Value";
			controller.dateEditing = NO;
			controller.wrapper = tableCellObject;
			self.selectedIndexPath = indexPath;
			[self.navigationController pushViewController:controller animated:YES];
		} break;
    }
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}


- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
}


- (void)viewDidDisappear:(BOOL)animated {
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}


- (void)dealloc {
	SyncObjectRelease(tableCellObject.wrappedObject);
	[tableCellObject release];
    [super dealloc];
}

@end

