//
//  DateTimePickerViewController.h
//  rhorunner
//
//  Created by Dmitry Soldatenkov on 02.03.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoViewController.h"

@interface DateTimePickerViewController : RhoViewController {
    CGRect myrect;
}
    
- (id)initWithRect:(CGRect)rect;

-(void)makeTransparencyBackground;
-(void)makeGrayBacground;

@end
