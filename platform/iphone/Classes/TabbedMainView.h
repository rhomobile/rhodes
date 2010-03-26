//
//  TabbedMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"

@interface TabbedMainView : UIViewController<RhoMainView> {

}

- (id)initWithParentWindow:(UIWindow *)w items:(NSArray*)items andDelegate:(id)delegate;

@end
