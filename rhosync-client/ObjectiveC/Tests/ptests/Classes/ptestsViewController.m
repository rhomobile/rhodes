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

	product = [[RhomModel alloc] init];
	product.name = @"Product";
	product.sync_type = RST_INCREMENTAL;

	customer = [[RhomModel alloc] init];
	customer.name = @"Customer";
	customer.sync_type = RST_INCREMENTAL;
	
	sclient = [[RhoSyncClient alloc] init];
	NSArray* models = [NSArray arrayWithObjects: perftest, product, customer, nil];	
	
	[sclient addModels:models];
	
	[sclient setConfigString:@"MinSeverity" param:@"1"];
    sclient.threaded_mode = FALSE;
	sclient.poll_interval = 0;
	
	[sclient database_full_reset_and_logout];
	sclient.bulksync_state = 1;
	
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

- (void) createTestData
{
	[product startBulkUpdate];	
	for (int i = 0; i < 100; i++) 
	{
		for (int j = 0; j < 100; j++) 
		{
			NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
			[item setValue: [NSString stringWithFormat:@"Name%d", i] forKey:@"name"];							 
			[item setValue: [NSString stringWithFormat:@"HTC%d", i] forKey:@"brand"];							 
			
			[item setValue: [NSString stringWithFormat:@"%d", i] forKey:@"price"];							 
			
			[product create:item];
			
			[item release];
		}
	}
	[product stopBulkUpdate];
	
}

- (void) benchSearch
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool
	[self beforeTests];
	
	sclient.sync_server = @"http://192.168.0.93:9292/application";
	RhoSyncNotify* res = [sclient loginWithUser:@"" pwd:@""];
	[res release];
	
	double startTime = CACurrentMediaTime();	
	//[self createTestData];
	[customer sync];
	result = [result stringByAppendingString:
			  [NSString stringWithFormat:@"   %@ (ms): %f\n",
			   @"Sync data(10,000)", (CACurrentMediaTime()-startTime)*1000.0 ]];
	
	startTime = CACurrentMediaTime();	
	NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
	[cond setValue:[NSString stringWithFormat:@"PerfManager"] forKey:@"JobTitle"];							 
	
	NSMutableArray* items = [customer find_all:cond];	
	if (items) {
		[items release];
	}
	
	result = [result stringByAppendingString:
			  [NSString stringWithFormat:@"   %@ (ms): %f\n",
			   @"Search", (CACurrentMediaTime()-startTime)*1000.0 ]];
	
	NSLog(@"BENCH search results: \n%@", result);
	
	[self performSelectorOnMainThread:@selector(testComplete:) withObject:result waitUntilDone:false];
	[pool release];  // Release the objects in the pool.
}

- (void) benchCreate
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool
	
	[self beforeTests];
	
	sclient.sync_server = @"http://rhodes-store-server.heroku.com/application";
	RhoSyncNotify* res = [sclient loginWithUser:@"" pwd:@""];
	int nErr = res.error_code;
	[res release];
	if ( nErr!= RHO_ERR_NONE || ![sclient is_logged_in]) 
	{
    	result = [result stringByAppendingString:@"ERROR: cannot login\n"];		
	}else 
	{
		[sclient syncAll];
		
		double startTime = CACurrentMediaTime();	
	
		NSMutableDictionary* item = [[NSMutableDictionary alloc] init];
		[item setValue: [NSString stringWithFormat:@"Name00"] forKey:@"name"];							 
		[item setValue: [NSString stringWithFormat:@"HTC00"] forKey:@"brand"];							 
		[item setValue: [NSString stringWithFormat:@"0"] forKey:@"price"];							 
	
		[product create:item];
		result = [result stringByAppendingString:
			  [NSString stringWithFormat:@"   %@ (ms): %f\n",
			   @"Create 1 item", (CACurrentMediaTime()-startTime)*1000.0 ]];

		startTime = CACurrentMediaTime();		
		[product sync];
		result = [result stringByAppendingString:
			  [NSString stringWithFormat:@"   %@ (ms): %f\n",
			   @"Sync 1 item", (CACurrentMediaTime()-startTime)*1000.0 ]];

		startTime = CACurrentMediaTime();		
		
		NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
		[cond setValue:[NSString stringWithFormat:@"Name00"] forKey:@"name"];							 
		
		NSMutableArray* items = [product find_all:cond];	
		if (items) {
			[items release];
		}

		result = [result stringByAppendingString:
				  [NSString stringWithFormat:@"   %@ (ms): %f\n",
				   @"Search", (CACurrentMediaTime()-startTime)*1000.0 ]];
		
		NSLog(@"BENCH create results: \n%@", result);
		
	}
	
	[self performSelectorOnMainThread:@selector(testComplete:) withObject:result waitUntilDone:false];
	[pool release];  // Release the objects in the pool.
}

- (void) benchBulkSync
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool
	[self beforeTests];
	
	double startTime = CACurrentMediaTime();	
	
	sclient.sync_server = @"http://192.168.0.93:9292/application";
	RhoSyncNotify* res = [sclient loginWithUser:@"" pwd:@""];
	//sclient.sync_server = @"http://184.73.159.63/application";
	//RhoSyncNotify* res = [sclient loginWithUser:@"smladenova" pwd:@"password"];
	
	[res release];
	sclient.bulksync_state = 0;

	result = [result stringByAppendingString:
			  [NSString stringWithFormat:@"   %@ (ms): %f\n",
			   @"Reset and login", (CACurrentMediaTime()-startTime)*1000.0 ]];
	
	startTime = CACurrentMediaTime();	
	[sclient syncAll];
	result = [result stringByAppendingString:
			  [NSString stringWithFormat:@"   %@ (ms): %f\n",
			   @"Bulk sync", (CACurrentMediaTime()-startTime)*1000.0 ]];
	
	startTime = CACurrentMediaTime();	
	NSMutableDictionary* cond = [[NSMutableDictionary alloc] init];
	[cond setValue:[NSString stringWithFormat:@"PerfManager"] forKey:@"JobTitle"];							 
	
	NSMutableArray* items = [customer find_all:cond];	
	if (items) {
		[items release];
	}
	
	result = [result stringByAppendingString:
			  [NSString stringWithFormat:@"   %@ (ms): %f\n",
			   @"Search", (CACurrentMediaTime()-startTime)*1000.0 ]];
	
	NSLog(@"BENCH bulk results: \n%@", result);
	
	[self performSelectorOnMainThread:@selector(testComplete:) withObject:result waitUntilDone:false];
	[pool release];  // Release the objects in the pool.
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
	result = [[NSString alloc]initWithString:@""];
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

- (void)testThreadMainRoutineBench
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool
	
	NSLog(@"starting test");	
	double startTime;
	result = [[NSString alloc]initWithString:@""];
	NSArray* ops = [NSArray arrayWithObjects: @"Create bench", @"Search bench", @"Bulk bench", nil];//, @"bench_", @"read by one", @"update", @"delete", nil];
	[self beforeTests];
	
	sclient.sync_server = @"http://rhodes-store-server.heroku.com/application";
	
	RhoSyncNotify* res = [sclient loginWithUser:@"" pwd:@""];
	int nErr = res.error_code;
	[res release];
	if ( nErr!= RHO_ERR_NONE || ![sclient is_logged_in]) 
	{
    	result = [result stringByAppendingString:@"ERROR: cannot login\n"];		
	}else 
	{
		[sclient syncAll];
		for(NSString* op in ops) 
		{
			startTime = CACurrentMediaTime();
		
			if ( [op isEqualToString:@"Search bench"])
				[self benchSearch];
			else if ( [op isEqualToString:@"Create bench"])
				[self benchCreate];
			else if ( [op isEqualToString:@"Bulk bench"])
				[self benchBulkSync];
		
			NSLog(@"bench %@ is done", op);
		
			result = [result stringByAppendingString:
				  [NSString stringWithFormat:@"%@ (ms): %f\n",
				   op, (CACurrentMediaTime()-startTime)*1000.0 ]];
		}
	}
	
	NSLog(@"BENCH results:");
	NSLog(result);
	[self performSelectorOnMainThread:@selector(testComplete:) withObject:result waitUntilDone:false];
    
	[pool release];  // Release the objects in the pool.
}

- (IBAction)runTest:(id)sender {
	[btnStart setEnabled:NO];
	[indicator startAnimating];	
	[NSThread detachNewThreadSelector:@selector(testThreadMainRoutine) toTarget:self withObject:nil];
}

- (IBAction)runBench:(id)sender 
{
	[btnStart setEnabled:NO];
	[indicator startAnimating];	
	
	result = [[NSString alloc]initWithString:@""];
	
	if ( sender == btnBenchSearch )
		[NSThread detachNewThreadSelector:@selector(benchSearch) toTarget:self withObject:nil];
	else if ( sender == btnBenchCreate)
		[NSThread detachNewThreadSelector:@selector(benchCreate) toTarget:self withObject:nil];
	else if ( sender == btnBenchBulk)		
		[NSThread detachNewThreadSelector:@selector(benchBulkSync) toTarget:self withObject:nil];
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
