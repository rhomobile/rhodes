//
//  SignatureView.m
//  rhorunner
//
//  Created by Soldatenkov Dmitry on 27/07/10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import "SignatureViewController.h"

#import "SignatureView.h"
#import "AppManager.h"
#import "Rhodes.h"

#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SignatureViewController"


@implementation SignatureViewController


- (id)initWithRect:(CGRect)rect  delegate:(SignatureDelegate*)delegate {
	[self init];
	
	signatureDelegate = delegate;
	self.view.frame = rect;

	//content.backgroundColor = [UIColor redColor];
	//1ontent.backgroundColor = [UIColor groupTableViewBackgroundColor];
	
	toolbar = [[UIToolbar alloc] init];
	
	toolbar.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
	toolbar.autoresizesSubviews = YES;
	
	{
		UIBarButtonItem *btn_fixed = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace target:nil action:nil];
		UIBarButtonItem* btn_cancel = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(doCancel:)];
		UIBarButtonItem* btn_clear = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemTrash target:self action:@selector(doClear:)];
		UIBarButtonItem* btn_space = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
		UIBarButtonItem* btn_done = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(doDone:)];
		
		NSMutableArray *btns = [NSMutableArray arrayWithCapacity:6];
		[btns addObject:btn_fixed];
		[btns addObject:btn_cancel];
		[btns addObject:btn_fixed];
		[btns addObject:btn_clear];
		[btns addObject:btn_space];
		[btns addObject:btn_done];
		
		[btn_fixed release];
		[btn_cancel release];
		[btn_clear release];
		[btn_space release];
		[btn_done release];
		
		[toolbar setItems:btns];
		
	}
	
	
	[toolbar sizeToFit];
	CGRect srect = self.view.frame;
	CGRect trect = toolbar.frame;
	srect.size.height -= trect.size.height;
	srect.origin.y = 0;
	trect.origin.x = 0;
	trect.origin.y = srect.origin.y+srect.size.height;
	toolbar.frame = trect;
	
	signatureView = [[SignatureView alloc] initWithFrame:CGRectZero];
	signatureView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	signatureView.autoresizesSubviews = YES;
	signatureView.frame = srect;
	
	[self.view addSubview:signatureView];
	[self.view addSubview:toolbar];
	
	return self;

}


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
	UIView* content = [[UIView alloc] initWithFrame:CGRectZero];

	self.view = content;
	[content release];
}

- (void)doDone:(id)sender {
	//NSData* data = UIImagePNGRepresentation([signatureView makeUIImage]);
	[signatureDelegate doDone:[signatureView makeUIImage]];
}

- (void)doClear:(id)sender {
	[signatureView doClear];
}

- (void)doCancel:(id)sender {
	[signatureDelegate doCancel];
}



///*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	//[self.view 
}
//*/


- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
//	[signatureView setNeedsDisplay];
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
	[signatureView setNeedsDisplay];
}



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
	[signatureView removeFromSuperview];
	[toolbar removeFromSuperview];
	[signatureView release];
	[toolbar release];
    [super dealloc];
}



@end
