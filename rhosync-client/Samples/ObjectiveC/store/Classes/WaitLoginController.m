//
//  WaitLoginController.m
//  store
//
//  Created by Vlad on 8/30/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SyncEngine.h"
#import "WaitLoginController.h"

@implementation WaitLoginController

@synthesize lblMessage, indicator, homePage;

- (void)loginComplete:(NSString*) errorMessage {
	NSLog(@"Login error message: \"%@\"", errorMessage);
	[indicator stopAnimating];
	if ([SyncEngine sharedInstance].loginState == logged_in) {
		[[self navigationController] pushViewController:homePage animated:YES];
	} else {
		lblMessage.text = errorMessage;
		self.navigationItem.hidesBackButton = false;
	}
}

- (void)viewDidAppear:(BOOL)animated {
	self.navigationItem.title = @"Wait";
	if ([SyncEngine sharedInstance].loginState == in_progress) {
		[indicator startAnimating];
		lblMessage.text = @"Working";
		self.navigationItem.hidesBackButton = true;
	} else {
		[indicator stopAnimating];
	}
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
