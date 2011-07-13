//
//  LogViewController.m
//  rhorunner
//
//  Created by Vlad on 4/15/09.
//  Copyright 2009 rhomobile, inc. All rights reserved.
//

#import "LogViewController.h"
#import "logging/RhoLogConf.h"

extern void rho_conf_send_log(const char*);

@implementation LogViewController

@synthesize actionTarget, onShowLogOptions;

- (void)loadLogText {
	char* logText = rho_logconf_getText();	
	textView.text = [NSString stringWithCString:logText encoding:[NSString defaultCStringEncoding]];
	rho_logconf_freeString(logText);
	
	// commented it out as it crashes somewhere in the sys code
	//int pos = grtLogTextPos();
	//[textView scrollRangeToVisible:NSMakeRange(pos,0)];	
}

- (UITextView *)createUITextView:(CGRect) frame
{
	textView = [[[UITextView alloc] initWithFrame:frame] autorelease];
	
	textView.editable = NO;
    textView.textColor = [UIColor blackColor];
    textView.backgroundColor = [UIColor whiteColor];
    textView.delegate = self;
		
	[self loadLogText];
	
	return textView;
}

- (UIToolbar *)newToolbar:(CGRect) frame
{
	UIToolbar* toolbar = [UIToolbar new];
	toolbar.barStyle = UIBarStyleBlackOpaque;
	
	// size up the toolbar and set its frame
	[toolbar sizeToFit];
	CGFloat toolbarHeight = [toolbar frame].size.height;
	
	[toolbar setFrame:CGRectMake(frame.origin.x,
								 frame.origin.y + frame.size.height - toolbarHeight,
								 frame.size.width,
								 toolbarHeight)];	
		
    UIBarButtonItem *doneItem = [[UIBarButtonItem alloc]
                                 initWithBarButtonSystemItem:UIBarButtonSystemItemStop
                                 target:self action:@selector(actionClose:)];
	doneItem.style = UIBarButtonItemStylePlain;
	    
    UIBarButtonItem *refreshItem = [[UIBarButtonItem alloc]
								    initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh
								    target:self action:@selector(actionRefresh:)];
	
    UIBarButtonItem *clearItem = [[UIBarButtonItem alloc]
                                  initWithTitle:@"Clear" style:UIBarButtonItemStylePlain
                                  target:self action:@selector(actionClear:)];
    
    UIBarButtonItem *sendItem = [[UIBarButtonItem alloc]
                                 initWithTitle:@"Send" style:UIBarButtonItemStylePlain
                                 target:self action:@selector(actionSend:)];
    
	UIBarButtonItem *settingsItem = [[UIBarButtonItem alloc]
                                     initWithImage:[UIImage imageNamed:@"gears.png"]
                                     style:UIBarButtonItemStylePlain
                                     target:self action:@selector(actionSettings:)];	
	
	// flex item used to separate the left groups items and right grouped items
	UIBarButtonItem *flexItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
																			  target:nil
																			  action:nil];
	
	NSArray *items = [NSArray arrayWithObjects: doneItem, flexItem, clearItem, sendItem, refreshItem, settingsItem, nil];
	[toolbar setItems:items animated:NO];
	[doneItem release];
	[refreshItem release];
	[settingsItem release];
	[flexItem release];
    [sendItem release];
    [clearItem release];
	
	return toolbar;	
}

- (void)loadView
{
	CGRect frame = [[UIScreen mainScreen] applicationFrame];
	self.view = [[UIView alloc] initWithFrame:frame];
	CGRect mainViewBounds = self.view.bounds;	
	
	UIToolbar* toolbar = [[self newToolbar:mainViewBounds] autorelease];
	[self.view addSubview:toolbar];
	
	CGFloat toolbarHeight = [toolbar frame].size.height;
	mainViewBounds.size.height -= toolbarHeight;
	[self.view addSubview:[self createUITextView:mainViewBounds]];
}

- (void)actionClose:(id)sender
{
	[self dismissModalViewControllerAnimated:YES]; 
	self.view.hidden = YES;
}

- (void)actionRefresh:(id)sender
{
	[self loadLogText];
}

- (void)actionClear:(id)sender
{
    textView.text = @"";
}

- (void)actionSend:(id)sender
{
    rho_conf_send_log("");
}

- (void)actionSettings:(id)sender
{
	if(actionTarget && [actionTarget respondsToSelector:onShowLogOptions]) {
		[actionTarget performSelector:onShowLogOptions];
	}	
}

@end
