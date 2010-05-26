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

@synthesize dateTime, pickerView, parentView, toolbar, barLabel;

- (void)dealloc
{	
    [pickerView release];
    [toolbar release];
    [barLabel release];
    [dateTime release];
    [super dealloc];
}

- (void)createPickerBar:(CGRect)pickerFrame
{
    self.toolbar = [UIToolbar new];
    self.toolbar.barStyle = UIBarStyleBlackOpaque;
    
    // size up the toolbar and set its frame
    [self.toolbar sizeToFit];
    CGFloat toolbarHeight = [self.toolbar frame].size.height;
    
    // TODO: This is an approximate y-origin, figure out why it is off by 3.7
    CGRect toolbarFrame = CGRectMake(pickerFrame.origin.x,
                                     pickerFrame.origin.y - toolbarHeight,
                                     pickerFrame.size.width,
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
    [flexItem release];
    [doneItem release];
}

- (void)createPicker:(UIView*)parent
{
    self.parentView = parent;
    CGRect parentFrame = parent.frame;
    
    // Create the picker
    if (self.pickerView == nil) {
        CGRect frame = parentFrame;
        frame.size.height = 220;
        frame.origin.y = parentFrame.origin.y + parentFrame.size.height - frame.size.height;
        self.pickerView = [[UIDatePicker alloc] initWithFrame:frame];
    }
    
    if (self.pickerView.superview == nil) {
        self.pickerView.autoresizingMask = UIViewAutoresizingFlexibleWidth;
        
        // Determine picker type
        int mode = self.dateTime.format;
        switch (mode) {
            case 1:
                self.pickerView.datePickerMode = UIDatePickerModeDate;
                break;
            case 2:
                self.pickerView.datePickerMode = UIDatePickerModeTime;
                break;
            case 0:
            default:
                self.pickerView.datePickerMode = UIDatePickerModeDateAndTime;
        }
        
        if (self.dateTime.initialTime) {
            self.pickerView.date = [NSDate dateWithTimeIntervalSince1970:self.dateTime.initialTime];
        }

        //CGSize pickerSize = CGSizeMake(parentFrame.size.width, parentFrame.size.height/2);
        CGSize pickerSize = [pickerView sizeThatFits:pickerView.frame.size];
        CGRect pickerFrame = CGRectMake(parentFrame.origin.x,
                                        parentFrame.origin.y + parentFrame.size.height - pickerSize.height,
                                        pickerSize.width,
                                        pickerSize.height);
        
        // Add toolbar to view
        [self createPickerBar:pickerFrame];
        
        // Add picker to view
        [parentView addSubview:self.pickerView];
        
        CGRect startFrame = pickerFrame;
        startFrame.origin.y = parentFrame.origin.y + parentFrame.size.height;
        self.pickerView.frame = startFrame;
        [UIView beginAnimations:nil context:NULL];
        [UIView setAnimationDuration:0.3];
        
        [UIView setAnimationDelegate:self];
        [UIView setAnimationDidStopSelector:@selector(slideUpDidStop)];
        
        self.pickerView.frame = pickerFrame;
        
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
    CGRect parentFrame = parentView.frame;
    CGRect endFrame = pickerView.frame;
    endFrame.origin.y = parentFrame.origin.y + parentFrame.size.height;
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
