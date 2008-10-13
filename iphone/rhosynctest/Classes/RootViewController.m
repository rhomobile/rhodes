/*
 *  RootViewController.m
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


#import "RootViewController.h"
#import "rhosynctestappdelegate.h"
#import "SyncObject.h"
#import "AppDelegateMethods.h"

static EditingViewController *__editingViewController = nil;

@implementation RootViewController

@synthesize detailView;

+ (EditingViewController *)editingViewController {
    // Instantiate the editing view controller if necessary.
    if (__editingViewController == nil) {
        __editingViewController = [[EditingViewController alloc] initWithNibName:@"EditViewController" bundle:nil];
    }
    return __editingViewController;
}

- (id)initWithStyle:(UITableViewStyle)style {
	if (self = [super initWithStyle:style]) {
		self.title = @"Accounts";
        self.tableView.delegate = self;
        self.tableView.dataSource = self;
        self.tableView.rowHeight = 48.0;
        self.tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
        self.tableView.sectionHeaderHeight = 0;
	}
	return self;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    NSUInteger count = [[self appDelegate] countOfList];
	return count;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
	static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithFrame:CGRectZero reuseIdentifier:CellIdentifier] autorelease];
    }    
    // Set up the cell.
	
	SyncObjectWrapper *syncObjectForRow = [[SyncObjectWrapper alloc] init];
	syncObjectForRow = [[self appDelegate] objectInListAtIndex:indexPath.row];
	NSString *text;
	if (syncObjectForRow.wrappedObject->_value) {
		text = [[[NSString alloc] initWithUTF8String:syncObjectForRow.wrappedObject->_value] autorelease];
	} else {
		text = @"";
	}
    cell.text = text;
	[cell setNeedsDisplay];
	
    return cell;
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	SyncObjectWrapper *syncObjectForRow = [[SyncObjectWrapper alloc] init];
	syncObjectForRow = (SyncObjectWrapper *)[[self appDelegate] objectInListAtIndex:indexPath.row];
	
	DetailViewController *objectDetailViewController = [[DetailViewController alloc] initWithSyncObject:syncObjectForRow];
	[[self navigationController] pushViewController:objectDetailViewController animated:YES];
}

// Puts > on each tableView item
- (UITableViewCellAccessoryType)tableView:(UITableView *)tableView accessoryTypeForRowWithIndexPath:(NSIndexPath *)indexPath
{
	return UITableViewCellAccessoryDisclosureIndicator;
}

- (void)viewDidLoad {
	UIBarButtonItem *menuButton = [[[UIBarButtonItem alloc]
								    initWithTitle:@"Menu"
								    style:UIBarButtonItemStyleBordered
								    target:self 
									action:@selector(loadMenu:)] autorelease];
	self.navigationItem.leftBarButtonItem = menuButton;
	self.navigationItem.rightBarButtonItem = self.editButtonItem;
	[super viewDidLoad];
}

- (void)loadMenu:(id)sender {
	SyncViewController *syncViewController = [[SyncViewController alloc] initWithNibName:@"SyncViewController" bundle:nil];
	[[self navigationController] pushViewController:syncViewController animated:YES];
}

- (void)tableView:(UITableView *)tv commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
	rhosynctestappdelegate *appDelegate = [self appDelegate];
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        [appDelegate removeSyncObject:[appDelegate.list objectAtIndex:indexPath.row]];
        // Animate the deletion from the table.
        [self.tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] 
		 withRowAnimation:UITableViewRowAnimationFade];
    }
}

- (rhosynctestappdelegate *) appDelegate {
  rhosynctestappdelegate *appDelegate = (rhosynctestappdelegate *)[[UIApplication sharedApplication] delegate];
  return appDelegate;
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	[self.tableView reloadData];
}


- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}


- (void)viewWillDisappear:(BOOL)animated {
}


- (void)viewDidDisappear:(BOOL)animated {
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}


@end

