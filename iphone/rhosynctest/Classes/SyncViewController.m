/*
 *  SyncViewController.m
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

#import "SyncViewController.h"
#import "rhosynctestappdelegate.h"
#import "AddViewController.h"
#import "RootViewController.h"
#include "SyncEngine.h"


@implementation SyncViewController


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}

- (void)syncClicked:(id)sender {
	rhosynctestappdelegate *appDelegate = (rhosynctestappdelegate *)[[UIApplication sharedApplication] delegate];
	[appDelegate runSync];
}

- (void)refreshClicked:(id)sender {
	rhosynctestappdelegate *appDelegate = (rhosynctestappdelegate *)[[UIApplication sharedApplication] delegate];
	NSString *message;
	if ([appDelegate runRefresh] > 0) {
		message = @"Error running refresh, server returned 500...";
	} else { 
		message = @"Refresh complete.";
	}
	UIAlertView *myAlertView = [[UIAlertView alloc] initWithTitle:nil message:message
													delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
	[myAlertView show];
	[myAlertView release];
}

- (void)refreshDbClicked:(id)sender {
	rhosynctestappdelegate *appDelegate = (rhosynctestappdelegate *)[[UIApplication sharedApplication] delegate];
	[appDelegate fetchRecordsFromDatabase];
	UIAlertView *myAlertView = [[UIAlertView alloc] initWithTitle:nil message:@"Refreshed database records."
														 delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
	[myAlertView show];
	[myAlertView release];
}

- (void)createClicked:(id)sender {
	AddViewController *controller = [[[AddViewController alloc] initWithNibName:@"AddViewController" bundle:nil] autorelease];
	controller.newObjectValue = [[[SyncObjectWrapper alloc] init] autorelease];
	[self.navigationController presentModalViewController:controller animated:YES];
	[controller setEditing:YES animated:NO];
}

- (void)dealloc {
    [super dealloc];
}


@end
