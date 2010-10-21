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
#import "SplittedMainView.h"

@interface LeftViewController : UITableViewController {
	SplittedMainView* splittedView;
    NSArray *itemsData;
	
}

@property (nonatomic,retain) NSArray *itemsData;

- (id)initWithItems:(NSArray*)items parent:(SplittedMainView*)p;
- (void)setSelection:(int)index;

@end
