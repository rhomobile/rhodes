//
//  EditingViewController.m
//  RhoSyncClient
//
//  Created by lars on 9/22/08.
//  Copyright 2008 Home. All rights reserved.
//

#import "EditingViewController.h"


@implementation EditingViewController

@synthesize textValue, editedObject, editedFieldKey, dateEditing, dateValue, textField, dateFormatter, wrapper;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Create a date formatter to convert the date to a string format.
        dateFormatter = [[NSDateFormatter alloc] init];
        [dateFormatter setDateStyle:NSDateFormatterMediumStyle];
        [dateFormatter setTimeStyle:NSDateFormatterNoStyle];
    }
    return self;
}

- (void)viewDidLoad {
    // Adjust the text field size and font.
    CGRect frame = textField.frame;
    frame.size.height += 10;
    textField.frame = frame;
    textField.font = [UIFont boldSystemFontOfSize:16];
    // Set the view background to match the grouped tables in the other views.
    self.view.backgroundColor = [UIColor groupTableViewBackgroundColor];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)dealloc {
    [dateFormatter release];
    [datePicker release];
    [textValue release];
    free((void *)editedObject);
    [editedFieldKey release];
    [dateValue release];
    [super dealloc];
}

- (IBAction)cancel:(id)sender {
    // cancel edits
    [self.navigationController popViewControllerAnimated:YES];
}

- (IBAction)save:(id)sender {
    // save edits
    if (dateEditing) {
        [editedObject setValue:datePicker.date forKey:editedFieldKey];
    } else {
        [editedObject setValue:textField.text forKey:editedFieldKey];
		// Call the wrappedUpdate method to access the SyncObject's value
		[wrapper wrappedUpdate];
    }
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)viewWillAppear:(BOOL)animated {
    NSString *capitalizedKey = [editedFieldKey capitalizedString];
    self.title = capitalizedKey;
    textField.placeholder = capitalizedKey;
    if (dateEditing) {
        textField.enabled = NO;
        if (dateValue == nil) self.dateValue = [NSDate date];
        textField.text = [dateFormatter stringFromDate:dateValue];
        datePicker.date = dateValue;
    } else {
        textField.enabled = YES;
        textField.text = textValue;
        [textField becomeFirstResponder];
    }
}

- (IBAction)dateChanged:(id)sender {
    if (dateEditing) textField.text = [dateFormatter stringFromDate:datePicker.date];
}

@end

