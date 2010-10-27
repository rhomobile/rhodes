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
	int preferredSize;
	UIFont* myFont;
}

@property (nonatomic,retain) NSArray *itemsData;
@property (nonatomic,assign) int preferredSize;
@property (nonatomic,retain) UIFont *myFont;

- (id)initWithItems:(NSArray*)items parent:(SplittedMainView*)p;
- (void)setSelection:(int)index;

- (int)getPreferredWidth;

@end
