//
//  BarItem.m
//  rhorunner
//
//  Created by lars on 8/25/09.
//  Copyright 2009 Home. All rights reserved.
//

#import "BarItem.h"


@implementation BarItem

@synthesize loaded, reload, label, location, icon, viewController;

- (id) init { 
	if ( self = [super init] ) { 
		self.loaded = NO; 
		self.reload = NO;
	} 
	return self; 
} 

-(void)dealloc {
	[self.label release];
	[self.location release];
	[self.icon release];
	[self.viewController release];
	[super dealloc];
}

@end
