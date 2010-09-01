//
//  ptestsViewController.m
//  ptests
//
//  Created by Vlad on 8/25/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ptestsViewController.h"
#import <QuartzCore/QuartzCore.h>
#import "../../../RhoSyncClient.h"

@implementation ptestsViewController

@synthesize txtResult, btnStart, indicator;

- (void)testComplete:(NSString*) result
{
	[indicator stopAnimating];	
	txtResult.text = result;
	[btnStart setEnabled:YES];
}

- (void) beforeTests
{
	if ( tests_initialized )
		return;
	
	[RhoSyncClient initDatabase];
	
	perftest = [[RhomModel alloc] init];
	perftest.name = @"Perftest";
	perftest.sync_type = RST_NONE;
	
	sclient = [[RhoSyncClient alloc] init];
	NSArray* models = [NSArray arrayWithObjects: perftest, nil];	
	
	[sclient addModels:models];
	
    sclient.threaded_mode = FALSE;
	sclient.poll_interval = 0;
	
	[sclient database_full_reset_and_logout];
	
	nCount = 1000;
	
	tests_initialized = TRUE;
}

- (void) testCreate
{
	[perftest startBulkUpdate];	
	for (int i = 0; i < nCount; i++) 
	{
		NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
		[item setValue: [NSString stringWithFormat:@"Test%d", i] forKey:@"name"];							 
		[perftest create:item];
		[item release];
	}
	[perftest stopBulkUpdate];
}

- (void) testRead
{
	for (int i = 0; i < 1; i++) 
	{
		NSMutableArray* items = [perftest find_all:NULL];	
		if (items) {
			[items release];
		}	
	}	
}

- (void) testReadByOne
{
	for (int i = 0; i < nCount; i++) 
	{
		NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
		[cond setValue:[NSString stringWithFormat:@"Test%d", i] forKey:@"name"];							 
		
		NSMutableDictionary* item = [perftest find_first:cond];	
		if (item) {
			[item release];
		}
		
		[cond release];
	}	
}

- (void) testUpdate
{
	[perftest startBulkUpdate];		
	for (int i = 0; i < nCount; i++) 
	{
		NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
		[cond setValue:[NSString stringWithFormat:@"Test%d", i] forKey:@"name"];							 
	
		NSMutableDictionary* item = [perftest find_first:cond];	
		if ( !item )
			continue;
	
		//NSString* saved_object = [NSString stringWithString: [item valueForKey:@"object"]];
		NSMutableString* new_name = [[NSMutableString alloc]init];
		[new_name appendString:[item valueForKey:@"name"]];
		[new_name appendString: @"_TEST"];
	
		[item setValue:new_name forKey:@"name"];
		[perftest save: item];
		
		[cond release];
		[item release];		
		[new_name release];				
	}
	[perftest stopBulkUpdate];	
}

- (void) testDelete
{
	NSMutableArray* items = [perftest find_all:NULL];	
	if ( !items )
		return;
	
	[perftest startBulkUpdate];
	for( NSDictionary* item in items)
	{
		[perftest destroy: item];
	}
	[perftest stopBulkUpdate];
	
	[items release];
}

- (void)testThreadMainRoutine
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool
	
	NSLog(@"starting test");	
	double startTime;
	NSString* result = [[NSString alloc]initWithString:@""];
	NSArray* ops = [NSArray arrayWithObjects:@"create", @"read all", @"read by one", @"update", @"delete", nil];
	
	[self beforeTests];
	
	result = [result stringByAppendingString:@"1000 records\n"];
	
	for(NSString* op in ops) 
	{
		startTime = CACurrentMediaTime();
		//NSLog(@"testing %@ records op", op);	
		
		//put calls to appropriate test methods here
		//sleep(rand() % 2 + 1);
		if ( [op isEqualToString:@"create"])
			[self testCreate];
		else if ( [op isEqualToString:@"read all"])
			[self testRead];
		else if ( [op isEqualToString:@"read by one"])
			[self testReadByOne];
		else if ( [op isEqualToString:@"update"])
			[self testUpdate];
		else if ( [op isEqualToString:@"delete"])
			[self testDelete];
		
//		result = [result stringByAppendingString:
//				  [NSString stringWithFormat:@"- to %@ 1000 records took %f milisecons\n",
//				   op, (CACurrentMediaTime()-startTime)*1000.0 ]];
		result = [result stringByAppendingString:
				  [NSString stringWithFormat:@"%@ (ms): %f\n",
				   op, (CACurrentMediaTime()-startTime)*1000.0 ]];
		
	}
	
	NSLog(@"test is done");
	[self performSelectorOnMainThread:@selector(testComplete:) withObject:result waitUntilDone:false];
    
	[pool release];  // Release the objects in the pool.
}

- (IBAction)runTest:(id)sender {
	[btnStart setEnabled:NO];
	[indicator startAnimating];	
	[NSThread detachNewThreadSelector:@selector(testThreadMainRoutine) toTarget:self withObject:nil];
}

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	txtResult.text = @"Please press Start to execute test";
}



/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

@end
