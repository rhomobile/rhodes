//
//  LogOptionsController.h
//  rhorunner
//
//  Created by Vlad on 4/17/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "RhoViewController.h"

@interface LogOptionsController : RhoViewController <UIScrollViewDelegate, UITextViewDelegate, UIPickerViewDelegate> {
	UITableView	*tableView;
	UITextView *textViewEnabledCategories;
	UITextView *textViewDisabledCategories;
	NSArray	*pickerViewArray;
	int selectedRow;
}

@property (nonatomic, retain) UITableView *tableView;


@end
