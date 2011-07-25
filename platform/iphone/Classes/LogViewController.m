/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#import "LogViewController.h"
#import "logging/RhoLogConf.h"

extern int rho_conf_send_log(const char*);

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
