//
//  LoginViewController.m
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SyncEngine.h"
#import "LoginViewController.h"


@implementation LoginViewController

@synthesize txtLogin, txtPassword, waitPage;

- (void)loginThreadMainRoutine:(WaitLoginController*) waitView {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init]; // Top-level pool

	if ( [[SyncEngine sharedInstance].syncClient is_logged_in] )
	{
		sleep(1);
		[SyncEngine sharedInstance].loginState = logged_in;	
		[waitView performSelectorOnMainThread:@selector(loginComplete:) withObject:@"no errors" waitUntilDone:false];
		[pool release];  // Release the objects in the pool.
		return;	
	}
	
	NSLog(@"login: %@, password: %@", txtLogin.text, txtPassword.text);
	
	RhoSyncNotify* res = [ [SyncEngine sharedInstance].syncClient loginWithUser:txtLogin.text pwd:txtPassword.text];
	if ( res.error_code != RHO_ERR_NONE || ![[SyncEngine sharedInstance].syncClient is_logged_in]) 
		[SyncEngine sharedInstance].loginState = failed;	
	else
	{
		[SyncEngine sharedInstance].loginState = logged_in;	
		[res release];
		
		res = [[SyncEngine sharedInstance].syncClient syncAll];
	}

	[waitView performSelectorOnMainThread:@selector(loginComplete:) withObject:[res error_message] waitUntilDone:false];
	
	[pool release];  // Release the objects in the pool.
}

- (IBAction)doLogin:(id)sender 
{
	[SyncEngine sharedInstance].loginState = in_progress;	
	[[self navigationController] pushViewController:waitPage animated:YES];
	[NSThread detachNewThreadSelector:@selector(loginThreadMainRoutine:) toTarget:self withObject:waitPage];	
}

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

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
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
