//
//  EditingViewController.h
//  RhoSyncClient
//
//  Created by lars on 9/22/08.
//  Copyright 2008 Home. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SyncObjectWrapper.h"
#include "Utils.h"


@interface EditingViewController : UIViewController {
    NSString *textValue;
    id editedObject;
    NSString *editedFieldKey;
    IBOutlet UITextField *textField;
    BOOL dateEditing;
    NSDate *dateValue;
    IBOutlet UIDatePicker *datePicker;
    NSDateFormatter *dateFormatter;
	SyncObjectWrapper *wrapper;
}

@property (nonatomic, retain) id editedObject;
@property (nonatomic, retain) NSString *textValue;
@property (nonatomic, retain) NSDate *dateValue;
@property (nonatomic, retain) NSString *editedFieldKey;
@property (nonatomic, assign) BOOL dateEditing;
@property (nonatomic, readonly) UITextField *textField;
@property (nonatomic, retain) NSDateFormatter *dateFormatter;
@property (nonatomic, retain) SyncObjectWrapper *wrapper;

- (IBAction)cancel:(id)sender;
- (IBAction)save:(id)sender;
- (IBAction)dateChanged:(id)sender;

@end

