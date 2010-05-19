//
//  DateTimePickerDelegate.h
//  rhorunner
//
//  Created by lars on 8/11/09.
//  Copyright 2009 Home. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "RhoDelegate.h"
#import "DateTime.h"

@interface DateTimePickerDelegate : RhoDelegate <UIPickerViewDelegate>
{
@private
	UIDatePicker *pickerView;
	UIView *parentView;
	UIToolbar *toolbar;
	UILabel *barLabel;
	DateTime *dateTime;
	NSDateFormatter *dateFormatter;
}

@property (nonatomic, retain) DateTime *dateTime;
@property (nonatomic, retain) UIDatePicker *pickerView;
@property (nonatomic, retain) UIView *parentView;
@property (nonatomic, retain) UIToolbar *toolbar;
@property (nonatomic, retain) UILabel *barLabel;
@property (nonatomic, retain) NSDateFormatter *dateFormatter; 

- (IBAction)dateAction:(id)sender;
- (IBAction)cancelAction:(id)sender;
- (void)createPicker:(UIView*)parent;

@end
