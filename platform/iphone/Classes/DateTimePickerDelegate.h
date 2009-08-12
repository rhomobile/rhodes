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

@interface DateTimePickerDelegate : RhoDelegate
{
@private
	UIDatePicker *pickerView;
	UIBarButtonItem *doneButton;	
	NSArray *dataArray;
	
	NSDateFormatter *dateFormatter;
}

@property (nonatomic, retain) IBOutlet UIDatePicker *pickerView; 
@property (nonatomic, retain) IBOutlet UIBarButtonItem *doneButton;

@property (nonatomic, retain) NSArray *dataArray; 
@property (nonatomic, retain) NSDateFormatter *dateFormatter; 

@end
