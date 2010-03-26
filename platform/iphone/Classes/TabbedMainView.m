    //
//  TabbedMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TabbedMainView.h"
#import "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "TabbedMainView"

@implementation TabbedMainView

- (id)initWithParentWindow:(UIWindow *)w items:(NSArray*)items andDelegate:(id)delegate {
    // TODO: implement
    return self;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
}

- (void)dealloc {
    [super dealloc];
}

// RhoMainView implementation

- (UIView*)getView {
    // TODO: implement
    return nil;
}

- (void)loadHTMLString:(NSString *)data {
    // TODO: implement
}

- (void)back:(int)index {
    // TODO: implement
}

- (void)forward:(int)index {
    // TODO: implement
}

- (void)navigate:(NSString *)url tab:(int)index {
    // TODO: implement
}

- (void)reload:(int)index {
    // TODO: implement
}

- (void)executeJs:(NSString*)js tab:(int)index {
    RAWLOG_INFO1("Executing JS: %s", [js UTF8String]);
    // TODO: implement
}

- (NSString*)currentLocation:(int)index {
    // TODO: implement
    return nil;
}

- (void)switchTab:(int)index {
    // TODO: implement
}

- (int)activeTab {
    // TODO: implement
    return 0;
}

@end
