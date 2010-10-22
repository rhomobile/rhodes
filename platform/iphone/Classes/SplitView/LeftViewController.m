    //
//  TabbedMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "LeftViewController.h"
#import "SimpleMainView.h"
#import "Rhodes.h"
#import "AppManager.h"

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "LeftViewController"



@interface RhoLeftItem : NSObject {
@public
    NSString *url;
    UIImage* image;
    NSString *title;
}

@property (retain) NSString *url;
@property (retain) UIImage *image;
@property (retain) NSString *title;

- (id)init;
- (void)dealloc;

@end

@implementation RhoLeftItem

@synthesize url, image, title;

- (id)init {
    url = nil;
    image = nil;
    title = nil;
    return self;
}

- (void)dealloc {
    [url release];
    [image release];
    [title release];
    [super dealloc];
}

@end



@implementation LeftViewController

@synthesize itemsData;

- (id)initWithItems:(NSArray*)items parent:(SplittedMainView*)parent {
	self = [self initWithStyle:UITableViewStylePlain];
	
    //UITableView* tv = [[UITableView alloc] init];
    //self.tableView = tv;
	
	
	splittedView = parent;

    int count = [items count]/4;
    NSMutableArray *tabs = [[NSMutableArray alloc] initWithCapacity:count];
    
    NSString *initUrl = nil;
    
    for (int i = 0; i < count; ++i) {
        int index = i*4 - 1;
        NSString *label = [items objectAtIndex:++index];
        NSString *url = [items objectAtIndex:++index];
        NSString *icon = [items objectAtIndex:++index];
        NSString *reload = [items objectAtIndex:++index];
        
        if (!initUrl)
            initUrl = url;
        
        if (label && url && icon) {
            RhoLeftItem *td = [[RhoLeftItem alloc] init];
            td.url = url;
			td.title = label;
            
			NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:icon];
			td.image = [UIImage imageWithContentsOfFile:imagePath];
            [tabs addObject:td];
            [td release];
        }
    }
    self.itemsData = tabs;
    [tabs release];
	
	self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight;

	[self.tableView reloadData];
	[self setSelection:0];
	
	return self;
}


- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    // Return the number of rows in the section.
    return [self.itemsData count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
	cell.imageView.image = [[self.itemsData objectAtIndex:indexPath.row] image];
	cell.textLabel.text = [NSString stringWithFormat:[[self.itemsData objectAtIndex:indexPath.row] title], indexPath.section, indexPath.row];
    
    return cell;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // MUST return YES to allow all orientations
    return YES;
}



- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	int selectedItem = indexPath.row;
	[splittedView switchTab:selectedItem];
}


- (void)setSelectionCommand:(NSIndexPath*)index {

	[self.tableView selectRowAtIndexPath:index animated:YES scrollPosition:UITableViewScrollPositionNone];
}

- (void)setSelection:(int)index {
	NSIndexPath* path = [NSIndexPath indexPathForRow:index inSection:0];
	[self performSelectorOnMainThread:@selector(setSelectionCommand:) withObject:path waitUntilDone:NO];	
}

- (int)getPreferredWidth {
	return 320;
}


@end
