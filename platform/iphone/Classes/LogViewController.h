//
//  LogViewController.h
//  rhorunner
//
//  Created by Vlad on 4/15/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "RhoViewController.h"

@interface LogViewController : RhoViewController <UIScrollViewDelegate, UITextViewDelegate> {
	UITextView *textView;

@public
	id   actionTarget;
	SEL  onShowLogOptions;
}

@property (assign) id   actionTarget;
@property (assign) SEL  onShowLogOptions;


@end
