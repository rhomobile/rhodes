//
//  DateTimePickerDelegate.m
//  rhorunner
//
//  Created by lars on 8/11/09.
//  Copyright 2009 Home. All rights reserved.
//

#import "DateTimePickerDelegate.h"
#import "common/RhodesApp.h"

@implementation DateTimePickerDelegate

@synthesize dateTime, pickerView, parentView, toolbar, barLabel, dateFormatter;

- (void)dealloc
{	
	[pickerView release];
	[toolbar release];
	[barLabel release];
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
	
	// TODO: This is an approximate y-origin, figure out why it is off by 3.7
	CGRect toolbarFrame = CGRectMake(frame.origin.x,
									 frame.origin.y + frame.size.height + 3.7,
									 frame.size.width,
									 toolbarHeight);
	[self.toolbar setFrame:toolbarFrame];	
	
    UIBarButtonItem *cancelItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel
																				target:self 
																				action:@selector(cancelAction:)];
	cancelItem.style = UIBarButtonItemStylePlain;
	
	// Setup label for toolbar
	self.barLabel = [[UILabel alloc] initWithFrame:toolbarFrame];
	barLabel.text = self.dateTime.title;
	barLabel.textColor = [UIColor whiteColor]; 
	barLabel.backgroundColor = [UIColor clearColor];
	barLabel.textAlignment = UITextAlignmentCenter;

	UIBarButtonItem *flexItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
																			  target:self action:nil];
	
	UIBarButtonItem *doneItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
																			  target:self 
																			  action:@selector(dateAction:)];
	doneItem.style = UIBarButtonItemStylePlain;

	[self.toolbar setItems:[NSArray arrayWithObjects: cancelItem, flexItem, doneItem, nil] animated:NO];
	[cancelItem release];
	[self.barLabel release];
	[flexItem release];
	[doneItem release];
}

- (void)createPicker:(UIView*)parent {
	// Create the picker
	if (self.pickerView == nil) {
		self.pickerView = [[UIDatePicker alloc] initWithFrame:CGRectZero];
		self.parentView = parent;
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
		
		if (self.dateTime.initialTime) {
			self.pickerView.date = [NSDate dateWithTimeIntervalSince1970:self.dateTime.initialTime];
		}
		
		// Add toolbar to view
		CGRect mainViewBounds = self.pickerView.bounds;
		[self createPickerBar:mainViewBounds];
		
		// Add picker to view
		[parentView addSubview:self.pickerView];
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
		[UIView setAnimationDidStopSelector:@selector(slideUpDidStop)];
		
		self.pickerView.frame = pickerRect;
		
		[UIView commitAnimations];
	}
}

- (void)slideUpDidStop
{
	// the date picker has finished sliding upwards, so add toolbar
	[self.parentView addSubview:self.toolbar];
	[self.parentView addSubview:self.barLabel];
}

- (void)slideDownDidStop
{
	// the date picker has finished sliding downwards, so remove it
	[self.pickerView removeFromSuperview];
	[self.barLabel removeFromSuperview];
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
	//NSString *message = @"status=cancel";
	//[self doCallback:message];
	rho_rhodesapp_callDateTimeCallback(
		[postUrl cStringUsingEncoding:[NSString defaultCStringEncoding]],
		0, [self.dateTime.data cStringUsingEncoding:[NSString defaultCStringEncoding]], 1 );	
	[self animateDown];
}

- (IBAction)dateAction:(id)sender
{	
	long ldate = [self.pickerView.date timeIntervalSince1970];
	//NSMutableString *message = [[NSMutableString alloc] initWithFormat:@"status=ok&result=%@", [NSNumber numberWithLong:ldate]];
	//if (self.dateTime.data) {
	//	[message appendFormat:@"&opaque=%@", self.dateTime.data];
	//}
	//[self doCallback:message];
	rho_rhodesapp_callDateTimeCallback(
		[postUrl cStringUsingEncoding:[NSString defaultCStringEncoding]],
		ldate, [self.dateTime.data cStringUsingEncoding:[NSString defaultCStringEncoding]], 0 );
	
	[self animateDown];
}

@end
