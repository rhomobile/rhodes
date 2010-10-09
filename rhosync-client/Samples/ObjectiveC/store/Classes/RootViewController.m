//
//  RootViewController.m
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "RootViewController.h"
#import "SyncEngine.h"

#define ROW_HEIGHT 40
#define CELL_WIDTH 320.0
#define LABEL_HEIGHT 20

//Lets define the dimensions of the two columns
//Set the column offset and the width.
#define NAME_OFFSET 10.0
#define NAME_WIDTH 150.0
#define NAME_TAG 1

#define TEXT_OFFSET 160.0
#define TEXT_WIDTH 160.0
#define TEXT_TAG 2


@implementation RootViewController

@synthesize CurrentLevel;
@synthesize CurrentTitle;
@synthesize arItems;

#pragma mark -
#pragma mark View lifecycle


- (void)viewDidLoad {
    [super viewDidLoad];


	if(CurrentLevel == 0) {
		self.navigationItem.title = @"Home";
		self.navigationItem.hidesBackButton = true;
	} else {
		self.navigationItem.title = CurrentTitle;
		self.navigationItem.hidesBackButton = false;
	}

    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

/*
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}
*/
/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
}
*/
/*
- (void)viewDidDisappear:(BOOL)animated {
	[super viewDidDisappear:animated];
}
*/

/*
 // Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	// Return YES for supported orientations.
	return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
 */


#pragma mark -
#pragma mark Table view data source

// Customize the number of sections in the table view.
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section 
{
	if (!arItems)
		arItems = [[SyncEngine sharedInstance].product find_all:nil];
	
	//warning here because this may be dictionary or an array when called.
    return (NSUInteger)[arItems count];
}

// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    static NSString *ColumnCellIdentifier = @"ColumnCell";
    
    UITableViewCell *cell; 
    
	if([arItems isKindOfClass:NSClassFromString(@"NSMutableArray")]) {
		cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
		if (cell == nil) {
			cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
		}
		
		// Configure the cell.
		NSDictionary* item = [(NSMutableArray *)arItems objectAtIndex: indexPath.row ];
		cell.textLabel.text = [item valueForKey:@"name"];
	} else if([arItems isKindOfClass:NSClassFromString(@"NSDictionary")]) {
		
		cell = [tableView dequeueReusableCellWithIdentifier:ColumnCellIdentifier];
		if(cell == nil)
			cell = [self reuseTableViewCellWithIdentifier:ColumnCellIdentifier];
		
		NSString *key = [[(NSDictionary *)arItems allKeys] objectAtIndex:indexPath.row];
		
		UILabel *lbl = (UILabel *)[cell viewWithTag:NAME_TAG];
		lbl.text = key;
		
		UILabel *lblText = (UILabel *)[cell viewWithTag:TEXT_TAG];
		lblText.text = [arItems valueForKey:key];
	}
    return cell;
}


/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/


/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source.
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view.
    }   
}
*/


/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/


/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    
	
	if([arItems isKindOfClass:NSClassFromString(@"NSMutableArray")]) {
		//Get the dictionary of the selected data source.
		NSDictionary *dictionary = [(NSMutableArray *)arItems objectAtIndex:indexPath.row];
	
		
		//Prepare to tableview.
		RootViewController *rvController = [[RootViewController alloc] initWithNibName:@"RootViewController" bundle:[NSBundle mainBundle]];
		
		//Increment the Current View
		rvController.CurrentLevel += 1;
		
		//Set the title;
		rvController.CurrentTitle = [dictionary objectForKey:@"name"];
		
		//Push the new table view on the stack
		[self.navigationController pushViewController:rvController animated:YES];
		
		rvController.arItems = dictionary;
		
		[rvController release];
	}
}


#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


#pragma mark -
#pragma mark Custom Cell
-(UITableViewCell *)reuseTableViewCellWithIdentifier:(NSString *)identifier {
	
	//Rectangle which will be used to create labels and table view cell.
	CGRect cellRectangle;
	
	//Returns a rectangle with the coordinates and dimensions.
	cellRectangle = CGRectMake(0.0, 0.0, CELL_WIDTH, ROW_HEIGHT);
	
	//Initialize a UITableViewCell with the rectangle we created.
	UITableViewCell *cell = [[[UITableViewCell alloc] initWithFrame:cellRectangle reuseIdentifier:identifier] autorelease];
	
	//Now we have to create the two labels.
	UILabel *label;
	
	//Create a rectangle container for the number text.
	cellRectangle = CGRectMake(NAME_OFFSET, (ROW_HEIGHT - LABEL_HEIGHT) / 2.0, NAME_WIDTH, LABEL_HEIGHT);
	
	//Initialize the label with the rectangle.
	label = [[UILabel alloc] initWithFrame:cellRectangle];
	
	//Mark the label with a tag
	label.tag = NAME_TAG;
	
	//Add the label as a sub view to the cell.
	[cell.contentView addSubview:label];
	[label release];
	
	//Create a rectangle container for the custom text.
	cellRectangle = CGRectMake(TEXT_OFFSET, (ROW_HEIGHT - LABEL_HEIGHT) / 2.0, TEXT_WIDTH, LABEL_HEIGHT);
	
	//Initialize the label with the rectangle.
	label = [[UILabel alloc] initWithFrame:cellRectangle];
	
	//Mark the label with a tag
	label.tag = TEXT_TAG;
	
	//Add the label as a sub view to the cell.
	[cell.contentView addSubview:label];
	[label release];
	
	return cell;
}

@end

