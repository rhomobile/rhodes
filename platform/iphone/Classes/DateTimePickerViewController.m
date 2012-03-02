//
//  DateTimePickerViewController.m
//  rhorunner
//
//  Created by Dmitry Soldatenkov on 02.03.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "DateTimePickerViewController.h"

@interface DateTimePickerViewController ()

@end

@implementation DateTimePickerViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (id)initWithRect:(CGRect)rect
{
    self = [super initWithNibName:nil bundle:nil];
    if (self) {
        // Custom initialization
        myrect = rect;
    }
    return self;
}


- (void)loadView
{
    // If you create your views manually, you MUST override this method and use it to create your views.
    // If you use Interface Builder to create your views, then you must NOT override this method.
    UIView* v = [[UIView alloc] initWithFrame:myrect];
    v.autoresizesSubviews = YES;
    v.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    v.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0];
    v.opaque = NO;
    self.view = v;
}

-(void)makeTransparencyBackground
{
    self.view.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0];
    [self.view setNeedsDisplay];
}

-(void)makeGrayBacground
{
    self.view.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0.4];
    [self.view setNeedsDisplay];
}


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}


@end
