//
//  LogOptionsController.m
//  rhorunner
//
//  Created by Vlad on 4/17/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "LogOptionsController.h"
#import "logging/RhoLogConf.h"

#define kUITextViewHeight 70.0
#define kUILabelHeight	  20.0

@implementation LogOptionsController

@synthesize tableView;

- (void)dealloc
{	
	[textViewEnabledCategories release];
	[textViewDisabledCategories release];
	[super dealloc];
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
	
	// create the system-defined "Save" button
    UIBarButtonItem *saveItem = [[UIBarButtonItem alloc]
								 initWithBarButtonSystemItem:UIBarButtonSystemItemSave
								 target:self action:@selector(actionSave:)];
	saveItem.style = UIBarButtonItemStylePlain;

	// create the system-defined "Save" button
    UIBarButtonItem *cancelItem = [[UIBarButtonItem alloc]
								 initWithBarButtonSystemItem:UIBarButtonSystemItemCancel
								 target:self action:@selector(actionCancel:)];
	cancelItem.style = UIBarButtonItemStylePlain;
	
	
	// flex item used to separate the left groups items and right grouped items
	UIBarButtonItem *flexItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
																			  target:nil
																			  action:nil];
	
	NSArray *items = [NSArray arrayWithObjects: saveItem, flexItem, cancelItem, nil];
	[toolbar setItems:items animated:NO];
	[saveItem release];
	[cancelItem release];
    [flexItem release];
	
	return toolbar;	
}

- (CGFloat)createTextView:(bool)categories yPos:(CGFloat)y width:(CGFloat)w
{
	CGRect frame = CGRectMake(0.0, y, w, kUITextViewHeight);	
	UITextView *textView = [[[UITextView alloc] initWithFrame:frame] autorelease];
    textView.textColor = [UIColor blackColor];
    textView.delegate = self;
    textView.backgroundColor = [UIColor whiteColor];
	textView.returnKeyType = UIReturnKeyDone;
	textView.keyboardType = UIKeyboardTypeDefault;	// use the default type input method (entire keyboard)
	textView.autocorrectionType = UITextAutocorrectionTypeNo;
	[self.view addSubview:textView];

	char* categoriesText;
	if (categories) {
		categoriesText = rho_logconf_getEnabledCategories();
		textViewEnabledCategories = textView;
	} else {
		categoriesText = rho_logconf_getDisabledCategories();
		textViewDisabledCategories = textView;
	}
	textView.text = [NSString stringWithCString:categoriesText encoding:[NSString defaultCStringEncoding]];
	rho_logconf_freeString(categoriesText);

	return kUITextViewHeight;
}

- (CGFloat)addLabel:(NSString*)text yPos:(CGFloat)y width:(CGFloat)w
{
	CGRect labelFrame = CGRectMake(	0.0, y, w, kUILabelHeight);
	UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];
	label.font = [UIFont systemFontOfSize: 12];
	label.text = text;
	label.textAlignment = UITextAlignmentLeft;
	label.textColor = [UIColor blackColor];
	label.backgroundColor = [UIColor clearColor];
	[self.view addSubview:label];
	[label release];
	return kUILabelHeight;
}

- (void)createPicker:(CGFloat)y width:(CGFloat)w
{
	// this list appears in the UIPickerView to pick the log's severity level
	pickerViewArray = [[NSArray arrayWithObjects:
								@"Trace",
								@"Info",
								@"Warning",  
								@"Error",  
								nil] retain];
	
	CGRect zeroRect = CGRectMake(0.0,0.0,0.0,0.0);
	UIPickerView *pickerView = [[UIPickerView alloc] initWithFrame:zeroRect];
	CGSize pickerSize = [pickerView sizeThatFits:zeroRect.size];
	CGRect pickerRect = CGRectMake(	0.0, y, pickerSize.width, pickerSize.height);	
	pickerView.frame = pickerRect;
	
	pickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	pickerView.delegate = self;
	pickerView.showsSelectionIndicator = YES;
	
	selectedRow = rho_logconf_getSeverity();
	[pickerView selectRow:selectedRow inComponent:0 animated:NO];
	
	[self.view addSubview:pickerView];
	[pickerView release];
}

- (void)loadView {
	CGRect frame = [[UIScreen mainScreen] applicationFrame];
	self.view = [[UIView alloc] initWithFrame:frame];
	[self.view setBackgroundColor:[UIColor groupTableViewBackgroundColor]];
	CGRect mainViewBounds = self.view.bounds;
	
	UIToolbar* toolbar = [[self newToolbar:mainViewBounds] autorelease];
	[self.view addSubview:toolbar];
	
	// create and configure the table view
	CGFloat toolbarHeight = [toolbar frame].size.height;
	mainViewBounds.size.height -= toolbarHeight;

	CGFloat y = 0.0;
	y += [self addLabel:@"Enabled Categories:" yPos:y width:mainViewBounds.size.width];
	y += [self createTextView:true yPos:y width:mainViewBounds.size.width];
	y += [self addLabel:@"Disabled Categories:" yPos:y width:mainViewBounds.size.width];
	y += [self createTextView:false yPos:y width:mainViewBounds.size.width];
	y += [self addLabel:@"Log Messages of this class and below:" yPos:y width:mainViewBounds.size.width];
	[self createPicker:y width:mainViewBounds.size.width];
}

- (void)actionSave:(id)sender
{
	NSString* text = textViewEnabledCategories.text;
	const char* categories = [text cStringUsingEncoding:[NSString defaultCStringEncoding]]; 	
	rho_logconf_setEnabledCategories(categories);
	text = textViewDisabledCategories.text;
	categories = [text cStringUsingEncoding:[NSString defaultCStringEncoding]]; 	
	rho_logconf_setDisabledCategories(categories);
	rho_logconf_setSeverity(selectedRow);
	rho_logconf_saveSettings();
	
	[self dismissModalViewControllerAnimated:YES]; 
	self.view.hidden = YES;
}

- (void)actionCancel:(id)sender
{
	[self dismissModalViewControllerAnimated:YES]; 
	self.view.hidden = YES;
}

// this helps dismiss the keyboard then the "done" button is clicked
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text {
	if ([text isEqualToString:@"\n"]) {
		[textView resignFirstResponder];
		return NO;
	}
	return YES;
}

#pragma mark -
#pragma mark PickerView delegate methods

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
	selectedRow = [pickerView selectedRowInComponent:0];
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
	return [pickerViewArray objectAtIndex:row];
}

- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
	return 240.0;
}

- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
	return 40.0;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
	return [pickerViewArray count];
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
	return 1;
}

@end
