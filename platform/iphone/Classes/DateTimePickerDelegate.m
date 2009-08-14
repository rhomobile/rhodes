//
//  DateTimePickerDelegate.m
//  rhorunner
//
//  Created by lars on 8/11/09.
//  Copyright 2009 Home. All rights reserved.
//

#import "DateTimePickerDelegate.h"


@implementation DateTimePickerDelegate

@synthesize dateTime, pickerView, toolbar, dateFormatter;

- (void)dealloc
{	
	[pickerView release];
	[toolbar release];
	[dateTime release];
	[dateFormatter release];
	[super dealloc];
}

- (void)createPickerBar:(CGRect) frame
{
	self.toolbar = [UIToolbar new];
	self.toolbar.barStyle = UIBarStyleBlackOpaque;
	
	// size up the toolbar and set its frame
	[self.toolbar sizeToFit];
	CGFloat toolbarHeight = [self.toolbar frame].size.height;
	
	// TODO: This is an approximate y-origin, figure out why it is off by 3.5
	[self.toolbar setFrame:CGRectMake(frame.origin.x,
									  frame.origin.y + frame.size.height + 3.7,
									  frame.size.width,
									  toolbarHeight)];	
	
    UIBarButtonItem *cancelItem = [[UIBarButtonItem alloc]
								 initWithBarButtonSystemItem:UIBarButtonSystemItemCancel
								 target:self action:@selector(cancelAction:)];
	cancelItem.style = UIBarButtonItemStylePlain;
	
	UIBarButtonItem *flexItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
																			  target:self action:nil];
	
	UIBarButtonItem *saveItem = [[UIBarButtonItem alloc]
								 initWithBarButtonSystemItem:UIBarButtonSystemItemDone
								 target:self action:@selector(dateAction:)];
	saveItem.style = UIBarButtonItemStylePlain;
	
	NSArray *items = [NSArray arrayWithObjects: cancelItem, flexItem, saveItem, nil];
	[self.toolbar setItems:items animated:NO];
	[saveItem release];
}

- (void)createPicker:(UIWindow*)window {
	// Create the picker
	if (self.pickerView == nil) {
		self.pickerView = [[UIDatePicker alloc] initWithFrame:CGRectZero];
	}
	
	self.dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
	
	if (self.pickerView.superview == nil) {
		self.pickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
		
		// Determine picker type
		int mode = self.dateTime.format;
		switch (mode) {
			case 0:
				self.pickerView.datePickerMode = UIDatePickerModeDateAndTime;
				[self.dateFormatter setDateStyle:NSDateFormatterLongStyle];
				[self.dateFormatter setTimeStyle:NSDateFormatterLongStyle];
				break;
			case 1:
				self.pickerView.datePickerMode = UIDatePickerModeDate;
				[self.dateFormatter setDateStyle:NSDateFormatterLongStyle];
				[self.dateFormatter setTimeStyle:NSDateFormatterNoStyle];
				break;
			case 2:
				self.pickerView.datePickerMode = UIDatePickerModeTime;
				[self.dateFormatter setDateStyle:NSDateFormatterNoStyle];
				break;
			default:
				self.pickerView.datePickerMode = UIDatePickerModeDateAndTime;
				[self.dateFormatter setDateStyle:NSDateFormatterLongStyle];
				[self.dateFormatter setTimeStyle:NSDateFormatterLongStyle];
				break;
		}
		
		// Add toolbar to view
		CGRect mainViewBounds = self.pickerView.bounds;
		[self createPickerBar:mainViewBounds];
		[window addSubview:self.toolbar];
		
		// Add picker to view
		[window addSubview:self.pickerView];
		CGRect screenRect = [[UIScreen mainScreen] applicationFrame];
		CGSize pickerSize = [self.pickerView sizeThatFits:CGSizeZero];
		CGRect startRect = CGRectMake(0.0,
									  screenRect.origin.y + screenRect.size.height,
									  pickerSize.width, pickerSize.height);
		self.pickerView.frame = startRect;
		
		// compute the end frame
		CGRect pickerRect = CGRectMake(0.0,
									   screenRect.origin.y + screenRect.size.height - pickerSize.height,
									   pickerSize.width,
									   pickerSize.height);

		[UIView beginAnimations:nil context:NULL];
		[UIView setAnimationDuration:0.3];
		
		[UIView setAnimationDelegate:self];
		
		self.pickerView.frame = pickerRect;
		
		[UIView commitAnimations];
	}
}

- (void)slideDownDidStop
{
	// the date picker has finished sliding downwards, so remove it
	[self.pickerView removeFromSuperview];
}

- (void)animateDown
{
	CGRect screenRect = [[UIScreen mainScreen] applicationFrame];
	CGRect endFrame = self.pickerView.frame;
	endFrame.origin.y = screenRect.origin.y + screenRect.size.height;
	// start the slide down animation
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.3];
	
	// we need to perform some post operations after the animation is complete
	[UIView setAnimationDelegate:self];
	[UIView setAnimationDidStopSelector:@selector(slideDownDidStop)];
	
	// Remove toolbar immediately
	[self.toolbar removeFromSuperview];
	self.pickerView.frame = endFrame;
	[UIView commitAnimations];	
}

- (IBAction)cancelAction:(id)sender
{	
	NSString *message = @"status=cancel";
	[self doCallback:message];
	[self animateDown];
}

- (IBAction)dateAction:(id)sender
{	
	long ldate = [self.pickerView.date timeIntervalSince1970];
	NSMutableString *message = [[NSMutableString alloc] initWithFormat:@"status=ok&result=%@", [NSNumber numberWithLong:ldate]];
	if (self.dateTime.data) {
		[message appendFormat:@"&opaque=%@", self.dateTime.data];
	}
	[self doCallback:message];
	[self animateDown];
}

@end
