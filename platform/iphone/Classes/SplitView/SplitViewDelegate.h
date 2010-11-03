//
//  TabbedMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"
#import "RhoViewController.h"
#import "RightViewController.h"

@interface SplitViewDelegate : NSObject <UISplitViewControllerDelegate> {

	RightViewController* rightController;

}

@property(nonatomic, retain) RightViewController* rightController;

@end
